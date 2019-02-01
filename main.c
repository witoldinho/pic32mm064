/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.26
        Device            :  PIC32MM0064GPL028
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB             :  MPLAB X 3.45
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
#include <stdio.h>
#include "mcc_generated_files/mcc.h"
#include "Ethernet/wizchip_conf.h"
#include "Ethernet/socket.h"
#include "Ethernet/dhcp.h"
#include "wiznet/w5500_init.h"

//
#define _MAIN_DEBUG_ 1


#define MY_MAX_DHCP_RETRY			2
uint8_t my_dhcp_retry = 0;
//enum http_metoda {GETh=0,POST,POST_OK,FLASH,ODZYSK};

/*****************************************************************************
 * Private functions
 ****************************************************************************/

const char * const sArray[] = { (const char *)"1String-aaaaaaaaaaaaaaaaaaaaaaaaaaaaa", (const char *)"2String_potezny string                             ssssssssstring   zzzzz"};
//const char *wRamie[]={"dupa", "super"};
const char* psArray=NULL;

// Callback function : User defined DHCP functions
void my_ip_assign(void);
void my_ip_conflict(void);


//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
// For example of ioLibrary_BSD //
//////////////////////////////////
								// Initialize Network information and display it
//int32_t loopback_tcps(uint8_t, uint8_t*, uint16_t);		// Loopback TCP server
//int32_t loopback_udps(uint8_t, uint8_t*, uint16_t);		// Loopback UDP server
//////////////////////////////////
 bool run_user_applications = false;
   // int32_t ret;

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalDisable();
 // enum  http_metoda s=GETh;
    uint8_t tmp=0;
   int32_t ret = 0;
   //uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
  // uint8_t txsize[8] = {2,2,2,2,2,2,2,2};
   //uint8_t rxsize[8] = {2,2,2,2,2,2,2,2};
   uint8_t pDestaddr[4] = {192,168,0,200};
   int8_t num;

   uint8_t ping_reply_received = 0;
   
   W5500_Init();
   Net_Conf();
   //printf("\n\r komorka: %08x",komorka);
   ///////////////////////////////////////////                                     
   // Host dependent peripheral initialized //
   ///////////////////////////////////////////
  // platform_init();

PHYStatus_check_enable = true;

#ifdef _MAIN_DEBUG_
	uint8_t tmpstr[6] = {0,};

	ctlwizchip(CW_GET_ID,(void*)tmpstr);

    printf("\r\n=======================================\r\n");
	printf(" WIZnet %s EVB - DHCP client v%d.%.2d\r\n", tmpstr, VER_H, VER_L);
    printf("| %s |\r\n ",sArray[0]);
	printf("=======================================\r\n");

	//Display_Net_Conf(); // handled by dhcp_run function
#endif

	/* DHCP client Initialization */
	if(gWIZNETINFO.dhcp == NETINFO_DHCP)
	{
		DHCP_init(SOCK_DHCP, gDATABUF);
		// if you want different action instead default ip assign, update, conflict.
		// if cbfunc == 0, act as default.
		reg_dhcp_cbfunc(my_ip_assign, my_ip_assign, my_ip_conflict);

		run_user_applications = false; 	// flag for running user's code
	}
	else
	{
		// Static
#ifdef _MAIN_DEBUG_
		Display_Net_Conf();
#endif
		run_user_applications = true; 	
    }
	/*******************************/
	/* WIZnet W5500 Code Examples  */
	/* TCPS/UDPS IPRAW test     */
	/*******************************/
    printf("------------PING_TEST_START-----------------------\r\n");
   // tmp = ping_auto(0,pDestaddr);
    //tmp = ping_count(0,3,pDestaddr);
    if(tmp == SUCCESS)
    	printf("-----------PING TEST OK----------\r\n");
    else
        printf("----------ERROR  = %d----------\r\n",tmp);


    while (1)
    {
        // Add your application code
        /* PHY Status checker: Check every 'SEC_PHYSTATUS_CHECK' seconds */
		if(PHYStatus_check_flag)
		{
			PHYStatus_check_flag = false;
			PHYStatus_Check();
		}

    	/* DHCP */
		/* DHCP IP allocation and check the DHCP lease time (for IP renewal) */
    	if(gWIZNETINFO.dhcp == NETINFO_DHCP)
    	{
			switch(DHCP_run())
			{
				case DHCP_IP_ASSIGN:
				case DHCP_IP_CHANGED:
					/* If this block empty, act with default_ip_assign & default_ip_update */
					//
					// This example calls my_ip_assign in the two case.
					//
					// Add to ...
					//
					break;
				case DHCP_IP_LEASED:
					//
					// TODO: insert user's code here
					run_user_applications = true;
					//
					break;
				case DHCP_FAILED:
					/* ===== Example pseudo code =====  */
					// The below code can be replaced your code or omitted.
					// if omitted, retry to process DHCP
					my_dhcp_retry++;
					if(my_dhcp_retry > MY_MAX_DHCP_RETRY)
					{
						gWIZNETINFO.dhcp = NETINFO_STATIC;
						DHCP_stop();      // if restart, recall DHCP_init()
#ifdef _MAIN_DEBUG_
						printf(">> DHCP %d Failed\r\n", my_dhcp_retry);
						Net_Conf();
						Display_Net_Conf();   // print out static netinfo to serial
#endif
						my_dhcp_retry = 0;
					}
					break;
				default:
					break;
			}
    	}

    	// TODO: insert user's code here
    	if(run_user_applications)
    	{
            web_tcps(SOCKS_WEB,webBUF,PORT_HTTP);
    		

    		
    	} // End of user's code
	} // End of Main loop

   // return 0;
    }

/**
 End of File
*/

//void platform_init(void){         }
    //IO_RB10_SetHigh(); // linia cs dla Wizchip jako nieaktywna
    






void PHYStatus_Check(void)
{
	uint8_t tmp;
	static bool LED_status_backup;

	//LED_status_backup = RGBLED_enable;

	do
	{
		ctlwizchip(CW_GET_PHYLINK, (void*) &tmp);

		// Error indicator: LED Green ON when no PHY link detected
		if(tmp == PHY_LINK_OFF)
		{
			//RGBLED_enable = false;
		//	Board_LED_Set(2, true);
            printf("!");
		}
	}while(tmp == PHY_LINK_OFF);

	//RGBLED_enable = LED_status_backup;
}

/*******************************************************
 * @ brief Call back for ip assing & ip update from DHCP
 *******************************************************/
void my_ip_assign(void)
{
   getIPfromDHCP(gWIZNETINFO.ip);
   getGWfromDHCP(gWIZNETINFO.gw);
   getSNfromDHCP(gWIZNETINFO.sn);
   getDNSfromDHCP(gWIZNETINFO.dns);
   gWIZNETINFO.dhcp = NETINFO_DHCP;
   /* Network initialization */
   Net_Conf();      // apply from DHCP
#ifdef _MAIN_DEBUG_
   Display_Net_Conf();
   printf("DHCP LEASED TIME : %ld Sec.\r\n", getDHCPLeasetime());
   printf("\r\n");
#endif
}

/************************************
 * @ brief Call back for ip Conflict
 ************************************/
void my_ip_conflict(void)
{
#ifdef _MAIN_DEBUG_
	printf("CONFLICT IP from DHCP\r\n");
#endif
   //halt or reset or any...
   while(1); // this example is halt.
}
