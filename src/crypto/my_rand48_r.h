﻿/* Copyright 2016-2018 The Ulord Core Foundation */

#ifndef MY_RAND48_R_H
#define MY_RAND48_R_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef SSE_VERSION
#include <immintrin.h> 

struct vrand48_data {
	__m128i vxl, vxh;
	__m128i val, vah;
	__m128i va2l, va2h;
	__m128i vc, vac_c;
	__m128i vmask, vmask2;
};

inline void vseed48(uint64_t *s, struct vrand48_data *buffer) {
	buffer->vc = _mm_set_epi32(0, 0xb, 0, 0xb);
	buffer->vac_c = _mm_set_epi32(0x40, 0x942DE6BA, 0x40, 0x942DE6BA);

	buffer->vah = _mm_set_epi32(0, 0x5, 0, 0x5);
	buffer->val = _mm_set_epi32(0, 0xdeece66d, 0, 0xdeece66d);
	
	buffer->va2h = _mm_set_epi32(0, 0x7760BB20, 0, 0x7760BB20);
	buffer->va2l = _mm_set_epi32(0, 0xB4600A69, 0, 0xB4600A69);
	
	buffer->vxl = _mm_set_epi32(0, (uint32_t)s[1], 0, (uint32_t)s[0]);
	buffer->vxh = _mm_set_epi32(0, (uint32_t)(s[1] >> 32), 0, (uint32_t)(s[0] >> 32));
	
	buffer->vmask = _mm_set_epi32(0x0000FFFF, 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF);
	buffer->vmask2 = _mm_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);
}

inline __m128i vrand48(struct vrand48_data *buffer) {
	__m128i vmask = _mm_set_epi32(0x0000FFFF, 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF);
	__m128i vmask2 = _mm_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);
	
	__m128i vrl, vrh, vresult;
	vrl = _mm_mul_epu32(buffer->val, buffer->vxl);
	vrh = _mm_add_epi64(_mm_mul_epu32(buffer->val, buffer->vxh), _mm_mul_epu32(buffer->vah, buffer->vxl));
	vresult = _mm_and_si128(_mm_add_epi64(_mm_add_epi64(vrl, _mm_slli_epi64(vrh, 32)), buffer->vc), vmask);

	buffer->vxl = _mm_and_si128(vresult, vmask2);	buffer->vxh = _mm_srli_epi64 (vresult, 32);
	
	return vresult;
}

inline __m128i vrand64_r(struct vrand48_data *buffer) {
	__m128i vrl, vrh, vresult;
	vrl = _mm_mul_epu32(buffer->val, buffer->vxl);
	vrh = _mm_add_epi64(_mm_mullo_epi32(buffer->val, buffer->vxh), _mm_mullo_epi32(buffer->vah, buffer->vxl));
	vresult = _mm_and_si128(_mm_add_epi64(_mm_add_epi64(vrl, _mm_slli_epi64(vrh, 32)), buffer->vc), buffer->vmask);
	
	__m128i vresult1;
	vrl = _mm_mul_epu32(buffer->va2l, buffer->vxl);
	vrh = _mm_add_epi64(_mm_mullo_epi32 (buffer->va2l, buffer->vxh), _mm_mullo_epi32 (buffer->va2h, buffer->vxl));
	vresult1 = _mm_and_si128(_mm_add_epi64(_mm_add_epi64(vrl, _mm_slli_epi64(vrh, 32)), buffer->vac_c), buffer->vmask);
	
	buffer->vxl = _mm_and_si128(vresult1, buffer->vmask2);	buffer->vxh = _mm_srli_epi64(vresult1, 32);
	
	return _mm_xor_si128(vresult, _mm_slli_epi64(vresult1, 16));
}

inline void vrand64(uint8_t *b, struct vrand48_data *buffer) {
	__m128i vresult0,  vresult1;
	vresult0 = vrand64_r(&buffer[0]);
	vresult1 = vrand64_r(&buffer[1]);
	
	_mm_store_si128((__m128i *)b, vresult0);
	_mm_store_si128((__m128i *)(b + 16), vresult1);

}

#endif

struct my_rand48_data {
    uint64_t __x;       	/* Current state.  */
    uint16_t __c;        	/* Additive const. in congruential formula.  */
    uint64_t __a;  			/* Factor in congruential formula.  */
};

#define MY_SEED48_R(seedVal, bufferPtr)           \
do {                                              \
	uint64_t seedval = seedVal;                   \
	struct my_rand48_data *buffer = bufferPtr;    \
	                                              \
	buffer->__x = seedval & 0xffffffffffffULL;    \
                                                  \
	buffer->__a = 0x5deece66dULL;                 \
	buffer->__c = 0xb;                            \
} while(0)

#define MY_RAND48_R(bufferPtr, resultPtr)                                      \
do {                                                                           \
	uint64_t *result = resultPtr;                                              \
	struct my_rand48_data *buffer = bufferPtr;                                 \
	                                                                           \
	*result = (buffer->__x * buffer->__a + buffer->__c) & 0xffffffffffffULL;   \
	buffer->__x = *result;                                                     \
} while(0)

#define MY_RAND64_R(bufferPtr, resultPtr)                                      \
do {                                                                           \
	uint64_t *result = resultPtr;                                              \
	struct my_rand48_data *buffer = bufferPtr;                                 \
	uint64_t X = buffer->__x;                                                  \
                                                                               \
	X = (X * buffer->__a + buffer->__c) & 0xffffffffffffULL;                   \
	buffer->__x = X;                                                           \
	                                                                           \
	buffer->__x = (X * buffer->__a + buffer->__c) & 0xffffffffffffULL;         \
	X ^= buffer->__x << 16;                                                    \
	                                                                           \
	*result = X;                                                               \
} while(0)

inline int my_seed48_r (uint64_t seedval, struct my_rand48_data *buffer) {
	buffer->__x = seedval & 0xffffffffffffULL;

	buffer->__a = 0x5deece66dULL;
	buffer->__c = 0xb;

	return 0;
}

inline int my_rand48_r (struct my_rand48_data *buffer, uint64_t *result) {
	*result = (buffer->__x * buffer->__a + buffer->__c) & 0xffffffffffffULL;
	buffer->__x = *result;
	
	return 0;
}

inline int my_rand64_r (struct my_rand48_data *buffer, uint64_t *result) {
	uint64_t X;

	X = (buffer->__x * buffer->__a + buffer->__c) & 0xffffffffffffULL;
	
	buffer->__x = (X * buffer->__a + buffer->__c) & 0xffffffffffffULL;
	X ^= buffer->__x << 16;
	
	*result = X;
	
	return 0;
}

	
#endif
