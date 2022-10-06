#include <main.h>
#include <string.h>
#include <flex_lcd420.c>
#include <internal_eeprom.c>

#use rs232(baud = 9600, parity = N, xmit = PIN_C6, rcv = PIN_C7, bits = 8, stream = PORT1)

#define A1 PIN_C2
#define A2 PIN_C3
#define B1 PIN_C0
#define B2 PIN_C1

#define button0 PIN_D1 // AUTO MAN
#define button1 PIN_D6 // L
#define button2 PIN_D4 // D
#define button3 PIN_D5 // R
#define button4 PIN_D3 // U
#define button5 PIN_D7 // SET MODE
#define button6 PIN_D2 // CENTER

#define proxy_right PIN_C5
#define proxy_left PIN_C4

#define three_dot 0
#define swon 1
#define swoff 2

int8 delay_s = 5;
int speed = 0;
int8 accel = 5;
int accel_value = 30;
int c = 0;

char mode_auto = False;
char mode_manuel = True;
char flag = TRUE;

char data;
char str[4];
int counter = 0;
char correct = False;
char quit1 = False;
char quit2 = False;

char proxy = False;
int control_left = 1;
int control_right = 1;

char centered = False;
unsigned int16 centercounter = 65535;
int8 cc1, cc2;
char center_flag = False;

unsigned int16 step = 350;
int8 step1, step2;
int line1 = 0;
int linecheck = 0;
int8 threshold = 0;
int comm_counter = threshold - 1;
unsigned int16 last_centercounter=65535;

char start = True;
char q = False;
char return_value = True;
char check=False;

int dir = 0;
char array1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
int8 array2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
TICK_TYPE StartTick, FinishTick, CurrentTick;

void delay_func(loop)
{

    // delay_cycles(200);delay_cycles(200);delay_cycles(200);delay_cycles(200);delay_cycles(200);delay_cycles(250);delay_cycles(200);delay_cycles(200);
    delay_us(330);
    for (int i = 0; i < 10 - loop; i++)
    {
        // delay_cycles(200);
        delay_us(40);
    }
}

void stop()
{  
    disable_interrupts(INT_TIMER1);
    output_low(A1);
    output_low(A2);
    output_low(B1);
    output_low(B2);
}

char check_right_proxy()
{

    if (!(input(proxy_right)))
    {
        proxy = True;

        if (!(control_right))
        {
            lcd_gotoxy(1, 4);
            lcd_putc(swon);
            //delay_ms(1);
            //printf("y");

            lcd_gotoxy(2, 4);
            lcd_putc(" ");

            control_right = True;
        }
    }

    else
    {

        proxy = False;

        if (control_left)
        {
            lcd_gotoxy(20, 4);
            lcd_putc(swoff);
            control_left = False;
        }
    }
    return proxy;
}

char check_left_proxy()
{

    if (!(input(proxy_left)))
    {
        proxy = True;
        step = centercounter;
        
        if (!(control_left))
        {

        lcd_gotoxy(20, 4);
        lcd_putc(swon);
        //delay_ms(1);
        //printf("x");
        control_left = True;

    }
    }

    else
    {

        proxy = False;

        if (control_right)
        {
            lcd_gotoxy(1, 4);
            lcd_putc(swoff);
            control_right = False;
        }
    }

    return proxy;
}

void distance_measure()
{
    comm_counter++;

    if (step < threshold)
    {
        step = threshold - 1;
    }

    if (comm_counter == 20)
    { 
        int8 step_data = step / 10;
        //printf("q%d", step_data);
    }

    if (comm_counter == threshold | step <= threshold) //centered &
    { 
        comm_counter = 0;
        line1 = (step / threshold);

        if (line1 > 18)
        {
            line1 = 19;
        }

        if (line1 < 2)
        {
            line1 = 2;
        }

        if (line1 < linecheck)
        {

            if (line1 == 18)
            {
                lcd_gotoxy(19, 4);
                lcd_putc(" ");
                linecheck = line1;
                return;
            }

            if (line1 == 1)
            {
                lcd_gotoxy(2, 4);
                lcd_putc(" ");
                linecheck = line1;
                return;
            }

            lcd_gotoxy(line1, 4);
            lcd_putc("  ");
        }

        lcd_gotoxy(line1, 4);
        lcd_putc(three_dot);

        linecheck = line1;
    }
}

