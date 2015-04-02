#include <stdint.h>
#include "basic_io.h"

#include "LCD.h"

#include <stdio.h>
#include <string.h>
#include <system.h>
#include <drivers/inc/altera_avalon_uart_regs.h>
#include <drivers/inc/altera_avalon_pio_regs.h>

#include "dm9000a.h"
#include "inet/ethernet_header.h"
//#include "inet\work.h"

#include "inet\ncp.h"
#include "inet\ethernet_header.h"
#include "inet\packet.h"
unsigned int aaa,rx_len,packet_num;

extern unsigned char data[ETHER_MAX_LEN];
extern uint32_t data_size ;


extern unsigned char result[ETHER_MAX_LEN];
extern uint32_t result_size ;


volatile unsigned int tx_control ;
volatile unsigned int link_control;


void button_interrupt(void* context, unsigned long id)
{
 IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_PIO_BASE, 0);
}

void uart_interrupt(void* context, unsigned long id)
{
	IOWR_ALTERA_AVALON_UART_STATUS(UART_0_BASE, 0);

}

/**
 * Reciving singlas from ethernet controller -
 * TX - transmited packet
 * RX - recived packet
 * LinkChange - Connected/Diconncted Ethernet Cable
 *
 */
void ethernet_interrupts_simple()
{
    frame *curr_frame = 0;
	int filtered;
	uint8_t interruption =ior(ISR);    //which interruption

    packet_num++;

    iow(IMR, 0);
 //   iow(IMR, PAR_set);
   // printf("ISR 0x%2X \n",interruption);

    if( interruption & 0x01 )      //RX
    {

    	iow(ISR,0x01);
    	curr_frame = get_free_frame();
    	if(curr_frame == 0)
    	{
    		return; //ERROR no more free frames
    	}

    	aaa=ReceivePacket(curr_frame->f_data,&curr_frame->f_len);
    	if(aaa == 0)
    	{
        	filtered =   filter_packiets(curr_frame->f_data,curr_frame->f_len);

        	if (filtered == 0)
        	{
        		printf(" RX:UDP \n");
        		add_rx_frame(curr_frame);

        	}
        	else if (filtered == 10) //arp
        	{
        		printf(" RX:ARP \n");
        		add_rx_frame(curr_frame);
        	}
        	else
        	{
    			release_frame(curr_frame);
    		}
        	curr_frame = 0;

    	}
    	else
    	{
    		printf("%s Recived Bad Frame \n",__FUNCTION__);

    		release_frame(curr_frame);
    		curr_frame = 0;
    	}

    	/* ethernet_header *tmp = (ethernet_header *) RXT;
    	 ether_addr adres =  { 0x01, 0x60, 0x6E, 0x11, 0x01, 0x1F  };


    	 tmp->eth_src_addr = adres;
         TransmitPacket(RXT,rx_len);*/
    }


    if(interruption & 0x02)    //TX
    {
    	iow(ISR,0x02);
    	tx_control = 0;

    }
    if(interruption & 0x20)    //LinkChange
    {
    	iow(ISR,0x20);

/*    	iow(0x00,0x01);  // software reset
    	usleep(10);
    	  enable interrupts to activate DM9000 ~on
    	  iow(IMR, INTR_set2);    IMR REG. FFH PAR=1 only, or + PTM=1& PRM=1 enable RxTx interrupts
    	  iow(0xFF, 0x80);
    	   enable RX (Broadcast/ ALL_MULTICAST) ~go
    	  iow(RCR , RCR_set | RX_ENABLE | PASS_MULTICAST);   RCR REG. 05 RXEN Bit [0] = 1 to enable the RX machine/ filter */

    	//printf(" LinkCHange \n");
    	if(link_control == 0)
    		link_control = 1;
    	else
    		link_control = 0;
    }




    iow(IMR, INTR_set2);
}

