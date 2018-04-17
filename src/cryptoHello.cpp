#include <stdio.h>
#include <stdlib.h>

#include "crypto/PoW.h"
#include "crypto/oneWayFunction.h"
#include "cryptoHello.h"

#include "net/Job.h"

bool cryptoHello::init(){

    initOneWayFunction();
  {
	uint8_t *Maddr = (uint8_t*)malloc(WORK_MEMORY_SIZE*sizeof(uint8_t));
	uint8_t input_hashcat[] = "hashcat";
	uint8_t output_hashcat[32];
    hash(input_hashcat, 7, output_hashcat, Maddr);
	for (uint32_t i = 0; i < 32; ++i)
        printf("%.2x", output_hashcat[i]);
    printf("\n");
	free(Maddr);
  } 
	return true;
}



bool cryptoHello::hash(const Job &job, uint8_t *result, uint8_t *memory)
{
	cryptoHello::hash(const_cast<uint8_t*>(job.blob()), (uint32_t)job.size(), result, memory);
	
	if(*reinterpret_cast<uint64_t*>(result + 24) < job.target()){
		for (uint32_t i = 0; i < 32; ++i)
        	printf("%.2x", result[i]);
     	printf("\n");
	}

	return *reinterpret_cast<uint64_t*>(result + 24) < job.target();
}



void cryptoHello::hash(uint8_t *input, uint32_t size, uint8_t *output, uint8_t *memory)
{

    powFunction(input, size, memory, output);
/*
	uint8_t *Maddr = (uint8_t*)malloc(WORK_MEMORY_SIZE*sizeof(uint8_t));
	uint8_t test_input[8], test_output[32];
    memset(test_input, 0, 8);
    memcpy(test_input, "hashcat", 7);
    powFunction(test_input, 7, Maddr, test_output);
    for (uint32_t i = 0; i < 32; ++i)
        printf("%.2x", test_output[i]);
    printf("\n");
   	free(Maddr);
*/
}
