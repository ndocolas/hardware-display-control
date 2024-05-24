#ifndef HARDWARE_H
#define HARDWARE_H

void set_display_state(unsigned short *r0, int mode);
void set_display_mode(unsigned short *r0, int mode);
void set_refresh_rate(unsigned short *r0, int value);
void set_led_operation_state(unsigned short *r0, int state);
void set_led_color(unsigned short *r0, int bit, int value);
void def_led_color(unsigned short *r0, int value);
void activate_default_state(unsigned short *r0);
void def_color_red(unsigned short *r1, int value);
void def_color_green(unsigned short *r1, int value);
void def_color_blue(unsigned short *r2, int value);
void def_word(unsigned short *r[], char word[]);

char* read_status_display(unsigned short *r0);
char* read_display_mode(unsigned short *r0);
int read_refresh_rate(unsigned short *r0);
char* read_led_operation(unsigned short *r0);
char* read_color_led(unsigned short *r0);
int read_battery_status_int(unsigned short *r3);
char* read_battery_status(unsigned short *r3);
int read_number_of_times_screen(unsigned short *r3);
int read_temperature(unsigned short *r3);
void run_program(unsigned short *registers[]);

#endif