void normal()
{
    dir = 1;
    enable_interrupts(INT_TIMER1);
    distance_measure();
    disable_interrupts(INT_TIMER1);

    /*
          output_high (A1) ;
          output_low (A2) ;
          output_low (B1) ;
          output_low (B2) ;
          delay_func (loop) ;

          output_high (A1) ;
          output_low (A2) ;
          output_high (B1) ;
          output_low (B2) ;
          delay_func (loop) ;

          output_low (A1) ;
          output_low (A2) ;
          output_high (B1) ;
          output_low (B2) ;
          delay_func (loop) ;

          output_low (A1) ;
          output_high (A2) ;
          output_high (B1) ;
          output_low (B2) ;
          delay_func (loop) ;

          output_low (A1) ;
          output_high (A2) ;
          output_low (B1) ;
          output_low (B2) ;
          delay_func (loop) ;

          output_low (A1) ;
          output_high (A2) ;
          output_low (B1) ;
          output_high (B2) ;
          delay_func (loop) ;

          output_low (A1) ;
          output_low (A2) ;
          output_low (B1) ;
          output_high (B2) ;
          delay_func (loop) ;

          output_high (A1) ;
          output_low (A2) ;
          output_low (B1) ;
          output_high (B2) ;
          delay_func (loop) ;

          if(center_flag==True){
          centercounter=centercounter+1;
          }

          step--;

      */
}

void reverse()
{
    dir = 0;
    enable_interrupts(INT_TIMER1);
    distance_measure();
    disable_interrupts(INT_TIMER1);

    /*
       output_high (A1);
       output_low (A2);
       output_low (B1);
       output_high (B2);
       delay_func (loop) ;

       output_low (A1);
       output_low (A2);
       output_low (B1);
       output_high (B2);
       delay_func (loop) ;

       output_low (A1);
       output_high (A2);
       output_low (B1);
       output_high (B2);
       delay_func (loop) ;

       output_low (A1);
       output_high (A2);
       output_low (B1);
       output_low (B2);
       delay_func (loop) ;

       output_low (A1);
       output_high (A2);
       output_high (B1);
       output_low (B2);
       delay_func (loop) ;

       output_low (A1);
       output_low (A2);
       output_high (B1);
       output_low (B2);
       delay_func (loop) ;

       output_high (A1);
       output_low (A2);
       output_high (B1);
       output_low (B2);
       delay_func (loop) ;

       output_high (A1);
       output_low (A2);
       output_low (B1);
       output_low (B2);
       delay_func (loop) ;

       step++;
    */
}

char acc_start_normal_man()
{
    output_high(pin_d4);
    while (start)
    {   
      enable_interrupts(INT_RDA);
        c = abs(delay_s - (10 - accel));
        for (int i = 0; i < 10 - accel; i++)
        {   
            enable_interrupts(INT_RDA);
            for (int j = 0; j < accel_value; j++)
            {
               enable_interrupts(INT_RDA);
                if ( !(input(button3)) | check_right_proxy() | (input(button1)) ) //!(input(button1)) 
                {
                    stop();
                    return return_value = False;
                }

                speed = c;
                normal();
            }
            c = c + 1;
        }
        start = False;
        return return_value = True;
    }
    output_low(pin_d4);
}

char acc_start_reverse_man()
{
    output_high(pin_d4);
    while (start)
    {
         enable_interrupts(INT_RDA);
        c = abs(delay_s - (10 - accel));
        for (int i = 0; i < 10 - accel; i++)
        {   
            enable_interrupts(INT_RDA);
            for (int j = 0; j < accel_value; j++)
            {
                enable_interrupts(INT_RDA);
                if ( !(input(button1))  | check_left_proxy() | (input(button3)) ) //!(input(button3))
                {
                    stop();
                    return return_value = False;
                }
                speed = c;
                reverse();
            }
            c = c + 1;
        }
        start = False;
        return return_value = True;
    }
    output_low(pin_d4);
}

char acc_start_normal_auto()
{
    while (start)
    {
        enable_interrupts(INT_RDA);
        c = abs(delay_s - (10 - accel));
        for (int i = 0; i < 10 - accel; i++)
        {   
            enable_interrupts(INT_RDA);
            for (int j = 0; j < accel_value; j++)
            {
                enable_interrupts(INT_RDA);
                if (!(input(button1)) | !(input(button3)) | check_right_proxy() | str[0] != 'l' | str[1] != '1' )
                {
                    stop();
                    return return_value = False;
                }
                speed = c;
                normal();
            }
            c = c + 1;
        }
        start = False;
        return return_value = True;
    }
}

