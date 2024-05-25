#ifndef HARDWARE_H
#define HARDWARE_H


//1
void def_display_state(unsigned short *r0, int mode);
//2
void def_display_mode(unsigned short *r0, int mode);
//3
void def_refresh_rate(unsigned short *r0, int value);
//4
void det_led_operation_state(unsigned short *r0, int state);
//5
void def_color_red(unsigned short *r1, int value);
void def_color_green(unsigned short *r1, int value);
void def_color_blue(unsigned short *r2, int value);
//6
void def_word(unsigned short *r[], char word[]);
void clear_words(unsigned short *r[]);
//8
void restore_deafult(unsigned short *r0);
//manipulacao automatica
void def_led_color_value(unsigned short *r0, int bit, int value);
void def_led_color(unsigned short *r0, int value);


//LER------------------------

//1
int read_display_state_value(unsigned short *r0);
char* read_display_state(unsigned short *r0);
//2
int read_display_mode_value(unsigned short *r0);
char* read_display_mode(unsigned short *r0);
//3
int read_refresh_rate(unsigned short *r0);
//4
int read_led_operation_value(unsigned short *r0);
char* read_led_operation(unsigned short *r0);
//5
int read_color_display_red(unsigned short *r1);
int read_color_display_green(unsigned short *r1);
int read_color_display_blue(unsigned short *r2);
char* read_color_display(unsigned short *r1, unsigned short *r2);
//6
int read_battery_status_int(unsigned short *r3);
char* read_battery_status(unsigned short *r3);
//7
int read_number_of_times_screen(unsigned short *r3);
//8
int read_temperature(unsigned short *r3);
//9
void read_word(unsigned short *r[]);

void run_program(unsigned short *registers[]);

#endif
