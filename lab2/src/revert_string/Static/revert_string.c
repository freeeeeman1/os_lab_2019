#include "revert_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void RevertString(char *str)
{
    int len=strlen(str);
	char*NewStr = (char*) calloc(len, sizeof(char));
    int i=0;
    for(i=len-1;i>-1;i--)
    {
    NewStr[len-i-1]= str[i]; 
    }
    strcpy(str,NewStr);    
}
