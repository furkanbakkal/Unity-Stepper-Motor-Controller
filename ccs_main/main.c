#include <main.h>
#include <string.h>
#include <flex_lcd420.c>
#INCLUDE <stdlib.h>
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)


#define A1 PIN_D2
#define A2 PIN_B1
#define B1 PIN_B0
#define B2 PIN_C5

#define button0  PIN_D5 // select adc or pot
#define button1  PIN_D1 //right
#define button2  PIN_D0 //left
#define button3  PIN_C3 //mid
#define button4  PIN_C2 //mode
#define button5  PIN_C1 //mode

#define proxy_right PIN_D6
#define proxy_left PIN_D7

#define one_dot  0
#define two_dot 1
#define three_dot 2
#define blank 3
#define m1 4
#define m2 5
#define m3 6

int delay_s=5;
int accel=6;
int accel_value=30;
int c=0;

char mode_auto=False;
char mode_manuel=True;
char flag=TRUE;

char data;
char str[8];
int counter=0;
char correct= False;
char quit1=False;
char quit2=False;

char proxy=False;
int16 centercounter=0;

TICK_TYPE StartTick,FinishTick,CurrentTick;


#int_rda

void serial_comm()
{
   //lcd_putc ("data received: ") ;
   data = getc () ;
   //printf(data);
   //lcd_putc (data) ;
   str[counter] = data;
   //lcd_putc (str[counter]) ;
   counter++;
   //lcd_putc (str[counter]) ;

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
      printf("data: %s",data);
      //delay_ms (500);
    //  lcd_putc ('\f');
   }
   
disable_interrupts (int_rda) ;
   
}

void delay_func(loop){

delay_cycles(200);delay_cycles(200);delay_cycles(200);
for(int i=0;i<10-loop;i++){
   delay_cycles(150);
}
}

void normal(loop)
{

//HALF STEPPING
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
 


/*
//ONE STEPPING  
   output_high (A1) ;
   output_low (A2) ;
   output_low (B1) ;
   output_low (B2) ;
   delay_ms (delay) ;
   
   output_low (A1) ;
   output_low (A2) ;
   output_high (B1) ;
   output_low (B2) ;
   delay_ms (delay) ;
   
   output_low (A1) ;
   output_high (A2) ;
   output_low (B1) ;
   output_low (B2) ;
   delay_ms (delay) ;
   
   output_low (A1) ;
   output_low (A2) ;
   output_low (B1) ;
   output_high (B2) ;
   delay_ms (delay) ;


*/
/*
//TWO STEPPING

output_high (A1) ;
   output_low (A2) ;
   output_high (B1) ;
   output_low (B2) ;
   delay_ms (delay) ;
   
   output_low (A1) ;
   output_high (A2) ;
   output_high (B1) ;
   output_low (B2) ;
   delay_ms (delay) ;
   
   output_low (A1) ;
   output_high (A2) ;
   output_low (B1) ;
   output_high (B2) ;
   delay_ms (delay) ;
   
   output_high (A1) ;
   output_low (A2) ;
   output_low (B1) ;
   output_high (B2) ;
   delay_ms (delay) ;
   */

}

void reverse(loop)
{
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
}

void stop()
{
   output_low (A1);
   output_low (A2);
   output_low (B1);
   output_low (B2);
   delay_ms (1);
}

char check_right_proxy(){

   if (input(proxy_right)==1) {
        proxy=True;
      }
       
   else {
      proxy=False;
   }
   return proxy;
}

char check_left_proxy(){

   if (input(proxy_left)==1) {
        proxy=True;
      }
       
   else {
      proxy=False;
   }
   return proxy;
}
void acc_start_normal_man(){

    
}

float read_serial_speed() 
{
   if (str[0] == 's')
   {
      correct = False;
      
      if (str[1] == '9')
      {
         delay_s = 5;
      }

      
      if (str[1] == '8')
      {
         delay_s = 5;
      }

      
      if (str[1] == '7')
      {
         delay_s = 6;
      }

      
      if (str[1] == '6')
      {
         delay_s = 7;
      }

      
      if (str[1] == '5')
      {
         delay_s = 8;
      }

      
      if (str[1] == '4')
      {
         delay_s = 9;
      }

      
      if (str[1] == '3')
      {
         delay_s = 10;
      }

      
      if (str[1] == '2')
      {
         delay_s = 11;
      }

      
      if (str[1] == '1')
      {
         delay_s = 12;
      }
   }

   return delay_s;
}

