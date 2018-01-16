#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MEM 16384  // memory size
#define EQ 0
#define NE 1
#define CS 2
#define CC 3
#define MI 4
#define PL 5  
#define HI 8
#define LS 9
#define AL 0XE
#define imm 0x000000FF
#define process 0
#define regTransfer 0x4
#define blockTransfer 0x8
#define branch 0x10
#define STOP 0xE 
#define STP 1
#define AND  0x0
#define EOR  0x1
#define SUB  0x2
#define ADD  0x4
#define ADC  0x5
#define TST  0x8	
#define CMP  0xA  
#define ORR  0xC
#define MOV  0xD
#define BIC  0xE
#define MVN  0XF
#define TEQ  0x9
#define LDR 2
#define STR 0
#define LDB 3
#define STB 1
#define PSH 0
#define PULL 1 
#define byteMask 0x0000F000
#define BRA 0
#define BRL 1
#define LSR 21
#define LSL 20



/*
   Sabir Rustaqi
   January 29 2012

   Program Objective- virtual PC  only l, h , ? q ,z, r, t commands works rest are not implemented
 */

unsigned char buffer[MEM];// memory

unsigned long R[14];  // registers
unsigned long PC ;
unsigned long IR;
unsigned long MBR;
unsigned long MAR ;
unsigned long Condition;
int SP =13 ;
unsigned long Rn;
unsigned long Rd;
unsigned long Rm;
unsigned long ZF; // flags
unsigned long CF;
unsigned long NF;
unsigned long alu;
int i;
int temp;
unsigned long LR;
unsigned long list,list1;
unsigned long testreg;
unsigned long regtype = 0;
char p;
int x;
    char ch;
    int a;
    int offset;
    int length;
    int result;

int Execute ()    // execute the code
{ 
    if (conditionType()) // check if condition is true
    {


	switch (instructionType()) {
	    case process:{
			     switch (whichOpcode()){  // check which opcode

				 case MOV:

				     alu = op2();

				     R[Rd] = alu;

				     break;
				 case MVN :

				     R[Rd] = ~op2();
				     break;
				 case ADD :
				     CF = isFlagCarry(R[Rn],op2(),CF);     // check if it is a carry flag


				     R[Rd] = R[Rn] + op2();
				     break;


				 case AND:
				     CF =isFlagCarry(R[Rn],op2(),CF);
				    

					 R[Rd] = R[Rn] & op2();
				     break;
				 case BIC: 
				     R[Rd] = R[Rn]  & ~op2();
				     break;
				 case CMP: 
				     CF=isFlagCarry(R[Rn],op2(),CF);

				     R[Rn]=R[Rn]  - op2();
				     break;
				 case ORR:
				     R[Rd] = R[Rn] | op2();
				     break;
				 case SUB:
				     CF =isFlagCarry(R[Rn],op2(),CF);
				     R[Rd] = R[Rn] - op2();
				     break;
				 case TST:
				     R[Rn] = R[Rn] & op2();
				     break;
				 case TEQ:
				     R[Rd] = R[Rd] ^ op2();

				     break;
				 case EOR:
				     R[Rd] = R[Rn] ^ op2();
				     break;
				 case ADC:


				     CF= isFlagCarry(R[Rn],op2(),CF);

				     temp = CF;

				     R[Rd] = R[Rn] + op2() + temp;
				     break; 
			     }

			     NZflags();
			 }break;






	    case blockTransfer:{

				   switch(Blocktransfer())   // chech if it is a PSH or PULL instruction
				   {


				       case PSH:
					   list = IR & 0x0000FFFF;
					   while (SP > 0)
					   {	
					       testreg =  list & 0x0001;
					       if(testreg ==1)
					       {
						   R[SP]= R[regtype];
					       }
					       regtype++;
					       SP--;
				list =list >> 1;
					   }

					   break;
				       case PULL:
					   list1 = IR & 0x0000FFFF;
					   while (SP < 13 )
					   {
					       testreg =  list1 & 0x0001;
					       if(testreg ==1)
					       {
						   R[SP] = R[regtype];
					       }
					       regtype--;
					       SP++;
			list1 =list1 >> 1;
					   }
					   break;
				   }
				   break;
				   case branch:{     // check if there is a branch instruction
						   switch(whichBranch())
						   {
						       case BRA:
							   PC = offsetmem();

							   break;
						       case BRL:
							   LR = PC;
							   PC = offsetmem();
							   break;
						   }
					       }break;
				   case STOP: {
               		return 0;
						  case STP:


						      break;
					      }
					      break;


				   case regTransfer: {
							 switch (whatRegisterInstruction()){

							     case LDR:
								 MBR= R[Rd];


								 R[Rd] = (R[Rd] << 8) | buffer[MAR];
								 MAR = MAR +1;
								 R[Rd] = (R[Rd] << 8) | buffer[MAR];
								 MAR = MAR +1;
								 R[Rd] = (R[Rd] << 8) | buffer[MAR];
								 MAR = MAR +1;
								 R[Rd] = (R[Rd] << 8) | buffer[MAR];
								 MBR =R[Rd];
								 MAR = R[Rn];
								 break;
							     case STR:
								 MAR = R[Rn];
								 R[Rn] = (R[Rn] << 8) | buffer[MAR];
								 MAR = MAR +1;
								 R[Rn] = (R[Rn] << 8) | buffer[MAR];
								 MAR = MAR +1;
								 R[Rn] = (R[Rn] << 8) | buffer[MAR];
								 MAR = MAR +1;
								 R[Rn] = (R[Rn] << 8) | buffer[MAR];


								 R[Rd] =MBR;

								 break;

							     case LDB:
								 MBR = R[Rd];
								 MAR= R[Rn];

								 break;

							     case STB:
								 MAR = R[Rn];
								 R[Rd]=MBR;
								 break;
							 }


						     }break;


			       }





	}   
    }
}
int shiftAmount()
{
    int type =IR & 0x1F0;
    return type;
}

