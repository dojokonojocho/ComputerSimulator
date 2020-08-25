#include "memory_system.h"
#include "memory.h"
#include "decoder.h"
#include "bit_functions.h"
#include <stdio.h>
#include <stdlib.h>

char * decToHex(int n) {
	char *str = (char *) malloc(sizeof(char) * 4);
	for (int j = 0; j < 4; j++){
		str[j] = '0';
	}      
    for (int i = 3; i >= 0; i--) {    
        int temp  = 0; 
        temp = n % 16; 
        if(temp < 10) { 
            str[i] = temp + 48; 
        } else { 
            str[i] = temp + 55; 
        }          
        n = n/16; 
    }
    return str;
} 

int address_to_row(int address){
	int row = address/31;
	if (row < 0)
		row = 0;
	row = decoder(row);
	row = bit_find(row);
	return row;
}

int address_to_col(int address){
	int col = decoder(address);
	col = bit_find(col);
	return col;
}

void memory_store(int address, unsigned char value){
	int col = address_to_col(address);
	int row = address_to_row(address);
	mem_put(row, col, value);
	return;
}


void memory_store_word(int address, unsigned int value){
	for (int i = 0; i < 4; i++){
		int col = address_to_col(address);
		int row = address_to_row(address);
		int x = (value >> (8*i)) & 0xff;
		mem_put(row, col, x);
		address++;
	}
	
	return;
}


unsigned char memory_fetch(int address){
	int col = address_to_col(address);
	int row = address_to_row(address);
	return mem_get(row, col);
}


unsigned int memory_fetch_word(int address){
	unsigned char a[4];
	for (int i = 0; i < 4; i++){
		int col = address_to_col(address);
		int row = address_to_row(address);
		a[i] = mem_get(row, col);
		address++;
	}
	return *(int *)a;
}


void memory_fill(int address, unsigned char value, int num_bytes){
	for (int i = 0; i < num_bytes; i++){	
		int row = address_to_row(address);
		int col = address_to_col(address);
		mem_put(row, col, value);
		address++;
	}
	
}


void memory_dump(int start_address, int num_bytes){
	int address = start_address;
	for (int i = 0; i < num_bytes; i++){
		printf("0x%s   (0d%.4d)  0x%.2x  0x%.2x  0x%.2x  0x%.2x  0x%.2x  0x%.2x  0x%.2x  0x%.2x\n", decToHex(address), address, memory_fetch(address+7), memory_fetch(address+6), memory_fetch(address+5), memory_fetch(address+4), memory_fetch(address+3), memory_fetch(address+2), memory_fetch(address+1), memory_fetch(address));
		address += 8;
	}
}



void load_memory(char *filename){
	FILE *file_ptr;
	file_ptr = fopen(filename, "r");
	
	if (!file_ptr){
		printf("There was an error opening %s\n", filename);
		return;
	}	
	
	int first = 0;
	int num = 0;
	int address = 0;
	while(fscanf(file_ptr, "%x", &num) != EOF){
		if (first == 0){
			address = num;
			first = 1;
		} else {
		memory_store_word(address, num);
		address += 4;
		}
	}
	
	fclose(file_ptr);

}


