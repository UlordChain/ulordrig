/* Copyright 2016-2018 The Ulord Core Foundation */

#include "PoW.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
// #include <omp.h>

#include "my_time.h"
#include "common.h"
#include "my_rand48_r.h"
#include "oneWayFunction.h"

/* 
 * Step 1: Initialize working memory.
*/
void initWorkMemory(uint8_t *input, uint32_t inputLen, uint8_t *Maddr, const uint32_t K) {
	uint32_t i, j, k;
#ifdef _MSC_VER
	__declspec(align(16)) uint8_t a[OUTPUT_LEN], b[OUTPUT_LEN], a_rrs[OUTPUT_LEN], b_rrs[OUTPUT_LEN];
#else
	__attribute__((aligned(16))) uint8_t a[OUTPUT_LEN], b[OUTPUT_LEN], a_rrs[OUTPUT_LEN], b_rrs[OUTPUT_LEN];
#endif
	uint64_t *a_u64 = (uint64_t *)a, *b_u64 = (uint64_t *)b;

	funcInfor[0].func(input, inputLen, a);

	uint64_t randSeed[4] = {0, 0, 0, 0};
	const uint32_t iterNum = WORK_MEMORY_SIZE >> 5;
	for (i = 0; i < iterNum; i += K) {
		uint8_t t = 0, shift_num = 0;
		reduce_bit(a, 32, (uint8_t *)&t, 8);
		t = (t & 0x0f) ^ (t >> 4);
		// reduce_bit((uint8_t *)&i, 4, (uint8_t *)&shift_num, 8);
		shift_num = reduce32_8bits(i);
		
		// rrs(a, OUTPUT_LEN, a_rrs, shift_num);
		rrs_32Byte_256(a, a_rrs, shift_num);
		funcInfor[t].func(a_rrs, 32, a);
		
		reduce_bit(a,      8, (uint8_t *)&randSeed[0], 48);
		reduce_bit(a +  8, 8, (uint8_t *)&randSeed[1], 48);
		reduce_bit(a + 16, 8, (uint8_t *)&randSeed[2], 48);
		reduce_bit(a + 24, 8, (uint8_t *)&randSeed[3], 48);

#ifndef SSE_VERSION
		struct my_rand48_data randBuffer[4];
		my_seed48_r(randSeed[0], &randBuffer[0]);
		my_seed48_r(randSeed[1], &randBuffer[1]);
		my_seed48_r(randSeed[2], &randBuffer[2]);
		my_seed48_r(randSeed[3], &randBuffer[3]);
#else
		struct vrand48_data randBuffer[2];
		vseed48(randSeed    , &randBuffer[0]);
		vseed48(randSeed + 2, &randBuffer[1]);
#endif
		memcpy(Maddr + (i << 5), a, 32*sizeof(uint8_t));

		for (k = 1; k < K; ++k) {
#ifndef SSE_VERSION
			my_rand64_r(&randBuffer[0], &b_u64[0]);
			my_rand64_r(&randBuffer[1], &b_u64[1]);
			my_rand64_r(&randBuffer[2], &b_u64[2]);
			my_rand64_r(&randBuffer[3], &b_u64[3]);
			
			uint8_t shift_num;
			uint8_t result[OUTPUT_LEN];
			// reduce_bit((uint8_t *)&i, 4, (uint8_t *)&shift_num, 8);
			shift_num = reduce32_8bits(i + k);
			// rrs(b, OUTPUT_LEN, result, shift_num);
			rrs_32Byte_256(b, b_rrs, shift_num);
			
			uint64_t *b_rrs_u64 = (uint64_t *)b_rrs;
			a_u64[0] ^= b_rrs_u64[0];
			a_u64[1] ^= b_rrs_u64[1];
			a_u64[2] ^= b_rrs_u64[2];
			a_u64[3] ^= b_rrs_u64[3];
#else
			vrand64(b, randBuffer);
		
			uint8_t shift_num;
			shift_num = reduce32_8bits(i + k);
			// rrs(b, OUTPUT_LEN, result, shift_num);
			rrs_32Byte_256(b, b_rrs, shift_num);
					
			__m128i val = _mm_load_si128((__m128i *)a);
			__m128i vah = _mm_load_si128((__m128i *)(a + 16));
			__m128i vresultl = _mm_load_si128((__m128i *)b_rrs);
			__m128i vresulth = _mm_load_si128((__m128i *)(b_rrs + 16));

			vresultl = _mm_xor_si128(val, vresultl);
			vresulth = _mm_xor_si128(vah, vresulth);

			_mm_store_si128((__m128i *)a, vresultl);
			_mm_store_si128((__m128i *)(a + 16), vresulth);
#endif
			memcpy(Maddr + ((i + k) << 5), b_rrs, OUTPUT_LEN*sizeof(uint8_t));
		}
	}
}

