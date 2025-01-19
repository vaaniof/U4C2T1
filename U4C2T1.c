#include <stdio.h>
#include "pico/stdlib.h"
#include <stdint.h>
#include "hardware/gpio.h"

// Definição dos pinos dos LEDs
#define led_pin_green 11
#define led_pin_blue  12
#define led_pin_red   13
#define buzzer_pin    21

// Definição dos pinos das colunas e linhas do teclado
const char cols[4] = {1, 2, 3, 4};
const char rows[4] = {5, 6, 7, 8};

// Definição das teclas do teclado
const char teclas[4][4] = {
    {'D', '#', '0', '*'}, 
    {'C', '9', '8', '7'},
    {'B', '6', '5', '4'},
    {'A', '3', '2', '1'}  
};


// Inicializa LEDs e buzzer
void init_pins() {
    gpio_init(led_pin_green);
    gpio_set_dir(led_pin_green, GPIO_OUT);
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_blue, GPIO_OUT);
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, GPIO_OUT);
}

// Inicializa as linhas e colunas do teclado
void init_teclado() {
    for (int i = 0; i < 4; i++) {
        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_OUT);
        gpio_put(rows[i], 0); // Inicializa as linhas em estado baixo

        gpio_init(cols[i]);
        gpio_set_dir(cols[i], GPIO_IN);
        gpio_pull_up(cols[i]); // Habilita pull-up nas colunas
    }
}

char leiturateclado() {
    char tecla = 0;

    for (int i = 0; i < 4; i++) {
        gpio_put(rows[i], 0); // Ativa a linha atual (nível baixo)
        sleep_us(20);         // Espera a estabilização do sinal

        for (int j = 0; j < 4; j++) {
            if (gpio_get(cols[j]) == 0) { // Verifica se a coluna está em nível baixo
                sleep_ms(50); // Debounce simples
                if (gpio_get(cols[j]) == 0) { // Confirma a tecla pressionada
                    tecla = teclas[i][j]; // Salva a tecla pressionada
                    gpio_put(rows[i], 1); // Desativa a linha atual
                    i = 0; // Reseta a variável `i`
                    j = 0; // Reseta a variável `j`
                    break;
                }
            }
        }

        gpio_put(rows[i], 1); // Desativa a linha atual (nível alto)
    }
   return tecla; // Retorna 0 se nenhuma tecla foi pressionada
   sleep_us(20);         // Espera a estabilização do sinal
}

// Função para desligar LEDs e buzzer
void desligar_leds_buzzer() {
    gpio_put(led_pin_green, 0);
    gpio_put(led_pin_blue, 0);
    gpio_put(led_pin_red, 0);
    gpio_put(buzzer_pin, 0);
    }
// Inicializa hardware geral
void init_hardware() {
    stdio_init_all();
    init_pins();
    init_teclado();
}

// Função principal
int main() {
    init_hardware(); // Inicializa o hardware
    init_teclado(); // Inicializa o teclado
    while (true) {
        char key = leiturateclado(); // Lê uma tecla do teclado
        if (key) { // Se uma tecla foi pressionada
            printf("Tecla pressionada: %c\n", key);
            // Controle de LEDs e buzzer
            if (key == 'A') {
                printf("LED ativo: Verde\n");
                gpio_put(led_pin_green, 1);
                gpio_put(led_pin_blue, 0);
                gpio_put(led_pin_red, 0);
            } else if (key == 'B') {
                printf("LED ativo: Azul\n");
                gpio_put(led_pin_green, 0);
                gpio_put(led_pin_blue, 1);
                gpio_put(led_pin_red, 0);
            } else if (key == 'C') {
                printf("LED ativo: Vermelho\n");
                gpio_put(led_pin_green, 0);
                gpio_put(led_pin_blue, 0);
                gpio_put(led_pin_red, 1);
            } else if (key == 'D') {
                printf("LED ativo: Verde, Azul e Vermelho\n");
                gpio_put(led_pin_green, 1);
                gpio_put(led_pin_blue, 1);
                gpio_put(led_pin_red, 1);
            } else if (key == '*') {
                printf("Buzzer ativo\n");
                gpio_put(buzzer_pin, 1);
                sleep_ms(100);
                gpio_put(buzzer_pin, 0);
            } else if (key == '#') {
                printf("Desligando LEDs e Buzzer\n");
                desligar_leds_buzzer();
            } else if (key == '0') {
                gpio_put(led_pin_green, 0);
                gpio_put(led_pin_blue, 0);
                gpio_put(led_pin_red, 0);
                gpio_put(buzzer_pin, 0);
            }
        }
        sleep_ms(100); // Delay para reduzir uso da CPU
    }
    return 0;
}