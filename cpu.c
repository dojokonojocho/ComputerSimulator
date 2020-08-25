#include "cpu.h"
#include "memory_system.h"
#include "bit_functions.h"
#include <stdio.h>
#include <stdlib.h>
void set_reg(int reg, int value){
	registers[reg] = value;
}

int get_reg(int reg){
	return registers[reg];
}

int get_cpsr(){
	return cpsr;
}

void show_regs(){
	for (int i = 0; i < 16; i++){
		printf("Reg%d: %d\n", i, registers[i]);
	}
}

void step(){
	int inst = memory_fetch_word(registers[PC]);
	int opcode = (inst >> 24) & 0xff;
	printf("PC: 0x%.8x\tInst: 0x%.8x  ", registers[PC], inst);
	switch (opcode) {  
		case LDR: ;
			int reg = inst >> 16 & 0Xff;
			int address = inst & 0xffff;
			if (address > 1023 || reg > 15) {
				printf("\nAddress/Register out of bounds\n");
				exit(1);
			}
			registers[reg] = memory_fetch_word(address);
			printf("ARM: LDR R%d, %d\n", reg, address);
			printf("reg: %d\treg val: %d\taddress: 0x%04x\n", reg, registers[reg], address);
			break;
		case LDI: ;
			reg = inst >> 16 & 0xff;
			int immediate = inst & 0xffff;
			if (reg > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			registers[reg] = immediate;
			printf("ARM: LDI R%d, #%d\n", reg, immediate);
			printf("reg: %d\treg val: %d\n", reg, registers[reg]);
			break;
		case LDX: ;
			int dest_reg = inst >> 16 & 0Xff;
			int offset = inst >> 8 & 0xff;
			reg = inst & 0xff;
			if (dest_reg > 15 || reg > 15) {
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			registers[dest_reg] = memory_fetch_word(registers[reg]+offset);
			printf("ARM: LDX R%d, [R%d], #%d\n", dest_reg, reg, offset);
			printf("reg: %d\treg val: %d\taddress: 0x%04x\n", dest_reg, registers[dest_reg], registers[reg]+offset);
			break;
		case STR: ;
			reg = inst >> 16 & 0Xff;
			address = inst & 0xffff;
			if (address > 1023 || reg > 15) {
				printf("\nAddress/Register out of bounds\n");
				exit(1);
			}
			memory_store_word(address, registers[reg]);
			printf("ARM: STR R%d, %x\n", reg, address);
			printf("address: %x\taddress val: %d\n", address, registers[reg]);
			break;
		case ADD: ;
			dest_reg = inst >> 16 & 0xff;
			int reg1 = inst >> 8 & 0xff;
			int reg2 = inst & 0xff;
			if (dest_reg > 15 || reg1 > 15 || reg2 > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			registers[dest_reg] = registers[reg1] + registers[reg2];
			printf("ARM: ADD R%d, R%d, R%d\n", dest_reg, reg1, reg2);
			printf("reg: %d\treg val: %d\n", dest_reg, registers[dest_reg]);
			break;
		case SUB: ;
			dest_reg = inst >> 16 & 0xff;
			reg1 = inst >> 8 & 0xff;
			reg2 = inst & 0xff;
			if (dest_reg > 15 || reg1 > 15 || reg2 > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			printf("ARM: SUB R%d, R%d, R%d\n", dest_reg, reg1, reg2);
			registers[dest_reg] = registers[reg1] - registers[reg2];
			printf("reg: %d\treg val: %d\n", dest_reg, registers[dest_reg]);
			break;
		case MUL: ;
			dest_reg = inst >> 16 & 0xff;
			reg1 = inst >> 8 & 0xff;
			reg2 = inst & 0xff;
			if (dest_reg > 15 || reg1 > 15 || reg2 > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			printf("ARM: MUL R%d, R%d, R%d\n", dest_reg, reg1, reg2);
			registers[dest_reg] = registers[reg1] * registers[reg2];
			printf("reg: %d\treg val: %d\n", dest_reg, registers[dest_reg]);
			break;
		case DIV: ;
			dest_reg = inst >> 16 & 0xff;
			reg1 = inst >> 8 & 0xff;
			reg2 = inst & 0xff;
			if (dest_reg > 15 || reg1 > 15 || reg2 > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			registers[dest_reg] = registers[reg1] / registers[reg2];
			printf("ARM: DIV R%d, R%d, R%d\n", dest_reg, reg1, reg2);
			printf("reg: %d\treg val: %d\n", dest_reg, registers[dest_reg]);
			break;
		case CMP: ;
			reg1 = inst >> 8 & 0xff;
			reg2 = inst & 0xff;
			int reg_val1 = registers[reg1];
			int reg_val2 = registers[reg2];
			printf("ARM: CMP R%d, R%d\n", reg1, reg2);
			if (reg1 > 15 || reg2 > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			printf("Comparing: %d, %d\n", reg_val1, reg_val2);
			if (reg_val1 > reg_val2){
				bit_set(&cpsr, GT);
				bit_clear(&cpsr, Z);
				bit_clear(&cpsr, LT);
				printf("GT flag set\n");
			} else if (reg_val1 == reg_val2){
				bit_set(&cpsr, Z);
				bit_clear(&cpsr, GT);
				bit_clear(&cpsr, LT);
				printf("Z flag set\n");
			} else{
				bit_set(&cpsr, LT);
				bit_clear(&cpsr, Z);
				bit_clear(&cpsr, GT);
				printf("LT flag set\n");
			}
			break;
		case B: ;
			address = inst & 0xffffff;
			if (address > 1023) {
				printf("\nAddress out of bounds\n");
				exit(1);
			}
			registers[PC] = address-4;
			printf("ARM: B 0x%x\n", address);
			printf("PC: 0x%x\n", address);
			break;
		case BEQ: ;
			address = inst & 0xffffff;
			if (address > 1023) {
				printf("\nAddress out of bounds\n");
				exit(1);
			}
			printf("ARM: BEQ 0x%x\n", address);
			if (bit_test(cpsr, Z)){
				registers[PC] = address-4;
				printf("PC: 0x%x\n", address);
			} else
				printf("PC: 0x%x\n", registers[PC]);
			break;
		case BNE: ;
			address = inst & 0xffffff;
			if (address > 1023) {
				printf("\nAddress out of bounds\n");
				exit(1);
			}
			printf("ARM: BNE 0x%x\n", address);
			if (!bit_test(cpsr, Z)){
				registers[PC] = address-4;
				printf("PC: 0x%x\n", address);
			} else
				printf("PC: 0x%x\n", registers[PC]);
			break;
		case BGT: ;
			address = inst & 0xffffff;
			if (address > 1023) {
				printf("Address out of bounds\n");
				exit(1);
			}
			printf("ARM: BGT 0x%x\n", address);
			if (bit_test(cpsr, GT)){
				registers[PC] = address-4;
				printf("PC: 0x%x\n", address);
			} else
				printf("PC: 0x%x\n", registers[PC]);
			break;
		case BLT: ;
			address = inst & 0xffffff;
			if (address > 1023) {
				printf("\nAddress out of bounds\n");
				exit(1);
			}
			printf("ARM: BLT 0x%x\n", address);
			if (bit_test(cpsr, LT)){
				registers[PC] = address-4;
				printf("PC: 0x%x\n", address);
			} else
				printf("PC: 0x%x\n", registers[PC]);
			break;
		case BL: ;
			address = inst & 0xffffff;
			printf("ARM: BL 0x%d\n", address);
			if (address > 1023) {
				printf("\nAddress out of bounds\n");
				exit(1);
			}
			registers[LR] = registers[PC];
			registers[PC] = address-4;
			printf("PC: 0x%x\tLR: %x\n", registers[PC]+4, registers[LR]);
			break;
		case MOV: ;
			reg1 = inst >> 8 & 0xff;
			reg2 = inst & 0xff;
			if (reg1 > 15 || reg2 > 15){
				printf("\nRegister out of bounds\n");
				exit(1);
			}
			printf("ARM: MOV R%d, R%d\n", reg1, reg2);
			registers[reg1] = registers[reg2];
			printf("reg: %d\treg val: %d\n", reg1, registers[reg1]);
			break;
		case END: ;
			printf("\nExiting\n");
			exit(1);
			break;
	}
	registers[PC] += 4;
}

void step_n(int n){
	for (int i = 0; i < n; i++){
		step();
	}
}
