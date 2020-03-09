/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project :
Version :
Date    : 5/18/2019
Author  :
Company :
Comments:


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>
#include <stdio.h>
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <delay.h>


// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
unsigned int IR[4];
unsigned char buffer[16];
int rand_num;
int cnt;
bool flag;
int mode;

int st_mv;
int turn_val;

bool check_straight();
bool check_2_r     ();
bool check_2_l     ();
bool check_3       ();

void turn_left     ();
void turn_right    ();
void go_straight   ();
void disable_motor ();
void enable_motor  ();
void set_right     ();
void set_right_low ();
void set_left      ();
void move          (int time);
void stable        ();


void get_IR        ();
void show_IR       ();

// Voltage Reference: Int., cap. on AREF
#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (1<<ADLAR))

// Read the 8 most significant bits
// of the AD conversion result
unsigned char read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_ms(250);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCH;
}


void main(void)
{
// Declare your local variables here
// Input/Output Ports initialization
//Bit7=In Bit6=In Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=In Bit0=In
DDRA=(0<<DDA7) | (0<<DDA6) | (1<<DDA5) | (1<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
//Bit7=T Bit6=T Bit5=1 Bit4=1 Bit3=T Bit2=T Bit1=T Bit0=T
PORTA=(0<<PORTA7) | (1<<PORTA6) | (1<<PORTA5) | (1<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);


//Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=Out Bit2=In Bit1=In Bit0=Out
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (1<<DDB4) | (1<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
//Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=0 Bit2=T Bit1=T Bit0=0
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);


//Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
//Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);


//Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=In Bit0=In
DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (0<<DDD1) | (0<<DDD0);
//Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=T Bit0=T
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);


// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 3
// D5 - PORTC Bit 4
// D6 - PORTC Bit 5
// D7 - PORTC Bit 6
// Characters/line: 16
lcd_init(16);

mode = 1;
turn_val = 250;
st_mv = 120;

while (1)
      {
      if(PINA.6 == 0){
        while(PINA.6 == 0);
        mode = 1 - mode;
        if(mode == 1){
            lcd_gotoxy(5,1);
            lcd_puts("Mode 1");
            PORTA.4 = 0;
            // vars value
            turn_val = 250;
            st_mv = 120;
            delay_ms(2000);
            lcd_clear();
        }
        else {
            lcd_gotoxy(5,1);
            lcd_puts("Mode 2");
            PORTA.4 = 1;
            // vars value
            turn_val = 150;
            st_mv = 90;
            delay_ms(2000);
            lcd_clear();
        }
      }
      get_IR();
      show_IR();
      stable();

      if(check_3() == true){

            go_straight();
            move(80);
            disable_motor();


            rand_num=rand() % 3;
            if     (rand_num == 0)         turn_right ();
            else if(rand_num == 1)         go_straight();
            else if(rand_num == 2)         turn_left  ();
      }
      else if(check_2_r() == true){


            go_straight();
            move(80);
            disable_motor();

            rand_num=rand() % 2;
            if     (rand_num == 0)         turn_right ();
            else if(rand_num == 1)         go_straight();
      }
      else if(check_2_l() == true){

            go_straight();
            move(80);
            disable_motor();

            rand_num=rand() % 2;
            if     (rand_num == 0)         turn_left ();
            else if(rand_num == 1)         go_straight();
      }

      else if(check_straight() == true ) {
            go_straight();
            if(flag == true){
                move(50);
                flag = false;
            }
            else{
                move(st_mv);
            }

      }


      }
}



// ***** IR*****
void get_IR(){
      int i;
      for(i=0;i<4;i++){
        IR[3-i] = read_adc(i);
      }
}

void show_IR(){
      sprintf(buffer , "%3d %3d %3d %3d" , IR[0], IR[1] , IR[2] , IR[3] );
      lcd_gotoxy(0,0);
      lcd_puts(buffer);
}

// ***** MOVES FUNCTIONS *****
void go_straight(){
    PORTD.2=1;
    PORTD.3=0;
    PORTD.6=0;
    PORTD.7=1;
}

void turn_right(){
      enable_motor();
      go_straight();
      delay_ms(30);
      set_right();
      delay_ms(turn_val);
      //go_straight();
      //move();
      //set_right();

      do{
            set_right();
            move(50); //50
           // go_straight();
            //move(30);//20

            get_IR();
            show_IR();
      }while(check_straight() == false);


      disable_motor();
}