/* 
 * Step 2: Modify the working memory contents.
*/
void modifyWorkMemory(uint8_t *Maddr, const uint32_t L, const uint32_t C,
		uint8_t *result) {
	uint32_t i, j;
#ifdef _MSC_VER
	__declspec(align(16)) uint8_t a[OUTPUT_LEN], b[64], a_rrs[OUTPUT_LEN];
#else
	__attribute__((aligned(16))) uint8_t a[OUTPUT_LEN], b[64], a_rrs[OUTPUT_LEN];
#endif
	uint64_t *a_u64 = (uint64_t *)a, *b_u64 = (uint64_t *)b;
	
	funcInfor[0].func(Maddr + WORK_MEMORY_SIZE - 32, 32, a);
	memcpy(result, a, OUTPUT_LEN*sizeof(uint8_t));
	
	uint64_t r = 0;
	reduce_bit(a, 32, (uint8_t *)&r, 64);
	
	const uint32_t iterNum = L << 6;
	for (i = 0; i < C; ++i) {
		uint64_t randSeed = 0;
		reduce_bit(a, 32, (uint8_t *)&randSeed, 48);
		
		struct my_rand48_data randBuffer;
		my_seed48_r(randSeed, &randBuffer);
		
		uint8_t t1, t2, s;
		uint64_t randNum = 0, base = 0;
		
		for (j = 0; j < iterNum; ++j) {
			my_rand48_r(&randBuffer, &randNum);
			
			base = randNum + r;
			
			uint64_t offset = 0;
			// reduce_bit((uint8_t *)&r, 8, (uint8_t *)&offset, 8);
			offset = reduce64_8bits(r);
			offset = (offset << 8) + 1;
			
			uint64_t addr1 = (base + WORK_MEMORY_SIZE - offset) % WORK_MEMORY_SIZE;
			uint64_t addr2 = (base + offset) % WORK_MEMORY_SIZE;
			
			t1 = Maddr[addr1];
			t2 = Maddr[addr2]; 
			s = a[j & 0x1f];
			
			Maddr[addr1] = t2 ^ s;
			Maddr[addr2] = t1 ^ s;
			b[j & 0x3f] = t1 ^ t2;
			
			r = r + s + t1 + t2;
		}
		
		uint8_t t = 0;
		// reduce_bit((uint8_t *)&r, 8, (uint8_t *)&t, 8);
		// t = (t & 0x0f) ^ (t >> 4);
		t = reduce64_4bits(r);
		
		reduce_bit(b, 64, a, 256);
		
		uint8_t shift_num = 0;
		uint64_t ir = r + i;
		// reduce_bit((uint8_t *)&ir, 8, (uint8_t *)&shift_num, 8);
		shift_num = reduce64_8bits(ir);

		// rrs(a, OUTPUT_LEN, a_rrs, shift_num);
		rrs_32Byte_256(a, a_rrs, shift_num);
		funcInfor[t].func(a_rrs, 32, a);
		
		uint64_t *result_u64 = (uint64_t *)result;
		result_u64[0] ^= a_u64[0];
		result_u64[1] ^= a_u64[1];
		result_u64[2] ^= a_u64[2];
		result_u64[3] ^= a_u64[3];
	}
}

/* 
 * Step 3: Calculate the final result.
*/
void calculateFinalResult(uint8_t *Maddr, uint8_t *c, const uint32_t D, uint8_t *output) {
	uint32_t i = 0, j = 0, k = 0;
#ifdef _MSC_VER
	__declspec(align(16)) uint8_t result[OUTPUT_LEN], result_rrs[OUTPUT_LEN];
#else
	__attribute__((aligned(16))) uint8_t result[OUTPUT_LEN], result_rrs[OUTPUT_LEN];
#endif
	uint64_t *Maddr_u64 = (uint64_t *)Maddr;
	uint64_t *result_u64 = (uint64_t *)result;
	
	const uint32_t num = (WORK_MEMORY_SIZE >> 5) - 1;
	
	uint32_t it = 0;
	memcpy(result, c, OUTPUT_LEN * sizeof(uint8_t));
	while(1) {
		uint8_t t = 0, shift_num = 0;
		uint32_t d = 0;
		reduce_bit(result, 32, (uint8_t *)&t, 8);
		t = (t & 0x0f) ^ (t >> 4);
		
		reduce_bit(result, 32, (uint8_t *)&d, D);
		++d;
		
		for (j = 0; j < d; ++j) {
			uint32_t index = i << 2;
			
			result_u64[0] ^= Maddr_u64[index + 0];
			result_u64[1] ^= Maddr_u64[index + 1];
			result_u64[2] ^= Maddr_u64[index + 2];
			result_u64[3] ^= Maddr_u64[index + 3];
			
			++i;
			if (i == num) {
				it = i + t;
				// reduce_bit((uint8_t *)&it, 4, (uint8_t *)&shift_num, 8);
				shift_num = reduce32_8bits(it);

				// rrs(result, OUTPUT_LEN, result_rrs, shift_num);
				rrs_32Byte_256(result, result_rrs, shift_num);
				funcInfor[0].func(result_rrs, 32, result);
				
				memcpy(output, result, OUTPUT_LEN * sizeof(uint8_t));

				return;
			}
		}
		it = t + i;
		// reduce_bit((uint8_t *)&it, 4, (uint8_t *)&shift_num, 8);
		shift_num = reduce32_8bits(it);

		// rrs(result, OUTPUT_LEN, result_rrs, shift_num);
		rrs_32Byte_256(result, result_rrs, shift_num);
		funcInfor[t].func(result_rrs, 32, result);
	}
}

/* 
 * Correctness & Performance test for Proof of work
*/
/*
void testPowFunction(uint8_t *mess, uint32_t messLen, const int64_t iterNum) {
	int64_t j;

	uint32_t inputLen = messLen;
	uint8_t input[INPUT_LEN], output[OUTPUT_LEN];
	memset(input, 0, INPUT_LEN*sizeof(uint8_t));
	memcpy(input, mess, messLen*sizeof(char));

	// Init all one-way function
	initOneWayFunction();
	
	uint8_t *Maddr = (uint8_t *)malloc(64 * WORK_MEMORY_SIZE*sizeof(uint8_t));
	assert(NULL != Maddr);
	memset(Maddr, 0, 64 * WORK_MEMORY_SIZE*sizeof(uint8_t));
	
	
	printf("****************************** Correctness test (PoW function) ******************************\n");
	printf("Test message: %s\n", mess);
	powFunction(input, inputLen, Maddr, output);
	view_data_u8("PoW", output, OUTPUT_LEN);
	printf("*********************************************************************************************\n");
	
	printf("*************************************************** Performance test (PoW function) ***************************************************\n");
	uint8_t *result = (uint8_t *)malloc(iterNum * OUTPUT_LEN * sizeof(uint8_t));
	assert(NULL != result);
	memset(result, 0, iterNum * OUTPUT_LEN * sizeof(uint8_t));
	
	uint32_t threadNumArr[] = {1, 4, 8, 12, 16, 20, 24, 32, 48, 64};
	uint32_t threadNumTypes = sizeof(threadNumArr) / sizeof(uint32_t);
	printf("   %-18s", "Algorithm");
	for (uint32_t ix = 0; ix < threadNumTypes; ++ix)
		printf("%12d", threadNumArr[ix]);
	printf("\n");
	
	printf("00 %-18s\t", "PoW");
	for (uint32_t ix = 0; ix < threadNumTypes; ++ix) {
		omp_set_num_threads(threadNumArr[ix]);
		double startTime = get_wall_time();
		if (threadNumArr[ix] == 1) {
			for (j = 0; j < iterNum; ++j) {
				powFunction(input, inputLen, Maddr, result + j * OUTPUT_LEN);
			}
		} else {
			#pragma omp parallel for firstprivate(input), private(j) shared(result)
			for (j = 0; j < iterNum; ++j) {
				powFunction(input, inputLen, Maddr + omp_get_thread_num() * WORK_MEMORY_SIZE, result + j * OUTPUT_LEN);
			}
		}
		double endTime = get_wall_time();
		double costTime = endTime - startTime;
		printf("%5.0f bps   ", iterNum / costTime); fflush(stdout);
		
		// Check result
		for (j = 0; j < iterNum; j += 1) {
			if (memcmp(output, result + j * OUTPUT_LEN, OUTPUT_LEN)) {
				printf("Thread num: %d, j: %ld\n", threadNumArr[ix], j);
				view_data_u8("output", output, OUTPUT_LEN);
				view_data_u8("result", result + j * OUTPUT_LEN, OUTPUT_LEN);
				abort();
			}
		}
	}
	printf("\n");
	printf("***************************************************************************************************************************************\n");
	
	if (NULL != result) {
		free(result);
		result = NULL;
	}
	
	if (NULL != Maddr) {
		free(Maddr);
		Maddr = NULL;
	}
}
*/

#define OUTPUT_BUFFER_SIZE	(32 * 1024UL * 1024UL)
#define MAX_TEST_INPUT_LEN		140
#define MAX_OUT_FILE_NAME_LEN	25

const char testInputCase[][MAX_TEST_INPUT_LEN] = {
	"",
	"HelloWorld",
	"0123456789"
};

void powNistTest(const char *outFileName) {
	const uint64_t iterNum = 1024UL * 1024UL;
	// const uint64_t iterNum = 1024UL;
	
	uint8_t *outputBuffer = (uint8_t *)malloc(OUTPUT_BUFFER_SIZE * sizeof(uint8_t));
	assert(NULL != outputBuffer);
	memset(outputBuffer, 0, OUTPUT_BUFFER_SIZE * sizeof(uint8_t));
	
	uint8_t *Maddr = (uint8_t *)malloc(WORK_MEMORY_SIZE*sizeof(uint8_t));
	assert(NULL != Maddr);
	memset(Maddr, 0, WORK_MEMORY_SIZE*sizeof(uint8_t));
	
	initOneWayFunction();
	
	uint32_t testInputCaseNum = sizeof(testInputCase) / sizeof(const char [MAX_TEST_INPUT_LEN]);
	for (uint32_t testCaseIx = 0; testCaseIx < testInputCaseNum; ++testCaseIx) {
		char curOutFileName[MAX_OUT_FILE_NAME_LEN] = "";
		sprintf(curOutFileName, "%s-%u.txt", outFileName, testCaseIx);
		
		FILE *fp = NULL;
		if (NULL != (fp = fopen(curOutFileName, "wb"))) {
			const uint32_t testInputCaseLen = strlen((char *)testInputCase[testCaseIx]);
			
			uint8_t input[MAX_TEST_INPUT_LEN];
			memset(input, 0, MAX_TEST_INPUT_LEN*sizeof(uint8_t));
			memcpy(input, testInputCase[testCaseIx], testInputCaseLen*sizeof(uint8_t));

			double startTime = get_wall_time();
			powFunction(input, testInputCaseLen, Maddr, outputBuffer);
			for (uint64_t i = 1, j = 0; i < iterNum; ++i) {
				memcpy(input, outputBuffer +  j, OUTPUT_LEN * sizeof(uint32_t));
				j += OUTPUT_LEN;
				
				powFunction(input, OUTPUT_LEN, Maddr, outputBuffer + j);
				
				/* if (j == OUTPUT_BUFFER_SIZE) {
					fwrite(outputBuffer, sizeof(uint8_t), OUTPUT_BUFFER_SIZE / sizeof(uint8_t), fp);
					j = 0;
				} */
			}
			double endTime = get_wall_time();
			double costTime = endTime - startTime;
			fprintf(stdout, "TestCaseIx: %d, Input: %s, IterNum: %lu, Time: %4.2f, Performance: %5.2f bps\n", testCaseIx, \
				testInputCase[testCaseIx], iterNum, costTime, ((double)(iterNum * OUTPUT_LEN)) / costTime); fflush(stdout);

			fwrite(outputBuffer, sizeof(uint8_t), OUTPUT_BUFFER_SIZE / sizeof(uint8_t), fp);

			fclose(fp);
		} else {
			fprintf(stderr, "Error: Open %s failed!\n", curOutFileName);
			abort();
		}
	}
	
	if (NULL != outputBuffer) {
		free(outputBuffer);
		outputBuffer = NULL;
	}

	if (NULL != Maddr) {
		free(Maddr);
		Maddr = NULL;
	}
}

