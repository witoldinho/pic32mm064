/* 
 * File:   w5500_init.h
 * Author: witoldo
 *
 * Created on 8 lipca 2017, 15:31
 */
#include "stdbool.h"
#include "../Ethernet/wizchip_conf.h"
#include "../Ethernet/socket.h"
#include "../Ethernet/flesz.h"

#ifndef W5500_INIT_H
#define	W5500_INIT_H

///////////////////////////
// Demo Firmware Version //
///////////////////////////
#define VER_H		1
#define VER_L		00
/////////////////////////////////////////
// SOCKET NUMBER DEFINION for Examples //
/////////////////////////////////////////
#define SOCK_TCPS        7
#define SOCKS_WEB        0  
#define SOCK_UDPS        1
#define SUCCESS		     3
#define SOCK_DHCP       6
#define PORT_TCPS		40000
#define PORT_HTTP       80

#define	_LOOPBACK_DEBUG_
/************************/
/* Select LOOPBACK_MODE */
/************************/
#define LOOPBACK_MAIN_NOBLOCK    0
#define LOOPBACK_MODE   LOOPBACK_MAIN_NOBLOCK

#define _MAIN_DEBUG_ 1
#define delay_reset 64000

#ifdef	__cplusplus
extern "C" {
#endif
    
#define _Metoda_2    

/////////////////////
// PHYStatus check //
/////////////////////
// #define SEC_PHYSTATUS_CHECK 		1		// sec
extern bool PHYStatus_check_enable; //= false;
extern bool PHYStatus_check_flag; // = true;
enum http_metoda ;//{GET=0,POST, POST_OK, FLASH,ODZYSK};


extern wiz_NetInfo gWIZNETINFO;

#define DATA_BUF_SIZE   2048 // 2048 
#define WEB_BUF_size    1024
// ---- web_page  
#define HTTP_OK  "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
#define napis_H1 "<!DOCTYPE html> <html> <head> <meta charset=\"utf-8\" /> <title>www strona</title></head><body><span style=\"color:#0000A0\"><fieldset><h1> wwwwwwwwww </h1><h3> ip ustawienia\"   </h3><h4>Strona konfiguracyjna </h4></fieldset>"
#define metoda_POST "<p><form method=\"POST\">\r\n"
#define fieldset_start  "<fieldset>"
#define fieldset_stop   "</fieldset>"
#define eter_konf       "<b> adresy IP</b>  <br><br>"
#define dmx_konf        "<u>-----</u> <br><br>"
#define linia_ip1       "IP adres______:<input type=\"text\" size=\"12\" name=\"ip_\" value=\""
#define linia_maska     "Maska________:<input type=\"text\" size=\"12\" name=\"mask_\" value=\""
#define linia_MAC       "MAC ADRES__:<input type=\"text\" size=\"12\" name=\"mac_\" value=\""
#define linia_GW        "IP Bramy_____:<input type=\"text\" size=\"12\" name=\"gw_\" value=\""
#define lina_end        "\">"
#define BR              "<BR>"
#define HR               "<hr>"   
#define obIP        " obecny adres IP___: "
#define obGW        " obecny adres IP GW: "
#define obMask      " obecna MAska______: "
#define obMAC       " obecny adres MAC__: "
#define napis_H2  "<!DOCTYPE html> <html> <head> <meta charset=\"utf-8\" /> <title> www stron </title></head><body><span style=\"color:#0000A0\"><fieldset><h1> Dane zatwierdzono ! zapis do flash </h1></body></html>"

//------end_of_webpage
// min=\"2\" max=\"192\"


extern uint8_t gDATABUF[DATA_BUF_SIZE];
extern uint8_t webBUF[WEB_BUF_size];
//extern const uint8_t flashbuf[1000];
uint8_t * httpOK ;//[]="HTTP/ 1.0 200 OK\r\n Content-type : text/html \r\n Pragma : no-cache \r\n\r\n";
int strindex(char *s,char *t);
uint8_t webstrona(char *TX_BUF,char *bufor,enum http_metoda stan);
enum http_metoda SprawdzStanWeb(char *bufor);

/////////
// TODO //
//////////////////////////////////////////////////////////////////////////////////////////////
// Call back function for W5500 SPI - Theses used as parameter of reg_wizchip_xxx_cbfunc()  //
// Should be implemented by WIZCHIP users because host is dependent                         //
//////////////////////////////////////////////////////////////////////////////////////////////
void  wizchip_select(void);
void  wizchip_deselect(void);
void  wizchip_write(uint8_t wb);
uint8_t wizchip_read();

 void Net_Conf();
void Display_Net_Conf();
 void PHYStatus_Check(void);
//void network_init(void);
 void delay_cnt(volatile uint16_t liczba);

void W5500_Init();
int32_t web_tcps(uint8_t sn, uint8_t* buf, uint16_t port);
//int strindex(char *s,char *t);



#ifdef	__cplusplus
}
#endif

#endif	/* W5500_INIT_H */

// uint8_t * httpOK="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n";
  // const char * webpage="<html><head><title>tytu?</title></head><body>RTS!</body></html> ";
//zwrot=send(sn,httpOK,strlen((char *)httpOK));
               
                //printf("\n\r OK strncmp zwrot=%i \n\r",zwrot);
               
         
	    //strcat(buf_TX,"</body></html>\r\n");
                //zwrot=send(sn,(uint8_t *)webpage,strlen(webpage)); //(uint8_t *)