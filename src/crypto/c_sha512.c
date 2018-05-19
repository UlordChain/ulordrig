#include "c_sha512.h"

#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>

#include "common.h"

void crypto_sha512(uint8_t *input, uint32_t inputLen, uint8_t *output) {
	uint8_t result[SHA512_DIGEST_LENGTH];
	
	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, input, inputLen);
	SHA512_Final(result, &ctx);
	
	reduce_bit(result, SHA512_DIGEST_LENGTH, output, 256);
}