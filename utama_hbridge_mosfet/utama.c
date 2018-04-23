/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.9 Standard
Automatic Program Generator
© Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 10/26/2012
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega128
Program type        : Application
Clock frequency     : 11.059200 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*****************************************************/
                                               

// ***************************************** Included *****************************************
#include <mega128.h>    
#include <stdio.h>
#include <delay.h>
// ********************************************************************************************


// ****************************************** Define ******************************************                 
#define swmode PINA.2
#define swon PINA.1
#define arahki PORTB.4
#define arahka PORTB.3
#define pwmki OCR1B
#define pwmka OCR1A 
#define pwmservol OCR1CL
#define pwmservoh OCR1CH                                                                           
#define vrc PORTE.3
// ********************************************************************************************


// ***************************************** Variabel *****************************************
char isimenu;                   // variabel USART
eeprom int kp=38, def_speed=3456, kd=2;
eeprom int mode=1; 
int kamera;                  
int tujuan=268;                                                                             
int flagparse=0;
int tempkompas=0, kompas=0;                                                                                    

int err,rv;                     // variabel PID
int l_speed,r_speed;

int sw=0;                         // variabel global        
int i;                     
int range1=-5, range2=5;
// ********************************************************************************************


// ******************************************** PID *******************************************
void pid_compass()
{    
     // CARI ERROR
     if (tujuan>=kompas)
     {
        if ((tujuan-kompas)>=(360+kompas-tujuan))       // ct tujuan = 300, kompas = 10, err = 70
            {err=360+kompas-tujuan;}
        else                                            // ct tujuan = 180, kompas = 70, err = -110
            {err=kompas-tujuan;}
     }
     else 
     {
        if ((kompas-tujuan)>(360+tujuan-kompas))        // ct kompas = 250, tujuan = 40, err = - 150 
            {err=kompas-(360+tujuan);}
        else
            {err=kompas-tujuan;}                        // ct kompas = 50, tujuan = 30, err = 20
     }   
     
     // PERBESARAN ERROR
     rv=kp*err;                                         // rv = 38*180=6840
     if (rv<-6500)
        {rv=-6500;}
     else if(rv>6500)
        {rv=6500;}        
           
     // TINDAKAN (LSPEED & RSPEED): -3384 < V < 3456
     if (err>=0)                                        // kompas di kanan tujuan -> harus ke kiri
     {
        l_speed=def_speed-rv;                           // motor kiri lambatin. -3384 < v < 3456
        r_speed=def_speed;                              // motor kanan tetep
     }
     else if (err<0)                                    // kompas di kanan tujuan -> harus ke kiri                 
     {
        l_speed=def_speed;                              // motor kiri tetep
        r_speed=def_speed+rv;                           // motor kanan lambatin. -3384 < v < 3456
     }   
     
     //printf("l%d r%d \n\r",l_speed,r_speed);
     // KONVERSI LSPEED & RSPEED KE PORT ARAH & PWM              
     if(l_speed<0)                                      // definisi port arah & pwm motor kiri
     {
        arahki=0;
        pwmki = -1 * l_speed;
     }
     else
     {
        arahki=1;   
        pwmki = l_speed;
     }            
     
     if(r_speed<0)                                      // definisi port arah & pwm motor kanan
     {
        arahka=0;
        pwmka = -1 * r_speed;
     }
     else
     {
        arahka = 1;
        pwmka = r_speed;
     }
}
// ********************************************************************************************