int shiftType()
{
    int type = IR & 0xC0;
    return type + 20;
} 


int NZflags()  // check negative and zero flags
{
    if (R[Rd] > (0xFFFFFFF + op2()))
	NF = 1; 
    if (R[Rd] ==0)
    {
	ZF =1;
    }
}

int offsetmem(){    // chech offset for branch
    int offset = IR & 0x00FFFFFF;
    return offset;
}

int whichBranch(){  // check for which branch

    int type;
    type = (IR >> 24) & 0x01;
    return type;


}


int Blocktransfer(){  // check for which  transer
    int type;
    type = (IR >> 24) & 0x01;
    return type;
}
int isFlagCarry(unsigned long op1, unsigned long op2, int n)

{

    if ((op1 + n) == 0) return (1);


    return ((op1 > (0xFFFFFFFF - op2)) ? 1 : 0);

    fprintf(stdout,"op1, %08x, op2, %08x\n",op1,op2);

}


int whatRegisterInstruction()  // check for register instruction
{
    unsigned long bitmask = 0x3;
    unsigned long type= (IR >> 23) & bitmask;
    return type;
}


int op2(){ // operand 2 for the immediate type instuctions 
    int type;
    unsigned long result;
    int mode =1;
    int Regmode = 0x01; 
    type = (IR >> 24) & Regmode ; // shift it and AND to the right byte for figure out the immediate type 
    if (type == mode) // if it is the immediate AND it to get operand 2 
    {
	result = IR & imm; 

    }
    else 
    {
	result = R[Rm]; // if not immediate op2() = 0
    }
    return result; // returns operand 2
}

int instructionType() // code that determines the instruction types
{
    int type ;
    type = (IR >> 24) & 0x0E; // shift it to the type byte
    return type;

}

int whichOpcode(){
    int Opcode = (IR >> 20) & 0x0F;

    return Opcode;
}
int conditionType () // condition  code for that controls the conditional execution of the instructions
{



    Condition = IR; 
    Condition = Condition >> 28; // shift Condition to 28 bits to get the condition 

    if (Condition == EQ) // run the flags with the conditions 
    {
	if (ZF==1) return 1;
    }
    else if (Condition == NE)
    {
	if (ZF ==0) return 1;
    }
    else if (Condition == CS)
    {
	if (CF ==1) return 1;
    }
    else if (Condition == CC)
    {
	if (CF ==0 ) return 1;
    }

    else if (Condition == MI)
    {
	if (NF ==1 ) return 1;
    }

    else if (Condition == PL)
    {
	if (NF == 0 ) return 1;
    }
    else if (Condition == HI)
    {
	if (CF = 0 && ZF == 0) return 1; 
    }
    else if (Condition == LS)
    {
	if ( CF ==0 || ZF == 1) return 1;
    }
    else if (Condition == AL)
    {
	return 1;
    }	
    else{
	return 0;
    }
}


int quit(){   // quites the program
    fprintf(stdout, "exit successful\n" );
    exit(0);
}
int go() // rents the entire program
{

    Fetch();
    while(IR != 0)
    {    
	Fetch();
	Execute();
    }
    registers();	
}

