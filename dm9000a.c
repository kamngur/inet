#include <io.h>
#include <stdio.h>
#include <stdint.h>
#include "inet/config.h"

#include "dm9000a.h"


#include "system.h"
#define msleep(msec)                                  usleep(1000*msec);
unsigned char ether_addres[6]={ 0x01, 0x60, 0x6E, 0x11, 0x01, 0x1F  };
//-------------------------------------------------------------------------
unsigned int   ior100(unsigned int reg)
{

  IOWR (DM9000A_BASE,IO_addr,reg);
  usleep(STD_DELAY);
  return IORD (DM9000A_BASE,IO_data);
}
//-------------------------------------------------------------------------
void fiow(unsigned int reg, unsigned int data)
{

  IOWR (DM9000A_BASE,IO_addr,reg);
//  usleep(STD_DELAY);
  IOWR (DM9000A_BASE,IO_data,data);
}
//-------------------------------------------------------------------------
unsigned int fior(unsigned int reg)
{

	unsigned char i = 0;
  IOWR (DM9000A_BASE,IO_addr,reg);
// usleep(STD_DELAY);
  i = IORD (DM9000A_BASE,IO_data);

  return i;
}
//-------------------------------------------------------------------------
void iow(unsigned int reg, unsigned int data)
{
  IOWR (DM9000A_BASE,IO_addr,reg);
  //usleep(STD_DELAY);
  IOWR (DM9000A_BASE,IO_data,data);
}
//-------------------------------------------------------------------------
unsigned int ior(unsigned int reg)
{
	unsigned char i = 0;
  IOWR (DM9000A_BASE,IO_addr,reg);

  i = IORD (DM9000A_BASE,IO_data);


  return i;
}
//-------------------------------------------------------------------------
void phy_write (unsigned int reg, unsigned int value)
{ 
  /* set PHY register address into EPAR REG. 0CH */
  iow(0x0C, reg | 0x40);              /* PHY register address setting, and DM9000_PHY offset = 0x40 */
  
  /* fill PHY WRITE data into EPDR REG. 0EH & REG. 0DH */
  iow(0x0E, ((value >> 8) & 0xFF));   /* PHY data high_byte */
  iow(0x0D, value & 0xFF);            /* PHY data low_byte */

  /* issue PHY + WRITE command = 0xa into EPCR REG. 0BH */
  iow(0x0B, 0x8);                     /* clear PHY command first */
  IOWR (DM9000A_BASE, IO_data, 0x0A);  /* issue PHY + WRITE command */
  usleep(STD_DELAY);
  IOWR (DM9000A_BASE, IO_data, 0x08);  /* clear PHY command again */
  usleep(30);  /* wait 1~30 us (>20 us) for PHY + WRITE completion */
}

//-------------------------------------------------------------------------
void DM9000_read_registers()
{

	ior(0x28);
	ior(0x29);
	printf("product");
	ior(0x2A);
	ior(0x2B);
}

