#ifndef SYSTEM_UTIL
#define SYSTEM_UTIL


#include <sys/types.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>

int invoke_system_call(const char* call, char* buff, int buff_len);
int getProcessNumber();

#endif