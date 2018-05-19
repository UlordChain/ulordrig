#ifndef C_BLAKE2S256_H
#define C_BLAKE2S256_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_blake2s256(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif


#endif