/* 
 * File:   flesz.h
 * Author: witoldo
 *
 * Created on 14 listopada 2017, 17:33
 */
#include "../mcc_generated_files/mcc.h"

#ifndef FLESZ_H
#define	FLESZ_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned  int  NVMUnlock (unsigned int nvmop);
unsigned int NVMWriteWord (void* address, unsigned int data);
unsigned int NVMWriteDoubleWord (void* address, unsigned int data_l,unsigned int data_h);


#ifdef	__cplusplus
}
#endif

#endif	/* FLESZ_H */

/*
8 Kbytes RAM  0x00000000 0x00001FFF
Reserved 0x00002000 0x1CFFFFFF
64 Kbytes Flash 0x1D000000 0x1D00FFFF
Reserved 0x1D010000 0x1F7FFFFF
SFRs 0x1F800000 0x1F80FFFF
Reserved 0x1F810000 0x1FBFFFFF
Boot Flash 0x1FC00000 0x1FC016FF
Configuration Bits (3  0x1FC01700 0x1FC017FF
 reserved 0x1FC01800 0xFFFFFFFF
 * kseg0
 * flash prog 0x9D000000 0x9D00FFFF
 *
 * kseg1
Reserved
0x9FFFFFFF 0xA0000000
8 Kbytes RAM
0xA0001FFF 0xA0002000
Reserved
0xBCFFFFFF 0xBD000000
64 Kbytes Flash
0xBD00FFFF 0xBD010000
Reserved
0xBF7FFFFF 0xBF800000
 SFRs
0xBF80FFFF 0xBF810000
Reserved
0xBFBFFFFF  0xBFC00000
Boot Flash
0xBFC016FF 0xBFC01700
Configuration Bits (3)
0xBFC017FF 0xBFC01800
Reserved
0xFFFFFFFF
 */