int client_register()
{
	uint32_t output_size = 0;

	ncp_header m_ncp ;
	volatile frame * my_frame = get_free_frame();
	ncp_register(&m_ncp,NCP_HEADER_SIZE);

	my_frame->f_len = create_packiet(my_frame->f_data,ETHER_MAX_LEN,&m_ncp,NCP_HEADER_SIZE);
	TransmitPacket(my_frame->f_data, my_frame->f_len);
	release_frame(my_frame);
	return 0;
}

int arp_register()
{
	uint32_t output_size = 0;
	ncp_header m_ncp ;
	volatile frame * my_frame = get_free_frame();

	create_arp_req(my_frame);
	TransmitPacket(my_frame->f_data, my_frame->f_len);
	release_frame(my_frame);
	return 0;
}

int arp_response(frame * arp)
{
	frame *output;
	uint32_t output_size = 0;
	frame  *m_frame = get_free_frame();

	create_arp_res(arp,output);
	TransmitPacket(output->f_data, output->f_len);
	release_frame(output);
	return 0;



}

/**
 * Main processing loop
 */
int ethernet_main_loop()
{
	frame * my_frame =0 ;
	ethernet_header * eth =0;
	int a=0;
	printf("Info:Inited_app\n");
	  while (1)
	  {
	  //  TransmitPacket(TXT,0x40);
		  if(link_control == 0) // no ethernet connection
			  continue;
		  else
		  {
			  my_frame = get_rx_frame();
			  if(my_frame != 0)
			  {
				  eth = my_frame->f_data;
				  if(eth->ether_type == 0x0608)
				  {
					  printf("Info:ARP response\n");
					  arp_response(my_frame);
				  }
				  else if(eth->ether_type == 0x0008)
				  {
					  printf("Info:Frame to process\n");
					  manage_ncp_loop(my_frame->f_data,my_frame->f_len);
				  }
				  my_frame->f_len = 0;
				  release_frame(my_frame);
				  my_frame = 0;
			  }
			  else
				usleep(100);
		  }

		 if (tx_control == 0)
		 {
			 a++;
			 //a - zlicza pakiety
			 //tx_control - prosty mutex na wysy³anie pakietu, zwalniany gdy karta sieciowa przesle sygna³ potwierdzaj¹cy wys³anie pakietu

			// tx_control = 1;
			 //send_packiet();
		 }

		  if(a%100000 == 0)
		  {
			  printf("Working\n");

		  }
	/*
		  my_frame = get_rx_frame();
		  if (my_frame != NULL)
		  {
			  printf( "przetwarzam ramke ! \n");
			  release_frame(my_frame);
			  my_frame = 0;
		  }
	*/


		  //TransmitPacket(TXT,0x40);

	  }
	  return 0;
}



int main(void)
{
	int i = 0;

	tx_control = 0;
	link_control = 0;
	int a=0;


    unsigned char TXT[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                          0x01,0x60,0x6E,0x11,0x02,0x0F,
                          0x08,0x00,0x11,0x22,0x33,0x44,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x55,0x66,0x77,0x88,0x99,0xAA,
                          0x00,0x00,0x00,0x20 };
 // LCD_Test();
  init_lists();
  DM9000_init();



  //alt_irq_register( JTAG_UART_0_IRQ, NULL, NULL);
  //alt_irq_register( UART_0_IRQ, NULL, (void*)uart_interrupt);
//  alt_irq_register( TIMER_0_IRQ, NULL, NULL );
//  alt_irq_register( TIMER_1_IRQ, NULL, NULL);
//  alt_irq_register( BUTTON_PIO_IRQ, NULL, (void*)button_interrupt );
 alt_irq_register( DM9000A_IRQ, NULL, (void*)ethernet_interrupts_simple );
//  alt_irq_disable_all();

  for(i=0; i <2 ; i++)
    {
  	  arp_register();
  	  client_register();
    }
  printf("Client registered\n");


  return ethernet_main_loop();
 // return 0;
}

//-------------------------------------------------------------------------


