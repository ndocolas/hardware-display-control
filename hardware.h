void set_display_state();
void set_display_mode();
void set_refresh_rate();
void set_led_operation_state();
void set_led_color(); //adicionar def_color
void activate_default_state();
void def_color_red();
void def_color_green();
void def_color_blue();
char* read_status_display();
char* read_display_mode();
int read_refresh_rate();
char* read_led_operation();
char* read_color_led();
int read_battery_status_int();
char* read_battery_status();
int read_number_of_times_screen();
int read_temperature();
void run_program();