unsigned int DM9000_reset(void)
{
	  /* set the internal PHY power-on (GPIOs normal settings) */
	  iow(0x1E, 0x01);  /* GPCR REG. 1EH = 1 selected GPIO0 "output" port for internal PHY */
	  iow(0x1F, 0x00);  /* GPR  REG. 1FH GEPIO0 Bit [0] = 0 to activate internal PHY */
	  msleep(10);        /* wait > 2 ms for PHY power-up ready */

	  /* software-RESET NIC */
	  iow(NCR, 0x03);   /* NCR REG. 00 RST Bit [0] = 1 reset on, and LBK Bit [2:1] = 01b MAC loopback on */
	  usleep(20);       /* wait > 10us for a software-RESET ok */

	  iow(NCR, 0x00);   /* normalize */
	  iow(NCR, 0x03);
	  usleep(20);
	  iow(NCR, 0x00);

	  /* set GPIO0=1 then GPIO0=0 to turn off and on the internal PHY */
	  iow(0x1F, 0x01);  /* GPR PHYPD Bit [0] = 1 turn-off PHY */
	  iow(0x1F, 0x00);  /* PHYPD Bit [0] = 0 activate phyxcer */
	  msleep(10);       /* wait >4 ms for PHY power-up */

	  /* set PHY operation mode */
	  phy_write(0,PHY_reset);   /* reset PHY: registers back to the default states */
	  usleep(50);               /* wait >30 us for PHY software-RESET ok */
	  phy_write(32, 0x404);     /* turn off PHY reduce-power-down mode only */
	  phy_write(4, PHY_txab);   /* set PHY TX advertised ability: ALL + Flow_control */
	  phy_write(0, 0x1200);     /* PHY auto-NEGO re-start enable (RESTART_AUTO_NEGOTIATION + AUTO_NEGOTIATION_ENABLE) to auto sense and recovery PHY registers */
	  msleep(5);               /* wait >2 ms for PHY auto-sense linking to partner */
	return 0;
}
//-------------------------------------------------------------------------
/* DM9000_init I/O routine */
/*unsigned int DM9000_init (void)   initialize DM9000 LAN chip
{
  unsigned int  i;

   set the internal PHY power-on (GPIOs normal settings)
  iow(0x1E, 0x01);   GPCR REG. 1EH = 1 selected GPIO0 "output" port for internal PHY
  iow(0x1F, 0x00);   GPR  REG. 1FH GEPIO0 Bit [0] = 0 to activate internal PHY
  msleep(5);         wait > 2 ms for PHY power-up ready

   software-RESET NIC
  iow(NCR, 0x03);    NCR REG. 00 RST Bit [0] = 1 reset on, and LBK Bit [2:1] = 01b MAC loopback on
  usleep(20);        wait > 10us for a software-RESET ok
  iow(NCR, 0x00);    normalize
  iow(NCR, 0x03);
  usleep(20);
  iow(NCR, 0x00);

   set GPIO0=1 then GPIO0=0 to turn off and on the internal PHY
  iow(0x1F, 0x01);   GPR PHYPD Bit [0] = 1 turn-off PHY
  iow(0x1F, 0x00);   PHYPD Bit [0] = 0 activate phyxcer
  msleep(10);        wait >4 ms for PHY power-up

   set PHY operation mode
  phy_write(0,PHY_reset);    reset PHY: registers back to the default states
  usleep(50);                wait >30 us for PHY software-RESET ok
  phy_write(16, 0x404);      turn off PHY reduce-power-down mode only
  phy_write(4, PHY_txab);    set PHY TX advertised ability: ALL + Flow_control
  phy_write(0, 0x1200);      PHY auto-NEGO re-start enable (RESTART_AUTO_NEGOTIATION + AUTO_NEGOTIATION_ENABLE) to auto sense and recovery PHY registers
  msleep(5);                wait >2 ms for PHY auto-sense linking to partner

   store MAC address into NIC
  for (i = 0; i < 6; i++)
   iow(32 + i, ether_addres[i]);

   clear any pending interrupt
  iow(ISR, 0x3F);      clear the ISR status: PRS, PTS, ROS, ROOS 4 bits, by RW/C1
  iow(NSR, 0x2C);      clear the TX status: TX1END, TX2END, WAKEUP 3 bits, by RW/C1

   program operating registers~
  iow(NCR,  NCR_set);    NCR REG. 00 enable the chip functions (and disable this MAC loopback mode back to normal)
  iow(0x08, BPTR_set);   BPTR  REG.08  (if necessary) RX Back Pressure Threshold in Half duplex moe only: High Water 3KB, 600 us
  iow(0x09, FCTR_set);   FCTR  REG.09  (if necessary) Flow Control Threshold setting High/ Low Water Overflow 5KB/ 10KB
  iow(0x0A, RTFCR_set);  RTFCR REG.0AH (if necessary) RX/TX Flow Control Register enable TXPEN, BKPM (TX_Half), FLCE (RX)
  iow(0x0F, 0x00);       Clear the all Event
  iow(0x2D, 0x80);       Switch LED to mode 1

   set other registers depending on applications
  iow(ETXCSR, ETXCSR_set);  Early Transmit 75%

   enable interrupts to activate DM9000 ~on
  iow(IMR, INTR_set);    IMR REG. FFH PAR=1 only, or + PTM=1& PRM=1 enable RxTx interrupts

   enable RX (Broadcast/ ALL_MULTICAST) ~go
  iow(RCR , RCR_set | RX_ENABLE | PASS_MULTICAST);   RCR REG. 05 RXEN Bit [0] = 1 to enable the RX machine/ filter

   RETURN "DEVICE_SUCCESS" back to upper layer
  return  (ior(0x2D)==0x80) ? DMFE_SUCCESS : DMFE_FAIL;
}*/
//-------------------------------------------------------------------------
//----------------------------
 //DM9000_init I/O routine
