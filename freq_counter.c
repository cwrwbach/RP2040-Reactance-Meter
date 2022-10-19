#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 15; //This is GPIO 15 = PIN 20 = Slice 7B
const uint LED_PIN = 13;
const uint LEFT = 27;
const uint RIGHT = 22;
const uint TOP = 28;
const uint BOT = 26;

uint slice_num;
pwm_config cfg;

char i_val[16];
int lcd_startup();
void lcd_set_cursor(int,int);
void lcd_string(const char *s);

//globals
double f1,c1,l1;

//----

int measure_duty_cycle() 
{
pwm_init(slice_num, &cfg, false);
sleep_ms(10); 
pwm_set_enabled(slice_num, true);
sleep_ms(1000);
pwm_set_enabled(slice_num, false);

unsigned int val = pwm_get_counter(slice_num);

itoa(val*2,i_val,10);
lcd_set_cursor(0,0);
lcd_string(i_val);
return val*2;
}

relay_on()
{
gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);
gpio_put(LED_PIN, 1);
sleep_ms(2);
}
relay_off()
{

gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);
gpio_put(LED_PIN, 0);
sleep_ms(2);

}

do_calib()
{
uint32_t freq;
//double c1;
double f2;
printf("\nStarting Calib \n");
relay_off();
freq = measure_duty_cycle();
sleep_ms(1000);
f1=(double) freq;
f1 = f1 * 20;

relay_on();
freq = measure_duty_cycle();

sleep_ms(1000);    
f2=(double) freq;
f2 = f2 * 20;

printf(" f1: %f f2: %f \n",f1,f2); 
c1= ((f2*f2) / ((f1*f1) - (f2*f2))) * 1000e-12;
printf(" C1: %6.3f pF \n",c1 * 1e12);

l1= 1/(4 * (M_PI * M_PI) * (f1 * f1) * c1);
printf(" L1: %6.3f uH \n",l1 * 1e6);

}

cap_measure()
{
char msg[16];
uint32_t freq;
double cx;
double f2;
printf("\nStarting Capacitor Measure \n");
relay_off();
freq = measure_duty_cycle();
sleep_ms(1000);
f2=(double) freq;
f2 = f2 * 20;

printf(" f1: %f f2: %f \n",f1,f2); 
cx= ( ((f1*f1)/(f2*f2)) -1) * c1;
printf(" CX: %6.3f pF \n",cx * 1e12);

sprintf(msg,"Cap: %3.1f",cx*1e12);

lcd_set_cursor(0,0);
lcd_string(msg);


}

ind_measure()
{
char msg[16];
uint32_t freq;
double lx;
double f2;
printf("\nStarting Inductor Measure \n");
relay_off();
freq = measure_duty_cycle();
sleep_ms(1000);
f2=(double) freq;
f2 = f2 * 20;

printf(" f1: %f f2: %f \n",f1,f2); 
lx= ( ((f1*f1)/(f2*f2)) -1) * l1;
printf(" LX: %6.3f uH \n",lx * 1e6);

sprintf(msg,"Ind: %3.3f",lx*1e6);

lcd_set_cursor(0,0);
lcd_string(msg);


}

//---

int main() 
{
uint64_t test_val;
uint gpio;
uint test;
char msg[16];

gpio_init(TOP);
gpio_set_dir(TOP, GPIO_IN);
gpio_pull_up(TOP) ; 

gpio_init(BOT);
gpio_set_dir(BOT, GPIO_IN);
gpio_pull_up(BOT) ; 

gpio_init(LEFT);
gpio_set_dir(LEFT, GPIO_IN);
gpio_pull_up(LEFT) ; 

gpio_init(RIGHT);
gpio_set_dir(RIGHT, GPIO_IN);
gpio_pull_up(RIGHT) ; 

gpio = MEASURE_PIN;

stdio_init_all();
printf("\nFrequency counter V0.0.6-juliet \n");

lcd_startup();

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


while(1)
    {
    if(gpio_get(TOP) ==0)
        {
        do_calib();
        printf("Done Calib\n");
        }

    if(gpio_get(BOT) ==0)
        {
        cap_measure();
        printf(" Done Cap measure \n");
        }

    if(gpio_get(LEFT) ==0)
        {
        ind_measure();
        printf(" Done Ind measure \n ");
        }





    if(gpio_get(RIGHT) ==0)
        printf("Right pressed \n");
    }
}
