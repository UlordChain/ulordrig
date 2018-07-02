/* Copyright 2016-2018 The Ulord Core Foundation */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define INPUT_LEN	32
#define OUTPUT_LEN	32

// Least common multiple
inline uint32_t lcm(uint32_t num1, uint32_t num2) {
	uint32_t m = num1, n = num2;
	while(num2) {
		uint32_t r = num1 % num2;
		num1 = num2;
		num2 = r;
	}
	uint32_t lcm = m * n / num1;
	return lcm;
}

inline void reduce_bit_2(uint8_t *input, uint32_t inputLen, 
		uint8_t *output, uint32_t bits) {
	uint32_t i, outputLen = (bits) >> 3;
	uint32_t lcmBytes = lcm(inputLen, outputLen);
	memcpy(output, input, outputLen * sizeof(uint8_t));
	for (i = outputLen; i < lcmBytes; ++i) {
		output[i % outputLen] ^= input[i % inputLen];
	}
}

inline void reduce_bit(uint8_t *input, uint32_t inputLen, 
		uint8_t *output, uint32_t bits) {
	uint32_t i, outputLen = (bits) >> 3;
	memcpy(output, input, outputLen * sizeof(uint8_t));
	for (i = outputLen; i < inputLen; ++i) {
		output[i % outputLen] ^= input[i % inputLen];
	}
}

inline uint8_t reduce32_8bits(uint32_t input){
	uint32_t t=(input>>16)^input;
	return ((t>>8)^t)&0xFF;
}	
inline uint8_t reduce64_8bits(uint64_t input){
	uint32_t t=(input>>32)^input;
	return reduce32_8bits(t);
}	
inline uint8_t reduce64_4bits(uint64_t input){
	uint32_t t=reduce64_8bits(input);
	return ((t>>4)^t)&0xF;
}

#ifndef SSE_VERSION

inline void rrs_32Byte_256(uint8_t *input, uint8_t *output, uint32_t bits) {
	if (bits == 0) { memcpy(output, input, 32); return; }
	const uint32_t inputLen = 32;
	uint32_t shiftBytes = (bits) >> 3, shiftBits = (bits) & 0x7;
	uint32_t rIndex = (inputLen) - shiftBytes;
	uint32_t lIndex = (rIndex + (inputLen) - 1) % (inputLen);
	for (uint32_t i = 0; i < inputLen; ++i) {
		output[i] = (input[(rIndex++) % (inputLen)] >> shiftBits) |
			(input[(lIndex++) % (inputLen)] << (8 - shiftBits));
	}
}

#else

#include <immintrin.h>

inline void rrs_32Byte_256(uint8_t *input, uint8_t *output, uint32_t bits){
	if(bits==0){memcpy(output,input,32);return;}
	uint32_t word_shf=bits>>5;
	uint32_t bit_shf=bits&0x1F;
	uint32_t *ip=(uint32_t *)input;
	uint32_t *op=(uint32_t *)output;
	if(bit_shf==0){
		op[((0+word_shf)&0x07)]=ip[0];
		op[((1+word_shf)&0x07)]=ip[1];
		op[((2+word_shf)&0x07)]=ip[2];
		op[((3+word_shf)&0x07)]=ip[3];
		op[((4+word_shf)&0x07)]=ip[4];
		op[((5+word_shf)&0x07)]=ip[5];
		op[((6+word_shf)&0x07)]=ip[6];
		op[((7+word_shf)&0x07)]=ip[7];
	}else{
		__m128i va, vb;
		va = _mm_load_si128((__m128i *)ip);
		vb = _mm_load_si128((__m128i *)(ip + 4));
		
		__m128i vSet = _mm_set_epi32(0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203);
		va = _mm_shuffle_epi8(va, vSet);
		vb = _mm_shuffle_epi8(vb, vSet);
		
		__m128i var = _mm_srli_epi32(va, bit_shf);
		__m128i val = _mm_slli_epi32(va, 32 - bit_shf);
		
		__m128i vbr = _mm_srli_epi32(vb, bit_shf);
		__m128i vbl = _mm_slli_epi32(vb, 32 - bit_shf);
		
		val = _mm_shuffle_epi8(val, vSet);
		var = _mm_shuffle_epi8(var, vSet);
		vbl = _mm_shuffle_epi8(vbl, vSet);
		vbr = _mm_shuffle_epi8(vbr, vSet);

		uint32_t tpl[8], tpr[8];
		_mm_store_si128((__m128i *)tpl, val);
		_mm_store_si128((__m128i *)tpr, var);
		_mm_store_si128((__m128i *)(tpl + 4), vbl);
		_mm_store_si128((__m128i *)(tpr + 4), vbr);
		
		op[((0+word_shf)&0x07)]=(tpr[0])|(tpl[7]);
		op[((1+word_shf)&0x07)]=(tpr[1])|(tpl[0]);
		op[((2+word_shf)&0x07)]=(tpr[2])|(tpl[1]);
		op[((3+word_shf)&0x07)]=(tpr[3])|(tpl[2]);
		op[((4+word_shf)&0x07)]=(tpr[4])|(tpl[3]);
		op[((5+word_shf)&0x07)]=(tpr[5])|(tpl[4]);
		op[((6+word_shf)&0x07)]=(tpr[6])|(tpl[5]);
		op[((7+word_shf)&0x07)]=(tpr[7])|(tpl[6]);
	}
}

#endif

#ifdef __cplusplus
extern "C" {
#endif

	void view_data_u8(const char *mess, uint8_t *data, uint32_t len);
	void view_data_u32(const char *mess, uint32_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif	


#endif
