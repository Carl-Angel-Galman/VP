/******************************************************************************
 * @file startup.s
 *
 * @author Andreas Schmidt (a.v.schmidt81@googlemail.com)
 * @date   03.01.2026
 *
 * @copyright Copyright (c) 2026
 *
 ******************************************************************************
 *
 * @brief Startup Code for VPTemplate Project
 *
 *
 *****************************************************************************/

.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

/**
 * @brief  This is the code that gets called when the processor first
 * starts execution following a reset event. Only the absolutely
 * necessary set is performed, after which the application
 * supplied main() routine is called.
 *
 * @param  None
 *
 * @retval : None
 */
.section .text.Reset_Handler
.type Reset_Handler, %function
.global Reset_Handler
Reset_Handler:
    /* Copy the data segment initializers from flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sloaddata
    movs r3, #0
    b .loopCopyData

.copyData:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

.loopCopyData:
    adds r4, r0, r3
    cmp r4, r1
    bcc .copyData

    /* Zero fill the bss segment. */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b .loopFillZerobss

.fillZerobss:
    str  r3, [r2]
    adds r2, r2, #4

/* ... (Anfang bis .loopFillZerobss bleibt gleich) ... */

.loopFillZerobss:
    cmp r2, r4
    bcc .fillZerobss

    /* --- START STACK MONITOR INITIALIZATION --- */
    /* Füllen des Bereichs von _sstack bis _estack mit dem Muster */
    ldr r0, =_sstack             /* Startadresse laden */
    ldr r1, =_estack             /* Endadresse laden */
    ldr r2, = STACK_MAGIC_PATTERN          /* Magic Pattern (Assembler nutzt Literal Pool) */
    b .loopFillStack

.fillStack:
    str  r2, [r0]                /* Wort (4 Byte) in RAM schreiben */
    adds r0, r0, #4              /* Adresse um 4 erhöhen */

.loopFillStack:
    cmp r0, r1                   /* Grenze erreicht? */
    bcc .fillStack               /* Wenn r0 < r1, weiter füllen */
    /* --- END STACK MONITOR INITIALIZATION --- */

    /* Initialize the Stack-Pointer */
    /* Setzen des SP auf die höchste Adresse (_estack) */
    ldr r0, =_estack
    mov sp, r0

    /* Call the clock system initialization function.*/
    bl  SystemInit

    /* Call the application's entry point.*/
    bl main
    bx lr

/* Definition der Konstante am Ende des Codes */
.align 4
STACK_MAGIC_PATTERN:
    .word 0xDEADBEEF
/* Ende des Reset_Handlers */
.size Reset_Handler, .-Reset_Handler
