#ifndef C_SHA512_H
#define C_SHA512_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_sha512(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif


#endif