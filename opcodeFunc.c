#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

//uint32_t??
int memory[4096]; //4096? //1025
int stack[512]; //512?? //129
int reg[32];
//PC is register 16??? 15??
//SP & FP registers should be initialized to size of stack

void add(int *des, int *reg1, int *reg2); //adds two registers to a destination register
void addI(int *des, int *reg1, int val); //adds a value to a register
void and(int *des, int *reg1, int *reg2);
void andI(int *des, int *reg1, int val);
// FILE *b(int val, *char file, int *pcReg); //32 * number of lines you want to skip
void sub(int *des, int *reg1, int *reg2);
void subI(int *des, int *reg1, int val);
void udiv(int *des, int *reg1, int *reg2);
void sdiv(int *des, int *reg1, int *reg2);
void prnl();
void prnt(int *reg);
// FILE *bl(int val, *char file, int *pcReg, int *blReg);
// FILE *br(*char file, int *reg, int *pcReg);
// FILE *cbz(*char file, int *reg, int val, int *pcReg);
void eor(int *des, int *reg1, int *reg2);
void mul(int *des, int *reg1, int *reg2);
void orr(int *des, int *reg1, int *reg2);
void orrI(int *des, int *reg1, int val);
//void smulh(long long *des, long long *reg1, long long *reg2) //????
void ldur(int *des, int *reg1, int memory[], int offset);
void ldurB(int *des, int *reg1, int memory[], int offset);
void ldurH(int *des, int *reg1, int memory[], int offset);
void ldurSW(int *des, int *reg1, int memory[], int offset);
void stur(int *des, int memory[], int *reg, int offset);
void sturB(int *des, int memory[], int *reg, int offset);
void sturH(int *des, int memory[], int *reg, int offset);
void sturW(int *des, int memory[], int *reg, int offset);
void lsl(int *des, int *reg1, int offset);
void umulh(int *des, int *reg1, int *reg2); //???????
void dump(FILE *f, int8_t *start, size_t size);
char printable_char(uint8_t c);

int main()
{

  for(int i = 0; i < 32; i ++)
  {
    reg[i] = 4;
  }

  memory[0] = 10;
  memory[1] = 12;
  memory[2] = 46;

  FILE *read;
  read = fopen("test.txt", "r");
  //stur(&reg[0], memory[], &reg[1], 24);
  //smulh(&reg[1], &reg[2], &reg[3]);
  dump(read, &memory[0], 4096);
  dump(read, &stack[0], 129);
  return 0;
}

void add(int *des, int *reg1, int *reg2)
{
    *des = *reg1 + *reg2;
}

void addI(int *des, int *reg1, int val)
{
    *des = *reg1 + val;
}

void and(int *des, int *reg1, int *reg2)
{
    *des = *reg1 & *reg2;
}

void andI(int *des, int *reg1, int val)
{
    *des = *reg1 & val;
}

// FILE *b(int val, *char file, int *pcReg) //test later
// {
//     FILE *f = fopen(*file, "r");
//     for(int i = 0; i < (32 * val); i ++)
//     {
//         fgetc(f);
//     }
//     *pcReg = val;
//     return f;
//
// }

//B.cond

// FILE *bl(int val, *char file, int *pcReg, int *blReg)
// {
//       FILE *f = fopen(*file, "r");
//       for(int i = 0; i < (32 * val); i ++)
//       {
//           fgetc(f);
//       }
//       *blReg = *pcReg + 1;
//       return f;
// }
//
// FILE *br(*char file, int *reg, int *pcReg)
// {
//   FILE *f = fopen(*file, "r");
//   int val = *reg
//   for(int i = 0; i < (32 * val); i ++)
//   {
//       fgetc(f);
//   }
//   *pcReg = val;
//   return f;
// }
//
// FILE *cbz(*char file, int *reg, int val, int *pcReg)
// {
//   FILE *f = fopen(*file, "r");
//   int currReg = *reg
//
//   if(currReg == 0)
//   {
//     for(int i = 0; i < (32 * val); i ++)
//     {
//         fgetc(f);
//     }
//     *pcReg = val;
//   } else
//   {
//       int pc = *pcReg;
//       for(int i = 0; i < (32 * (pc + 1)); i ++)
//       {
//           fgetc(f);
//       }
//       *pcReg = pc + 1;
//   }
//
//   return f;
//
// }
//
// FILE *cbnz(*char file, int *reg, int val, int *pcReg)
// {
//   FILE *f = fopen(*file, "r");
//   int currReg = *reg
//
//   if(currReg != 0)
//   {
//     for(int i = 0; i < (32 * val); i ++)
//     {
//         fgetc(f);
//     }
//     *pcReg = val;
//
//   } else
//   {
//       int pc = *pcReg;
//       for(int i = 0; i < (32 * (pc + 1)); i ++)
//       {
//           fgetc(f);
//       }
//       *pcReg = pc + 1;
//   }
//
//   return f;
//
// }

//DO DUMP
char printable_char(uint8_t c)
{
  return isprint(c) ? c : '.';
}

