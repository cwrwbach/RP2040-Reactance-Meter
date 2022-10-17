#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 15; //This is GPIO 15 = PIN 20 = Slice 7B
uint slice_num;
pwm_config cfg;

char i_val[16];
int lcd_startup();
void lcd_set_cursor(int,int);
void lcd_string(const char *s);

//----

int measure_duty_cycle() 
{

pwm_init(slice_num, &cfg, false);
sleep_ms(10); 
pwm_set_enabled(slice_num, true);
sleep_ms(1000);
pwm_set_enabled(slice_num, false);

unsigned int val = pwm_get_counter(slice_num);
printf(" Count = %d \n",val*20);

itoa(val*2,i_val,10);
lcd_set_cursor(0,0);
lcd_string(i_val);


return val;
}

//---

int main() 
{
uint64_t test_val;
uint gpio;

char msg[16];

gpio = MEASURE_PIN;

stdio_init_all();
printf("\nFrequency counter V0.0.4-hotel \n");

lcd_startup();
//sleep_ms(20000);
//lcd_set_cursor(0,0);

strcpy(msg,"her again");

//lcd_string(msg);

sleep_ms(2000);
//  lcd_clear();

lcd_string(msg);

assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B); //the input chan
slice_num = pwm_gpio_to_slice_num(gpio);

cfg = pwm_get_default_config();
pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_RISING);
pwm_config_set_clkdiv(&cfg, 20); //pre scale by 10 to prevent overflow in 16 bits
pwm_init(slice_num, &cfg, false);
gpio_set_function(gpio, GPIO_FUNC_PWM);

float aaa,bbb,ccc;

aaa = sin(ccc);

for(int i = 0; i<100;i++)
    {
        test_val = measure_duty_cycle();
    }
printf("\n test val = %d DONE \n",test_val);
}
