#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hardware.h"

void def_display_state(unsigned short *r0, int value) {
    (value == 1) ? (*r0 |= (1 << 0)) : (*r0 &= ~(1 << 0));}

void def_display_mode(unsigned short *r0, int mode) {
    if (mode < 0 || mode > 3) {
        printf("Erro: Modo display inválido. O valor deve ser entre 0 e 3.\n");
        return;
    }
    *r0 &= ~(3 << 1);
    *r0 |= (mode << 1);
}

void def_refresh_rate(unsigned short *r0, int value) {
    if (value < 0 || value > 63) {
        printf("Erro: %d Valor inválido. O valor deve ser entre 0 e 63.\n", value);
        return;
    }
    *r0 &= ~(63 << 3);
    *r0 |= (value << 3);
}

void det_led_operation_state(unsigned short *r0, int state) {
    (state == 1) ? (*r0 |= (1 << 9)) : (*r0 &= ~(1 << 9));
}

void def_led_color_value(unsigned short *r0, int value, int bit) {*r0 |= (value << bit);}

void def_led_color(unsigned short *r0, int value) {
        *r0 &= ~(7 << 10);
        switch(value) {
            case 0: *r0 |= (1 << 12);break;
            case 1: *r0 |= (3 << 11);break;
            case 2: *r0 |= (1 << 11);break;
            case 3: *r0 |= (1 << 11);break;
        }
}

void restore_deafult(unsigned short *r0) {*r0 |= (1 << 13);}

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

void clear_words(unsigned short *r[]) {for(int i = 4; i < 16; i++) {*r[i] = 0;}}

void def_word(unsigned short *r[], char word[]) {
    int word_length = strlen(word);
    int register_index = 4;

    clear_words(r);

    int word_index = 0;
    while (word_index < word_length) {
        int ascii_value = (int)toupper(word[word_index]);
        *r[register_index] |= (ascii_value << 0);

        word_index++;

        int ascii_value_two = (int)toupper(word[word_index]);
        *r[register_index] |= (ascii_value_two << 8);

        word_index++;
        register_index++;
    }

    while(register_index < 16) {
        *r[register_index] = 0;
        register_index++;
    }
}

int read_display_state_value(unsigned short *r0) {return ((*r0 >> 0) & 0b1) == 0;}

char* read_display_state(unsigned short *r0) {return read_display_state_value(r0) ? "OFF" : "ON";}

int read_display_mode_value(unsigned short *r0) {return ((*r0 >> 1) & 0b11);}

char* read_display_mode(unsigned short *r0) {
    switch(read_display_mode_value(r0)) {
        case 0: return "Estatico";
        case 1: return "Deslizante";
        case 2: return "Piscante";
        case 3: return "Deslizante/Piscante";
        default: return "Invalido";
    }
}

int read_refresh_rate(unsigned short *r0) {return ((*r0 >> 3) & 0b111111);}

int read_led_operation_value(unsigned short *r0) {return ((*r0 >> 9) & 0b1) == 0;}
char* read_led_operation(unsigned short *r0) {return read_led_operation_value(r0) ? "OFF" : "ON";}

int read_color_display_red(unsigned short *r1) {return((*r1 >> 0) & 0b11111111);}
int read_color_display_green(unsigned short *r1) {return((*r1 >> 8) & 0b11111111);}
int read_color_display_blue(unsigned short *r2) {return((*r2 >> 0) & 0b11111111);}

char* read_color_display(unsigned short *r1, unsigned short *r2) {
    static char result[64];
    snprintf(result, sizeof(result), "Valor:\n [R] : %d\n [G] : %d\n [B] : %d",
             read_color_display_red(r1), 
             read_color_display_green(r1), 
             read_color_display_blue(r2));
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

void read_word(unsigned short *r[]) {
    for(int i = 4; i<16; i++) {
        int bit_first = ((*r[i] >> 0) & 0b01111111);
        int bit_second = ((*r[i] >> 8) & 0b01111111);
        printf("R%d: %d-%d\n", i, bit_first, bit_second);
    }
}

void run_program(unsigned short *registers[]) {
    def_led_color(registers[0], read_battery_status_int(registers[3]));
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
                if (sub == 1 || sub == 0) def_display_state(registers[0], sub);
                else printf("Escolha inválida. Tente novamente.\n");
                break;
            case 2:
                printf("\nModo de Exibicao:\n [0] Estatico\n [1] Deslizante\n [2] Piscante\n [3] Deslizante/Piscante\n");
                scanf("%d", &sub);
                if(sub >= 0 && sub <= 3) def_display_mode(registers[0], sub);
                else printf("Escolha inexistente. Tente novamente.\n");
                break;
            case 3:
                printf("\nRefresh Rate: \n\n Insira um valor entre 0 e 63!\nvalor: ");
                scanf("%d", &sub);
                if(sub >= 0 && sub <= 63) def_refresh_rate(registers[0], sub);
                else printf("Valor invalido. Tente novamente.\n");
                break;
            case 4:
                printf("\nLigar/Desligar: \n [0] Desligar\n [1] Ligar\n");
                scanf("%d", &sub);
                if(sub <= 1 && sub >= 0) det_led_operation_state(registers[0], sub);
                else printf("Valor invalido. Tente novamente.\n");
                break;
            case 5:
                while(1) {
                    printf("Alteracao de cor do display: \n [1] Vermelho (%d)\n [2] Verde (%d)\n [3] Azul (%d)\n\n [0] Sair\n", 
                    read_color_display_red(registers[1]),
                    read_color_display_green(registers[1]),
                    read_color_display_blue(registers[2]));
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
                char word[22];
                scanf("%s", word);
                def_word(registers, word);
                break;
            case 7:
                printf("\nRestaurar Padrao: \nVoce deseja restaurar para o padrao de fabrica? \n [0] Nao \n [1] Sim\n");
                scanf("%d", &sub);
                if(sub == 1) restore_deafult(registers[0]);
                break;
            case 8:
                while(1) {
                    printf("Menu de leitura: \n\n [1] Status display \n [2] Modo display \n [3] valor refresh rate \n");
                    printf(" [4] Status led operation\n [5] Status RGB LED\n [6] Nivel da bateria\n");
                    printf(" [7] Contagem de passagens no modo deslizante\n [8] Temperatura Atual\n [9] Registers\n");
                    printf("\n [0] Sair\n");
                    int sub_case7;
                    scanf("%d", &sub_case7);
                    if(sub_case7 == 0) break;
                    switch(sub_case7) {
                        case 1: printf("\nDisplay: %s\n\n", read_display_state(registers[0])); break;
                        case 2: printf("\nModo display: %s\n\n", read_display_mode(registers[0])); break;
                        case 3: printf("\nValor do refresh rate: %d\n\n", read_refresh_rate(registers[0])); break;
                        case 4: printf("\nValor da led: %s\n\n", read_led_operation(registers[0])); break;
                        case 5: printf("\nValor das cores: %s\n\n", read_color_display(registers[1], registers[2])); break;
                        case 6: printf("\nNivel da bateria: %s\n\n", read_battery_status(registers[3])); break;
                        case 7: printf("\nContagem de passagens no modo deslizante: %d\n\n", read_number_of_times_screen(registers[3])); break;
                        case 8: printf("\nTemperatura atual: %d\n\n", read_temperature(registers[3])); break;
                        case 9: read_word(registers); break;
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

