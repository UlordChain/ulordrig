#ifndef C_HMAC_MD5_H
#define C_HMAC_MD5_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void crypto_hmac_md5(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif
