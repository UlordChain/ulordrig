/* Copyright 2016-2018 The Ulord Core Foundation */

#include "common.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

void view_data_u8(const char *mess, 
		uint8_t *data, uint32_t len) {
	printf("%-18s\t", mess);
	for (uint32_t i = 0; i < len; ++i)
		printf("%.2x", data[i]);
	printf("\n");
}

void view_data_u32(const char *mess, 
		uint32_t *data, uint32_t len) {
	printf("%s: ", mess);
	for (uint32_t i = 0; i < len; ++i)
		printf("%.8x ", data[i]);
	printf("\n");
}
