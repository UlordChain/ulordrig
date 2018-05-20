/* Copyright 2016-2018 The Ulord Core Foundation */

#include <stdio.h>
#include <stdlib.h>

#include "cryptoHello.h"
#include "PoW.h"
#include "oneWayFunction.h"

bool cryptoHello::init(){

    initOneWayFunction();

	bool res = false;
	char result_hashcat[65] = "224afc7c2525c33eb428167c1fa1b2fa7cf90576ad875064d9948276218a3a75";
	uint8_t *Maddr = (uint8_t*)malloc(WORK_MEMORY_SIZE*sizeof(uint8_t));
	uint8_t input_hashcat[] = "hashcat";
	uint8_t output_hashcat[32];

    hash(input_hashcat, 7, output_hashcat, Maddr);

	char* output_str = (char*)malloc(65);
	for (uint32_t i = 0; i < 32; ++i)
        sprintf(output_str + (i * 2), "%02x", output_hashcat[i]);
	output_str[64] = '\0';

	if(memcmp(result_hashcat, output_str, 64) == 0){
		// printf("initial test passed, result: %s\n", output_str);
		res = true;
	}
	free(output_str);
	free(Maddr);
   
	return res;
}



bool cryptoHello::hash(const Job &job, uint8_t *result, uint8_t *memory)
{
	cryptoHello::hash(const_cast<uint8_t*>(job.blob()), (uint32_t)job.size(), result, memory);

	return *reinterpret_cast<uint64_t*>(result + 24) < job.target();
}



void cryptoHello::hash(uint8_t *input, uint32_t size, uint8_t *output, uint8_t *memory)
{
    powFunction(input, size, memory, output);
}
