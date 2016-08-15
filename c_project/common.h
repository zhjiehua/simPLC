#ifndef __COMMON_H__
#define __COMMON_H__

//#include <REG52.H>                /* special function register declarations   */
//                                  /* for the intended 8051 derivative         */
#include "STC89C5xRC_RDP.H"
#include "intrins.h"

//typedef unsigned char BYTE; //sizeof(BYTE) = 1
//typedef unsigned int WORD;  //sizeof(WORD) = 2

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

//----------------------------------------------- 
/* define constants */
#define FOSC 11059200L      //System frequency

#endif