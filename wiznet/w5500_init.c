#include <stdio.h>
#include<string.h>
#include "w5500_init.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../Ethernet/flesz.h"

//#if LOOPBACK_MODE == LOOPBACK_MAIN_NOBLCOK
uint8_t gDATABUF[DATA_BUF_SIZE];
uint8_t webBUF[WEB_BUF_size];
//-----------------------------------------
#define FLASH_ADRES 0x1D00FF00
#define FLASH_ADRES_VT 0x9D00FF00
#define FLASH_ADRES_VT_KSEG1 0xBD00FF00
#define FLASH_ADRES_VT_KSEG2 0xBD00FF04
//uint32_t komorka  __attribute__((space(prog), address(FLASH_ADRES_VT_KSEG1)));
const uint32_t komorka_1  __attribute__((space(prog), address(FLASH_ADRES_VT_KSEG2)));
//-------------------------------------------------------------------------
enum http_metoda {GETh=0,POST,POST_OK,FLASH,ODZYSK};

//http_metoda x=GETh;
//const char flashbuf[1000];
// char radiostat0[10],radiostat1[10],temp[4];

//uint8_t * httpOK="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n";
 bool PHYStatus_check_enable= false;
 bool PHYStatus_check_flag= true;
 wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xba, 0xbb, 0xbc},
                            .ip = {192,168,1,222},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 1, 1}, 
                            .dns = {0, 0, 0, 0},
                            .dhcp = NETINFO_DHCP };//  NETINFO_DHCP =2 dla dhcp w?aczonego //static NETINFO_STATIC 
 
 void  wizchip_select(void)
{
    IO_RB10_SetLow();
}
void  wizchip_deselect(void)
{
    IO_RB10_SetHigh();
}
void  wizchip_write(uint8_t wb)
{
   __uint8_t dupa;
    // dupa=
          dupa=SPI2_Exchange8bit(wb);
}
uint8_t wizchip_read()
{
    return SPI2_Exchange8bit(0xff);
    
}
/*
void network_init(void)
{
   uint8_t tmpstr[6];
	//ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
		  gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	printf("======================\r\n");
}
*/
  void Net_Conf()
{
      
	/* wizchip netconf */
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
}

  void Display_Net_Conf()
{
#ifdef _MAIN_DEBUG_
	uint8_t tmpstr[6] = {0,};
#endif

	ctlnetwork(CN_GET_NETINFO, (void*) &gWIZNETINFO);

#ifdef _MAIN_DEBUG_
	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);

	if(gWIZNETINFO.dhcp == NETINFO_DHCP) printf("\r\n===== %s NET CONF : DHCP =====\r\n",(char*)tmpstr);
		else printf("\r\n===== %s NET CONF : Static =====\r\n",(char*)tmpstr);
	printf(" MAC : %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf(" IP : %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf(" GW : %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf(" SN : %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("=======================================\r\n");
#endif
}
  
  void delay_cnt(volatile uint16_t liczba){
    for(;liczba!=0;liczba--);  
  }

void W5500_Init()
{
    uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
   uint8_t txsize[8] = {2,2,2,2,2,2,2,2};
   uint8_t rxsize[8] = {2,2,2,2,2,2,2,2};
   
   IO_RB9_SetLow(); // RESET W5500
   delay_cnt(delay_reset);//delay
   IO_RB9_SetHigh(); //Reset W5500 
           
           
     //////////
   // TODO //
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP //
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   /* Critical section callback - No use in this example */
   //reg_wizchip_cris_cbfunc(0, 0);
   /* Chip selection call back */
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_select);  // CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
    ////////////////////////////////////////////////////////////////////////

    /* WIZCHIP SOCKET Buffer initialize */

    wizchip_init(txsize,rxsize);

    /* Network initialization */
   // network_init();
    
    
}



int32_t web_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
  uint32_t ret,zwrot,flesz[3];
  char bufor_TX[DATA_BUF_SIZE]; // const nie idzie do flash , tylko i tak jest w RAM
   uint16_t size = 0, sentsize=0;
   uint8_t i=0,x=0 ,y[50];
   unsigned int addres=FLASH_ADRES; //=; &adresik=0x1d00ffff;
   unsigned int anlok;
  
   int strToInt(char *s);
struct adres_bajtowy {
 uint8_t a;
 uint8_t b;
  uint8_t c;
 uint8_t d;
  };

 union adres {
   uint32_t ip;
   struct adres_bajtowy badres,sradres;
   };
   
   union adres addr,unia_x[3];
   
#ifdef _LOOPBACK_DEBUG_
   uint8_t destip[4];
   uint16_t destport;
