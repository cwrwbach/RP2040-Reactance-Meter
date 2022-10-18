#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 15; //This is GPIO 15 = PIN 20 = Slice 7B
const uint LED_PIN = 13;
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
//printf(" Count = %d \n",val*20);

itoa(val*2,i_val,10);
lcd_set_cursor(0,0);
lcd_string(i_val);


return val*2;
}

//---


relay_on()
{


gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);
//while (1) 
    {
//   gpio_put(LED_PIN, 0);
//  sleep_us(1);
    gpio_put(LED_PIN, 1);
   sleep_ms(2);
    }
}
relay_off()
{

gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);
//while (1) 
    {
//   gpio_put(LED_PIN, 0);
//  sleep_us(1);
    gpio_put(LED_PIN, 0);
   sleep_ms(2);
    }
}



//---

int main() 
{
uint64_t test_val;
uint gpio;

char msg[16];
uint32_t freq;

double f1,f2,c1,l1;


gpio = MEASURE_PIN;

stdio_init_all();
printf("\nFrequency counter V0.0.5-india \n");

lcd_startup();
//sleep_ms(20000);
//lcd_set_cursor(0,0);

//strcpy(msg,"her again");

//lcd_string(msg);

sleep_ms(200);
//  lcd_clear();

lcd_string(msg);

assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B); //the input chan
slice_num = pwm_gpio_to_slice_num(gpio);

cfg = pwm_get_default_config();
pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_RISING);
pwm_config_set_clkdiv(&cfg, 20); //pre scale to prevent overflow in 16 bits
pwm_init(slice_num, &cfg, false);
gpio_set_function(gpio, GPIO_FUNC_PWM);




//for(int i = 0; i<100;i++)
    {
relay_off();
freq = measure_duty_cycle();
printf("\n F1: %d\n",freq);
sleep_ms(1000);
f1=(double) freq;
f1 = f1 * 20;

relay_on();
freq = measure_duty_cycle();
printf("\n F2: %d\n",freq);

sleep_ms(1000);    
f2=(double) freq;
f2 = f2 * 20;

printf(" f1: %f f2: %f \n",f1,f2); 
//f1=738e3;
//f2=477e3;

c1= ((f2*f2) / ((f1*f1) - (f2*f2))) * 1000e-12;

printf(" C1: %6.3f pF \n",c1 * 1e12);




    }






}