char acc_start_reverse_auto()
{

    while (start == True)
    {   
        enable_interrupts(INT_RDA);
        c = abs(delay_s - (10 - accel));
        for (int i = 0; i < 10 - accel; i++)
        {   
            enable_interrupts(INT_RDA);
            for (int j = 0; j < accel_value; j++)
            {
                enable_interrupts(INT_RDA);
                if (!(input(button1))  | !(input(button3)) | check_left_proxy() | str[0] != 'r' | str[1] != '1' )
                {
                    stop();
                    return return_value = False;
                }
                speed = c;
                reverse();
            }
            c = c + 1;
        }
        start = False;
        return return_value = True;
    }
}

float read_serial_speed()
{
    /*
     if (str[0] == 's'){
        output_toggle(pin_d4);

        if (str[1] == '0'){

           for(int i=0;i<=8;i++){
              if(str[2]==array1[i])
              {
                 delay_s=array2[i];
                 break;
              }
           }

        }

        if (str[1] == '1'){
           delay_s=10;
        }

     }

     return delay_s;
     */

    if (str[0] == 'h')
    {
        output_toggle(pin_d4);
        if (str[1] == '0')
        {

            if (str[2] == '1')
            {
                delay_s = 1;
            }

            if (str[2] == '2')
            {
                delay_s = 2;
            }

            if (str[2] == '3')
            {
                delay_s = 3;
            }

            if (str[2] == '4')
            {
                delay_s = 4;
            }

            if (str[2] == '5')
            {
                delay_s = 5;
            }

            if (str[2] == '6')
            {
                delay_s = 6;
            }

            if (str[2] == '7')
            {
                delay_s = 7;
            }

            if (str[2] == '8')
            {
                delay_s = 8;
            }

            if (str[2] == '9')
            {
                delay_s = 9;
            }
        }

        if (str[1] == '1')
        {
            delay_s = 10;
        }
    }
    return delay_s;
}

void read_serial_mode()
{

    if (str[0] == 'm')
    {
        output_toggle(pin_d4);

        if (str[1] == '0')
        {
            mode_AUTO = False;
            mode_manuel = True;
        }

        if (str[1] == '1')
        {
            mode_AUTO = True;
            mode_manuel = False;
        }
    }
}

float read_serial_acc()
{

    if (str[0] == 'a')
    {
        output_toggle(pin_d4);
        if (str[1] == '0')
        {

            if (str[2] == '1')
            {
                accel = 1;
            }

            if (str[2] == '2')
            {
                accel = 2;
            }

            if (str[2] == '3')
            {
                accel = 3;
            }

            if (str[2] == '4')
            {
                accel = 4;
            }

            if (str[2] == '5')
            {
                accel = 5;
            }

            if (str[2] == '6')
            {
                accel = 6;
            }

            if (str[2] == '7')
            {
                accel = 7;
            }

            if (str[2] == '8')
            {
                accel = 8;
            }

            if (str[2] == '9')
            {
                accel = 9;
            }
        }

        if (str[1] == '1')
        {
            accel = 10;
        }
    }
    return accel;
}

void centerline(delay)
{     
    lcd_gotoxy(13, 1);
    lcd_putc("<Center>");
            
    last_centercounter=0;
    lcd_gotoxy(1, 4);
    lcd_putc(swoff);
    lcd_gotoxy(20, 4);
    lcd_putc(swoff);
    lcd_gotoxy(2, 4);
    //lcd_putc("     Homing...    ");
    speed = 6;
    //centered = False;
    //centercounter = 0;

    while (1)
    {
        enable_interrupts(INT_RDA);

        if (check_left_proxy())
        {
            stop();
            delay_ms(250);
            output_toggle(PIN_D4);
            correct = False;

            break;
        }

        if (!(input(button1)) | !(input(button3)))
        {
            stop();
            lcd_gotoxy(1, 4);
            centered = False;

            return;
        }

        reverse();
    }

    while (1)
    {
        enable_interrupts(INT_RDA);

        center_flag = True;

        if (check_right_proxy())
        {
            stop();
            delay_ms(250);
            correct = False;
            step = 0;
            break;
        }

        if (!(input(button1)) | !(input(button3)))
        {
            stop();
            lcd_gotoxy(1, 4);
            centered = False; 
            return;
        }

        normal();

        // printf("data: %s", centercounter);
    }

    while (step < last_centercounter / 2)
    {
        enable_interrupts(INT_RDA);

        center_flag = False;
        CurrentTick = get_ticks();

        if (!(input(button1)) | !(input(button3)) | check_left_proxy())
        {
            stop();
            return;
        }

        reverse();
    }

    stop();
    lcd_putc("\f");

    lcd_gotoxy(1, 3);
    centercounter = last_centercounter;
    centercounter=centercounter+3;
    printf(lcd_putc, "count: %Lu", centercounter);
    step = centercounter / 2;
    centered = True;
    center_flag = False;
    threshold = (centercounter / 20);
      
    comm_counter = threshold - 1;
    
    //printf("t%d", threshold);
    //delay_ms(35);
    int8 step_data = step / 10;
    //printf("q%d", step_data);

    line1 = (step / threshold);

    lcd_gotoxy(1, 4);
    lcd_putc(swoff);
    lcd_gotoxy(20, 4);
    lcd_putc(swoff);

    for (int i = 2; i <= 10; i++)
    {
        lcd_gotoxy(i, 4);
        lcd_putc(three_dot);
    }

    return;
}

