#ifndef C_GOST_H
#define C_GOST_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void gost_init_table(void);
	void crypto_gost(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif
