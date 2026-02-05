/*
 * StackMonitor.c
 *
 *  Created on: Feb 4, 2026
 *      Author: kali
 */

#include "StackMonitor.h"

#include "stdbool.h"

#include "Util/Global.h"


extern uint32_t _estack;

extern uint32_t _sstack;

#define ENDMARKER 0xEA1DADAB

#define MARKER 0xdec0adde

#define STACK_SIZE 0x1000


uint32_t GetFreeBytes(void)
{
    uint32_t *p  = &_sstack;
    uint32_t *high = &_estack;

    uint32_t byte_counter = 0;

    while (p < high && *p == MARKER) {
        p++;
        byte_counter++;
    }

    return byte_counter;
}

uint32_t GetUsedBytes(void)
{
	uint32_t *p  = &_sstack;
	uint32_t *high = &_estack;

	uint32_t byte_counter = 0;

	while (p < high && *p == MARKER) {
		p++;
		byte_counter++;
}

	  return STACK_SIZE - byte_counter;

}

uint8_t GetUsage(void)
{

	return GetUsedBytes() >> 12;
}

bool isCorrupted(void)
{
	uint32_t* p = &_sstack;
	if(*p == MARKER)
	{
		return false;
	}
	return true;

}