unsigned int DM9000_init (void) //  initialize DM9000 LAN chip
{
  unsigned int  i;

  DM9000_reset();
  for (i = 0; i < 6; i++)
   ior(32 + i);
 //  store MAC address into NIC

  for (i = 0; i < 6; i++)
  iow(32 + i, ether_addres[i]);
 // for (i = 0; i < 6; i++)
 //  ior(32 + i);
 //  clear any pending interrupt
  iow(ISR, 0x3F);      //clear the ISR status: PRS, PTS, ROS, ROOS 4 bits, by RW/C1
  iow(NSR, 0x2C);     // clear the TX status: TX1END, TX2END, WAKEUP 3 bits, by RW/C1

  // program operating registers~
  iow(NCR,  NCR_set);   // NCR REG. 00 enable the chip functions (and disable this MAC loopback mode back to normal)
  iow(0x08, BPTR_set);  // BPTR  REG.08  (if necessary) RX Back Pressure Threshold in Half duplex moe only: High Water 3KB, 600 us
  iow(0x09, FCTR_set); //  FCTR  REG.09  (if necessary) Flow Control Threshold setting High/ Low Water Overflow 5KB/ 10KB
  iow(0x0A, RTFCR_set);//  RTFCR REG.0AH (if necessary) RX/TX Flow Control Register enable TXPEN, BKPM (TX_Half), FLCE (RX)
  iow(0x0F, 0x00);     //  Clear the all Event
  iow(0x2D, 0x80);     //  Switch LED to mode 1
//
  iow(OTCR, OTCR_set);  // 100Mhz Internal clock

//   set other registers depending on applications
  iow(ETXCSR, ETXCSR_set);  //Early Transmit 75%
  iow(TCSCR, TX_CRC_set);  // UDP and IP Checksum active.

//   enable interrupts to activate DM9000 ~on
  iow(IMR, INTR_set2);   // IMR REG. FFH PAR=1 only, or + PTM=1& PRM=1 enable RxTx interrupts

//   enable RX (Broadcast/ ALL_MULTICAST) ~go
  iow(RCR , RCR_set | RX_ENABLE );  // RCR REG. 05 RXEN Bit [0] = 1 to enable the RX machine/ filter


 //  RETURN "DEVICE_SUCCESS" back to upper layer
 // printf("\n");
  i= ior(0x2D);

  return  (i & 0x80) ? DMFE_SUCCESS : DMFE_FAIL;
}
//-------------------------------------------------------------------------
/*  Transmit one Packet TX I/O routine  */
unsigned int  TransmitPacket(unsigned char *data_ptr,unsigned int tx_len)
{
  unsigned int  i;
  
   /* mask NIC interrupts IMR: PAR only */
  fiow(IMR, PAR_set);
  
  /* issue TX packet's length into TXPLH REG. FDH & TXPLL REG. FCH */
  fiow(0xFD, (tx_len >> 8) & 0xFF);  /* TXPLH High_byte length */
  fiow(0xFC, tx_len & 0xFF);         /* TXPLL Low_byte  length */

  /* write transmit data to chip SRAM */
  IOWR (DM9000A_BASE, IO_addr, MWCMD);   /* set MWCMD REG. F8H TX I/O port ready */
  for (i = 0; i < tx_len; i += 2)
  {
    IOWR (DM9000A_BASE, IO_data, (data_ptr[i+1]<<8)|data_ptr[i] );
  }

  /* issue TX polling  command activated */
  fiow(TCR , TCR_set |0x02 | 0x08 );  /* TXCR Bit [0] TXREQ auto clear after TX completed */

  /* wait TX transmit done */
  while(!(fior(NSR)&0x0C));

  /* clear the NSR Register */
  fiow(NSR,0x00);
  
  /* re-enable NIC interrupts */
 fiow(IMR, INTR_set2);

  /* RETURN "TX_SUCCESS" to upper layer */
  return  DMFE_SUCCESS;
}




