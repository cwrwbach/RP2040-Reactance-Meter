#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 15; //This is GPIO 15 = PIN 20 = Slice 7B
uint slice_num;
pwm_config cfg;

int measure_duty_cycle() 
{

pwm_init(slice_num, &cfg, false);
sleep_ms(10); 
pwm_set_enabled(slice_num, true);
sleep_ms(1000);
pwm_set_enabled(slice_num, false);

unsigned int val = pwm_get_counter(slice_num);
printf(" Count = %d \n",val);
return val;
}

int main() 
{
int test_val;
uint gpio;

gpio = MEASURE_PIN;

stdio_init_all();
printf("\nFrequency counter V0.0.3-echo \n");


assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B); //the input chan
slice_num = pwm_gpio_to_slice_num(gpio);

cfg = pwm_get_default_config();
pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_RISING);
pwm_config_set_clkdiv(&cfg, 10); //pre scale by 10 to prevent overflow in 16 bits
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
