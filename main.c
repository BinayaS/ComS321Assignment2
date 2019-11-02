#include <stdio.h>
#include <stdlib.h>
#include "opcodetable.h"
#include "readFile.h"
//#include "decoderStructure.h"

OpPair opcodeTable[] = {
   {5, "B", 'B'},
   {37, "BL", 'B'}, //End 6 bit opcodes
   {84, "B.cond", 'C'},
   {180, "CBZ", 'C'},
   {181, "CBNZ", 'C'}, //End 8 bit opcodes
   {580, "ADDI", 'I'},
   {584, "ANDI", 'I'},
   {712, "ORRI", 'I'},
   {836, "SUBI", 'I'},
   {840, "EORI", 'I'},
   {964, "SUBIS", 'I'}, //End of 10 bit opcodes
   {448, "STURB", 'D'},
   {450, "LDURB", 'D'},
   {960, "STURH", 'D'},
   {962, "LDURH", 'D'},
   {1104, "AND", 'R'},
   {1112, "ADD", 'R'},
   {1238, "SDIV", 'R'},
   {1238, "UDIV", 'R'}, //Fo'R' now, all division is unsigned, be careful
   {1240, "MUL", 'R'},
   {1242, "SMULH", 'R'},
   {1246, "UMULH", 'R'},
   {1360, "ORR", 'R'},
   {1472, "STURW", 'D'},
   {1476, "LDURSW", 'D'},
   {1690, "LSR", 'R'},
   {1691, "LSL", 'R'},
   {1712, "BR", 'R'},
   {1616, "EOR", 'R'},
   {1624, "SUB", 'R'},
   {1880, "SUBS", 'R'},
   {1984, "STUR", 'D'},
   {1986, "LDUR", 'D'},
   {2044, "PRNL", 'J'},
   {2045, "PRNT", 'J'},
   {2046, "DUMP", 'J'},
   {2047, "HALT", 'J'} //End 11 bit opcodes
};

#define MAX_INSTRUCTION_SIZE 10000000

void decode(unsigned int a) {

  int shiftAmount = 6;
  int shift = 32 - shiftAmount;
  int breakout = 0;
  int foundOpcode = 0;
  int opcodeIndex = -1;

  //TODO compare and find the opcode that is given in a
  while(shiftAmount > 0) {

    //printf("%d\n", a>>shift);

    //search for opcode
    opcodeIndex = searchTable(a>>shift, opcodeTable);
    if(opcodeIndex >= 0){
      printf("shiftAmount: %d -- %d -- %s\n",
              shiftAmount, a>>shift, opcodeTable[opcodeIndex].opname);
      foundOpcode = 1;
      breakout = 1;
    }

    switch(opcodeTable[opcodeIndex].opformat) {
      case  'I':
        printf("I");
      break;

      case 'R':
        printf("R");
      break;

      case  'D':
        printf("D");
      break;

      case 'B':
        printf("B");
      break;

      case 'C':
        printf("CB");
      break;

      case 'W':
        printf("IW");
      break;

      case 'J':
        printf("JS");
      break;

      default:
        printf("Didn't find opFormat");
      break;
    }

    //update shiftAmount
    switch(shiftAmount) {
      case 11:
        shiftAmount = -1;
      break;

      case 10:
        shiftAmount = 11;
      break;

      case 9:
        shiftAmount = 10;
      break;

      case 8:
        shiftAmount = 9;
      break;

      case 6:
        shiftAmount = 8;
      break;
    }

    //exit while loop
    if(breakout == 1) {
      break;
    }

    //update shift
    shift = 32 - shiftAmount;
  }

  if(foundOpcode == 0) {
    printf("Failed to find opcode\n");
  }

}

int main(int argc, char const *argv[])
{

  //set local vars.
  int mainMemorySize = 4096;
  int stackSize = 512;
  int instructionArray[MAX_INSTRUCTION_SIZE] = {-1};
  int counter = 0;


  //parse command line arguments if there are two or more arguments
  if(argc >= 2) {
    printf("The arguments supplied are:\n");

    //print out all arguments
    for(int i = 1; i < argc; i++) {
        printf("%s\t ", argv[i]);
    }

    printf("\n");

    //if atleast three arguments set mainMemorySize
    if(argc >= 3) {
      mainMemorySize = atoi(argv[2]);
      printf("mainMemorySize = %d\n", mainMemorySize);
    }

    //if atleas four arguments set stackSize
    if(argc >= 4) {
      stackSize = atoi(argv[3]);
      printf("stackSize = %d\n", stackSize);
    }

    //if there is not two or more arguments then print the input format
  } else {
      printf("<.machine file> [-m <main memory size>] [-s <stack size>]\n");
      return 0;
  }

  //read from file and put all instructions into instructionArray
  counter = readFile(argc, argv, instructionArray);

  //for visual purposes print out the instructions in hex
  for(int i = 0; i < counter; i++) {
    printf("%x\n", instructionArray[i]);
  }

  //TODO compare opcode by taking the instruction and shifting right till you have just the beggining and compare it to the decimal version of the op code
  for(int i = 0; i < counter; i++) {
    unsigned int a = instructionArray[i];

    decode(a);

    /*
    int shift10 = 32-10;
    int shift11 = 32-11;
    int ADDI = 580;
    int STUR = 1984;
    printf("shift10 decimal: %d -- shift11 decimal: %d -- ", a>>shift10, a>>shift11);


    if(STUR == a>>shift11) {
      printf("hex: %x ", a);
      printf("MATCH!\n");
    }  else {
      printf("hex: %x\n", a);
    }
    */
  }



  return 0;
};