//-------------------------------------------------------------------------
/* Receive One Packet I/O routine */
unsigned int  ReceivePacket (unsigned char *data_ptr,unsigned int *rx_len)
{
  unsigned char rx_READY,GoodPacket;
  unsigned int  Tmp, RxStatus, i;
  
  RxStatus = rx_len[0] = 0;
  GoodPacket=FALSE;

   /* mask NIC interrupts IMR: PAR only */
  iow(IMR, PAR_set);
  
  /* dummy read a byte from MRCMDX REG. F0H */
  rx_READY = ior(MRCMDX);
  
  /* got most updated byte: rx_READY */
  rx_READY = IORD (DM9000A_BASE,IO_data)&0x03;
 // usleep(STD_DELAY);
  
  /* check if (rx_READY == 0x01): Received Packet READY? */
  if (rx_READY == DM9000_PKT_READY)
  {
    /* got RX_Status & RX_Length from RX SRAM */
    IOWR (DM9000A_BASE, IO_addr, MRCMD); /* set MRCMD REG. F2H RX I/O port ready */
   // usleep(STD_DELAY);
    RxStatus = IORD (DM9000A_BASE,IO_data);
  //  usleep(STD_DELAY);
    rx_len[0] = IORD (DM9000A_BASE,IO_data);

    /* Check this packet_status GOOD or BAD? */
    if ( !(RxStatus & 0xBF00) && (rx_len[0] < MAX_PACKET_SIZE) )
    {
      /* read 1 received packet from RX SRAM into RX buffer */
      for (i = 0; i < rx_len[0] ; i += 2)
      {
        //usleep(STD_DELAY);
        Tmp = IORD (DM9000A_BASE, IO_data);
        data_ptr[i] = Tmp&0xFF;
        data_ptr[i+1] = (Tmp>>8)&0xFF;
      }
      GoodPacket=TRUE;
    } /* end if (GoodPacket) */
    else
    {
      /* this packet is bad, dump it from RX SRAM */
      for (i = 0; i < rx_len[0]; i += 2)
      {
       // usleep(STD_DELAY);
        Tmp = IORD (DM9000A_BASE, IO_data);
      }
      printf("\nError\n");
      rx_len[0] = 0;
    } /* end if (!GoodPacket) */
  } /* end if (rx_READY == DM9000_PKT_READY) ok */
  else if(rx_READY) /* status check first byte: rx_READY Bit[1:0] must be "00"b or "01"b */
  {
    /* software-RESET NIC */
    iow(NCR, 0x03);   /* NCR REG. 00 RST Bit [0] = 1 reset on, and LBK Bit [2:1] = 01b MAC loopback on */
    usleep(20);       /* wait > 10us for a software-RESET ok */
    iow(NCR, 0x00);   /* normalize */
    iow(NCR, 0x03);
    usleep(20);
    iow(NCR, 0x00);    
    /* program operating registers~ */
    iow(NCR,  NCR_set);   /* NCR REG. 00 enable the chip functions (and disable this MAC loopback mode back to normal) */
    iow(0x08, BPTR_set);  /* BPTR  REG.08  (if necessary) RX Back Pressure Threshold in Half duplex moe only: High Water 3KB, 600 us */
    iow(0x09, FCTR_set);  /* FCTR  REG.09  (if necessary) Flow Control Threshold setting High/ Low Water Overflow 5KB/ 10KB */
    iow(0x0A, RTFCR_set); /* RTFCR REG.0AH (if necessary) RX/TX Flow Control Register enable TXPEN, BKPM (TX_Half), FLCE (RX) */
    iow(0x0F, 0x00);      /* Clear the all Event */
    iow(0x2D, 0x80);      /* Switch LED to mode 1 */
    /* set other registers depending on applications */
    iow(ETXCSR, ETXCSR_set); /* Early Transmit 75% */
    iow(TCSCR, TX_CRC_set);  /* UDP and IP Checksum active. */
    /* enable interrupts to activate DM9000 ~on */
   // iow(IMR, INTR_set);   /* IMR REG. FFH PAR=1 only, or + PTM=1& PRM=1 enable RxTx interrupts */
    /* enable RX (Broadcast/ ALL_MULTICAST) ~go */
    iow(RCR , RCR_set | RX_ENABLE );  /* RCR REG. 05 RXEN Bit [0] = 1 to enable the RX machine/ filter */
  } /* end NIC H/W system Data-Bus error */
  iow(IMR, INTR_set2);
  return GoodPacket ? DMFE_SUCCESS : DMFE_FAIL;
}
//-------------------------------------------------------------------------

struct dm9000_rxhdr {
uint8_t		RxPktReady;
uint8_t		RxStatus;
uint16_t 	RxLen;
};


