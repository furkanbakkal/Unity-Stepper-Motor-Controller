#include <main.h>
#include <string.h>
#include <flex_lcd420.c>
#include <internal_eeprom.c>

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

#define three_dot 0
#define swon 1
#define swoff 2


int8 delay_s=5;
int speed=0;
int8 accel=6;
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
int control_left=1;
int control_right=1;

char centered=False;
unsigned int16 centercounter=65535;
int8 cc1,cc2;
char center_flag=False;

unsigned int16 step=350;
int8 step1,step2;
int line1=0;
int linecheck=0;
int8 threshold=0;

char start=True;
char q=False;
char return_value=True;

int dir=0;
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

delay_cycles(200);delay_cycles(200);delay_cycles(200);delay_cycles(200);delay_cycles(200);delay_cycles(250);delay_cycles(200);delay_cycles(200);
for(int i=0;i<10-loop;i++){
   delay_cycles(200);
}
}

void stop(){
   output_low (A1);
   output_low (A2);
   output_low (B1);
   output_low (B2);
   
}

char check_right_proxy(){

   if (input(proxy_right)){
        proxy=True;
         
          if(!(control_right)){
            lcd_gotoxy(1,4);
            lcd_putc(swon); 
            
            lcd_gotoxy(2,4);
            lcd_putc(" ");
            
            control_right=True;
        }
        
      }
      
   else {
   
      proxy=False;
      
      if(control_left){
         lcd_gotoxy(20,4);
         lcd_putc(swoff);
         control_left=False;
      }
   }
   return proxy;
}

char check_left_proxy(){

   if (input(proxy_left)) {
        proxy=True;
        step=centercounter;
         
        //if(!(control_left)){
            lcd_gotoxy(20,4);
            lcd_putc(swon); 
            control_left=True;
        //}
        
      }
       
   else {
   
      proxy=False;
      
      if(control_right){
         lcd_gotoxy(1,4);
         lcd_putc(swoff);
         control_right=False;
      }
     
   }
   
   return proxy;
}

void distance_measure(){

if(step<threshold){
   step=threshold-1;
}
/*
   if(!centered){
      delay_us(20);
   }
*/
   if(centered & step % threshold ==0 | step<=threshold){
   
     line1=(step/threshold);
     
     if(line1>18){
        line1=19;
     }
    
     if(line1<2) {
        line1=2;
     }
   /*
     if(step<threshold){
        step=1;
        lcd_gotoxy(2,4);
        lcd_putc(" "); 
        
        line1=2;
        linecheck=line1;
      
        return;
     }
 */   
     if (line1<linecheck){
      
        if(line1==18){
           lcd_gotoxy(19,4);
           lcd_putc(" "); 
           linecheck=line1;   
           return;
        }
        
        if(line1==1){
           lcd_gotoxy(2,4);
           lcd_putc(" "); 
           linecheck=line1;
           return;
        }
        
        lcd_gotoxy(line1+1,4);
        lcd_putc("  "); 

     }
   
     lcd_gotoxy(line1,4);
     lcd_putc(three_dot);  
     
     linecheck=line1;
     
     }
     

}

void normal(){
dir=1;
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

void reverse(){
dir=0;
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

char acc_start_normal_man(){
 output_high(pin_d4);  
 while (start==True){
   c=abs(delay_s-(10-accel));
   for(int i=0;i<10-accel;i++){      
      for(int j=0;j<accel_value;j++)
      {
         if(!(input (button1)) | input(button3) | check_right_proxy()){
            stop();
            return return_value=False;
            }
            
         speed=c;
         normal();
         
 
         }
      c=c+1;  
      }
      start=False;
      return return_value=True;
   }  
  output_low(pin_d4);         
}

char acc_start_reverse_man(){
output_high(pin_d4); 
 while (start==True){
   c=abs(delay_s-(10-accel));
   for(int i=0;i<10-accel;i++){   
      for(int j=0;j<accel_value;j++){
      
         if(!(input (button3)) | input (button1)  | check_left_proxy()){
            stop();
            return return_value=False;
            }
         speed=c;
         reverse();
         

         }
   c=c+1;  
   }
   start=False;
   return return_value=True;
   }
 output_low(pin_d4); 
}

char acc_start_normal_auto(){
 while (start==True){
            c=abs(delay_s-(10-accel));
            for(int i=0;i<10-accel;i++){                
               for(int j=0;j<accel_value;j++){
               
                  if(input (button1) | input (button3) | check_right_proxy()){
                     stop();
                     return return_value=False;
                     }  
                  speed=c;
                  normal();
                  }
               c=c+1;  
               }
               start=False;
               return return_value=True;
            }
}

char acc_start_reverse_auto(){

while (start==True){
            c=abs(delay_s-(10-accel));
            for(int i=0;i<10-accel;i++){       
               for(int j=0;j<accel_value;j++){
               
                  if(input (button1) == 1 | input (button3) == 1 | check_left_proxy()){
                     stop();
                     return return_value=False;
                     }   
                  speed=c;  
                  reverse();
                  }
               c=c+1;  
               }
               start=False;
               return return_value=True;
            }
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

  lcd_gotoxy(1,4);
  lcd_putc(swoff);
  lcd_gotoxy(20,4);
  lcd_putc(swoff);
  lcd_gotoxy(2,4);
  lcd_putc("     Homing...    ");
  speed=6;
  centered=False;
  centercounter=0;
  
  while (1){

    if (check_left_proxy()){
      stop();
      output_toggle (PIN_D4);
      correct = False;
      
      break;
    }
    
    if(input (button1) | input (button3) ){
      stop();
      lcd_gotoxy(1,4);
      centered=False;
      
      return;
      }
    
      reverse();
   }
   
   StartTick = get_ticks();
   
   
   while (1){
      
      center_flag=True;
      
      if (check_right_proxy()){
         stop();
         correct = False;
         break;
      }
      
      if (input (button1) | input (button3)){
      stop();
      lcd_gotoxy(1,4);
      centered=False;
      return;
      }
      
      normal();
      
      //printf("data: %s", centercounter);
     }
     
    FinishTick = get_ticks();
    CurrentTick = get_ticks();
   
   while (CurrentTick-FinishTick <= (FinishTick-StartTick)/2 ){
     center_flag=False;
     CurrentTick=get_ticks();
     
     if(input (button1) | input (button3) | check_left_proxy()){
      stop();
      return;
      }

    reverse();
    
   }
   
   stop();
   lcd_putc("\f");
   
   lcd_gotoxy(1,3);
   printf(lcd_putc,"count: %Lu", centercounter);
   step=centercounter/2;
   centered=True;
   center_flag=False;
   threshold=(centercounter/20);
   
   line1=(step/threshold);
   
   
   lcd_gotoxy(1,4);
   lcd_putc(swoff);
   lcd_gotoxy(20,4);
   lcd_putc(swoff);
   
   for(int i=2;i<=10;i++){
     lcd_gotoxy(i,4);
     lcd_putc(three_dot);
  }
   
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
            
               if(input (button1)| input (button3) | input (button4) ){
                  stop();
                  return;
                  }
                  
                 if (check_left_proxy()){
                 start=True;
                  stop();
                  delay_ms(500);
                  break;
                 }
               speed=c;     
               reverse();
               }
            c=c+1;  
            }
            start=False;
         }
         
         
       if (check_left_proxy()){
         stop();
         output_toggle (PIN_D4);
         delay_ms(500);
         start=True;
         correct = False;     
         break;
       }
       
       if (input (button5) == 1 | input (button1) == 1 | input (button3) == 1)
      {   
         stop();
         return;
      }
         speed=c;
         reverse();
      }
      
      
      while (1){
      
      while(start==True){
       c=abs(delay_s-(10-accel));
         for(int i=0;i<10-accel;i++){
         
              
            for(int j=0;j<accel_value;j++){
            
               if(input (button1)  | input (button3)){
                  stop();
                  return;
                  }
                      
                 if (check_right_proxy()){
                  start=True;
                  stop();
                  delay_ms(500);
                  break;
                 }
                speed=c;     
               normal();
               }
            c=c+1;  
            }
            start=False;
         }
         
      if (check_right_proxy()){
         stop();
         delay_ms(500);
         start=True;
         correct = False;
         break;
      }
      
       if (input (button5) == 1 | input (button1) == 1 | input (button3) == 1)
      {
         stop();
         return;
      }
      
      speed=c;
      normal();
      
      
     }
     
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
  
 //2  Sw off
 
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
for(i = 0; i < sizeof(lcd_custom_chars); i++)
   {
    lcd_send_byte(1, lcd_custom_chars[i]);
   }