void gogo()
{
    char start = True;

    output_toggle(PIN_D4);

    while (1)
    {     
        enable_interrupts(INT_RDA);
        while (1)
        {
            enable_interrupts(INT_RDA);
            while (start == True)
            {
                enable_interrupts(INT_RDA);

                c = abs(delay_s - (10 - accel));
                for (int i = 0; i < 10 - accel; i++)
                {    

                enable_interrupts(INT_RDA);
                
                    for (int j = 0; j < accel_value; j++)
                    {

                enable_interrupts(INT_RDA);

                        if (!(input(button1)) | !(input(button3)) | !(input(button4)) | str[0]!='g' )
                        {
                            stop();
                            return;
                        }

                        if (check_left_proxy())
                        {
                            start = True;
                            stop();
                            delay_ms(100);
                            break;
                        }
                        speed = c;
                        reverse();
                    }
                    c = c + 1;
                }
                start = False;
            }

            if (check_left_proxy())
            {
                stop();
                output_toggle(PIN_D4);
                delay_ms(100);
                start = True;
                correct = False;
                break;
            }

            if (!(input(button5)) | !(input(button1))  | !(input(button3)) | str[0]!='g' )
            {
                stop();
                return;
            }
            speed = c;
            reverse();
        }

        while (1)
        {         
            enable_interrupts(INT_RDA);
            while (start == True)
            {     

                enable_interrupts(INT_RDA);

                c = abs(delay_s - (10 - accel));
                for (int i = 0; i < 10 - accel; i++)
                {    

                enable_interrupts(INT_RDA);

                    for (int j = 0; j < accel_value; j++)
                    {      

                enable_interrupts(INT_RDA);

                        if (!(input(button1)) | !(input(button3)) | str[0]!='g' )
                        {
                            stop();
                            return;
                        }

                        if (check_right_proxy())
                        {
                            start = True;
                            stop();
                            delay_ms(500);
                            break;
                        }
                        speed = c;
                        normal();
                    }
                    c = c + 1;
                }
                start = False;
            }

            if (check_right_proxy())
            {
                stop();
                delay_ms(100);
                start = True;
                correct = False;
                break;
            }

            if (!(input(button5)) | !(input(button1)) | !(input(button3)) | str[0]!='g' )
            {
                stop();
                return;
            }

            speed = c;
            normal();
        }
    }
}

void seri_check(){

enable_interrupts(INT_RDA);

if(correct){
      enable_interrupts(INT_RDA);
      
      read_serial_mode();
      read_serial_speed();
      read_serial_acc();

      if (str[0] == 'c')
      {
          centerline(6);
      }

      if (str[0] == 'g')
      {
          gogo();
      }

      if (str[0] == 'd')// usb connected, send commands to pc
      { 
          //printf("x");
          //delay_ms(50);
          //printf("h0%d", delay_s);
          //delay_ms(50);
          //printf("a0%d", accel);
          //delay_ms(50);

          if (mode_manuel)
          {
              //printf("m0");
          }

          if (mode_auto)
          {
              //printf("m1");
          }

          delay_ms(50);

          if (centered)
          {
              //printf("t%d", threshold);
          }

          //delay_ms(50);
          int8 step_data = step / 10;
          //printf("q%d", step_data);
          
          str[0]='0';
      }
}
}

#INT_RDA fast
void serial_comm()
{
    disable_interrupts(int_rda);
    // lcd_putc ("data received: ") ;
    data = getc();
    //printf(data);
    // lcd_putc (data) ;
    str[counter] = data;
    // lcd_putc (str[counter]) ;
    counter++;
    // lcd_putc (str[counter]) ;
    if (data == '!')
    {
        /*
            lcd_putc ("Data: ");
            for (int i = 0; i < counter - 1; i++)
            {
               lcd_putc (str[i]);
            }
        */
        counter = 0;
        correct = True;
        output_toggle (PIN_D4);
        // printf("OK");

        // lcd_putc ('\f');
    }
    
}