void centerline(delay){
  centercounter=0;
  lcd_gotoxy(1,4);
  lcd_putc("      Homing...   ");
         
  while (1){
   
    
    if (check_left_proxy()){
      stop();
      output_toggle (PIN_D4);
      correct = False;
      
      break;
    }
    
    if(input (button1) == 1 | input (button3) == 1){
      stop();
      return;
      }
   
      reverse(delay);
   }
   
   StartTick = get_ticks();
   
   
   
   while (1){

      if (check_right_proxy()){
         stop();
         correct = False;
         break;
      }
      
      if(input (button1) == 1 | input (button3) == 1){
      stop();
      return;
      }
      
      
      centercounter++;
      normal(delay);
      
      //printf("data: %s", centercounter);
     }
     
    FinishTick = get_ticks();
    CurrentTick = get_ticks();
    
   while (CurrentTick-FinishTick <= (FinishTick-StartTick)/2 ){
    CurrentTick=get_ticks();
    
     if(input (button1) == 1 | input (button3) == 1){
      stop();
      return;
      }
      
    reverse(delay);
   }
   

   stop();
   lcd_gotoxy(1,4);
   lcd_putc("                       ");
   return;
   
   
}

void gogo(){

char start=True;

output_toggle (PIN_D4);

while(1){

    while (1){
    
       while(start==True){
       c=abs(delay_s-(10-accel));
         for(int i=0;i<10-accel;i++){
            for(int j=0;j<accel_value;j++){
            
               if(input (button1) == 1 | input (button3) == 1 | input (button4) == 1){
                  stop();
                  return;
                  }
                  
                 if (check_left_proxy()){
                 start=True;
                  stop();
                  break;
                 }
                     
               reverse(c);
               }
            c=c+1;  
            }
            start=False;
         }
         
         
       if (check_left_proxy()){
         stop();
         output_toggle (PIN_D4);
         start=True;
         correct = False;     
         break;
       }
       
       if (input (button5) == 1 | input (button1) == 1 | input (button3) == 1)
      {   
         stop();
         return;
      }
      
         reverse(delay_s);
      }
      
      
      while (1){
      
      while(start==True){
       c=abs(delay_s-(10-accel));
         for(int i=0;i<10-accel;i++){
         
              
            for(int j=0;j<accel_value;j++){
            
               if(input (button1) == 1){
                  stop();
                  return;
                  }
                  
                if(input (button3) == 1){
                stop();
                return;
                  }
                  
                 if (check_right_proxy()){
                 start=True;
                  stop();
                  break;
                 }
                     
               normal(c);
               }
            c=c+1;  
            }
            start=False;
         }
         
      if (check_right_proxy()){
         stop();
         start=True;
         correct = False;
         break;
      }
      
       if (input (button5) == 1 | input (button1) == 1 | input (button3) == 1)
      {
         stop();
         return;
      }
      
      normal(delay_s);
      
      
     }
     
}
}

////////////////////////////////////////

const int8 lcd_custom_chars[] =
{
// Char Number 0 -- ONE
  0x00,
  0x00,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x00,
  
 // Char Number 1 -- TWO
  0x00,
  0x00,
  0x14,
  0x14,
  0x14,
  0x14,
  0x14,
  0x00,
  
  // Char Number 2 -- THREE
  0x00,
  0x00,
  0x15,
  0x15,
  0x15,
  0x15,
  0x15,
  0x00,

// Char Number 3 -- BLANK
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  
  // Char Number 4 -- M1
  0x01,
  0x01,
  0x01,
  0x01,
  0x01,
  0x01,
  0x01,
  0x01,
  
   // Char Number 5 -- M2
  0x1F,
  0x05,
  0x0A,
  0x0A,
  0x0E,
  0x0E,
  0x0E,
  0x1F,
  
   // Char Number 6 -- M3
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10
};
void lcd_load_custom_chars()
{
int8 i;

// Set address counter pointing to CGRAM address 0.
lcd_send_byte(0, 0x40); 

// Load custom lcd character data into CGRAM.
// It can only hold a maximum of 8 custom characters.
for(i = 0; i < sizeof(lcd_custom_chars); i++)
   {
    lcd_send_byte(1, lcd_custom_chars[i]);
   }

// Set address counter pointing back to the DDRAM.
lcd_send_byte(0, 0x80);
}

