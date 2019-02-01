
/**
  SPI2 Generated Driver API Source File

  Company:
    Microchip Technology Inc.

  File Name:
    spi2.c

  @Summary
    This is the generated source file for the SPI2 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for SPI2.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.35
        Device            :  PIC32MM0064GPL028
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB             :  MPLAB X 3.60
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "spi2.h"

/**
 Section: File specific functions
*/

/**
  SPI2 Transfer Mode Enumeration

  @Summary
    Defines the Transfer Mode enumeration for SPI2.

  @Description
    This defines the Transfer Mode enumeration for SPI2.
 */
typedef enum {
    SPI2_TRANSFER_MODE_32BIT  = 2,
    SPI2_TRANSFER_MODE_16BIT = 1,
    SPI2_TRANSFER_MODE_8BIT = 0
}SPI2_TRANSFER_MODE;

inline __attribute__((__always_inline__)) SPI2_TRANSFER_MODE SPI2_TransferModeGet(void);
void SPI2_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
uint16_t SPI2_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData);

/**
 Section: Driver Interface Function Definitions
*/


void SPI2_Initialize (void)
{
    // FRMERR disabled; 
    SPI2STAT = 0x0;
    // SPI2BRG 5; 
    SPI2BRG = 0x5;
    // AUDMONO disabled; AUDEN disabled; SPITUREN disabled; FRMERREN disabled; IGNROV disabled; SPISGNEXT disabled; SPIROVEN disabled; AUDMOD disabled; IGNTUR disabled; 
    SPI2CON2 = 0x0;
    // MCLKSEL PBCLK; DISSDO disabled; SRXISEL Last Word is Read; CKP Idle:Low, Active:High; FRMEN disabled; FRMSYPW One-Clock; SSEN disabled; FRMCNT 1; MSSEN disabled; MSTEN Master; MODE16 disabled; FRMPOL disabled; SMP Middle; SIDL disabled; FRMSYNC disabled; CKE Idle to Active; MODE32 disabled; SPIFE Frame Sync pulse precedes; STXISEL Complete; DISSDI disabled; ON enabled; ENHBUF enabled; 
    SPI2CON = 0x18020;

}


void SPI2_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData )
{

    while( SPI2STATbits.SPITBF == true )
    {

    }

    if (SPI2_TransferModeGet() == SPI2_TRANSFER_MODE_32BIT)
    {
        SPI2BUF = *((uint32_t*)pTransmitData);
    }
    else if (SPI2_TransferModeGet() == SPI2_TRANSFER_MODE_16BIT)
    {
        SPI2BUF = *((uint16_t*)pTransmitData);
    }
    else
    {
        SPI2BUF = *((uint8_t*)pTransmitData);
    }

    while ( SPI2STATbits.SPIRBE == true)
    {
    
    }


    if (SPI2_TransferModeGet() == SPI2_TRANSFER_MODE_32BIT)
    {
        *((uint32_t*)pReceiveData) = SPI2BUF;
    }
    else if (SPI2_TransferModeGet() == SPI2_TRANSFER_MODE_16BIT)
    {
        *((uint16_t*)pReceiveData) = SPI2BUF;
    }
    else
    {
        *((uint8_t*)pReceiveData) = SPI2BUF;
    }

}

uint16_t SPI2_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData)
{

    uint16_t dataSentCount = 0;
    uint16_t count = 0;
    uint16_t dummyDataReceived = 0;
    uint16_t dummyDataTransmit = SPI2_DUMMY_DATA;
    
    uint8_t fifoMultiplier = 1;

    uint8_t  *pSend, *pReceived;
    uint16_t addressIncrement;
    uint16_t receiveAddressIncrement, sendAddressIncrement;

    SPI2_TRANSFER_MODE spiModeStatus;

    spiModeStatus = SPI2_TransferModeGet();
    // set up the address increment variable
    if (spiModeStatus == SPI2_TRANSFER_MODE_32BIT)
    {
        addressIncrement = 4;
        byteCount >>= 2;
        fifoMultiplier = 1;
    }  
    else if (spiModeStatus == SPI2_TRANSFER_MODE_16BIT)
    {
        addressIncrement = 2;
        byteCount >>= 1;
        fifoMultiplier = 2;
    }        
    else
    {
        addressIncrement = 1;
        fifoMultiplier = 4;
    }

    // set the pointers and increment delta 
    // for transmit and receive operations
    if (pTransmitData == NULL)
    {
        sendAddressIncrement = 0;
        pSend = (uint8_t*)&dummyDataTransmit;
    }
    else
    {
        sendAddressIncrement = addressIncrement;
        pSend = (uint8_t*)pTransmitData;
    }
        
    if (pReceiveData == NULL)
    {
       receiveAddressIncrement = 0;
       pReceived = (uint8_t*)&dummyDataReceived;
    }
    else
    {
       receiveAddressIncrement = addressIncrement;        
       pReceived = (uint8_t*)pReceiveData;
    }


    while( SPI2STATbits.SPITBF == true )
    {

    }

    while (dataSentCount < byteCount)
    {
        if ((count < ((SPI2_FIFO_FILL_LIMIT)*fifoMultiplier)))
        {
            if (spiModeStatus == SPI2_TRANSFER_MODE_32BIT)
            {
                SPI2BUF = *((uint32_t*)pSend);
            }
            else if (spiModeStatus == SPI2_TRANSFER_MODE_16BIT)
            {
                SPI2BUF = *((uint16_t*)pSend);
            }
            else
            {
                SPI2BUF = *pSend;
            }
            pSend += sendAddressIncrement;
            dataSentCount++;
            count++;
        }

        if (SPI2STATbits.SPIRBE == false)
        {
            if (spiModeStatus == SPI2_TRANSFER_MODE_32BIT)
            {
                *((uint32_t*)pReceived) = SPI2BUF;
            }
            else if (spiModeStatus == SPI2_TRANSFER_MODE_16BIT)
            {
                *((uint16_t*)pReceived) = SPI2BUF;
            }
            else
            {
                *pReceived = SPI2BUF;
            }
            pReceived += receiveAddressIncrement;
            count--;
        }

    }
    while (count)
    {
        if (SPI2STATbits.SPIRBE == false)
        {
            if (spiModeStatus == SPI2_TRANSFER_MODE_32BIT)
            {
                *((uint32_t*)pReceived) = SPI2BUF;
            }
            else if (spiModeStatus == SPI2_TRANSFER_MODE_16BIT)
            {
                *((uint16_t*)pReceived) = SPI2BUF;
            }
            else
            {
                *pReceived = SPI2BUF;
            }
            pReceived += receiveAddressIncrement;
            count--;
        }
    }

    return dataSentCount;
}



uint8_t SPI2_Exchange8bit( uint8_t data )
{
    uint8_t receiveData;
    
    SPI2_Exchange(&data, &receiveData);

    return (receiveData);
}


uint16_t SPI2_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived)
{
    return (SPI2_ExchangeBuffer(dataTransmitted, byteCount, dataReceived));
}

inline __attribute__((__always_inline__)) SPI2_TRANSFER_MODE SPI2_TransferModeGet(void)
{
    if (SPI2CONbits.MODE32 == 1)
        return SPI2_TRANSFER_MODE_32BIT;
    else if (SPI2CONbits.MODE16 == 1)
        return SPI2_TRANSFER_MODE_16BIT;
    else
        return SPI2_TRANSFER_MODE_8BIT;
}

SPI2_STATUS SPI2_StatusGet()
{
    return(SPI2STAT);
}

/**
 End of File
*/
