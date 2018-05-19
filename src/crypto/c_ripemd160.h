#ifndef C_RIPEMD160_H
#define C_RIPEMD160_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_ripemd160(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif	

#endif