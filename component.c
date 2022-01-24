
/* 
 * Designer:   CHEN HANDONG, 1609853Z-I011-0050, chd19972011@163.com 
*/

#include "minicpu.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	if (ALUControl == 0x0)//add
	{
		*ALUresult = A + B;
		if (*ALUresult == 0)  
			*Zero = 1;
		else
			*Zero = 0;
	}
		
	else if(ALUControl == 0x1)//sub
	{
		*ALUresult = A - B;
		if (*ALUresult == 0)
			*Zero = 1;
		else
			*Zero = 0;
	}
	else if(ALUControl==0x2)//signed
	{
		if((int)A < (int)B)
		{
			*ALUresult = 1;
			*Zero = 0;
		} 
		else
		{ 
			*ALUresult = 0;
			*Zero = 1;
		} 
	}
	else if(ALUControl==0x3)//unsigned 
	{
		if(A < B)
		{ 
			*ALUresult = 1;
			*Zero = 0;
		} 
		else
		{ 
			*ALUresult = 0;
			*Zero = 1;
		} 
	}
	else if (ALUControl == 0x4)//and
	{
		*ALUresult = A & B;
		if (*ALUresult == 0)
			*Zero = 1;
		else
			*Zero = 0;
	}
	else if(ALUControl==0x5)//or
	{
		*ALUresult = A | B;
		if (*ALUresult == 0)
			*Zero = 1;
		else
			*Zero = 0;
	}
	else if(ALUControl==0x6)//shift
	{
   			*ALUresult = B << 16;
			if (*ALUresult == 0)
				*Zero = 1;
			else
				*Zero = 0;
	}
	else if(ALUControl==0x7)//nor
	{
		if (A == 0 && B == 0)
		{
			*ALUresult = 1;
			*Zero = 0;
		}
		else
		{
			*ALUresult = 0;
			*Zero = 1;
		}
	}
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if (Mem[PC >> 2] == NULL)
		return 1;
	else
		*instruction = Mem[PC >> 2];
		return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op = instruction >> 26; 
	if (*op == 0x0) {//R-format
		*r1 = (instruction >> 21) & 0x1f;
		*r2 = (instruction >> 16) & 0x1f;
		*r3 = (instruction >> 11) & 0x1f;
		*funct = instruction & 0x3f;
	}
	else if (*op == 0x3 || *op == 0x2) {//J-format
		*jsec = instruction & 0x3ffffff;
	}
	else {//I-format
		*r1 = (instruction >> 21) & 0x1f;
		*r2 = (instruction >> 16) & 0x1f;
		*offset = instruction & 0xffff;
	}
}



