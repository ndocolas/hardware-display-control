#include <stdio.h>
#include <string.h>
#include "hardware.h"

void set_display_state(unsigned short *r0, int mode) {
    if(mode == 1) {
        *r0 |= (1 << 0);
    } else if(mode == 0){
        *r0 &= ~(1 << 0);
    }
}

void set_display_mode(unsigned short *r0, int mode) {
    if (mode < 0 || mode > 3) {
        printf("Erro: Modo display inválido. O valor deve ser entre 0 e 3.\n");
        return;
    }
    *r0 &= ~(3 << 1);
    *r0 |= (mode << 1);
}

void set_refresh_rate(unsigned short *r0, int value) {
    if (value < 0 || value > 63) {
        printf("Erro: %d Valor inválido. O valor deve ser entre 0 e 63.\n", value);
        return;
    }
    *r0 &= ~(63 << 3);
    *r0 |= (value << 3);
}

void set_led_operation_state(unsigned short *r0, int state) {
    if(state == 1) {
        *r0 |= (1 << 9);
    } else {
        *r0 &= ~(1 << 9);
    }
}

void set_led_color(unsigned short *r0, int value, int bit) {*r0 |= (value << bit);}

void def_led_color(unsigned short *r0, int value) {
    *r0 &= ~(7 << 10);
    switch(value) {
        case 0: set_led_color(r0, 1, 12);
        case 1: set_led_color(r0, 3, 11);
        case 2: set_led_color(r0, 1, 11);
        case 3: set_led_color(r0, 1, 11);
    }
}

void activate_default_state(unsigned short *r0) {*r0 |= (1 << 9);}

void def_color_red(unsigned short *r1, int value) {
    *r1 &= ~(255 << 0);
    if(value <= 255 && value >= 0) {
        *r1 |= (value << 0);
    }
}

void def_color_green(unsigned short *r1, int value) {
    *r1 &= ~(255 << 8);
    if(value <= 255 && value >= 0) {
        *r1 |= (value << 8);
    }
}

void def_color_blue(unsigned short *r2, int value) {
    *r2 &= ~(255 << 0);
    if(value <= 255 && value >= 0) {
        *r2 |= (value << 0);
    }
}

char* read_status_display(unsigned short *r0) {return (((*r0 >> 0) & 0b1) == 0) ? "OFF" : "ON";}

char* read_display_mode(unsigned short *r0) {
    switch((*r0 >> 1) & 0b11) {
        case 0: return "Estatico";
        case 1: return "Deslizante";
        case 2: return "Piscante";
        case 3: return "Deslizante/Piscante";
        default: return "Invalido";
    }
}

int read_refresh_rate(unsigned short *r0) {return ((*r0 >> 3) & 0b111111);}

char* read_led_operation(unsigned short *r0) {return (((*r0 >> 9) & 0b1) == 0) ? "OFF" : "ON";}

char* read_color_led(unsigned short *r0) {
    static char result[64];
    snprintf(result, sizeof(result), "Valor:\n [R] : %d\n [G] : %d\n [B] : %d", 
             ((*r0 >> 12) & 0b1), ((*r0 >> 11) & 0b1), ((*r0 >> 10) & 0b1));
    return result;
}

int read_battery_status_int(unsigned short *r3) {return (*r3 >> 0) & 0b11;}

char* read_battery_status(unsigned short *r3) {
    switch ((*r3 >> 0) & 0b11) {
        case 0: return "Critico";
        case 1: return "Baixo";
        case 2: return "Medio";
        case 3: return "Alto";
        default: return "Invalido";
    }
}

int read_number_of_times_screen(unsigned short *r3) {return ((*r3 >> 2) & 0b1111);}

int read_temperature(unsigned short *r3) {
    int temperature_bits = (*r3 >> 6) & 0b1111111111;
    return (temperature_bits & 0b1000000000) ? ((temperature_bits ^ 0b1111111111) + 1) : temperature_bits;
}

void def_word(unsigned short *r[], char word[]) {
    int word_length = strlen(word);
    int register_index = 4;

    int word_index = 0;
    while (word_index < word_length && register_index <= 15) {
        char current_char = word[word_index++];
        
        int index = register_index - 4;
        
        r[index][0] &= ~(255);
        r[index][1] &= ~(255 << 8);
        
        int ascii_value = (int)current_char;
        
        r[index][0] |= (ascii_value);

        current_char = word[word_index++];

        r[index][1] |= (ascii_value << 8);
        
        register_index++;
    }
}

