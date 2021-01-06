#include "../inc/asm.h"

		
//------------------------------------------------------------------------------
//   void CS_contextSave(void)
//   void CS_contextRestore(void)
//
// These two functions are needed for the context switch during the power
// save procedure. The purpose of the CS_contextSave() function is to 
// either save the current context and trigger sleeping through the 'WFI' 
// instruction. 
// The CS_contextRestore() function restores the context saved before 
// to go in deep sleep.  
// All the Cortex M0 registers are saved and restored plus after wakeup
// -----------------------------------------------------------------------------
                 __CODE__
                 __THUMB__
                 __EXPORT__ CS_contextSave
                 __EXPORT__ CS_contextRestore
                 __IMPORT__ savedMSP
                 __IMPORT__ savedICSR
                 __IMPORT__ savedSHCSR
                 __IMPORT__ savedNVIC_ISPR
EXPORT_FUNC(CS_contextSave)
                 MRS    R2, CONTROL             /* load the CONTROL register into R2 */
                 MRS    R1, PSP                 /* load the process stack pointer into R1 */
                 LDR    R0, =0
                 MSR    CONTROL, R0             /* Switch to Main Stack Pointer */
                 ISB
#ifdef CONTEXT_SAVE_V2
                 SUB    SP, #0x44			    /* Move stack pointer in order to be sure that after wake up the __low_level_init()
                                                   will not corrupt data saved in the stack by CS_contextSave routine.
                                                   WARNING: this instruction breaks backward compatibility with previous CS_contextRestore. */ 
#endif

                 PUSH   { r4 - r7, lr }         /* store R4-R7 and LR (5 words) onto the stack */
                 MOV    R3, R8                  /* move {r8 - r12} to {r3 - r7} */    
                 MOV    R4, R9
                 MOV    R5, R10
                 MOV    R6, R11        
                 MOV    R7, R12        
                 PUSH   {R3-R7}                 /* store R8-R12 (5 words) onto the stack */
                 
                 LDR    R4, =savedMSP           /* load address of savedMSP into R4 */
                 MRS    R3, MSP                 /* load the stack pointer into R3 */
                 STR    R3, [R4]                /* store the MSP into savedMSP */

                 PUSH   { r1, r2 }               /*  store PSP, CONTROL */
                 
                 LDR    R4, =0xE000ED04         /* load address of ICSR register into R4 */
                 LDR    R0, [R4]                /* load the ICSR register value into R0 */
                 LDR    R4, =savedICSR          /* load address of savedICSR into R4 */
                 STR    R0, [R4]                /* store the ICSR register value into savedICSR */

                 LDR    R4, =0xE000ED24         /* load address of SHCSR register into R4 */
                 LDR    R0, [R4]                /* load the SHCSR register value into R0 */
                 LDR    R4, =savedSHCSR         /* load address of savedSHCSR into R4 */
                 STR    R0, [R4]                /* store the SHCSR register value into savedSHCSR */

                 LDR    R4, =0xE000E200         /* load address of NVIC_ISPR register into R4 */
                 LDR    R0, [R4]                /* load the NVIC_ISPR register value into R0 */
                 LDR    R4, =savedNVIC_ISPR     /* load address of savedNVIC_ISPR into R4 */
                 STR    R0, [R4]                /* store the NVIC_ISPR register value into savedNVIC_ISPR */

                 LDR    R4, =0x40200008         /* setup the  SYSTEM_CTRL->CTRL_b.MHZ32_SEL = 0 */
                 LDR    R7, [R4]
                 MOVS   R0, #0                 
                 STR    R0, [R4]

                 DSB
                 WFI                            /* all saved, trigger deep sleep */
                 
                 STR    R7, [R4]                /* if WFI will be skipped restore the content of the 
                                                   SYSTEM_CTRL->CTRL_b.MHZ32_SEL with the original value */                 
                 ENDFUNC                 
EXPORT_FUNC(CS_contextRestore)
                /* Even if we fall through the WFI instruction, we will immediately
                 * execute a context restore and end up where we left off with no
                 * ill effects.  Normally at this point the core will either be
                 * powered off or reset (depending on the deep sleep level). */
                LDR    R4, =savedMSP            /* load address of savedMSP into R4 */
                LDR    R4, [R4]                 /* load the MSP from savedMSP */
                MSR    MSP, R4                  /* restore the MSP from R4 */
              
                SUB    SP, #0x8               
                POP    { R0, R1 }               /* load PSP from the stack in R0, and  load CONTROL register from the stack in R1 */
                
                POP    { R3-R7 }                /* load R8-R12 (5 words) from the stack */
                MOV    R8, R3                   /* mov {r3 - r7} to {r8 - r12} */
                MOV    R9, R4
                MOV    R10, R5
                MOV    R11, R6
                MOV    R12, R7
                POP    { R4 - R7 }              /* load R4-R7 (4 words) from the stack */
                POP    { R2 }                   /* load LR from the stack */
#ifdef CONTEXT_SAVE_V2
                ADD	   SP, #0x44                /* Restore MSP to the point where it was before pushing data to the stack in CS_contextSave */
#endif
                
                MSR    PSP, R0                   /* restore PSP from R0 */
                MSR    CONTROL , R1              /* restore CONTROL register from R1 */
                ISB
 
                BX  R2                          /*load PC (1 words) from the stack */
                                 
                ENDFUNC
               
    ALIGN_MEM(4)
	__END__