// Set address counter pointing back to the DDRAM.
lcd_send_byte(0, 0x80);
}

#int_timer1
void  Timer1_isr(void)
{
  //output_toggle(PIN_D4);
  
  if (dir==0){
      delay_func (speed) ;
      output_high (A1);
      output_low (A2);
      output_low (B1);
      output_high (B2);
      delay_func (speed) ;
      
      output_low (A1);
      output_low (A2);
      output_low (B1);
      output_high (B2);
      delay_func (speed) ;
      
      output_low (A1);
      output_high (A2);
      output_low (B1);
      output_high (B2);
      delay_func (speed) ;
      
      output_low (A1);
      output_high (A2);
      output_low (B1);
      output_low (B2);
      delay_func (speed) ;
      
      output_low (A1);
      output_high (A2);
      output_high (B1);
      output_low (B2);
      delay_func (speed) ;
      
      output_low (A1);
      output_low (A2);
      output_high (B1);
      output_low (B2);
      delay_func (speed) ;
      
      output_high (A1);
      output_low (A2);
      output_high (B1);
      output_low (B2);
      delay_func (speed) ;
      
      output_high (A1);
      output_low (A2);
      output_low (B1);
      output_low (B2);
      
      
      step++;
      
      if(step>centercounter){
      step=centercounter;
      } 
     
  }
  if (dir==1){
      delay_func (speed) ;
      output_high (A1) ;
      output_low (A2) ;
      output_low (B1) ;
      output_low (B2) ;
      delay_func (speed) ;
      
      output_high (A1) ;
      output_low (A2) ;
      output_high (B1) ;
      output_low (B2) ;
      delay_func (speed) ;
      
      output_low (A1) ;
      output_low (A2) ;
      output_high (B1) ;
      output_low (B2) ;
      delay_func (speed) ;
      
      output_low (A1) ;
      output_high (A2) ;
      output_high (B1) ;
      output_low (B2) ;
      delay_func (speed) ;
      
      output_low (A1) ;
      output_high (A2) ;
      output_low (B1) ;
      output_low (B2) ;
      delay_func (speed) ;
      
      output_low (A1) ;
      output_high (A2) ;
      output_low (B1) ;
      output_high (B2) ;
      delay_func (speed) ;
      
      output_low (A1) ;
      output_low (A2) ;
      output_low (B1) ;
      output_high (B2) ;
      delay_func (speed) ;
      
      output_high (A1) ;
      output_low (A2) ;
      output_low (B1) ;
      output_high (B2) ;
      
      
      step--;

      
      if(center_flag){
      centercounter++;;
      }
     
  }
  
  
  set_timer1(65488);
  
  clear_interrupt(INT_TIMER1); 
  //disable_interrupts(INT_TIMER1);
}