// ************************************** Speed tes kiri2 **************************************
void speed_kaki2()
{
        /*while(1){
                pid_compass();
                delay_ms(500);
        }*/
        kamera=0;
        pwmservoh=0x0;                  // servo ngadep kiri            
        pwmservol=0x68;
                                                
        kamera = 0;
        while(kamera!=1)                // jalan lurus sampe nemu bola
                pid_compass();
                        
        if(tujuan<90)                   // tujuan belok kiri counter clockwise
                tujuan=tujuan+270;
        else if(tujuan>=90)
                tujuan=tujuan-90;      
        err=90;                
        pwmservoh=0x1;                  // servonya palingin dulu
        pwmservol=0x9F;
                        
        while((err < range1) || (err > range2))  // selama ga belok kiri
                pid_compass();
                        
        //kamera=0;                       // servonya dipalingin lg
        pwmservoh=0x0;
        pwmservol=0x68;        
                        
        /*while(kamera!=1)                // selama ga liat bola lg
                pid_compass();         */
                        
        if(tujuan<90)                   // liat bola -> tujuan ganti lg
                tujuan=tujuan+270;
        else if(tujuan>=90)
                tujuan=tujuan-90;
        pwmservoh=0x1;
        pwmservol=0x00;      
                        
        while(1)
                pid_compass();
}
// ********************************************************************************************


// ************************************** Speed tes kiri **************************************
void speed_kaki()
{
        /*while(1){
                pid_compass();
                delay_ms(500);
        }*/
        kamera=0;
        pwmservoh=0x0;                  // servo ngadep kiri            
        pwmservol=0x68;
                                                
        kamera = 0;
        while(kamera!=1)                // jalan lurus sampe nemu bola
                pid_compass();
                        
        if(tujuan<90)                   // tujuan belok kiri counter clockwise
                tujuan=tujuan+270;
        else if(tujuan>=90)
                tujuan=tujuan-90;      
        err=90;                
        pwmservoh=0x1;                  // servonya palingin dulu
        pwmservol=0x9F;
                        
        while((err < range1) || (err > range2))  // selama ga belok kiri
                pid_compass();
                        
        kamera=0;                       // servonya dipalingin lg
        pwmservoh=0x0;
        pwmservol=0x68;        
                        
        while(kamera!=1)                // selama ga liat bola lg
                pid_compass();
                        
        if(tujuan<90)                   // liat bola -> tujuan ganti lg
                tujuan=tujuan+270;
        else if(tujuan>=90)
                tujuan=tujuan-90;
        pwmservoh=0x1;
        pwmservol=0x00;      
                        
        while(1)
                pid_compass();
}
// ********************************************************************************************


// ************************************** Speed tes kanan *************************************
void speed_kika()
{
        kamera=0;
        pwmservoh=0x1;                     // servo ngadep kanan            
        pwmservol=0x9F;
                                                
        while(kamera!=1)                // jalan lurus sampe nemu bola
                pid_compass();
                        
        if(tujuan<=269)                   // tujuan belok kanan clockwise
                tujuan=tujuan+90;
        else if(tujuan>269)
                tujuan=tujuan-270;      
        err=-90;                
        pwmservoh=0x0;                  // servonya palingin dulu
        pwmservol=0x68;     
                        
        while((err < range1) || (err > range2))  // selama ga belok kanan
                pid_compass();
                        
        kamera=0;                      // servonya dipalingin lg
        pwmservoh=0x1;
        pwmservol=0x9F;        
                        
        while(kamera!=1)               // selama ga liat bola lg
                pid_compass();
                        
        if(tujuan<=269)                 // liat bola -> tujuan ganti lg
                tujuan=tujuan+90;
        else if(tujuan>269)
                tujuan=tujuan-270;
        pwmservoh=0x1;
        pwmservol=0x00;      
                        
        while(1)
                pid_compass();
}
// ********************************************************************************************

