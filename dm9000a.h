#ifndef   __DM9000A_H__
#define   __DM9000A_H__



#include <io.h>     //IOWR IORD
#include "system.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"

#include <sys/alt_cache.h>
#include <unistd.h> //usleep msleep





#define IO_addr     0
#define IO_data     1

#define NCR         0x00  /* Network  Control Register REG. 00 */
#define NSR         0x01  /* Network  Status Register  REG. 01 */
#define TCR         0x02  /* Transmit Control Register REG. 02 */
#define TCR2         0x03  /* Transmit Status Register 1 REG. 03 */

#define RCR         0x05  /* Receive  Control Register REG. 05 */
#define ETXCSR      0x30  /* TX early Control Register REG. 30 */
#define TCSCR       0x31  /* Checksum Control Register REG. 31 */
#define RCSCSR      0x32  /* RX CheckSum Control Status Register */
#define MRCMDX      0xF0  /* RX FIFO I/O port command READ  for dummy read a byte from RX SRAM */
#define MRCMD       0xF2  /* RX FIFO I/O port command READ  from RX SRAM */
#define MWCMD       0xF8  /* TX FIFO I/O port command WRITE into TX FIFO */
#define TXPLL		0xFC  /* TX Packet Length Low Byte Register */
#define TXPLH		0xFD  /* TX Packet Length High Byte Register */
#define ISR         0xFE  /* NIC Interrupt Status Register REG. FEH */
#define IMR         0xFF  /* NIC Interrupt Mask   Register REG. FFH */
#define OTCR        0x2E  /* NIC Operation Test Control Register REG. 2EH */


/*Recive packet status errors */
#define RSR_RF              (0x80) /*Packet length error*/
#define RSR_MF              (0x40)
#define RSR_LCS             (0x20)
#define RSR_RWTO            (0x10)
#define RSR_PLE             (0x08)
#define RSR_AE              (0x04)
#define RSR_CE              (0x02) /*CRC error*/
#define RSR_FOE             (0x01) /*Fifo error*/

#define NCR_set         0x00
#define TCR_set         0x01
#define TCR_CRC1		0x01
#define TCR_CRC2		0x04
#define TX_REQUEST      0x01  /* TCR REG. 02 TXREQ Bit [0] = 1 polling Transmit Request command */
#define TCR_long        0x40  /* packet disable TX Jabber Timer */
#define RCR_set         0x30  /* skip CRC_packet and skip LONG_packet */
#define RX_ENABLE       0x01  /* RCR REG. 05 RXEN Bit [0] = 1 to enable RX machine */
#define RCR_long        0x40  /* packet disable RX Watchdog Timer */
#define PASS_MULTICAST  0x08  /* RCR REG. 05 PASS_ALL_MULTICAST Bit [3] = 1: RCR_set value ORed 0x08 */
#define BPTR_set        0x3F  /* BPTR REG. 08 RX Back Pressure Threshold: High Water Overflow Threshold setting 3KB and Jam_Pattern_Time = 600 us */
#define FCTR_set        0x5A  /* FCTR REG. 09 High/ Low Water Overflow Threshold setting 5KB/ 10KB */
#define RTFCR_set       0x29  /* RTFCR REG. 0AH RX/TX Flow Control Register enable TXPEN + BKPM(TX_Half) + FLCE(RX) */
#define ETXCSR_set      0x03  /* Early Transmit Bit [7] Enable and Threshold 0~3: 12.5%, 25%, 50%, 75% */
#define TCSCR_set       0x04  /* Checksum for UDP and  not for IP */
#define TX_CRC_set      0x00  /* TCP/UDP/IP CheckSum Generation Disable */
#define RX_DCSE         0x03  /* Discard CheckSum Error packet */
#define INTR_set        0x81  /* IMR REG. FFH: PAR +PRM, or 0x83: PAR + PRM + PTM */
#define INTR_set2        0xa3  /* IMR REG. FFH: PAR +PRM, or 0x83: PAR + PRM + PTM+ LINKCHGI */

// Warning !! 0x80 Have the PRM disabled
#define PAR_set         0x80  /* IMR REG. FFH: PAR only, RX/TX FIFO R/W Pointer Auto Return enable */
#define OTCR_set        0x80  /* System Clock set = 100Mhz */

#define PHY_reset       0x8000  /* PHY reset: some registers back to default value */
#define PHY_txab        0x05e1  /* set PHY TX advertised ability: Full-capability + Flow-control (if necessary) */
#define PHY_mode        0x3100  /* set PHY media mode: Auto negotiation (AUTO sense) */

#define STD_DELAY       20    /* standard delay 20 us */

#define DMFE_SUCCESS    0
#define DMFE_FAIL       1

#define TRUE            1
#define FALSE           0

#define DM9000_PKT_READY  0x01  /* packets ready to receive */
#define PACKET_MIN_SIZE   0x40  /* Received packet min size */
#define MAX_PACKET_SIZE   1536  /* RX largest legal size packet with fcs & QoS */
#define DM9000_PKT_MAX    3072  /* TX 1 packet max size without 4-byte CRC */
/*
#ifndef DM9000A_BASE
#define DM9000A_BASE DM9000A_0_BASE+3
#endif
*/
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void          fiow(unsigned int reg, unsigned int data);
void          iow(unsigned int reg, unsigned int data);
unsigned int fior(unsigned int reg);
unsigned int  ior(unsigned int reg);
unsigned int  ior100(unsigned int reg);
void          phy_write(unsigned int reg, unsigned int value);

/* DM9000_init I/O routine */
unsigned int  DM9000_init (void);
/* Transmit One Packet TX I/O routine */
unsigned int  TransmitPacket(unsigned char *data_ptr,unsigned int tx_len);
/* Receive One Packet I/O routine */
unsigned int  ReceivePacket (unsigned char *data_ptr,unsigned int *rx_len);

unsigned int  ReceivePacket2 (unsigned char *data_ptr,unsigned int *rx_len);
void DM9000_read_registers();

void DM9000A_interrupt(void * context, unsigned long id);
//-------------------------------------------------------------------------

#endif





