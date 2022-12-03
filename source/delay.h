#ifndef _DELAY_H
#define _DELAY_H

#define CPU_CLOCK       8000000
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))

#endif