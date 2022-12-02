#ifndef _MAIM_H
#define _MAIM_H

extern "C" {
#include "msp430x14x.h"
}
#include "key.h"
#include "GC7721.h"
#include "uart.h"
#include "GPIO.h"
#include "warner.h"
#include "interupt.h"

#define CPU_CLOCK       8000000
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))


#endif