//-------------------------------------------------------------------------
/* Receive One Packet I/O routine */
unsigned int ReceivePacket2(unsigned char *data_ptr, unsigned int *rx_len) {
	unsigned char rx_READY, GoodPacket;
	unsigned int Tmp, RxStatus, i;
	uint16_t RxLen;

	do {
		RxStatus = RxLen = 0;
		GoodPacket = FALSE;

		/* mask NIC interrupts IMR: PAR only */
		//iow(IMR, PAR_set);

		/* dummy read a byte from MRCMDX REG. F0H */
		rx_READY = ior(MRCMDX);

		/* got most updated byte: rx_READY */
		rx_READY = IORD (DM9000A_BASE,IO_data);

		if (rx_READY > DM9000_PKT_READY) /*This byte must be 0x00 or 0x01. If diffrent device is unstable need reset*/
		{

			printf("\nReceivePacket warning rx_READY : %02x ", rx_READY);


		    /* software-RESET NIC */
		    iow(NCR, 0x03);   /* NCR REG. 00 RST Bit [0] = 1 reset on, and LBK Bit [2:1] = 01b MAC loopback on */
		    usleep(20);       /* wait > 10us for a software-RESET ok */
		    iow(NCR, 0x00);   /* normalize */
		    iow(NCR, 0x03);
		    usleep(20);
		    iow(NCR, 0x00);
		    /* program operating registers~ */
		    iow(NCR,  NCR_set);   /* NCR REG. 00 enable the chip functions (and disable this MAC loopback mode back to normal) */
		    iow(0x08, BPTR_set);  /* BPTR  REG.08  (if necessary) RX Back Pressure Threshold in Half duplex moe only: High Water 3KB, 600 us */
		    iow(0x09, FCTR_set);  /* FCTR  REG.09  (if necessary) Flow Control Threshold setting High/ Low Water Overflow 5KB/ 10KB */
		    iow(0x0A, RTFCR_set); /* RTFCR REG.0AH (if necessary) RX/TX Flow Control Register enable TXPEN, BKPM (TX_Half), FLCE (RX) */
		    iow(0x0F, 0x00);      /* Clear the all Event */
		    iow(0x2D, 0x80);      /* Switch LED to mode 1 */
		    /* set other registers depending on applications */
		    iow(ETXCSR, ETXCSR_set); /* Early Transmit 75% */
		    iow(TCSCR, TX_CRC_set);  /* UDP and IP Checksum active. */
		    /* enable interrupts to activate DM9000 ~on */
		  //  iow(IMR, INTR_set);   /* IMR REG. FFH PAR=1 only, or + PTM=1& PRM=1 enable RxTx interrupts */
		    /* enable RX (Broadcast/ ALL_MULTICAST) ~go */
		    iow(RCR , RCR_set | RX_ENABLE );  /* RCR REG. 05 RXEN Bit [0] = 1 to enable the RX machine/ filter */
		    return DMFE_FAIL;
		}

		if (rx_READY != DM9000_PKT_READY) {
			/* No more packet*/
			printf("\nReceivePacket warning rx_READY : %02x ", rx_READY);
			return DMFE_FAIL;
		}
		/* Packet/packets is ready now get status and length*/
		GoodPacket = TRUE;
		IOWR(DM9000A_BASE, IO_addr, MRCMD);
		/* set MRCMD REG. F2H RX I/O port ready */
		usleep(STD_DELAY);
		RxStatus = IORD (DM9000A_BASE,IO_data);
		usleep(STD_DELAY);
		RxLen = IORD (DM9000A_BASE,IO_data);
		if (RxLen < 0x40) {
			GoodPacket = FALSE;
			printf("\nReceivePacket bad packet 1");
		}

		if (RxLen > MAX_PACKET_SIZE) {

		}

		/* RxStatus is identical to RSR register. */
		//if (RxStatus & (RSR_FOE | RSR_CE | RSR_AE | RSR_PLE | RSR_RWTO | RSR_LCS | RSR_RF))
		if (RxStatus & ( RSR_CE | RSR_AE | RSR_PLE | RSR_RWTO | RSR_LCS | RSR_RF))
		{
			GoodPacket = FALSE;
			if (RxStatus & RSR_FOE) {

				printf("fifo error\n");

			}
			if (RxStatus & RSR_CE) {

				printf("crc error\n");

			}
			if (RxStatus & RSR_RF) {

				printf("length error\n");

			}
		}

		*rx_len = RxLen;
		if(GoodPacket)
		{
			/*Read Packet*/
			// printf("\nRX Pck Len = %d\n",rx_len[0]);
			// printf("\nRX : %d\n",RxLen);
		      for (i = 0; i < RxLen ; i += 2)
		      {
		        //usleep(STD_DELAY);
		        Tmp = IORD (DM9000A_BASE, IO_data);
		        data_ptr[i] = Tmp&0xFF;
		        data_ptr[i+1] = (Tmp>>8)&0xFF;
		       // if(i%8==0)
		        //     printf("\n");
		      //  printf("%02X",data_ptr[i]);
		      //  printf("%02X",data_ptr[i+1]);
		      }
		}
		else
		{
			/*Dump wrong packed*/
			printf("\nReceivePacket dumping packet");

		      for (i = 0; i < RxLen ; i += 2)
		      {
		        //usleep(STD_DELAY);
		        Tmp = IORD (DM9000A_BASE, IO_data);
		      }
		}

	} while (rx_READY == DM9000_PKT_READY);

	return GoodPacket ? DMFE_SUCCESS : DMFE_FAIL;
}
//-------------------------------------------------------------------------


