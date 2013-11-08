/* header file for c functions called by assembly */
int C_SWI_Handler(int swi_Num, unsigned int *regs);
void C_IRQ_Handler(void);
unsigned int *pushArgsUntoUserStack(unsigned int argc, char **argv, unsigned int *stackPointer);