void dump(FILE *f, int8_t *start, size_t size) //displays contents of registers, memory, and disassembled program
{
  size_t i;

  for (i = 0; i < size - (size % 16); i += 16)
  {
    // fprintf(f,
    //         "%08x "
    //         " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
    //         " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
    //         " |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
    //         (int32_t) i,
    //         start[i +  0], start[i +  1], start[i +  2], start[i +  3],
    //         start[i +  4], start[i +  5], start[i +  6], start[i +  7],
    //         start[i +  8], start[i +  9], start[i + 10], start[i + 11],
    //         start[i + 12], start[i + 13], start[i + 14], start[i + 15],
    //         printable_char(start[i +  0]), printable_char(start[i +  1]),
    //         printable_char(start[i +  2]), printable_char(start[i +  3]),
    //         printable_char(start[i +  4]), printable_char(start[i +  5]),
    //         printable_char(start[i +  6]), printable_char(start[i +  7]),
    //         printable_char(start[i +  8]), printable_char(start[i +  9]),
    //         printable_char(start[i + 10]), printable_char(start[i + 11]),
    //         printable_char(start[i + 12]), printable_char(start[i + 13]),
    //         printable_char(start[i + 14]), printable_char(start[i + 15]));

    printf(
                    "%08x "
                    " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
                    " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
                    " |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
                    (int32_t) i,
                    start[i +  0], start[i +  1], start[i +  2], start[i +  3],
                    start[i +  4], start[i +  5], start[i +  6], start[i +  7],
                    start[i +  8], start[i +  9], start[i + 10], start[i + 11],
                    start[i + 12], start[i + 13], start[i + 14], start[i + 15],
                    printable_char(start[i +  0]), printable_char(start[i +  1]),
                    printable_char(start[i +  2]), printable_char(start[i +  3]),
                    printable_char(start[i +  4]), printable_char(start[i +  5]),
                    printable_char(start[i +  6]), printable_char(start[i +  7]),
                    printable_char(start[i +  8]), printable_char(start[i +  9]),
                    printable_char(start[i + 10]), printable_char(start[i + 11]),
                    printable_char(start[i + 12]), printable_char(start[i + 13]),
                    printable_char(start[i + 14]), printable_char(start[i + 15]));
  }
  //fprintf(f, "%08x\n", (int32_t) size);
  printf("%08x\n", (int32_t) size);
}

void eor(int *des, int *reg1, int *reg2)
{
    //*des = ~(*reg1 & *reg2) & ~(~(*reg1) & ~(*reg2));
    *des = *reg1 ^ *reg2;
}

void eorI(int *des, int *reg1, int val)
{
    //*des = ~(*reg1 & val) & ~(~(*reg1) & ~(val));
    *des = *reg1 ^ val;
}

//DO HALT
// void halt()
// {
//     dump();
//     exit(0); //terminates
// }

void ldur(int *des, int *reg1, int memory[], int offset)
{
    int val = offset / 8; //divides offset by 8
    *des = memory[val + *reg1]; //loads into des register from the memory array with specified value
}


void ldurB(int *des, int *reg1, int memory[], int offset)
{
    int val = offset / 8; //divides offset by 8
    *des = memory[val + *reg1] & 0x1; //????
}


void ldurH(int *des, int *reg1, int memory[], int offset)
{
    int val = offset / 8; //divides offset by 8
    *des = memory[val + *reg1] & 0xFFFF; //half word representation
}


void ldurSW(int *des, int *reg1, int memory[], int offset)
{
    int val = offset / 8; //divides offset by 8
    *des = memory[val + *reg1] & 0xFFFFFFFF; //in theory, word representation, if a half word --> 0xFFFF a word --> 0xFFFFFFFF
}

void lsl(int *des, int *reg1, int offset)
{
  // does the offset need to be divded by 8??
  //int val = offset / 8;
  *des = *reg1 << offset;
}

void mul(int *des, int *reg1, int *reg2)
{
    *des = (*reg1) * (*reg2);
}

void orr(int *des, int *reg1, int *reg2)
{
    *des = *reg1 | *reg2;
}

void orrI(int *des, int *reg1, int val)
{
    *des = *reg1 | val;
}

void prnl()
{
    printf("\n");
}

void prnt(int *reg)
{
    printf("Decimal: %d\nHex: %x\n", *reg, *reg);
}


void sdiv(int *des, int *reg1, int *reg2) //??
{
    *des = *reg1 / *reg2;
}

//DO SMULH
void smulh(int *des, int *reg1, int *reg2) //???
{
    *des = (*reg1) * (*reg2);
}


void stur(int *des, int memory[], int *reg, int offset)
{
    int val = offset / 8;
    memory[*reg + val] = *des;
}


void sturB(int *des, int memory[], int *reg, int offset)
{
    int val = offset / 8;
    memory[*reg + val] = *des & 0x1;
}


void sturH(int *des, int memory[], int *reg, int offset)
{
    int val = offset / 8;
    memory[*reg + val] = *des & 0xFFFF;
}


void sturW(int *des, int memory[], int *reg, int offset)
{
    int val = offset / 8;
    memory[*reg + val] = *des & 0xFFFFFFFF;
}


void sub(int *des, int *reg1, int *reg2)
{
    *des = *reg1 - *reg2;
}

void subI(int *des, int *reg1, int val)
{
    *des = *reg1 - val;
}

//SUBIS - Flag

//SUBS - Flag

void udiv(int *des, int *reg1, int *reg2) //??
{
    *des = *reg1 / *reg2;
}

//DO UMULH
void umulh(int *des, int *reg1, int *reg2) //???
{
    *des = (*reg1) * (*reg2);
}