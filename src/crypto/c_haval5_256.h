#ifndef C_HAVAL5_256_H
#define C_HAVAL5_256_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_haval5_256(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif
