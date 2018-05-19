#ifndef C_DES_H
#define C_DES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_des(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif
