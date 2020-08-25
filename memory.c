#include "bit_functions.h"
#include "memory.h"
#include <stdio.h>

unsigned char mem_get(unsigned int row_selected, unsigned int col_selected){
	return memArray[row_selected][col_selected];
}

void mem_put(unsigned int row_selected, unsigned int col_selected, unsigned char b){
	memArray[row_selected][col_selected] = b;	
}
