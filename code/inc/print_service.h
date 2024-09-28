#ifndef PRINT_SERVICE_H
#define PRINT_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ret_code.h"

void print_service_init(void);
ret_code_t put_str(char * str);

#ifdef __cplusplus
}
#endif

#endif /* PRINT_SERVICE_H */