////////////////////////////////////////

const int8 lcd_custom_chars[] =
    {

        // Char Number 0 -- THREE
        0x00,
        0x00,
        0x15,
        0x15,
        0x15,
        0x15,
        0x15,
        0x00,

        // Char Number 1 -- SW on
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x1F,

        // 2  Sw off

        0x1F,
        0x11,
        0x11,
        0x11,
        0x11,
        0x11,
        0x11,
        0x1F

};
void lcd_load_custom_chars()
{
    int8 i;

    // Set address counter pointing to CGRAM address 0.
    lcd_send_byte(0, 0x40);

    // Load custom lcd character data into CGRAM.
    // It can only hold a maximum of 8 custom characters.
    for (i = 0; i < sizeof(lcd_custom_chars); i++)
    {
        lcd_send_byte(1, lcd_custom_chars[i]);
    }

    // Set address counter pointing back to the DDRAM.
    lcd_send_byte(0, 0x80);
}

#int_timer1
void Timer1_isr(void)
{
    // output_toggle(PIN_D4);

    if (dir == 0)
    {
        delay_func(speed);
        output_high(A1);
        output_low(A2);
        output_low(B1);
        output_high(B2);
        delay_func(speed);

        output_low(A1);
        output_low(A2);
        output_low(B1);
        output_high(B2);
        delay_func(speed);

        output_low(A1);
        output_high(A2);
        output_low(B1);
        output_high(B2);
        delay_func(speed);

        output_low(A1);
        output_high(A2);
        output_low(B1);
        output_low(B2);
        delay_func(speed);

        output_low(A1);
        output_high(A2);
        output_high(B1);
        output_low(B2);
        delay_func(speed);

        output_low(A1);
        output_low(A2);
        output_high(B1);
        output_low(B2);
        delay_func(speed);

        output_high(A1);
        output_low(A2);
        output_high(B1);
        output_low(B2);
        delay_func(speed);

        output_high(A1);
        output_low(A2);
        output_low(B1);
        output_low(B2);

        step++;
      
        if (step > centercounter)
        {
            step = centercounter;
        }
        
    }
    if (dir == 1)
    {
        delay_func(speed);
        output_high(A1);
        output_low(A2);
        output_low(B1);
        output_low(B2);
        delay_func(speed);

        output_high(A1);
        output_low(A2);
        output_high(B1);
        output_low(B2);
        delay_func(speed);

        output_low(A1);
        output_low(A2);
        output_high(B1);
        output_low(B2);
        delay_func(speed);

        output_low(A1);
        output_high(A2);
        output_high(B1);
        output_low(B2);
        delay_func(speed);

        output_low(A1);
        output_high(A2);
        output_low(B1);
        output_low(B2);
        delay_func(speed);

        output_low(A1);
        output_high(A2);
        output_low(B1);
        output_high(B2);
        delay_func(speed);

        output_low(A1);
        output_low(A2);
        output_low(B1);
        output_high(B2);
        delay_func(speed);

        output_high(A1);
        output_low(A2);
        output_low(B1);
        output_high(B2);

        step--;

        if (center_flag)
        {
            last_centercounter++;
        }
    }

    set_timer1(65486);
    //restart_wdt();
    clear_interrupt(INT_TIMER1);
    // disable_interrupts(INT_TIMER1);
}

#INT_HLVD
void LOWVOLT_isr(void)
{
    disable_interrupts(INT_TIMER1);
    disable_interrupts(INT_RDA);

    delay_ms(10);
   
    write_eeprom(1, delay_s);  // speed

    
    write_eeprom(2, accel);    // accel

    
    write_eeprom(3, centered); // centered?

    
    step1 = make8(step, 0);
    step2 = make8(step, 1);
    write_eeprom(4, step1); // pos1
 
    write_eeprom(5, step2); // pos2

    
    write_eeprom(6, threshold); // threshold

    cc1 = make8(centercounter, 0);
    cc2 = make8(centercounter, 1);
    write_eeprom(7, cc1); // cc1
    write_eeprom(8, cc2); // cc2

    write_eeprom(9, line1);

    write_eeprom(10, control_right);
    write_eeprom(11, control_left);

    output_high(pin_E0);


    disable_interrupts(INT_HLVD);
}

