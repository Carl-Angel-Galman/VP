/*
 * StackMonitor.c
 *
 *  Created on: Feb 3, 2026
 *      Author: kali
 */




#include "StackMonitor.h"
#include "System.h" // Für State Machine Definitionen

// Symbols from the Linker Script
extern uint32_t _sstack;
extern uint32_t _estack;

#define STACK_MAGIC_PATTERN 0xDEADBEEF

void StackMonitor_Check(void) {
    uint32_t *canaryPtr = (uint32_t*)&_sstack;

    //check if the
    if (*canaryPtr != STACK_MAGIC_PATTERN) {


        Error_Handler();
    }
}

uint32_t StackMonitor_GetUsage(void) {
    uint32_t *searchPtr = (uint32_t*)&_sstack;
    uint32_t freeBytes = 0;

    /* 2. High-Watermark Berechnung */
    /* Suche von unten nach oben, wie viele Muster noch unberührt sind */
    while (searchPtr < (uint32_t*)&_estack && *searchPtr == STACK_MAGIC_PATTERN) {
        freeBytes += 4;
        searchPtr++;
    }

    /* Rückgabe des verbrauchten Speichers in Byte (Integer-Arithmetik) */
    uint32_t totalSize = (uint32_t)&_estack - (uint32_t)&_sstack;
    return (totalSize - freeBytes);
}
