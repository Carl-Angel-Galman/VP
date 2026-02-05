/******************************************************************************
 * @file main.c
 *
 * @author Andreas Schmidt (a.v.schmidt81@googlemail.com)
 * @date   03.01.2026
 *
 * @copyright Copyright (c) 2026
 *
 ******************************************************************************
 *
 * @brief Main file for the VP Template project
 *
 *
 *****************************************************************************/


/***** INCLUDES **************************************************************/
#include "stm32g4xx_hal.h"
#include "System.h"

#include "HardwareConfig.h"

#include "Util/Global.h"
#include "Util/Log/printf.h"
#include "Util/Log/LogOutput.h"

#include "UARTModule.h"
#include "ButtonModule.h"
#include "LEDModule.h"
#include "DisplayModule.h"
#include "ADCModule.h"
#include "TimerModule.h"
#include "Scheduler.h"

#include "GlobalObjects.h"

//#include "StackMonitor.h"


/***** PRIVATE CONSTANTS *****************************************************/


/***** PRIVATE MACROS ********************************************************/


/***** PRIVATE TYPES *********************************************************/


/***** PRIVATE PROTOTYPES ****************************************************/
static int32_t initializePeripherals();


/***** PRIVATE VARIABLES *****************************************************/
static Scheduler gScheduler;            // Global Scheduler instance


/***** PUBLIC FUNCTIONS ******************************************************/

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

/**
 * @brief Main function of System
 */
int main(void)
{
    // Initialize the HAL
    HAL_Init();

    // Initialize the System Clock
    SystemClock_Config();

    // Initialize Peripherals
    initializePeripherals();

    // Initialize Scheduler
    schedInitialize(&gScheduler);

    int globalCounter = 0;
    uint8_t left = 0;

    StackMonitor_Check();

    while (1)
    {
        // Read to buttons

    }
}

/***** PRIVATE FUNCTIONS *****************************************************/

/**
 * @brief Initializes the used peripherals like GPIO,
 * ADC, DMA and Timer Interrupts
 *
 * @return Returns ERROR_OK if no error occurred
 */
static int32_t initializePeripherals()
{
    // Initialize UART used for Debug-Outputs
    uartInitialize(115200);

    // Initialize GPIOs for LED and 7-Segment output
	ledInitialize();
    displayInitialize();

    // Initialize GPIOs for Buttons
    buttonInitialize();

    // Initialize Timer, DMA and ADC for sensor measurements
    timerInitialize();
    adcInitialize();

    return ERROR_OK;
}