void main()
{
   setup_adc_ports (AN0, VSS_VDD);
   setup_adc (ADC_CLOCK_INTERNAL|ADC_TAD_MUL_0);
   
   setup_CCP1 (CCP_OFF);
   enable_interrupts (GLOBAL);
   
   lcd_init();

   lcd_load_custom_chars(); 

  
   
   FinishTick = StartTick = CurrentTick = get_ticks(); //timers for centering
   
   while (TRUE)
   {
      enable_interrupts (int_rda);
      output_toggle (PIN_D4);
       
      lcd_gotoxy(1,1);
      
      lcd_gotoxy(1,1);
      lcd_putc("Speed: %");
      printf(lcd_putc,"%d",delay_s*10);
      
      lcd_gotoxy(13,1);
      if (mode_manuel==True){
      lcd_putc("<Manuel>");
      }
      
      if (mode_auto==True){
      lcd_putc("< Auto >");
      }
      
      lcd_gotoxy(1,2);
      lcd_putc("Accel: %");
      printf(lcd_putc,"%d",accel*10);
     
      
///////////////////>>>>>> MODE CHANGE  <<<<<//////////////////////

      if (input (button5) == 1)
      {
         while(input (button5) == 1);
         int mode_cursor=1;
         char mod_flag=False;
         
         StartTick=get_ticks();
         delay_ms(500);
         while(1){
         
          if(mode_cursor==1){ //SET SPEED
            
            CurrentTick=get_ticks();
            
            if(CurrentTick-StartTick>2500 & mod_flag==True){
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("%");
            printf(lcd_putc,"%d",delay_s*10);
            
            mod_flag=False;
            StartTick=CurrentTick;
            }
            
            if(CurrentTick-StartTick>2500 & mod_flag==False){
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("    ");
            
            mod_flag=True;
            StartTick=CurrentTick;
            }
            
            if(input(button1)==1){
            delay_s--;
            
            if(delay_s<=0){
               delay_s=1;
            }
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("%");
            printf(lcd_putc,"%d ",delay_s*10);
            
            delay_ms(500);
            
            }
            
            if(input(button3)==1){
            delay_s++;
            
            if(delay_s>=10){
               delay_s=10;
            }
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("%");
            printf(lcd_putc,"%d ",delay_s*10);
            
            delay_ms(500);
            
            }  
         }
 
 ////////////////////////
 
         if(mode_cursor==2){ // SET ACCEL
            CurrentTick=get_ticks();
            
            if(CurrentTick-StartTick>2500 & mod_flag==True){
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("%");
            printf(lcd_putc,"%d",accel*10);
            
            mod_flag=False;
            StartTick=CurrentTick;
            }
            
            if(CurrentTick-StartTick>2500 & mod_flag==False){
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("    ");
            
            mod_flag=True;
            StartTick=CurrentTick;
            }
            
              if(input(button1)==1){
               accel--;
               
               if(accel<=0){
                  accel=1;
               }
               
               lcd_gotoxy(8,mode_cursor);
               lcd_putc("%");
               printf(lcd_putc,"%d ",accel*10);
               
               delay_ms(500);
               
               }
            
            if(input(button3)==1){
               accel++;
               
               if(accel>=11){
                  accel=10;
               }
               
               lcd_gotoxy(8,mode_cursor);
               lcd_putc("%");
               printf(lcd_putc,"%d ",accel*10);
            
               delay_ms(500);
            
            } 
         }
         
         ////////////////////////
         
         if(input(button4)==1){
         
         lcd_gotoxy(8,1);
         lcd_putc("%");
         printf(lcd_putc,"%d",delay_s*10);
         
         lcd_gotoxy(8,2);
         lcd_putc("%");
         printf(lcd_putc,"%d",accel*10);
         
         mode_cursor--;
         
         if (mode_cursor<=0){
         mode_cursor=2;
         }
         delay_ms(500);
         }
         
         if(input(button2)==1){
                
         lcd_gotoxy(8,1);
         lcd_putc("%");
         printf(lcd_putc,"%d",delay_s*10);
         
         lcd_gotoxy(8,2);
         lcd_putc("%");
         printf(lcd_putc,"%d",accel*10);
               
          mode_cursor++;
          
         if (mode_cursor>=3){
         mode_cursor=1;
         }
         delay_ms(500);
         }  
         
         if (input (button5) == 1){
          delay_ms(500);
         break;
         }
         
      }   
      }
      

////////////////>>>>>>>>> SERIAL COMM <<<<<<<<///////////   
      if (correct == True){ // full data received
         read_serial_speed();
        
         if (str[0] == 'r'){
            if (str[1] == '0'){ //r0
            
               output_toggle (PIN_D4);
 
               for (int i=0; i<50;i++ )
               {
                  enable_interrupts (int_rda);
                  normal (read_serial_speed ());
                  
                  if (check_right_proxy()){
                     stop();
                     correct = False;
                     break;
                  }
                 
               }
               output_toggle (PIN_D4);
               stop();
               correct = False;
            }
         

            if (str[1] == '1'){ //r1
               output_toggle (PIN_D4);
 
               while ((str[0] == 'r' & str[1] == '1') | str[0] == 's' )
               {
                  enable_interrupts (int_rda);
                  normal (read_serial_speed () );
                  
                  if (check_right_proxy()){
                     stop();
                     correct = False;
                     break;
                  }
                  
                  if(str[0]!='s' & kbhit()){
                     output_toggle (PIN_D4);
                     stop();
                     correct = False;
                     break;
                  }           
               }  
            }
         }
///////////// ///////// ///////////////

         if (str[0] == 'l'){
            if (str[1] == '0'){ //l0
               output_toggle (PIN_D4);
 
               for (int i=0; i<50;i++ )
               {
                  enable_interrupts (int_rda);
                  reverse (read_serial_speed () );
                  
                  if (check_left_proxy()){
                     stop();
                     correct = False;
                     break;
                  }
                 
               }
               output_toggle (PIN_D4);
               stop();
               correct = False;
            }
         
            if (str[1] == '1'){ //l1
               output_toggle (PIN_D4);
 
               while ((str[0] == 'l' & str[1] == '1') | str[0] == 's' )
               {
                  enable_interrupts (int_rda);
                  reverse (read_serial_speed () );
                  
                  if (check_left_proxy()){
                     correct = False;
                     stop();
                     break;
                  }
                  
                  if(str[0]!='s' & kbhit()){
                     output_toggle (PIN_D4);
                     stop();
                     correct = False;
                     break;
                  }           
               }  
            }
            
         }
     }     
         

///////////>>>>>>> SELECT MANUEL or AUTO <<<<<<<<<///////
      while (input (button0)== 1)  //select auto
      {  
         char start=True;
         char q=False;
         delay_ms (500);
         
         if (flag == True)
         {
            flag = False;
            mode_AUTO = True;
            mode_manuel = False;
            
            lcd_gotoxy(13,1);
            lcd_putc("< Auto >");
            
            while (1) //start with normal() in auto
            {  
               
               while (start==True){
               c=abs(delay_s-(10-accel));
               for(int i=0;i<10-accel;i++){
               
                   if (q==True){
                     break;
                     }
                    
                  for(int j=0;j<accel_value;j++){
                  
                     if(input (button1) == 1 | input (button3) == 1){
                        stop();
                        q=True;
                        break;
                        }
                        
                       if (check_right_proxy()){
                        stop();
                        q=True;
                        break;
                       }
                           
                     normal(c);
                     }
                  c=c+1;  
                  }
                  start=False;
               }
         
               if (check_right_proxy()){
               stop();
               correct = False;
               break;
               }
                 
               normal (delay_s);
    
               
               if (input (button2) == 1 |input (button3) == 1 | input (button3) == 1 | input (button4) == 1)
               {
                  stop ();
                  break;
               }

            }

            
            break;
         }

         
         if (flag == FALSE)
         {
            flag = TRUE;
            mode_AUTO = FALSE;
            mode_manuel = TRUE;
            break;
         }
}
         
      

////////////////>>>>>> MANUEL <<<<< ////////////////////
char start=True;
char q=False;

      while (input (button1) == 1  & mode_manuel == TRUE)
      {
         if (check_right_proxy()){
               stop();
               correct = False;
               break;
          }
         
        while (start==True){
         c=abs(delay_s-(10-accel));
         for(int i=0;i<10-accel;i++){
         
             if (q==True){
               break;
               }
              
            for(int j=0;j<accel_value;j++){
            
               if(input (button1) == 0 | input(button3) == 1){
                  stop();
                  q=True;
                  break;
                  }
                  
                 if (check_right_proxy()){
                  stop();
                  q=True;
                  break;
                 }
                     
               normal(c);
               }
            c=c+1;  
            }
            start=False;
         }  
         
         if(input (button3) == 1){
         stop();
         break;
         }
         
         
         
          if (q==True){
            q=!q;
            break;
            }

         normal (delay_s);
         quit1=True;
      }
         
      while (input (button3) == 1 & mode_manuel == TRUE)
      {
         if (check_left_proxy()){
               stop();
               correct = False;
               break;
          }
          
         while (start==True){
         c=abs(delay_s-(10-accel));
         for(int i=0;i<10-accel;i++){   
            
            if (q==True){
            break;
            }
            
            for(int j=0;j<accel_value;j++){
            
               if(input (button3) == 0){
                  stop();
                  q=True;
                  break;
                  }
                  
                if(input (button1) == 1){
                   stop();
                  q=True;
                  break;
                  }
                  
                 if (check_left_proxy()){
                  stop();
                  q=True;
                  break;
                 }
                  
               reverse(c);
               }
         c=c+1;  
         }
         start=False;
         }
         
         if (input (button1) == 1){
         stop();
         break;
         }
         
         if (q==True){
            q=!q;
            break;
            }
        
         reverse (delay_s);
         quit2=True;
      }
      
      while(quit2==True){
         for(int i=0;i<10-accel;i++){                
            for(int j=0;j<accel_value;j++){        
               reverse(c);
               }
               c=c-1;  
  
               if(input (button1) == 1){
               stop();
               break;
               }
               
               if (check_left_proxy()){
                  stop();
                  break;
                 }
         }
         quit2=False;
         stop();
         }
       
      while(quit1==True){
         for(int i=0;i<10-accel;i++){                
            for(int j=0;j<accel_value;j++){        
               normal(c);
               }
               c=c-1;  
            
               if(input (button3) == 1){
               stop();
               break;
               }
               
               if (check_right_proxy()){
                  stop();
                  break;
                 }
         }
         quit1=False;
         stop();
         }
         
      stop();
      

////////////////////>>>>>> AUTO <<<<</////////////////////

      if (input (button1) == 1 & mode_auto == TRUE)
      {
      start=True;
      q=False;
      delay_ms(500);
      
         while (TRUE)
         {  
         while (start==True){
            c=abs(delay_s-(10-accel));
            for(int i=0;i<10-accel;i++){
            
                if (q==True){
                  break;
                  }
                 
               for(int j=0;j<accel_value;j++){
               
                  if(input (button1) == 1 | input (button3) == 1){
                     stop();
                     q=True;
                     break;
                     }
                     
                    if (check_right_proxy()){
                     stop();
                     q=True;
                     break;
                    }
                        
                  normal(c);
                  }
               c=c+1;  
               }
               start=False;
            }
         
            if (check_right_proxy()){
               stop();
               correct = False;
               break;
          }
          
            normal (delay_s);
            
            if (input (button2) == 1|input (button3) == 1|input (button4) == 1)
            {
               break;
            }
         }
      }

      if (input (button3) == 1 & mode_auto == TRUE)
      {
         start=True;
         q=False;
         delay_ms(500);
         
      
         while (TRUE)
         {  
            
            while (start==True){
            c=abs(delay_s-(10-accel));
            for(int i=0;i<10-accel;i++){
            
                if (q==True){
                  break;
                  }
                 
               for(int j=0;j<accel_value;j++){
               
                  if(input (button1) == 1 | input (button3) == 1){
                     stop();
                     q=True;
                     break;
                     }
                     
                    if (check_left_proxy()){
                     stop();
                     q=True;
                     break;
                    }
                        
                  reverse(c);
                  }
               c=c+1;  
               }
               start=False;
            }
            
         
            if (check_left_proxy()){
               stop();
               correct = False;              
               break;
          }
            reverse (delay_s);
            
            if (input (button2) == 1|input (button1) == 1|input (button4) == 1)
            {
               break;
            }
         }
      }

////////////////////>>>>>> CENTER <<<<<//////////////////////
      if (input (button2) == 1)
      {  
      delay_ms (500);
         centerline(6);
         
      }
  
     
////////////////////>>>>>> GOGO <<<<<//////////////////////

       if (input (button4) == 1)
      {
         delay_ms (500);
         gogo();
         
      }
      
      
   } 
   
 }


