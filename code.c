//FINAL CODE
/*
Author: code-Assasin
Title: Speed Checker
*/
#include<reg51.h>
#include<stdio.h>
#include<string.h>
#define LCD_clear() LCD_command(0x01)	/* Clear display LCD */
#define LCD_origin() LCD_command(0x02)	/* Set to origin LCD */
#define LCD_row1() LCD_command(0x80)	/* Begin at Line 1 */
#define LCD_row2() LCD_command(0xC0)
 

sbit rs =P1^0; // register select pin of LCD
sbit en =P1^3; //enable pin of LCD
sbit rw=P1^1; // Read write pin
sbit sw=P1^6;//Switch pin

unsigned int a=0,v,i=0,flag=-1,j=0;
unsigned int array[8];
//initializing functions
void que(int vel);
void que_disp();

void lcd_en ()//enabling the lcd display
{
		rw=0;
		en  = 1; en  = 1; en  = 1;
     en  = 0; en  = 0;
}
void LCD_delay(unsigned char ms) //delay function
{
	unsigned int n;
	unsigned int i;
	for (n=0; n<ms; n++)
	{
		for (i=0; i<1275; i++); /* For 1 ms */
	}
   
}
void LCD_command(unsigned char command)//function to give commands to lcd
{
     rw=0;
     rs=0; rs = 0;//read as command
 	   P2		= command;
 	   lcd_en ();
     LCD_delay(1);

}

void LCD_init()
{
	 rw=0;
	   LCD_command(0x38);//Use 2 lines and 5×7 matrix
	   LCD_command(0x06);//Increment cursor
	   LCD_command(0x0c);//Display on,cursor off
	   LCD_command(0x01);//clear lcd
     LCD_delay(06);
}
void LCD_putc(unsigned char ascii)//printing characters on the lcd
{     
    rw=0;
		rs=1;
 	   P2		= ascii;
 	   lcd_en ();
     LCD_delay(2);
//      LCD_command(0x0c);
}

void LCD_puts(unsigned char *lcd_string)//printing strings character by character
{
	while (*lcd_string)
	{
		LCD_putc(*lcd_string++);
	}
}


void ext_int0()interrupt 0
{
    flag=1;
	 LCD_command(0x01);
  LCD_puts("Check A");
  LCD_delay(100);
  LCD_command(0x01);
 }
 
void ext_int1()interrupt 2
{
	 flag=0;
 LCD_puts("Check B");
  LCD_delay(100);
  LCD_command(0x01);
}   
void que(int vel) //function to make the queue of fixed size 8
{
	 unsigned int dum[8];
	 for(j=0;j<8;j++)
	 {
		 dum[j]=0;
	 }
    dum[7]=array[7];
	 for(j=7;j>0;j--)
	 {
		  if(array[j]!=0)
			{
				dum[j-1]=array[j-1];
				array[j-1]=dum[j];				
			}
			else
			{
				continue;
			}
	 }
	 array[7]=vel;	
}
void que_disp()
{
	unsigned int p;
	unsigned char m,n,temp,o,t,rem;
	for(j=8;j>0;j--)
	{	
		p=array[j-1];
                      //dividing into thousands,hundreds,tens and units place to display on lcd
		t=(p)/1000;
		 rem=(p)%1000;
		 m=rem/100;
		temp=rem%100;
		n=temp/10;
		o=temp%10;
		LCD_putc(t+48);//add 48 to integer to convert to ASCII
		LCD_putc(m+48);
		LCD_putc(n+48);
		LCD_putc(o+48);	
		LCD_puts(" mm/sec ");			
		LCD_delay(1000);
		LCD_command(0x01);
	}
	
}

void main()
{
	  for(i=0;i<8;i++)//Initialize the array
	    array[i]=0;
	
    LCD_init();
    P2=0xA0;//register P2 is addressed with port 2 register
    IE=0x85;//enable global interrupts
    IT0=IT1=1;//Enable Edge Triggered
	
	 while(1)
	 {
	if(sw==0) //If switch is off
	{	
		 while(1)
		 {
			 if(flag)//Check A was crossed
			 {
				 //time();
				 LCD_delay(800);
				 a++;//counter begins
			 }
			 else//check B was crossed
			 {
				 unsigned char m,n,temp,o,t,rem;
				v=100/a;//a is seconds and 100 is 100 mm-->distance
				 que(v);
	  //dividing into thousands,hundreds,tens and units place to display on lcd
				 t=v/1000;
				 rem=v%1000;
				 m=rem/100;
			temp=rem%100;
			n=temp/10;
			o=temp%10;
				 LCD_putc(t+48);//+48 to convert to ascii
				 LCD_putc(m+48);
					LCD_putc(n+48);
					LCD_putc(o+48);	
				LCD_puts(" mm/sec ");			
				 break;
			 }	
		 }
		 flag=-1;//reinitializing the parameters
		 a=0;
	 }
	else{//if switch is on		
		LCD_puts("Data");
		 LCD_delay(100);
                       LCD_command(0x01);
		que_disp();
				
	}
}
	
    while(1);
}