void main()
{

    setup_CCP1(CCP_OFF);
    clear_interrupt(INT_TIMER1);

    setup_low_volt_detect(LVD_TRIGGER_BELOW | LVD_36);
    enable_interrupts(INT_HLVD);

    enable_interrupts(GLOBAL);
    enable_interrupts(INT_RDA);
    disable_interrupts(INT_TIMER1);
    
    //setup_wdt( WDT_ON | WDT_64MS    );
    
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // Timer1 configuration: internal clock source + 8 prescaler
    set_timer1(65486);                        // 65488 --- 63350
    //restart_wdt();
    lcd_init();

    lcd_load_custom_chars();

    FinishTick = StartTick = CurrentTick = get_ticks(); // timers for centering

    if (read_eeprom(1) <= 0 | read_eeprom(1) > 10 )//
    {
         
        lcd_gotoxy(1, 3);
        lcd_putc("Eeprom Fail");
        
        write_eeprom(1, 5); // speed
        write_eeprom(2, 5); // accel
        write_eeprom(3, 0); // centered?

        write_eeprom(4, 100); // pos1
        write_eeprom(5, 100); // pos2

        write_eeprom(6, 0); // threshold

        write_eeprom(7, 0); // cc1
        write_eeprom(8, 0); // cc2

        write_eeprom(9, 0); // line1

        write_eeprom(10, 1); // check_right
        write_eeprom(11, 1); // check_left

        
    }

    delay_s = read_eeprom(1);
    accel = read_eeprom(2);
    centered = read_eeprom(3);

    control_right = read_eeprom(10);
    control_left = read_eeprom(11);

    if (centered)
    {

        step1 = read_eeprom(4);
        step2 = read_eeprom(5);

        step = make16(step2, step1);
        lcd_gotoxy(1, 3);
        printf(lcd_putc, "%lu", step);

        threshold = read_eeprom(6);
        printf(lcd_putc, " %d", threshold);

        cc1 = read_eeprom(7);
        cc2 = read_eeprom(8);
        centercounter = make16(cc2, cc1);

        // ine1=step/threshold;
        // line1++;

        line1 = read_eeprom(9);

        lcd_gotoxy(1, 4);
        lcd_putc(swoff);
        lcd_gotoxy(20, 4);
        lcd_putc(swoff);

        if (step >= threshold)
        {
            for (int i = 2; i <= line1; i++)
            {
                lcd_gotoxy(i, 4);
                lcd_putc(three_dot);
            }
        }

        if (!(input(proxy_left)))
        {

            lcd_gotoxy(1, 4);
            lcd_putc(swoff);

            lcd_gotoxy(20, 4);
            lcd_putc(swon);
        }

        if (!(input(proxy_right)))
        {

            lcd_gotoxy(1, 4);
            lcd_putc(swon);

            lcd_gotoxy(20, 4);
            lcd_putc(swoff);
        }
    }

    else
    {
        lcd_gotoxy(3, 4);
        lcd_putc("Centee Machine");
    }
    
    //////////////////////////////////////

    while (TRUE)
    {    
        //restart_wdt();
        enable_interrupts(INT_RDA);
        // output_toggle (PIN_D4);
         
        lcd_gotoxy(1, 1);
        lcd_putc("Speed: %   ");
        lcd_gotoxy(9, 1);
        printf(lcd_putc, "%d", delay_s * 10);

        lcd_gotoxy(13, 1);
        if (mode_manuel)
        {
            lcd_putc("<Manuel>");
        }

        if (mode_auto)
        {
            lcd_putc("< Auto >");
        }

        lcd_gotoxy(1, 2);
        lcd_putc("Accel: %   ");
        lcd_gotoxy(9, 2);
        printf(lcd_putc, "%d", accel * 10);
   
        if (!(centered))
        {
            lcd_gotoxy(4, 4);
            lcd_putc("Center Machine");
        }
   
        ///////////////////>>>>>> MODE CHANGE  <<<<<//////////////////////

        if (!(input(button5)))
        {
            int mode_cursor = 1;
            char mod_flag = False;
            
            StartTick = get_ticks();
            
            while (!(input(button5))){};
        
            while (1)
            {
               seri_check();
               enable_interrupts(INT_RDA);
               
               //restart_wdt();
                if (mode_cursor == 1)
                { // SET SPEED

                    CurrentTick = get_ticks();

                    if (CurrentTick - StartTick > 2500 & mod_flag == True)
                    {

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("%");
                        printf(lcd_putc, "%d", delay_s * 10);

                        mod_flag = False;
                        StartTick = CurrentTick;
                    }

                    if (CurrentTick - StartTick > 2500 & mod_flag == False)
                    {

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("    ");

                        mod_flag = True;
                        StartTick = CurrentTick;
                    }

                    if (!(input(button1)))
                    {
                        delay_s=delay_s-1;

                        if (delay_s <= 0)
                        {
                            delay_s = 1;
                        }

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("%");
                        printf(lcd_putc, "%d ", delay_s * 10);

                        //printf("h0%d ", delay_s);
                        delay_ms(500);
                    }

                    if (!(input(button3)))
                    {
                        delay_s=delay_s+1;

                        if (delay_s >= 10)
                        {
                            delay_s = 10;
                        }

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("%");
                        printf(lcd_putc, "%d ", delay_s * 10);

                        if (delay_s < 10)
                        {
                            //printf("h0%d ", delay_s);
                        }
                        else
                        {
                            //printf("h10");
                        }

                        delay_ms(500);
                    }
                }

                ////////////////////////

                if (mode_cursor == 2){ // SET ACCEL

                    CurrentTick = get_ticks();

                    if (CurrentTick - StartTick > 2500 & mod_flag == True)
                    {

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("%");
                        printf(lcd_putc, "%d", accel * 10);

                        mod_flag = False;
                        StartTick = CurrentTick;
                    }

                    if (CurrentTick - StartTick > 2500 & mod_flag == False)
                    {

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("    ");

                        mod_flag = True;
                        StartTick = CurrentTick;
                    }

                    if (!(input(button1)))
                    {
                        accel=accel-1;

                        if (accel <= 0)
                        {
                            accel = 1;
                        }

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("%");
                        printf(lcd_putc, "%d ", accel * 10);

                        //printf("a0%d ", accel);

                        delay_ms(500);
                    }

                    if (!(input(button3)))
                    {
                        accel=accel+1;

                        if (accel >= 11)
                        {
                            accel = 10;
                        }

                        lcd_gotoxy(8, mode_cursor);
                        lcd_putc("%");
                        printf(lcd_putc, "%d ", accel * 10);

                        if (accel < 10)
                        {
                            //printf("a0%d ", accel);
                        }
                        else
                        {
                            //printf("a10");
                        }

                        delay_ms(500);
                    }
                }

                ////////////////////////

                if (!(input(button4)))
                {

                    lcd_gotoxy(8, 1);
                    lcd_putc("%");
                    printf(lcd_putc, "%d", delay_s * 10);

                    lcd_gotoxy(8, 2);
                    lcd_putc("%");
                    printf(lcd_putc, "%d", accel * 10);

                    mode_cursor--;

                    if (mode_cursor <= 0)
                    {
                        mode_cursor = 2;
                    }
                    delay_ms(500);
                }

                if (!(input(button2)))
                {

                    lcd_gotoxy(8, 1);
                    lcd_putc("%");
                    printf(lcd_putc, "%d", delay_s * 10);

                    lcd_gotoxy(8, 2);
                    lcd_putc("%");
                    printf(lcd_putc, "%d", accel * 10);

                    mode_cursor++;

                    if (mode_cursor >= 3)
                    {
                        mode_cursor = 1;
                    }
                    delay_ms(500);
                }

                if (!(input(button5)))
                {
                    delay_ms(500);
                    break;
                }
            }
            
            correct=False;
        }

        //////////////>>>>>>>>> SERIAL COMM <<<<<<<<///////////

        if (correct) { // full data received
            //restart_wdt();
            read_serial_mode();
            read_serial_speed();
            read_serial_acc();

            if (str[0] == 'c')
            {
                centerline(6);
            }

            if (str[0] == 'g')
            {
                gogo();
            }

            if (str[0] == 'd')// usb connected, send commands to pc
            { 
                printf("x");
                delay_ms(50);
                //printf("h0%d", delay_s);
                delay_ms(50);
                //printf("a0%d", accel);
                delay_ms(50);

                if (mode_manuel)
                {
                    //printf("m0");
                }

                if (mode_auto)
                {
                    //printf("m1");
                }

                delay_ms(50);

                if (centered)
                {
                    //printf("t%d", threshold);
                }

                delay_ms(50);
                int8 step_data = step / 10;
                //printf("q%d", step_data);
                
                str[0]='0';
            }

        // correct=False;
        
        }

        ///////////>>>>>>> SELECT MANUEL or AUTO <<<<<<<<<///////
        while (!(input(button0))) // select auto
        {

            delay_ms(500);

            if (flag)
            {
                flag = False;
                mode_AUTO = True;
                mode_manuel = False;

                lcd_gotoxy(13, 1);
                lcd_putc("< Auto >");
                //printf("m1");

                start = True;

                while (TRUE)
                {

                    acc_start_normal_auto();

                    if (return_value == False)
                    {
                        return_value = True;
                        break;
                    }

                    normal();

                    if (!(input(button2)) | !(input(button3)) | !(input(button4)) | check_right_proxy())
                    {
                        stop();
                        break;
                    }
                }
            }

            else
            {
                flag = TRUE;
                mode_AUTO = FALSE;
                mode_manuel = TRUE;
                //printf("m0");
                break;
            }
        }

        ////////////////>>>>>> MANUEL <<<<< ////////////////////
        start = True;
        q = False;

        while (!(input(button1)) & mode_manuel | ( str[0] == 'l' & str[1] == '0' )) //left
        {   
             enable_interrupts(INT_RDA);
             
               //restart_wdt();
            if (check_right_proxy() | !(input(button3)))
            {
                stop();
                // correct = False;
                disable_interrupts(INT_TIMER1);
                break;
            }
            
            acc_start_normal_man();

            if (!(return_value))
            {
                return_value = True;
                break;
            }

            speed = delay_s;
            normal();
            quit1 = True;
        }

        while (!(input(button3)) & mode_manuel |  (str[0] == 'r' & str[1] == '0' )) //right
        {   
            //restart_wdt();
             enable_interrupts(INT_RDA);
             
             
            if (check_left_proxy() | !(input(button1)) )
            {
                stop();
                // correct = False;
                disable_interrupts(INT_TIMER1);
                break;
            }
            
             
            acc_start_reverse_man();

            if (!(return_value ))
            {
                return_value = True;
                break;
            }

            speed = delay_s;
            reverse();
            quit2 = True;
        }

        while (quit2)
        {
        //restart_wdt();
            for (int i = 0; i < 10 - accel; i++)
            {  
                enable_interrupts(INT_RDA);
                for (int j = 0; j < accel_value; j++)
                {
                    enable_interrupts(INT_RDA);
                    if (!(input(button1)) | check_left_proxy())
                    {
                        stop();
                        disable_interrupts(INT_TIMER1);
                        break;
                    }

                    speed = c;
                    reverse();
                }
                c = c - 1;
            }
            quit2 = False;
            stop();
        }

        while (quit1)
        {
        //restart_wdt();
            for (int i = 0; i < 10 - accel; i++)
            {   
                 enable_interrupts(INT_RDA);
                for (int j = 0; j < accel_value; j++)
                {
                    enable_interrupts(INT_RDA);

                    if (!(input(button3)) | check_right_proxy())
                    {
                        stop();
                        disable_interrupts(INT_TIMER1);
                        break;
                    }

                    speed = c;
                    normal();
                }
                c = c - 1;
            }
            quit1 = False;
            stop();
        }

        stop();

        ////////////////////>>>>>> AUTO <<<<</////////////////////

        if (!(input(button1)) & mode_auto | (str[0] == 'l' & str[1] == '1'))
        {
         
            delay_ms(500);
            start = True;
            str[0] = 'l'; str[1] = '1';
            
            while (TRUE)
            {
               enable_interrupts(INT_RDA);
                acc_start_normal_auto();

                if (return_value == False)
                {
                    return_value = True;
                    break;
                }

                normal();

                if (!(input(button2)) | !(input(button3)) | !(input(button4)) | check_right_proxy() | str[0] != 'l' | str[1] != '1' )
                {
                    stop();
                    str[0]="0";
                    break;
                }
            }
        }

        if (!(input(button3)) & mode_auto | (str[0] == 'r' & str[1] == '1'))
        {   
            delay_ms(500);
            start = True;
            str[0] = 'r'; str[1] = '1';

            while (TRUE)
            {
               enable_interrupts(INT_RDA);
                acc_start_reverse_auto();

                if (return_value == False)
                {
                    return_value = True;
                    break;
                }

                reverse();

                if (!(input(button2)) | !(input(button1)) | !(input(button4)) | check_left_proxy() | str[0] != 'r' | str[1] != '1')
                {
                    stop();
                    str[0]="0";
                    break;
                }
            }
        }

        ////////////////////>>>>>> CENTER <<<<<//////////////////////
        if (!(input(button6)))
        {
            delay_ms(500);
            centerline(6);
        }

        ////////////////////>>>>>> GOGO <<<<<//////////////////////

        if (!(input(button4)))
        {
            delay_ms(500);
            str[0]='g';
            gogo();
        }
    }
}

