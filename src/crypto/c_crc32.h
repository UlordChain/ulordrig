#ifndef C_CRC32_H
#define C_CRC32_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void CRC32_Table_Init();
	void crypto_crc32(uint8_t *input, uint32_t inputLen, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif
