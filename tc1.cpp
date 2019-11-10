#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "macros.h"


void commit_TC1(const uint8_t *values, uint8_t size) {
    BIT_SET(DDRB, _BV(PB2));
    uint8_t bitcount = 7;
    uint8_t start = _BV3(WGM13, WGM12, CS10);
    uint8_t stop = 0x00;
    TCCR1A = _BV4(COM1A1, COM1B1, WGM11, WGM10);
    OCR1AH = 0;
    OCR1AL = 9;                              // TOP
    OCR1BH = 0;
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
                 [tccrxb]  "M" (&TCCR1B),               // The port to use for starting pwm
                 [ocrxb]   "M" (&OCR1BL)                 // The port to use setting duty cycle
                 );

    _delay_loop_2(1000);    // reset delay is 0.5ms = 1000 * CPU cycles @ 8MHz
}



void commit_TC1_ICR(const uint8_t *values, uint8_t size) {
    BIT_SET(DDRB, _BV2(PB1, PB2));
    uint8_t bitcount = 7;
    uint8_t start = _BV3(WGM13, WGM12, CS10);
    uint8_t stop = 0x00;
    TCCR1A = _BV3(COM1A1, COM1B1, WGM11);
    OCR1AH = 0;
    OCR1AL = 4;
    OCR1BH = 0;
    ICR1H = 0;
    ICR1L = 9;
    uint8_t zero = 0;                       // 0 <= i <= TOP
    uint8_t one = 7;                        // 0 <= i <= TOP
    TCNT1H = 0;
    TCNT1L = ICR1L - 1; // why do we need this??

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
                 [tccrxb]  "M" (&TCCR1B),               // The port to use for starting pwm
                 [ocrxb]   "M" (&OCR1BL)                // The port to use setting duty cycle
                 );

    _delay_loop_2(1000);    // reset delay is 0.5ms = 1000 * CPU cycles @ 8MHz
}
