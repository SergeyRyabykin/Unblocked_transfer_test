#ifndef PIN_H
#define PIN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IN_ANA = 0x0,
    OUT_PP = 0x3,
    IN_FLT = 0x4,
    OUT_OD = 0x7,
    IN_PD =  0x8,
    ALT_PP = 0xb,
    ALT_OD = 0xf,
    IN_PP =  0x18,
} gpio_mode_t;

void pin_init(uintptr_t port_base, unsigned int pin_num, gpio_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif /* PIN_H */