// ************************************** Speed tes kanan 2*************************************
void speed_kika2()
{
        kamera=0;
        pwmservoh=0x1;                     // servo ngadep kanan            
        pwmservol=0x9F;
                                                
        while(kamera!=1)                // jalan lurus sampe nemu bola
                pid_compass();
                        
        if(tujuan<=269)                   // tujuan belok kanan clockwise
                tujuan=tujuan+90;
        else if(tujuan>269)
                tujuan=tujuan-270;      
        err=-90;                
        pwmservoh=0x0;                  // servonya palingin dulu
        pwmservol=0x68;     
                        
        while((err < range1) || (err > range2))  // selama ga belok kanan
                pid_compass();
                        
        kamera=0;                      // servonya dipalingin lg
        pwmservoh=0x1;
        pwmservol=0x9F;        
                        
        /*while(kamera!=1)               // selama ga liat bola lg
                pid_compass();*/
                        
        if(tujuan<=269)                 // liat bola -> tujuan ganti lg
                tujuan=tujuan+90;
        else if(tujuan>269)
                tujuan=tujuan-270;
        pwmservoh=0x1;
        pwmservol=0x00;      
                        
        while(1)
                pid_compass();
}
// ********************************************************************************************


// ****************************************** Manuver *****************************************
void manuver()
{
        for(i=0;i<2;i++){
        // ******************* jalan dr kiri berakhir di kanan *********************                      
                pwmservoh=0x1;            //servo ngadep kanan
                pwmservol=0x9F;
                kamera=0;
                                
                while(kamera!=1)                // jalan lurus sblm liat bola
                        pid_compass();
                        
                if (tujuan <= 269)              // tujuan belok kanan clockwise
                        tujuan=tujuan+90;
                else if (tujuan > 269)
                        tujuan=tujuan-270;  
                err=-90;
                pwmservoh=0x0;
                pwmservol=0x68;            
                        
                while((err < range1) || (err > range2))
                        pid_compass();
                         
                pwmservoh=1;
                pwmservol=0x9F;
                kamera=0;              
                                
                while(kamera!=1)
                        pid_compass();
                        
                if(tujuan<90)                   // tujuan belok kiri counter clockwise
                        tujuan=tujuan+270;
                else if(tujuan>=90)
                        tujuan=tujuan-90; 
                err=90;
                        
                while((err < range1) || (err > range2))
                        pid_compass();
        // ***********************************************************************               
                        
        // ********************* jalan kanan berakhir di kiri ********************
                pwmservoh=0;            // servo ngadep kiri            
                pwmservol=0x68;                       
                kamera=0;
                        
                while(kamera!=1)        // jalan lurus sampe nemu bola
                        pid_compass();
                        
                if(tujuan<90)                   // tujuan belok kiri counter clockwise
                        tujuan=tujuan+270;
                else if(tujuan>=90)
                        tujuan=tujuan-90;      
                
                err=90;
                pwmservoh=0x1;                  // servonya palingin dulu
                pwmservol=0x9F;
                        
                while((err < range1) || (err > range2))          // selama ga belok kiri
                        pid_compass();  
                        
                pwmservoh=0;                    // servonya dipalingin lg
                pwmservol=0x68;        
                kamera=0;                      
                
                while(kamera!=1)               // selama ga liat bola lg
                        pid_compass();
                        
                if(tujuan>269)                   // liat bola -> belok kanan
                        tujuan=tujuan-270;
                else if(tujuan<=269)
                        tujuan=tujuan+90;
                err=-90;
                
                while((err < range1) || (err > range2))
                        pid_compass();
        // ******************************************************************* 
        }   
        
        // ******************* jalan dr kiri balik arah **********************                      
                pwmservoh=0x1;            //servo ngadep kanan
                pwmservol=0x9F;
                kamera=0;
                                
                while(kamera!=1)                // jalan lurus sblm liat bola
                        pid_compass();
                        
                if (tujuan <= 269)              // tujuan belok kanan clockwise
                        tujuan=tujuan+90;
                else if (tujuan > 269)
                        tujuan=tujuan-270;  
                err=-90;
                pwmservoh=0x0;
                pwmservol=0x68;            
                        
                while((err < range1) || (err > range2))
                        pid_compass();
                         
                pwmservoh=0x1;
                pwmservol=0x9F;
                kamera=0;              
                                
                while(kamera!=1)
                        pid_compass();
                        
                if(tujuan <= 269)                   // tujuan belok kanan clockwise
                        tujuan=tujuan+90;
                else if(tujuan > 269)
                        tujuan=tujuan-270; 
                err=-90;
                pwmservoh=0x0;
                pwmservol=0x68;
                        
                while((err < range1) || (err > range2))
                        pid_compass();
        // ***********************************************************************
        
        for(i=0;i<2;i++){
        // ******************* jalan dr kiri berakhir di kanan *********************                      
                pwmservoh=0x1;            //servo ngadep kanan
                pwmservol=0x9F;
                kamera=0;
                                
                while(kamera!=1)                // jalan lurus sblm liat bola
                        pid_compass();
                        
                if (tujuan <= 269)              // tujuan belok kanan clockwise
                        tujuan=tujuan+90;
                else if (tujuan > 269)
                        tujuan=tujuan-270;  
                err=-90;
                pwmservoh=0x0;
                pwmservol=0x68;            
                        
                while((err < range1) || (err > range2))
                        pid_compass();
                         
                pwmservoh=1;
                pwmservol=0x9F;
                kamera=0;              
                                
                while(kamera!=1)
                        pid_compass();
                        
                if(tujuan<90)                   // tujuan belok kiri counter clockwise
                        tujuan=tujuan+270;
                else if(tujuan>=90)
                        tujuan=tujuan-90; 
                err=90;
                        
                while((err < range1) || (err > range2))
                        pid_compass();
        // ***********************************************************************               
                        
        // ********************* jalan kanan berakhir di kiri ********************
                pwmservoh=0;            // servo ngadep kiri            
                pwmservol=0x68;                       
                kamera=0;
                        
                while(kamera!=1)        // jalan lurus sampe nemu bola
                        pid_compass();
                        
                if(tujuan<90)                   // tujuan belok kiri counter clockwise
                        tujuan=tujuan+270;
                else if(tujuan>=90)
                        tujuan=tujuan-90;      
                
                err=90;
                pwmservoh=0x1;                  // servonya palingin dulu
                pwmservol=0x9F;
                        
                while((err < range1) || (err > range2))          // selama ga belok kiri
                        pid_compass();  
                        
                pwmservoh=0;                    // servonya dipalingin lg
                pwmservol=0x68;        
                kamera=0;                      
                
                while(kamera!=1)               // selama ga liat bola lg
                        pid_compass();
                        
                if(tujuan>269)                   // liat bola -> belok kanan
                        tujuan=tujuan-270;
                else if(tujuan<=269)
                        tujuan=tujuan+90;
                err=-90;
                
                while((err < range1) || (err > range2))
                        pid_compass();
        // ******************************************************************* 
        }
        
        while(1)
                pid_compass(); 
}
// ********************************************************************************************


