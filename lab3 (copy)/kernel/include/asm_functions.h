/* function prototypes for functions written in assembly */
int S_Handler(void);
int I_Handler(void);
void setIRQStack(unsigned int sp);
int setUserConditions(int argc, char *argv[], int userAddr);
