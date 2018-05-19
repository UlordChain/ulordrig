#ifndef C_SKEIN512_256_H
#define C_SKEIN512_256_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_skein512_256(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif
