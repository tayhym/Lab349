/* header file for c functions called by assembly */
int C_SWI_Handler(int swi_Num, unsigned int *regs);
unsigned install_Handler(unsigned int swiLocation, unsigned int *uBootSwiAddr);   