void turn_left(){
    enable_motor();
    go_straight();
    delay_ms(30);
    set_left();
    delay_ms(turn_val); // 200 bood bra charkhesh

    do{
        set_left();
        move(50);
        //go_straight();
        //move(30);

        get_IR();
        show_IR();
    }while(check_straight() == false);

    disable_motor();
}


void move(int time){
    PORTD.4=1;
    PORTD.5=1;
    delay_ms(time);
    disable_motor();
}
void enable_motor(){
    PORTD.4=1;
    PORTD.5=1;
}
void disable_motor(){
    PORTD.4=0;
    PORTD.5=0;
}
void set_right(){
    PORTD.2=1;
    PORTD.3=0;
    PORTD.6=1;
    PORTD.7=0;
}
void set_left(){
    PORTD.2=0;
    PORTD.3=1;
    PORTD.6=0;
    PORTD.7=1;
}
void set_right_low(){
    PORTD.2=1;
    PORTD.3=0;
    PORTD.6=0;
    PORTD.7=0;
}
void stable(){
    if (IR[0]>=50 && IR[1]>=50 && IR[2]<50 && IR[3]<50){
        enable_motor();
        set_left();
        delay_ms(40);//35
        go_straight(); //f
        delay_ms(40);//50     //f
        disable_motor();
        flag=false;
    }
    else if(IR[0]<50 && IR[1]<50 && IR[2]>=50 && IR[3]>=50){
        enable_motor();
        set_right();
        delay_ms(40);//35
        go_straight();//f
        delay_ms(40);    //f
        disable_motor();
        flag=false;
    }
    else if(IR[0]<50 && IR[1]>=50 && IR[2]<50 && IR[3]<50){
        enable_motor();
        set_left();
        delay_ms(25);//20
        go_straight();//f
        delay_ms(45);    //f
        disable_motor();
        flag=false;
    }
    else if(IR[0]<50 && IR[1]<50 && IR[2]>=50 && IR[3]<50){
        enable_motor();
        set_right();
        delay_ms(25);//20
        go_straight();//f
        delay_ms(45);    //f
        disable_motor();
        flag=false;
    }
    else if( IR[0]>=50 && IR[1]<50 && IR[2]<50 && IR[3]<50){
        enable_motor();
        set_left();
        delay_ms(60);//20
        go_straight();//f
        delay_ms(35);    //f
        disable_motor();
        flag=false;
    }
    else if( IR[0]<50 && IR[1]<50 && IR[2]<50 && IR[3]>=50){
        enable_motor();
        set_right();
        delay_ms(60);//20
        go_straight();//f
        delay_ms(35);    //f
        disable_motor();
        flag=false;
    }
    else if(IR[0]<50 && IR[1]<50 && IR[2]<50 && IR[3]<50){
        lcd_gotoxy(3,1);
        lcd_puts("Dead LOCK");
        enable_motor();
        set_right();
        move(650);
        go_straight();
        lcd_clear();
        //move(80);
        cnt =0;
        do{
            set_right();
            move(60); //50
            cnt++;
            if(cnt == 9){
                cnt =0;
                go_straight();
                move(70);
            }
           // go_straight();
           // move(50);//20

            get_IR();
            show_IR();
      }while(check_straight() == false);
      disable_motor();
    }
    else if(IR[0]>=50 && IR[1]>=50 && IR[2]<50 && IR[3]>=50) {
        enable_motor();
        set_left();
        move(70);
        disable_motor();
    }

    else if(IR[0]>=50 && IR[1]<50 && IR[2]>=50 && IR[3]>=50) {
        enable_motor();
        set_right();
        move(70);
        disable_motor();
    }
}
// ***** CHECK FUNCTIONS *****

bool check_straight(){
    if (IR[1] >= 50 &&  IR[2] >= 50 && IR[0] < 50 && IR[3] <50)return true ;
    else    return false;
}
bool check_2_r(){
    if(IR[0]<50 && IR[1]>=50 && IR[2]>=50 && IR[3]>=50){
            return true;
    }
    else return false;
}
bool check_2_l(){
    if(IR[0]>=50 && IR[1]>=50 && IR[2]>=50 && IR[3]<50){
            return true;
    }
    else return false;
}
bool check_3(){
    if(IR[0]>=50 && IR[1]>=50 && IR[2]>=50 && IR[3]>=50) return true;
    else return false;
}