void run_program(unsigned short *registers[]) {
    int choice = -1;
    int sub;
    while (choice != 0) {
        printf("Menu de opcoes: \n\n [1] Ligar/Desligar display\n [2] Selecionar modo de exibição");
        printf("\n [3] Alterar Refresh Rate\n [4] Ligar/Desligar LED operação\n [5] Alterar cor do Display\n [6] Definir palavra Display\n\n");
        printf(" [0] Finalizar execução  [7] Restaurar Padrao  [8] Leitura de Informacoes\n\n");

        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("\nLigar/Desligar display: \n [0] Desligar\n [1] Ligar\n");
                scanf("%d", &sub);
                if (sub == 1 || sub == 0) set_display_state(registers[0], sub);
                else printf("Escolha inválida. Tente novamente.\n");
                break;
            case 2:
                printf("\nModo de Exibicao:\n [0] Estatico\n [1] Deslizante\n [2] Piscante\n [3] Deslizante/Piscante\n");
                scanf("%d", &sub);
                if(sub >= 0 && sub <= 3) set_display_mode(registers[0], sub);
                else printf("Escolha inexistente. Tente novamente.\n");
                break;
            case 3:
                printf("\nRefresh Rate: \n\n Insira um valor entre 0 e 63!\nvalor: ");
                scanf("%d", &sub);
                if(sub >= 0 && sub <= 63) set_refresh_rate(registers[0], sub);
                else printf("Valor invalido. Tente novamente.\n");
                break;
            case 4:
                printf("\nLigar/Desligar: \n [0] Desligar\n [1] Ligar\n");
                scanf("%d", &sub);
                if(sub <= 1 && sub >= 0) set_led_operation_state(registers[0], sub);
                else printf("Valor invalido. Tente novamente.\n");
                break;
            case 5:
                while(1) {
                    printf("Alteracao de cor do display: \n [1] Vermelho\n [2] Verde\n [3] Azul\n\n [0] Sair\n");
                    int sub_case5;
                    scanf("%d", &sub_case5);
                    if(sub_case5 == 0) break;
                    int value;
                    switch(sub_case5) {
                        case 1:
                            printf("\nDigite o valor para vermelho: ");
                            scanf("%d", &value);
                            def_color_red(registers[1], value);
                            break;
                        case 2:
                            printf("\nDigite o valor para verde: ");
                            scanf("%d", &value);
                            def_color_green(registers[1], value);
                            break;
                        case 3:
                            printf("\nDigite o valor para azul: ");
                            scanf("%d", &value);
                            def_color_blue(registers[2], value);
                            break;
                        default:
                            printf("Valor inválido. Tente novamente.\n");
                    }
                }
                break;
            case 6:
                printf("\nDigite a palavra desejada: ");
                char word[30];
                scanf("%s", word);
                def_word(registers, word);
                break;
            case 7:
                printf("\nRestaurar Padrao: \nVoce deseja restaurar para o padrao de fabrica? \n [0] Nao \n [1] Sim\n");
                scanf("%d", &sub);
                if(sub == 1) activate_default_state(registers[0]);
                break;
            case 8:
                while(1) {
                    printf("Menu de leitura: \n\n [1] Status display \n [2] Modo display \n [3] valor refresh rate \n");
                    printf(" [4] Status led operation\n [5] Status RGB LED\n [6] Nivel da bateria\n");
                    printf(" [7] Contagem de passagens no modo deslizante\n [8] Temperatura Atual");
                    printf("\n\n [0] Sair\n");
                    int sub_case7;
                    scanf("%d", &sub_case7);
                    if(sub_case7 == 0) break;
                    switch(sub_case7) {
                        case 1: printf("\nDisplay: %s\n\n", read_status_display(registers[0])); break;
                        case 2: printf("\nModo display: %s\n\n", read_display_mode(registers[0])); break;
                        case 3: printf("\nValor do refresh rate: %d\n\n", read_refresh_rate(registers[0])); break;
                        case 4: printf("\nValor da led: %s\n\n", read_led_operation(registers[0])); break;
                        case 5: printf("\nValor das cores: %s\n\n", read_color_led(registers[0])); break;
                        case 6: printf("\nNivel da bateria: %s\n\n", read_battery_status(registers[3])); break;
                        case 7: printf("\nContagem de passagens no modo deslizante: %d\n\n", read_number_of_times_screen(registers[3])); break;
                        case 8: printf("\nTemperatura atual: %d\n\n", read_temperature(registers[3])); break;
                        default: printf("Escolha inválida. Tente novamente.\n"); break;
                    }
                }
                break;
            case 0:
                break;
            default:
                printf("Escolha inválida. Tente novamente.\n");
                break;
        }
    }
}