#endif

   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)
         {
#ifdef _LOOPBACK_DEBUG_
			getSn_DIPR(sn, destip);
			destport = getSn_DPORT(sn);

			printf("%d:Connected - %d.%d.%d.%d : %d\r\n",sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
			setSn_IR(sn,Sn_IR_CON);
         }
		 if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
         {
			if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
			if(ret = recv(sn, buf, size)<=0)return ret;// check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
 
			//printf("odebrano[%d]: %s ",size,(char *)buf);//(char *)
            printf("odebrano[%d]: ",size);//(char *)
          // odpowiedz na rec.dane
            switch(webstrona(bufor_TX,(char *)buf ,SprawdzStanWeb((char *)buf)))
            {
                case 1:
                    if(send(sn,(uint8_t *)bufor_TX,strlen(bufor_TX))<=0) break;
                    
                    break;
                case 2: 
                    
                      if(send(sn,(uint8_t *) bufor_TX,strlen(bufor_TX))<=0) break;
                      char * wynik, *reszta;
                      wynik=strstr(buf,"ip_");
                   //   printf("\n\r wynik=%s \t",wynik);
                     // wynik=strstr(wynik,"mask0=");
                      //printf("\n\r wynik=%s \t",wynik);
                      // const char s[2] = "&";
                      char  korektor[]="=.&";


                wynik = strtok_r(wynik,korektor,&reszta);

                    #ifdef _Metoda_2
                    for(x=0;x<15;x++)
                       {
                            if((x==4)||(x==8)||(x==12))
                        {
                            y[i]=atoi(reszta);
                        }
                            else
                            {

                              y[i]=atoi(reszta);
                            // printf(" %i, ",y[i]);
                                i++;
                            }

    wynik = strtok_r(NULL,korektor,&reszta);


}
#endif // _Metoda_2
                printf("\n\r  ");
    // printf("\n wejscie=%s : tken=%s : reszta=%s \n",wejscie,tken,reszta);
    unia_x[0].sradres.a=y[0]; // addr.badres.a = y[0];
     unia_x[0].sradres.b=y[1];
      unia_x[0].sradres.c=y[2];
       unia_x[0].sradres.d=y[3];//addr.badres.b = y[1];
    //addr.badres.c = y[2];
    //addr.badres.d = y[3];
    //printf ("Adres IP w postaci 32-bitowej zmiennej: %08x\n",unia_x[0].ip);
    flesz[0]=unia_x[0].ip;
    //printf ("Adres IP w postaci 32-bitowej zmiennej: %08x\n",flesz[0]);
    //unia_x[0].ip=flesz[0];
    unia_x[1].sradres.a=y[4];
      unia_x[1].sradres.b=y[5];
        unia_x[1].sradres.c=y[6];
          unia_x[1].sradres.d=y[7];
          unia_x[2].sradres.a=y[8];
      unia_x[2].sradres.b=y[9];
        unia_x[2].sradres.c=y[10];
          unia_x[2].sradres.d=y[11];

        for(x=0;x<3;x++)
        {
            //printf("\n %d.%d.%d.%d ",unia_x[x].sradres.a,unia_x[x].sradres.b,unia_x[x].sradres.c,unia_x[x].sradres.d);
            flesz[x]=unia_x[x].ip;
              printf (" <Adr.IP.32b: %08x >",flesz[x]);
        }
   printf("\n  ");
   anlok=NVMWriteDoubleWord((unsigned int *) addres,flesz[0],flesz[1] );
   printf("\n\r anlok=%d\n\r",anlok);
  // printf("\n\r komorka: %08x \n\r",komorka);
   //komorka_1=flesz[1];
      printf("\n\r komorka: %08x \n\r",komorka_1);
   
                    break;
            }
            //if(webstrona(bufor_TX,SprawdzStanWeb((char *)buf))
           
            //---
            disconnect(sn);
          /* for(i=0;i<10;i++)
            {
                buf[i]=0;
            }//*/
           strcpy( (char *)buf, " ");
            
            //;//''
                    /*sentsize = 0; 

             * 
             * 
			while(size != sentsize)
			{
				ret = send(sn, buf+sentsize, size-sentsize);
				if(ret < 0)
				{
					close(sn);
					return ret;
				}
				sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
			}*/
         }
         break;
      case SOCK_CLOSE_WAIT :
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:CloseWait\r\n",sn);
#endif
         if((ret = disconnect(sn)) != SOCK_OK) return ret;
#ifdef _LOOPBACK_DEBUG_
         printf("%d:Socket Closed\r\n", sn);
#endif
         break;
      case SOCK_INIT :
#ifdef _LOOPBACK_DEBUG_
    	 printf("%d:Listen, TCP server loopback, port [%d]\r\n", sn, port);
#endif
         if( (ret = listen(sn)) != SOCK_OK) return ret;
         break;
      case SOCK_CLOSED:
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:TCP server loopback start\r\n",sn);
#endif
         if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret;
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:Socket opened\r\n",sn);
#endif
         break;
      default:
         break;
   }
   return 1;
}

