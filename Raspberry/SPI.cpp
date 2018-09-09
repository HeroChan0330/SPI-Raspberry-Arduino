/**********************************************************
 SPI_Hello_Arduino
   Configures an Raspberry Pi as an SPI master and  
   demonstrates bidirectional communication with an 
   Arduino Slave by repeatedly sending the text
   "Hello Arduino" and receiving a response
   
Compile String:
g++ -o SPI_Hello_Arduino SPI_Hello_Arduino.cpp
***********************************************************/
#include "SPI.h"


using namespace std;

/**********************************************************
Declare Global Variables
***********************************************************/
int fd;

void spiInit(){
  fd = open("/dev/spidev0.0", O_RDWR);
  unsigned int speed = 1000000;
  ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

/**********************************************************
spiTxRx
 Transmits one byte via the SPI device, and returns one byte
 as the result.

 Establishes a data structure, spi_ioc_transfer as defined
 by spidev.h and loads the various members to pass the data
 and configuration parameters to the SPI device via IOCTL

 Local variables txDat and rxDat are defined and passed by
 reference.  
***********************************************************/

unsigned char spiTxRx(unsigned char txDat)
{
  unsigned char rxDat;
  struct spi_ioc_transfer spi;
  memset (&spi, 0, sizeof (spi));
  spi.tx_buf        = (unsigned long)&txDat;
  spi.rx_buf        = (unsigned long)&rxDat;
  spi.len           = 1;
  ioctl (fd, SPI_IOC_MESSAGE(1), &spi);
  return rxDat;
}