/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	if(op==0x0)//R-format(add, sub, slt, sltu, and, or)
	{
		controls->RegDst = 1;
		controls->ALUSrc = 0;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 0;
		controls->MemWrite = 1;
		controls->Branch = 0;
		controls->ALUOp = 7;
		controls->Jump = 0;
	}
	else if (op == 0x2)//j
	{
		controls->RegDst = 2;
		controls->ALUSrc = 1;
		controls->MemtoReg = 2;
		controls->RegWrite = 0;
		controls->MemRead = 0;
		controls->MemWrite = 1;
		controls->Branch = 0;
		controls->ALUOp = 0;
		controls->Jump = 1;
	}
	else if(op == 0x4)//beq
	{
		controls->RegDst = 2;
		controls->ALUSrc = 0;
		controls->MemtoReg = 2;
		controls->RegWrite = 0;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->Branch = 1;
		controls->ALUOp = 1;
		controls->Jump = 0;
	}
	else if (op == 0x8)//addi
	{
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->Branch = 0;
		controls->ALUOp = 0;
		controls->Jump = 0;
	}
	else if (op == 0xb)//sltiu
	{
		controls->RegDst = 2;
		controls->ALUSrc = 1;
		controls->MemtoReg = 2;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->Branch = 1;
		controls->ALUOp = 3;
		controls->Jump = 0;
	}
	else if (op == 0xa)//slti
	{
		controls->RegDst = 2;
		controls->ALUSrc = 1;
		controls->MemtoReg = 2;
		controls->RegWrite = 1;
		controls->MemRead = 2;
		controls->MemWrite = 0;
		controls->Branch = 1;
		controls->ALUOp = 2;
		controls->Jump = 0;
	}
	else if (op == 0xf)//lui
	{
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 0;
		controls->RegWrite = 1;
		controls->MemRead = 0;
		controls->MemWrite = 0;
		controls->Branch = 0;
		controls->ALUOp = 6;
		controls->Jump = 0;
	}
	else if (op == 0x23)//lw
	{
		controls->RegDst = 0;
		controls->ALUSrc = 1;
		controls->MemtoReg = 1;
		controls->RegWrite = 1;
		controls->MemRead = 1;
		controls->MemWrite = 0;
		controls->Branch = 0;
		controls->ALUOp = 0;
		controls->Jump = 0;
		
	}
	else if (op == 0x2b)//sw
	{
		controls->RegDst = 2;
		controls->ALUSrc = 1;
		controls->MemtoReg = 2;
		controls->RegWrite = 0;
		controls->MemRead = 0;
		controls->MemWrite = 1;
		controls->Branch = 0;
		controls->ALUOp = 0;
		controls->Jump = 0;
	}

	else return 1;	//invalid instruction
	return 0;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	*extended_value = offset + (0xffff0000 & (offset >> 15));
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	switch(ALUOp){
		//add
		case 0:
			
			if (ALUSrc == 1)
				ALU(data1, extended_value, 0x0, ALUresult, Zero); 
			else 
				ALU(data1, data2, 0x0, ALUresult, Zero);	//addi

			break;

		//sub
		case 1:
			
			ALU(data1, data2, 0x1, ALUresult, Zero);	//subi
			
			break;

		//slti
		case 2:
	
			ALU(data1, extended_value, 0x2, ALUresult, Zero);	//slti
			
			break;

		//sltiu
		case 3:
			
			ALU(data1, extended_value, 0x3, ALUresult, Zero);	//sltiu

			break;

		//and
		case 4:

			ALU(data1, data2, 0x4, ALUresult, Zero);	//andi

			break;
			
		//or
		case 5:

			ALU(data1, data2, 0x5, ALUresult, Zero);	//ori

			break;

		//sign extend
		case 6:

			ALU(*Zero, extended_value, 0x6, ALUresult, Zero);	//sign extend

			break;

		// R-type
		case 7:
			// funct = 0x20 = 32
			if (funct==0x20)
				ALU(data1, data2,0x0,ALUresult,Zero);	//add

			else if (funct == 0x22)
				ALU(data1, data2, 0x1, ALUresult, Zero);	//sub

			else if (funct == 0x2a)
				ALU(data1, data2, 0x2, ALUresult, Zero);	//slt

			else if (funct == 0x2b)
				ALU(data1, data2, 0x3, ALUresult, Zero);	//sltu

			else if (funct == 0x24)
				ALU(data1, data2, 0x4, ALUresult, Zero);	//and

			else if (funct == 0x25)
				ALU(data1, data2, 0x5, ALUresult, Zero);	//or

			else if (funct == 0x0)
				ALU(*Zero, extended_value, 0x6, ALUresult, Zero);	//left shift

			else if (funct == 0x26)
				ALU(data1, data2, 0x7, ALUresult, Zero);	//nor

			else return 1;	//invalid funct

			break;

		default:
			return 1;		//invalid ALUop
	}
	return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if(MemRead == 1 && MemWrite == 0)//read
	{
			*memdata = Mem[ALUresult >> 2];
	}
	else if (MemRead == 0 && MemWrite == 1)//write
	{
			Mem[ALUresult >> 2] = data2;
	}
	else if (MemRead == 2 || MemWrite == 2)
	{
		return 0;
	}

	return 0;
}

 
/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if (RegWrite == 1)
	{
		if (MemtoReg == 1)//from memory
			if (RegDst == 1)//R-type
				Reg[r3] = memdata;
			else//I-type
				Reg[r2] = memdata;
		else//from register
			if (RegDst == 1)//R-type
				Reg[r3] = ALUresult;
			else//I-type
				Reg[r2] = ALUresult;
	}
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	if (Jump == 1)
		*PC = (*PC + 4) << 2;
	else if (Branch == 1 && Zero == 1)
	{
		*PC = *PC + 4 + (extended_value << 2);
	}
	else
		*PC = *PC + 4;
}
