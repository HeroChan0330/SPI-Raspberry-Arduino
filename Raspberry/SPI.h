#ifndef _SPI_H_
#define _SPI_H_


#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

void spiInit();
unsigned char spiTxRx(unsigned char txDat);

#endif