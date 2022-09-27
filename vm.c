#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

int base(int *stack, int BP, int L);
void print_instruction(int PC, instruction IR);
void print_stack(int PC, int BP, int SP, int *stack);
// Functional Stack
int stack[ARRAY_SIZE];
// Registers
int BP = 0;
int SP = -1;
int PC = 0;
instruction IR;
int halt = 0;

// Code is an instruction array which is the text section of the VM's memory
void execute(int trace_flag, instruction *code)
{
	if (trace_flag)
	{
		printf("VM Exectution:\n");
		printf("\t\t\t\tPC\tBP\tSP\tstack\n");
		printf("Initial Values:\t\t\t0\t0\t-1\n");
		// Print initial values of registers and the stack
		print_stack(PC,BP,SP,stack);
	}
	while(halt==0) {
		// Fetch
		PC += 1;
		IR = code[PC];

		// Print Instruction
		print_instruction(PC,IR);
		// Execute
		switch (IR.op)
	{
		case LIT :
			SP++;
			stack[SP] = IR.m;
			break;
		case OPR :
			switch (IR.m)
			{
				case ADD :
					SP--;
					stack[SP] = (stack[SP] + stack[SP+1]);
					break;
				case SUB :
					SP--;
					stack[SP] = (stack[SP] - stack[SP+1]);
					break;
				case MUL :
					SP--;
					stack[SP] = (stack[SP] * stack[SP+1]);
					break;
				case DIV :
					SP--;
					stack[SP] = (stack[SP] / stack[SP+1]);
					break;
				case EQL :
					SP--;
					stack[SP] = stack[SP] == stack[SP+1];
					break;
				case NEQ :
					SP--;
					stack[SP] = (stack[SP] != stack[SP+1]);
					break;
				case LSS :
					SP--;
					stack[SP] = (stack[SP] < stack[SP+1]);
					break;
				case LEQ :
					SP--;
					stack[SP] = (stack[SP] <= stack[SP+1]);
					break;
				case GTR : 
					SP--;
					stack[SP] = (stack[SP] > stack[SP+1]);
					break;
				case GEQ :
					SP--;
					stack[SP] = (stack[SP] >= stack[SP+1]);
					break;
			}
			break;
		case LOD :
			SP++;
			stack[SP] = stack[base(stack,BP,IR.l) + M];
			break;
		case STO :
			stack[base(stack,BP,IR.l) + M] = stack[SP];
			SP--;
			break;
		case CAL :
			stack[SP+1] = base(stack, BP, IR.l); //Static Link
			stack[SP+2] = BP;					// Dynamic Link
			stack[SP+3] = PC;				   // Return Address
			BP = SP + 1;
			PC = IR.m;
			break;
		case RTN : 
			SP = BP - 1;
			BP = stack[SP+2];	// Dynamic Link
			PC = stack[SP+3]; // Return Address
			break;
		case INC :
			SP += IR.m;
			break;
		case JMP :
			PC = IR.m;
			break;
		case JPC :
			if(stack[SP] == 0) {
				PC = IR.m;
			}
			SP--;
			break;
		case SYS :
			switch (IR.m)
			{
				case WRT :
				// Do I need to set the Stack Pointer to a value?
					printf("\nOutput :%d", stack[SP]);
					printf("\n\t\t\t\t");
					SP -= 1;
					break;
				case RED :
					SP +=1;
					printf("\nInput :");
					scanf("%d",&stack[SP]);
					printf("\t\t\t\t");
					break;
				case HLT : 
					halt=1; 
					break;
			}
			break;
	}
	}
}

int base(int *stack, int BP, int L)
{
	while (L > 0)
	{
		BP = stack[BP];
		L--;
	}
	return BP;
}


void print_stack(int PC, int BP, int SP, int *stack)
{
	int i;
	printf("%d\t%d\t%d\t", PC, BP, SP);
	for (i = 0; i <= SP; i++)
		printf("%d ", stack[i]);
	printf("\n");
}

void print_instruction(int PC, instruction IR)
{
	char opname[4];
	
	switch (IR.op)
	{
		case LIT : strcpy(opname, "LIT"); break;
		case OPR :
			switch (IR.m)
			{
				case ADD : strcpy(opname, "ADD"); break;
				case SUB : strcpy(opname, "SUB"); break;
				case MUL : strcpy(opname, "MUL"); break;
				case DIV : strcpy(opname, "DIV"); break;
				case EQL : strcpy(opname, "EQL"); break;
				case NEQ : strcpy(opname, "NEQ"); break;
				case LSS : strcpy(opname, "LSS"); break;
				case LEQ : strcpy(opname, "LEQ"); break;
				case GTR : strcpy(opname, "GTR"); break;
				case GEQ : strcpy(opname, "GEQ"); break;
				default : strcpy(opname, "err"); break;
			}
			break;
		case LOD : strcpy(opname, "LOD"); break;
		case STO : strcpy(opname, "STO"); break;
		case CAL : strcpy(opname, "CAL"); break;
		case RTN : strcpy(opname, "RTN"); break;
		case INC : strcpy(opname, "INC"); break;
		case JMP : strcpy(opname, "JMP"); break;
		case JPC : strcpy(opname, "JPC"); break;
		case SYS : 
			switch (IR.m)
			{
				case WRT : strcpy(opname, "WRT"); break;
				case RED : strcpy(opname, "RED"); break;
				case HLT : strcpy(opname, "HLT"); break;
				default : strcpy(opname, "err"); break;
			}
			break;
		default : strcpy(opname, "err"); break;
	}
	
	printf("%d\t%s\t%d\t%d\t", PC - 1, opname, IR.l, IR.m);
}