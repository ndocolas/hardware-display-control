#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_PATH "registers.bin"
#define FILE_SIZE 1024  // Same size as used in the first program

// Function to open or create the file and map it into memory
    char* registers_map(const char* file_path, int file_size, int* fd) {
        *fd = open(file_path, O_RDWR | O_CREAT, 0666);
        if (*fd == -1) {
            perror("Error opening or creating file");
            return NULL;
        }

        // Ensure the file is of the correct size
        if (ftruncate(*fd, file_size) == -1) {
            perror("Error setting file size");
            close(*fd);
            return NULL;
        }

        // Map the file into memory
    char *map = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
        if (map == MAP_FAILED) {
            perror("Error mapping file");
            close(*fd);
            return NULL;
        }

        return map;
    }

    int registers_release(void* map, int file_size, int fd) {
        if (munmap(map, file_size) == -1) {
            perror("Error unmapping the file");
            close(fd);
            return -1;
        }

        if (close(fd) == -1) {
            perror("Error closing file");
            return -1;
        }

        return 0;
    }

    void set_display_state(unsigned short *r0, int mode) {
        if(mode == 1) {
            *r0 |= (1 << 0);
        } else if(mode == 0){
            *r0 &= ~(1 << 0);
        }
    }   

    void set_display_mode(unsigned short *r0, int mode) {
        if (mode < 0 || mode > 3) {
            printf("Error: Invalid display mode. Mode must be between 0 and 3.\n");
            return;
        }
        *r0 &= ~(3 << 1);
        *r0 |= (mode << 1);
    }


    void set_refresh_rate(unsigned short *r0, int value) {
        if (value < 0 || value > 63) {
            printf("Error: Invalid value. Value must be between 0 and 63.\n");
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

    void set_led_color(unsigned short *r0, int valor) {//RGB
        *r0 &= ~(7 << 10);
        if(valor == 0) {
             *r0 |= (1 << 12);
        } else if(valor == 1) {
            *r0 |= (3 << 11);
        } else if(valor == 2) {
            *r0 |= (1 << 11);
        } else if(valor == 3) {
            *r0 |= (1 << 11);
        }
    }

    void activate_default_state(unsigned short *r0) {
        *r0 = 0;

        *r0 |= (1 << 0);
        *r0 |= (2 << 3);
        *r0 |= (1 << 10);
    }

    void def_color_red(unsigned short *r1, int valor) {
        *r1 &= ~(255 << 0);
        if(valor<= 255 && valor>=0) {
            *r1 |= (valor << 0);
        }
    }

    void def_color_green(unsigned short *r1, int valor) {
        *r1 &= ~(255 << 8);
        if(valor<= 255 && valor>=0) {
            *r1 |= (valor << 8);
        }
    }

    void def_color_blue(unsigned short *r2, int valor) {
        *r2 &= ~(255 << 0);
        if(valor<= 255 && valor>=0) {
            *r2 |= (valor << 0);
        }
    }

    char* read_status_display(unsigned short *r0) {return (((*r0 >> 0) & 0b1) == 0) ? "OFF" : "ON"; }

    char* read_display_mode(unsigned short *r0) {
        switch((*r0 >> 1) & 0b11) {
            case 0:
            return "Estatico";
                break;
            case 1:
            return "Deslizante";
                break;
            case 2:
            return "Piscante";
                break;
            case 3:
            return "Deslizante/Piscante";
                break;
            default:
                break;
        }
    }

    int read_refresh_rate(unsigned short *r0) {return ((*r0 >> 3) & 0b111111);}

    char* read_led_operation(unsigned short *r0) { return (((*r0 >>9) & 0b1) == 0) ? "OFF" : "ON";}

    char* read_color_led(unsigned short *r0) {
        static char result[64];
        snprintf(result, sizeof(result), "Valor:\n10: %d\n11: %d\n12: %d", ((*r0 >> 10) & 0b1), ((*r0 >> 11) & 0b1), ((*r0 >> 12) & 0b1));
        return result;
    }

    char* read_battery_status(unsigned short *r3) {
        switch ((*r3 >>0) & 0b11) {
            case 0:return "Critico";break;
            case 1:return "Baixo";break;
            case 2:return "Medio";break;
            case 3:return "Alto";break;
        }
    }

    void run_program(unsigned short *r0, unsigned short *r1, unsigned short *r2, unsigned short *r3) {
        int choice = -1;
        int sub;
        while (choice != 0) {
            printf("Menu de opcoes: \n [1] Ligar/Desligar display\n [2] Selecionar modo de exibição");
            printf("\n [3] Alterar Refresh Rate\n [4] Ligar/Desligar LED operação\n [5] Alterar cor do Display\n\n");
            printf(" [0] Finalizar execução  [6] Restaurar Padrao  [7] Leitura de Informacoes\n");

            scanf("%d", &choice);
            switch(choice) {
                case 1:
                        
                    printf("\nLigar/Desligar display: \n [0] Desligar\n [1] Ligar\n");
                    scanf("%d", &sub);

                    if (sub==1 || sub==0) set_display_state(r0, sub);
                    else printf("Escolha inválida. Tente novamente.\n");

                 break;
                case 2:
                    
                    printf("\nModo de Exibicao:\n [0] Estatico\n [1] Deslizante\n [2] Piscante\n [3] Deslizante/Piscante\n");
                    scanf("%d", &sub);

                    if(sub >= 0 && sub <= 3)  set_display_mode(r0, sub);
                    else printf("Escolha inexistente. Tente novamente.\n");
                 break;
                case 3:
                    
                    printf("\nRefresh Rate: \n\n Insira um valor entre 0 e 63!\nValor: ");
                    scanf("%d", &sub);
                    if(sub>=0 && sub<=63) set_refresh_rate(r0, sub);
                    else printf("Valor invalido. Tente novamente.\n");
                 break;
                case 4:
                    
                    printf("\nLigar/Desligar: \n [0] Desligar\n [1] Ligar\n");
                    scanf("%d", &sub);
                    if(sub<=1 && sub>=0) set_led_operation_state(r0, sub);
                    else printf("Valor invalido. Tente novamente.\n");
                 break;
                case 5:
                    while(sub != 0) {
                        printf("Alteracao de cor do display: \n [1] Vermelho\n [2] Verde\n [3] Azul\n [0] Sair\n");
                        scanf("%d", &sub);
                        switch(sub) {
                            case 1:
                                int valor1;
                                printf("\nDigite o valor para vermelho: ");
                                scanf("%d", &valor1);
                                def_color_red(r1, valor1);
                                break;
                            case 2:
                                int valor2;
                                printf("\nDigite o valor para verde: ");
                                scanf("%d", &valor2);
                                def_color_green(r1, valor2);
                                break;
                            case 3:
                                int valor3;
                                printf("\nDigite o valor para azul: ");
                                scanf("%d", &valor3);
                                def_color_blue(r2, valor3);
                                break;
                            default:
                                printf("Valor inválido. Tente novamente.\n");
                        }

                    }
                 break;
                case 6:
                    
                    printf("\nRestaurar Padrao: \nVoce deseja restaurar para o padrao de fabrica? \n [0] Nao \n [1] Sim\n");
                    scanf("%d", &sub);
                    if(sub == 1) {
                        activate_default_state(r0);
                    }
                 break;
                case 7:
                    while(sub !=0) {
                        printf("Menu de leitura: \n [1] Bit 0 (Display ON/OFF) \n [2] Modo display \n [3] Valor refresh rate \n");
                        printf(" [4] Valor led de operacao\n [5] Cores do led de status\n [6] Nivel da bateria");
                        printf("\n [0] Sair\n");
                        scanf("%d", &sub);
                        switch(sub) {
                            case 1:printf("\nDisplay: %s\n\n", read_status_display(r0));break;
                            case 2:printf("\nModo display: %s\n\n", read_display_mode(r0));break;
                            case 3:printf("\nValor do refresh rate: %d\n\n", read_refresh_rate(r0));break;
                            case 4:printf("\nValor da led: %s\n\n", read_led_operation(r0));break;
                            case 5:printf("\nValor das cores: %s\n\n", read_color_led(r0));break;
                            case 6:printf("\nNivel da bateria:%s\n\n", read_battery_status(r3));break;

                            default:
                            break;
                        }
                    }
                 break;
                default:
                printf("Escolha inválida. Tente novamente.\n");
                    break;
            }
        }
    }

int main() {
    int fd;
    char* map = registers_map(FILE_PATH, FILE_SIZE, &fd);
    if (map == NULL) {
        return EXIT_FAILURE;
    }

    unsigned short *base_address = (unsigned short *)map;
    unsigned short *r0 = base_address + 0x00;
    unsigned short *r1 = base_address + 0x01;
    unsigned short *r2 = base_address + 0x02;
    unsigned short *r3 = base_address + 0x03;

    set_led_color(r0, read_battery_status(r3));
    run_program(r0, r1, r2, r3);

    if (registers_release(map, FILE_SIZE, fd) == -1) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
