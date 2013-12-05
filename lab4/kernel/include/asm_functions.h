/* function prototypes for functions written in assembly */
int S_Handler(void);
void setIRQStack(unsigned int sp);
int setUserConditions(unsigned int sp, int userAddr);