// ******************************************** RC ********************************************
interrupt [EXT_INT4] void ext_int4_isr(void)
{
while(PINE.4==0)        // selama RC ga dimatiin
{                     
        if ((PINE.5==0) && (PINE.6==0) && (PINE.7==0)){         // 1. mundur
                arahki = arahka = 0;   
                pwmki = pwmka = 0xd80;
                
                while ((PINE.5==0) && (PINE.6==0) && (PINE.7==0) && (PINE.4==0)){}
        }
        
        else if((PINE.5==0) && (PINE.6==0) && (PINE.7==1)){     // 2. maju kanan
                arahki = 1;   
                pwmki = 0xd80;
                pwmka = 0;
                
                while ((PINE.5==0) && (PINE.6==0) && (PINE.7==1) && (PINE.4==0)){}
        }
        
        else if((PINE.5==0) && (PINE.6==1) && (PINE.7==0)){     // 3. maju kiri
                arahka = 1;   
                pwmka = 0xd80;  
                pwmki = 0;
                
                while ((PINE.5==0) && (PINE.6==1) && (PINE.7==0) && (PINE.4==0)){}
        }
        
        else if((PINE.5==0) && (PINE.6==1) && (PINE.7==1)){     // 4. Maju     
                arahki = arahka = 1;   
                pwmki = pwmka = 0xd80;
                
                while ((PINE.5==0) && (PINE.6==1) && (PINE.7==1) && (PINE.4==0)){}
        }
        
        else if((PINE.5==1) && (PINE.6==0) && (PINE.7==0)){     // 5. ga kepake 
        }
        
        else if((PINE.5==1) && (PINE.6==0) && (PINE.7==1)){     // 6. pivot kanan    
                arahki = 1;
                arahka = 0;   
                pwmki = pwmka = 0xd80;
                
                while ((PINE.5==1) && (PINE.6==0) && (PINE.7==1) && (PINE.4==0)){}
        }     
        
        else if((PINE.5==1) && (PINE.6==1) && (PINE.7==0)){     // 7. pivot kiri
                arahka = 1; 
                arahki = 0;  
                pwmka = pwmki = 0xd80;
                
                while ((PINE.5==1) && (PINE.6==1) && (PINE.7==0) && (PINE.4==0)){}
        } 
        
        else {
                pwmki = pwmka = arahki = arahka = 0;
                while ((PINE.5==1) && (PINE.6==1) && (PINE.7==1) && (PINE.4==0)){}
        } 
}                                                                                              
}   
// ********************************************************************************************

#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART0 Receiver buffer
#define RX_BUFFER_SIZE0 8
char rx_buffer0[RX_BUFFER_SIZE0];

#if RX_BUFFER_SIZE0<256
unsigned char rx_wr_index0,rx_rd_index0,rx_counter0;
#else
unsigned int rx_wr_index0,rx_rd_index0,rx_counter0;
#endif

// This flag is set on USART0 Receiver buffer overflow
bit rx_buffer_overflow0;


// USART0 Receiver interrupt service routine
interrupt [USART0_RXC] void usart0_rx_isr(void)
{
char status,data;
status=UCSR0A;
data=UDR0; 

if(sw==1) {  
        if (data=='p'){
                isimenu = 'p';  
                kp = 0;}                     
        else if(data=='i'){ 
                isimenu = 'i';
                def_speed = 0;}        
        else if(data=='d'){   
                isimenu = 'd';
                kd = 0;}
        else if(data=='m'){   
                isimenu = 'm';
                mode = 0;}
        else if(data=='t'){   
                isimenu = 't';
                tujuan = 0;}
        else if(data > 47 && data < 58){
                if (isimenu=='p')
                        kp = kp*10 + (data-48);
                else if (isimenu=='i')    
                        def_speed = def_speed*10 + (data-48);                        
                else if (isimenu=='d')    
                        kd = kd*10 + (data-48);
                else if (isimenu=='m')    
                        mode = (data-48);
                else if (isimenu=='t')    
                        tujuan = tujuan*10 + (data-48);
        /*else if(data=='k')
                printf("kp%d ki%d kd%d menu%d tujuan%d", kp, def_speed, kd, mode, tujuan);*/            
        }   
}

else if (sw==2){
        kamera=data-48;
}

if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer0[rx_wr_index0]=data;
   if (++rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   if (++rx_counter0 == RX_BUFFER_SIZE0)
      {
      rx_counter0=0;
      rx_buffer_overflow0=1;
      };
   };
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART0 Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter0==0);
data=rx_buffer0[rx_rd_index0];
if (++rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
#asm("cli")
--rx_counter0;
#asm("sei")
return data;
}
#pragma used-
#endif

// USART1 Receiver buffer
#define RX_BUFFER_SIZE1 8
char rx_buffer1[RX_BUFFER_SIZE1];

#if RX_BUFFER_SIZE1<256
unsigned char rx_wr_index1,rx_rd_index1,rx_counter1;
#else
unsigned int rx_wr_index1,rx_rd_index1,rx_counter1;
#endif

// This flag is set on USART1 Receiver buffer overflow
bit rx_buffer_overflow1;

// USART1 Receiver interrupt service routine
interrupt [USART1_RXC] void usart1_rx_isr(void)
{
char status,data;
status=UCSR1A;
data=UDR1;            


if(flagparse == 0){
        if(data > 47 && data < 58){
                flagparse = 1; 
                tempkompas = data-48;
        }
}

else if (flagparse == 1){
        if(data > 47 && data < 58){
                tempkompas = (tempkompas * 10) + (data-48);
        }
        else{
                flagparse = 0;
                kompas = tempkompas;
                tempkompas = 0;
        }
}

if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer1[rx_wr_index1]=data;
   if (++rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
   if (++rx_counter1 == RX_BUFFER_SIZE1)
      {
      rx_counter1=0;
      rx_buffer_overflow1=1;
      };
   };
}

// Get a character from the USART1 Receiver buffer
#pragma used+
char getchar1(void)
{
char data;
while (rx_counter1==0);
data=rx_buffer1[rx_rd_index1];
if (++rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
#asm("cli")
--rx_counter1;
#asm("sei")
return data;
}
#pragma used-
// Write a character to the USART1 Transmitter
#pragma used+
void putchar1(char c)
{
while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
UDR1=c;
}
#pragma used-     


void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=P State1=P State0=T 
PORTA=0x06;
DDRA=0x00;

// Port B initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0xF8;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Port E initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=In Func1=In Func0=In 
// State7=P State6=P State5=P State4=P State3=1 State2=T State1=T State0=T 
PORTE=0xF8;
DDRE=0x08;

// Port F initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTF=0x00;
DDRF=0x00;

// Port G initialization
// Func4=In Func3=In Func2=In Func1=In Func0=In 
// State4=T State3=T State2=T State1=T State0=T 
PORTG=0x00;
DDRG=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0 output: Disconnected
ASSR=0x00;
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 172.800 kHz
// Mode: Fast PWM top=ICR1
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// OC1C output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR1A=0xAA;
TCCR1B=0x1B;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x0D;
ICR1L=0x80;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
OCR1CH=0x00;
OCR1CL=0x68;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// Timer/Counter 3 initialization
// Clock source: System Clock
// Clock value: Timer 3 Stopped
// Mode: Normal top=FFFFh
// Noise Canceler: Off
// Input Capture on Falling Edge
// OC3A output: Discon.
// OC3B output: Discon.
// OC3C output: Discon.
// Timer 3 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR3A=0x00;
TCCR3B=0x00;
TCNT3H=0x00;
TCNT3L=0x00;
ICR3H=0x00;
ICR3L=0x00;
OCR3AH=0x00;
OCR3AL=0x00;
OCR3BH=0x00;
OCR3BL=0x00;
OCR3CH=0x00;
OCR3CL=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
// INT3: Off
// INT4: On
// INT4 Mode: Rising Edge
// INT5: Off
// INT6: Off
// INT7: Off
EICRA=0x00;
EICRB=0x00;
EIMSK=0x10;
EIFR=0x10;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;
ETIMSK=0x00;

// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 9600
UCSR0A=0x00;
UCSR0B=0x98;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x47;

// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 9600
UCSR1A=0x00;
UCSR1B=0x98;
UCSR1C=0x06;
UBRR1H=0x00;
UBRR1L=0x47;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// Global enable interrupts
#asm("sei") 

/*while(1){
putchar('a');
delay_ms(50);
}*/          

while ((swmode == 1) && (swon == 1)) {};        // selama ga pilih mode cuma bs RC

if (swmode==0){ 
        vrc=1;      
        sw = 1;
        while(1){};     
}

else if(swon==0){ 
        delay_ms(3000);                 
        kamera=0;
        sw = 2;        
        if (mode==1)                    // SPEED TEST KANAN - KIRI  
                speed_kaki();
        
        else if (mode==2)              // SPEED TEST KIRI - KANAN
                speed_kika();
        
        else if (mode==3)              // MANUVER   
                manuver();
        else if (mode==4)               //speed test kanan kiri 2
                speed_kaki2();                                  
        else if(mode==5)                //speed test kiri kanan 2
                speed_kika2();        
                
};
}