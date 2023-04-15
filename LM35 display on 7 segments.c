#include<16f887.h>
#device      adc=10
#Fuses   intrc_io
#use delay(clock=8M)
#use rs232(baud=2400,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

#define     coi      pin_e1

signed int32    nd,ndt,ch,chay;
char     data;
unsigned int8   ma7doan[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};

void nhan_dl()
{
   if(kbhit())
   {
      data=getc();
      if(data=='S')        
      {
         ndt=nd;
         printf("%ld",nd);   
      }
   }
   else
   {
      if(ndt!=nd)
      {
         ndt=nd;
         printf("%ld",nd);
      }   
   }   
}

void doc_nd()
{
   nd=read_adc();
   nd=nd/2.046;
   ch=ma7doan[nd/10];
   if(ch==0xc0)        
   ch=0xff;
   chay++;
   delay_ms(10);
   if(chay<51)
   {
      output_d(ch);     
	  output_b(ma7doan[nd%10]);
   }
   else if(chay<60)
   {
      if(nd<35)  
      {
         output_d(ch);     
         output_b(ma7doan[nd%10]);      
      } 
      else
      {
         output_d(0xff);    
		 output_b(0xff);    
      }
   }
   else     
chay=0;
}

void main()
{  
   set_tris_a(0xff);
   set_tris_b(0x00);       
   set_tris_c(0x80);
   set_tris_d(0x00);      
   
   setup_adc(adc_clock_div_32);  
   setup_adc_ports(san4);
   set_adc_channel(4);
   chay=0;        
   output_high(coi);
   while(true)
   {
      nhan_dl();
      doc_nd();
      if(nd<55) 
      output_high(coi);
      else  
      output_low(coi);  
   }
}