DisplayMemory(void *memptr, unsigned int size, unsigned int offset, unsigned length){ // displays the memory
    unsigned char *buffer= memptr;
    int line = 16;
    int i;
    int x = 0;
    int y = 0;
    long w;

    printf("Enter a desired offset \n");
    scanf("%d", &offset);
    fprintf(stdout,"Enter the number of bytes to be displayed from the offset\n");
    fscanf(stdin,"%d",&length);
    fprintf(stdout,"Displaying Memory from Start Address: %04X\n", offset);
    fprintf(stdout,"%04X  ", offset);
    for(i = 0; i <= length; i++)
    {

	if(i % line == 0 && i > 0)
	{
	    for(x = y*line; x < (y*line)+line; x++)
	    {
		if(isprint(*(buffer + x + offset)))
		    fprintf(stdout,"%c", *(buffer + x + offset));
		else
		    fprintf(stdout,".");
	    }


	    if (i!=length){
		fprintf(stdout,"\n%04X  ", i + offset);
		y++;
	    }
	}
	if(i<((y+1)*line))
	    fprintf(stdout,"%02X  ", *(buffer + i + offset));

    
    }
        fprintf(stdout," \n\nMust enter space after dump command on the user interface command line to run \n another command\n"); 
}
int Fetch(){ //fetch instruction to fetch an instuction

    MAR =0;

    IR =0 ; 

    MAR = PC;
    MBR = buffer[MAR];
    MAR = MAR +1;
    MBR = (MBR << 8) | buffer[MAR];
    MAR = MAR +1;
    MBR = (MBR << 8) | buffer[MAR];

    MAR = MAR +1;
    MBR = (MBR << 8) | buffer[MAR];
    MAR =MAR +1;

    IR=MBR;
    PC = PC +4;
    regDeclare();
}

int regDeclare()
{

    // R[14]=0;
    Rn= (IR >> 16) & 0x000F;
    Rd = (IR >> 12) & 0x0000F;
    Rm = IR & 0x0000000F;
}

int registers()// displays the values of the register
{
    fprintf(stdout, "R0: %08x, R1: %08x, R2: %08x \n R3: %08x, R4: %08x, R5: %08x \n R6: %08x, R7: %08x R8: %08x, R9: %08x \n RA: %08x, RB: %08x, RC:  %08x \n RD: %08x, RE: %08x\n", R[0], R[1], R[2], R[3], R[4], R[5], R[6], R[7],R[8], R[9], R[10], R[11], R[12], R[13], R[14]);

    fprintf(stdout,"IR: %08x,  PC: %08x, MBR: %08x, MAR: %08x, ZF: %d, NF: %d, CF: %d" , IR,  PC, MBR, MAR,ZF,NF, CF);

}

int trace() // executes the fetch and execute function
{
    Fetch();
    Execute();
    registers();
}

int reset() // turns all registers into zero
{
    int i;
    for (i=0;i< 14 ; i++)
    {
	R[i] = 0;
    }
    PC = 0;
    IR= 0;
    MBR= 0;
    MAR= 0;
    registers();
}
int display() // displays the help command
{
    fprintf(stdout,"d      dump memory\ng      go - run the entire program\nl      load a file into memory\nq      quit\nr      displays reqisters\nt      trace - execute one instruction\nz      reset all the registers to zero\n?, h   displays lists of commands\n please enter a command : \n  ");
}
int LoadFile ( char *buffer , unsigned long max) //reads a filie in buffer
{
    FILE *pfile;
    int size;
    char fname[200];
    int c;
    size_t result;
    fprintf (stdout, "Please enter the filename to load into memory: ");

    fgets(fname,sizeof fname,stdin);

    fname[strlen(fname)-1]='\0';

    if ((pfile= fopen (fname, "rb")) == NULL)
    {fprintf(stdout,"\nname null value\n");
	return 1;
    }


    fseek(pfile, 0, SEEK_END);
    size = ftell (pfile);
    rewind (pfile);
    if (size <= 0)
    {
	printf ("error too small");
	return -1;
    }
    if (size > max)
    {
	fprintf(stdout,"error too big\n");
	return -1;
    }
    result = fread(buffer, 1 , size, pfile);

    fclose(pfile);
    return size;


}

int main(int argc, char * argv[],char * envp[]) // the main program
{



    fprintf(stdout,"Welcome to Virtual PC  please enter a commmand ( ? or h to diplays all the commands) \n");

    x =2;
    a=0;

    while(a<x)
    {
	fprintf(stdout,"\nCommand:");
	ch = getchar();
	getchar();
	 p = tolower(ch);

	{
	    switch (p)
	    {

		case 'l':
		    result= LoadFile(buffer,MEM);
		    fprintf(stdout,"%d bytes and hex = %x\n", result, result);

		    break;
		case 'g':
		    go();
		    break;
		case 'd':

		    DisplayMemory(buffer, MEM, offset, length);


		    break;

		case 'r':
		    registers();
		    break;
		case 't':


		    trace();

		    whatRegisterInstruction();

		    break;
		case 'z':
		    reset();
		    break;
		case '?':
		    display();
		    break;
		case 'q':
		    quit();
		    break;
		case 'h':
		    display();
		    break;

	    }

	}

    }

    return 0;
}
