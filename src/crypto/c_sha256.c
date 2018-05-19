#include "c_sha256.h"

#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>

#include "common.h"

void crypto_sha256(uint8_t *input, uint32_t inputLen, uint8_t *output) {
	uint8_t result[SHA256_DIGEST_LENGTH];
	
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, input, inputLen);
	SHA256_Final(result, &ctx);
	
	memcpy(output, result, OUTPUT_LEN*sizeof(uint8_t));
}