#ifndef __SYS_H
#define __SYS_H	
#include "nrf.h"
#include "string.h"
#include "stdbool.h"
//#include "string.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32; 
typedef float float32_t;
typedef unsigned long long u64;/*!< represents an unsigned 64bit-wide type */
typedef signed long long s64;  /*!< represents n signed 64bit-wide type */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;
#if 0
void copybuf(u8 *dest,const u8 *str,u16 size);
void my_memset(u8 *dest,u16 size);
#endif

#define ERR_NONE 0
#define ERR_END  1
#define ERR_NOMEM 2	/*!< not enough memory to perform the requested operation */
#define ERR_BUSY    -2 /*!< device or resource busy */
#define ERR_IO      -3 /*!< generic IO error */
#define ERR_TIMEOUT -4 /*!< error due to timeout */
#define ERR_REQUEST -5 /*!< invalid request or requested function can't be executed at the moment */
#define ERR_NOMSG   -6 /*!< No message of desired type */
#define ERR_PARAM   -7 /*!< Parameter error */
#define ERR_PROTO   -8 /*!< Protocol error */



#endif