int strindex(char *s,char *t)
{
  uint16_t i,n;
  
  n=strlen(t);
  for(i=0;*(s+i); i++) {
    if (strncmp(s+i,t,n) == 0)
      return i;
  }
  return -1;
}
uint8_t webstrona(char *buf_TX,char *bufor,enum http_metoda stan)     
{
char ip_temp[16];
   char mac_temp[24];
   char mask_temp[16];
   char gw_temp[16];
   uint8_t zwrot;
   //----------------sprintf--------------
   sprintf(ip_temp,"%d.%d.%d.%d",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);   
         sprintf(mac_temp,"%02X:%02X:%02X:%02X:%02X:%02X", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	sprintf(gw_temp,"%d.%d.%d.%d", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	sprintf(mask_temp,"%d.%d.%d.%d", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);    
    
    switch(stan)
    {
        case GETh:    
        strcpy(buf_TX,HTTP_OK);
                //strcat(buf_TX,"<html><body><span style=\"color:#0000A0\">\r\n");
	   
	    strcat(buf_TX,napis_H1);
	  
         strcat(buf_TX,metoda_POST);
        // zwrot=send(sn,buf,strlen(buf_TX));
         strcat(buf_TX,eter_konf);
         strcat(buf_TX,linia_ip1);
         strcat(buf_TX,ip_temp); 
         strcat(buf_TX,lina_end);
         
         strcat(buf_TX,obIP);
          strcat(buf_TX,ip_temp); 
                    
          strcat(buf_TX,BR);
          strcat(buf_TX,linia_maska);
          strcat(buf_TX,mask_temp);
          strcat(buf_TX,lina_end);
           strcat(buf_TX,obMask);
          strcat(buf_TX,mask_temp); 
          strcat(buf_TX,BR);
          
          strcat(buf_TX,linia_GW);
          strcat(buf_TX,gw_temp);
          strcat(buf_TX,lina_end);
           strcat(buf_TX,obGW);
          strcat(buf_TX,gw_temp); 
          strcat(buf_TX,BR);
          
          strcat(buf_TX,linia_MAC);
          strcat(buf_TX,mac_temp);
          strcat(buf_TX,lina_end);
           strcat(buf_TX,obMAC);
          strcat(buf_TX,mac_temp); 
          strcat(buf_TX,BR);
          
           strcat(buf_TX,HR);
             strcat(buf_TX,("<input type=\"submit\">\r\n"));
	    strcat(buf_TX,("</form></span></body></html>\r\n"));
        zwrot=1;
        break;
        case POST:
            strcpy(buf_TX,HTTP_OK) ;
                strcat(buf_TX,napis_H2);
            zwrot=2;
            break;
            default:
                return 0;

            }

    
    
}

enum http_metoda SprawdzStanWeb(char *bufor){
    enum http_metoda s;
  if(!strncmp("GET",(char *)bufor,3))
            {
      //http_metoda s=GET;
              return s=GETh;// s=GET;
           } 
     //int zwrot;
             //   zwrot=send(sn,bufor,strlen(bufor));
                //printf("\n\r drugi zwrot %i \n\r",zwrot);
                //printf("\n\r OK strncmp zwrot=%i \n\r",zwrot);
                
            
            if(!strncmp("POST",(char *)bufor,4))
            {
                return s=POST;//s=POST;
                // strcpy(bufor_TX,HTTP_OK) ;
                //strcat(bufor_TX,napis_H2);
             //  zwrot=send(sn,bufor_TX,strlen(bufor_TX));// printf("\n\r ERROR strncmp"); 
                
            }
}

/*(uint8_t *)
    * strcat((char *)buf,("<p><input type=\"radio\" name=\"radio\" value=\"0\" "));
	    strcat((char *)buf,radiostat0);
	    strcat((char *)buf,(">Blinking LED\r\n"));
	    strcat((char *)buf,("<br><input type=\"radio\" name=\"radio\" value=\"1\" "));
	    strcat((char *)buf,radiostat1);
	    strcat((char *)buf,(">Scanning LED\r\n"));
 	    strcat((char *)buf,("</strong><p>\r\n"));
	    strcat((char *)buf,("<input type=\"submit\">\r\n"));
	    strcat((char *)buf,("</form></span></body></html>\r\n"));
    * // Create the HTTP Response	Header
	    strcpy_P((char *)buf,("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
	    strcat((char *)buf,("<html><body><span style=\"color:#0000A0\">\r\n"));
	    strcat((char *)buf,("<h1>Embedded Web Server</h1>\r\n"));
	    strcat((char *)buf,("<h3>AVRJazz Mega328 and WIZ811MJ</h3>\r\n"));
	    strcat((char *)buf,("</body></html>\r\n"));
    */

//strcat(buf_TX,linia_ip1);
	    //strcat(buf_TX,radiostat0);
	    //strcat(buf_TX,(">Blinking LED\r\n"));
	    //strcat(buf_TX,("<br><input type=\"radio\" name=\"radio\" value=\"1\" "));
	    //strcat(buf_TX,radiostat1);
	    //strcat(buf_TX,(">Scanning LED\r\n"));
 	    //strcat(buf_TX,("</strong><p>\r\n"));