#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bit_functions.h"
#include "decoder.h"
#include "memory.h"
#include "memory_system.h"
#include "cpu.h"
int main(){
	
	load_memory("function_asm.txt");
	set_reg(PC, 0x100);
	show_regs();
	
	
	while (1) {
		char cmd[100], file_name[100];
		int num, num1;
		printf("Enter cmd: ");
		scanf("%99[^\n]", cmd);
		printf("%s\n", cmd);
		if (strcmp(cmd, "set_reg") == 0) {
			printf("Enter reg: ");
			scanf("%d", &num);
			printf("Ender reg value: ");
			scanf("%d", &num1);
			set_reg(num, num1);
		} else if (strcmp(cmd, "dump") == 0) {
			printf("Enter address: ");
			scanf("%x", &num);
			printf("Enter num bytes: ");
			scanf("%d", &num1);
			memory_dump(num, num1);
		} else if (strcmp(cmd, "regs") == 0) {
			show_regs();
		} else if (strcmp(cmd, "step") == 0) {
			step();
		} else if (strcmp(cmd, "step25") == 0){
			for (int i = 0; i < 25; i++){
				step();
			}
		} else if (strcmp(cmd, "load") == 0) {
			printf("Enter file name: ");
			scanf("%s", file_name);
			load_memory(file_name);
		} else {
			printf("invalid cmd\n");
		}
	}
}
