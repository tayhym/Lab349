/* header file for c functions called by assembly */
int C_SWI_Handler(int swi_Num, unsigned int *regs);
void install_Handler(unsigned int location, unsigned int *vector);  
unsigned int *pushArgsUntoUserStack(unsigned int argc, char **argv, unsigned int *stackPointer);