#INT_LOWVOLT
void  LOWVOLT_isr(void) 
{
   disable_interrupts(INT_TIMER1); 
   disable_interrupts(INT_RDA); 
   
   write_eeprom(1,delay_s); //speed
   write_eeprom(2,accel); //accel
   write_eeprom(3,centered); //centered?
   
   step1= make8(step,0);
   step2= make8(step,1);
   write_eeprom(4,step1); //pos1
   write_eeprom(5,step2); //pos2
   
   write_eeprom(6,threshold); //threshold
   
   cc1= make8(centercounter,0);
   cc2= make8(centercounter,1);
   write_eeprom(7,cc1); //cc1
   write_eeprom(8,cc2); //cc2
   
   write_eeprom(9,line1);
   
   write_eeprom(10,control_right);
   write_eeprom(11,control_left);
   
   output_high(pin_b7);
   output_low(pin_b7);
   output_high(pin_b7);
   output_low(pin_b7);
   output_high(pin_b7);
   output_low(pin_b7);
   output_high(pin_b7);
   output_low(pin_b7);
   
   disable_interrupts(INT_LOWVOLT);
}


void main()
{

   setup_CCP1 (CCP_OFF);
   clear_interrupt(INT_TIMER1); 
   
   setup_low_volt_detect(LVD_TRIGGER_BELOW | LVD_36 );
   enable_interrupts(INT_LOWVOLT);
   
   enable_interrupts(GLOBAL);
   disable_interrupts(INT_TIMER1); 
   
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);      // Timer1 configuration: internal clock source + 8 prescaler
   set_timer1(65488);  //65488
   
   lcd_init();

   lcd_load_custom_chars(); 

   FinishTick = StartTick = CurrentTick = get_ticks(); //timers for centering 
   
  if(read_eeprom(1)<0 | read_eeprom(1)>10){
   
   write_eeprom(1,5); //speed
   write_eeprom(2,6); //accel
   write_eeprom(3,0); //centered?
   
   write_eeprom(4,100); //pos1
   write_eeprom(5,100); //pos2
   
   write_eeprom(6,0); //threshold
   
   write_eeprom(7,0); //cc1
   write_eeprom(8,0); //cc2
   
   write_eeprom(9,0); //line1 
   
   write_eeprom(10,1); //check_right
   write_eeprom(11,1); //check_left 
   
   lcd_gotoxy(1,3);
   lcd_putc("Eeprom Fail");
   
   
  }
  
  delay_s=read_eeprom(1);
  accel=read_eeprom(2);
  centered=read_eeprom(3);
  
  control_right=read_eeprom(10);
  control_left=read_eeprom(11);
  
  if(centered){
  
  step1=read_eeprom(4);
  step2=read_eeprom(5);
  
  step=make16(step2,step1);
  lcd_gotoxy(1,3);
  printf(lcd_putc,"%lu",step);
  
  threshold=read_eeprom(6);
  printf(lcd_putc," %d",threshold);
  
  cc1=read_eeprom(7);
  cc2=read_eeprom(8);
  centercounter=make16(cc2,cc1);
  
  //ine1=step/threshold;
  //line1++;
  
  line1=read_eeprom(9);
  
  lcd_gotoxy(1,4);
  lcd_putc(swoff);
  lcd_gotoxy(20,4);
  lcd_putc(swoff);
  
  if(step>=threshold){
  for(int i=2;i<=line1;i++){
     lcd_gotoxy(i,4);
     lcd_putc(three_dot);
  }
  }
 
  if (input(proxy_left)) {
  
      lcd_gotoxy(1,4);
      lcd_putc(swoff);
      
      lcd_gotoxy(20,4);
      lcd_putc(swon);
      
  }
  
  if (input(proxy_right)) {
  
      lcd_gotoxy(1,4);
      lcd_putc(swon);
      
      lcd_gotoxy(20,4);
      lcd_putc(swoff);
  }
     
  }
  
  else{
   lcd_gotoxy(4,4);
   lcd_putc("Centee Machine");
  
  }
  
   while (TRUE)
   {
      enable_interrupts (int_rda);
      //output_toggle (PIN_D4);
       
      lcd_gotoxy(1,1);
      lcd_putc("Speed: %");
      printf(lcd_putc,"%u",delay_s*10);
      
      lcd_gotoxy(13,1);
      if (mode_manuel==True){
         lcd_putc("<Manuel>");
      }
      
      if (mode_auto==True){
         lcd_putc("< Auto >");
      }
      
      lcd_gotoxy(1,2);
      lcd_putc("Accel: %");
      printf(lcd_putc,"%u",accel*10);
      
      if(centered==False){
         lcd_gotoxy(4,4);
         lcd_putc("Center Machine");
      }
     
      
///////////////////>>>>>> MODE CHANGE  <<<<<//////////////////////

      if (input (button5))
      {
         while(input (button5)){};
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
            
            if(input(button1)){
            delay_s--;
            
            if(delay_s<=0){
               delay_s=1;
            }
            
            lcd_gotoxy(8,mode_cursor);
            lcd_putc("%");
            printf(lcd_putc,"%d ",delay_s*10);
            
            delay_ms(500);
            
            }
            
            if(input(button3)){
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
             
              if(input(button1)){
               accel--;
               
               if(accel<=0){
                  accel=1;
               }
               
               lcd_gotoxy(8,mode_cursor);
               lcd_putc("%");
               printf(lcd_putc,"%d ",accel*10);
               
               delay_ms(500);
               
               }
           
            if(input(button3)){
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
      
/*
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
         
*/
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
                           
                     normal();
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
                 
               normal ();
    
               
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
start=True;
q=False;

      while (input (button1) & mode_manuel)
      {
         if (check_right_proxy() | input (button3) ){
            stop();
            correct = False;
            disable_interrupts(INT_TIMER1);
            break;
          }
         
         acc_start_normal_man();

         if (!(return_value)){
            return_value=True;
            break;
            }
            
         speed=delay_s;
         normal ();
         quit1=True;
      }
         
      while (input (button3) & mode_manuel)
      {
         if (check_left_proxy() | input (button1) ){
               stop();
               correct = False;
               disable_interrupts(INT_TIMER1);
               break;
          }
          
        acc_start_reverse_man();

        if (return_value==False){
            return_value=True;
            break;
            }
            
        speed=delay_s;
        reverse ();
        quit2=True;
      }
      
      while(quit2){
         for(int i=0;i<10-accel;i++){                
            for(int j=0;j<accel_value;j++){
               
               if(input (button1) | check_left_proxy()){
                  stop();
                  disable_interrupts(INT_TIMER1);
                  break;
               }
               
               speed=c;
               reverse();
               }
               c=c-1;  
    
         }
         quit2=False;
         stop();
         }
       
      while(quit1){
         for(int i=0;i<10-accel;i++){                
            for(int j=0;j<accel_value;j++){ 
               
               if(input (button3) | check_right_proxy()){
                  stop();
                  disable_interrupts(INT_TIMER1);
                  break;
               }
               
               speed=c;
               normal();
               }
               c=c-1;  
                   
         }
         quit1=False;
         stop();
         }
         
      stop();
      
      

////////////////////>>>>>> AUTO <<<<</////////////////////

      if (input (button1)  & mode_auto == TRUE)
      {
      start=True;
      while(input (button1)){};
      
         while (TRUE)
         {  
                    
            acc_start_normal_auto();

            if (return_value==False){
                  return_value=True;
                  break;
            }
            
            normal ();
            
            if (input (button2) | input (button3)  | input (button4) | check_right_proxy())
            {
               stop();
               break;
            }
         }
      }

      if (input (button3) == 1 & mode_auto == TRUE)
      {
         start=True;
         delay_ms(500);
         
      
         while (TRUE)
         {  
                     
             acc_start_reverse_auto();

            if (return_value==False){
                  return_value=True;
                  break;
            }

            reverse ();
            
            if (input (button2) |input (button1) |input (button4)  | check_left_proxy())
            {
               stop();
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
         //gogo();
         
         write_eeprom(1,delay_s); //speed
         write_eeprom(2,accel); //accel
         write_eeprom(3,centered); //centered?
         step1= make8(step,0);
         step2= make8(step,1);
         write_eeprom(4,step1); //pos1
         write_eeprom(5,step2); //pos2
         write_eeprom(6,threshold); //threshold
      }
      
      
   } 
   
 }


