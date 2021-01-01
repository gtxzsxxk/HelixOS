#ifndef HL_LIBRARY
#define HL_LIBRARY
#include "helix.h"

void HL_Lib_ARRAY_LEFTMOVE(uint8_t* buffer,uint8_t* tempbuffer,uint8_t len);
uint8_t HL_RegexString(uint8_t* beDetected,uint8_t* key);
uint8_t HL_CompareString(const char* a,const char* b);
void HL_ScanNext(uint8_t* from ,uint8_t* buffer);

void HL_CPU_USAGE_ADD(HL_Thread* ht);

#endif
