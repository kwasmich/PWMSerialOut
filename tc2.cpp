#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "macros.h"


void commit_TC2(const uint8_t *values, uint8_t size) {
    BIT_SET(DDRD, _BV(PD3));
    uint8_t bitcount = 7;
    uint8_t start = _BV2(WGM22, CS20);
    uint8_t stop = 0x00;
    TCCR2A = _BV3(COM2B1, WGM21, WGM20);
    OCR2A = 9;                              // TOP
    uint8_t zero = 0;                       // 0 <= i <= TOP
    uint8_t one = 7;                        // 0 <= i <= TOP

    asm volatile(
                 "start:    LDI  %[bits], 0x7               \n"
                 "          LD   __tmp_reg__, %a[dataptr]+  \n"
                 "          LSL  __tmp_reg__                \n"
                 "          BRCS start_1                    \n"
                 "start_0:  STS  %[ocrxb], %[rZero]         \n"
                 "          STS  %[tccrxb], %[rStart]       \n"
                 "          RJMP nextbit                    \n"
                 "start_1:  STS  %[ocrxb], %[rOne]          \n"
                 "          STS  %[tccrxb], %[rStart]       \n"
                 "          RJMP nextbit                    \n"
                 "nextbyte: LDI  %[bits], 0x7               \n"
                 "          LD   __tmp_reg__, %a[dataptr]+  \n"
                 "          LSL  __tmp_reg__                \n"
                 "bit:      BRCS bit_1                      \n"
                 "          NOP                             \n"
                 "bit_0:    STS  %[ocrxb], %[rZero]         \n"
                 "          RJMP nextbit                    \n"
                 "bit_1:    STS  %[ocrxb], %[rOne]          \n"
                 "          RJMP nextbit                    \n"
                 "nextbit:  LSL  __tmp_reg__                \n"
                 "          DEC  %[bits]                    \n"
                 "          BRNE bit                        \n"
                 "          DEC  %[rSize]                   \n"
                 "          BRCS last_1                     \n"
                 "          NOP                             \n"
                 "last_0:   STS  %[ocrxb], %[rZero]         \n"
                 "          BRNE nextbyte                   \n"
                 "          RJMP end_sfr                    \n"
                 "last_1:   STS  %[ocrxb], %[rOne]          \n"
                 "          BRNE nextbyte                   \n"
                 "          RJMP end_sfr                    \n"
                 "end_sfr:  RJMP .+0    ; NOP NOP           \n"
                 "          RJMP .+0    ; NOP NOP           \n"
                 "          NOP                             \n"
                 "          STS  %[tccrxb], %[rStop]        \n"
                 : // outputs (that are also inputs - to keep the registers)
                 [rZero]   "+a" (zero),                 // cached register state for zero
                 [rOne]    "+a" (one),                  // cached register state for one
                 [rStart]  "+a" (start),                // cached register state for start
                 [rStop]   "+a" (stop),                 // cached register state for stop
                 [rSize]   "+a" (size),                 // number of bytes to send
                 [bits]    "+a" (bitcount)              // track number of processed bits
                 : // inputs who's value
                 [dataptr] "e" (values),                // rgb color values
                 [tccrxb]  "M" (&TCCR2B),               // The port to use for starting pwm
                 [ocrxb]   "M" (&OCR2B)                 // The port to use setting duty cycle
                 );

    _delay_loop_2(1000);    // reset delay is 0.5ms = 1000 * CPU cycles @ 8MHz
}
