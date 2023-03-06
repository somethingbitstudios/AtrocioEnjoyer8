#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <math.h>
using namespace std;

enum  Wires
{
    NO = 0,
    WRx=1,
    RRx=2,
    SKIP3=3,
    SKIP25=4,
    RST = 5,
    ONE=6,
    THREE=7,
    RW = 8,
    CHCK_BRBS=9,
    CHCK_C=10,
    WRx2 = 11,
    RRx2 = 12,
    WRx3= 13,
    RRx3=14,
    RRS=15,
    RHPC=16,
    RLPC=17,
    APC=18,
    LPC=19,
    ADR=20,
    LDR=21,
    MV=22,
    RR0=23,
    RW0=24,
    RR1 = 25,
    RW1 = 26,
    RR2 = 27,
    RW2 = 28,
    RR3 = 29,
    RW4 = 30,

    RCREG1=51,
    WCREG1 = 52,
    RCREG2 = 53,
    WCREG2 = 54,
    LR1=55,
    LR2=56,
    aEN0=57,
    aEN1=58,
    aEN2=59,
    aEN3=60,
    aEN4=61,
    aEN5=62
  
};

const int MEMORY_SIZE = 1024;
short c2bin(char fb, char sb1, char sb2) {
    return   (short)fb << 12 | (short)sb1 << 6 | (short)sb2;
}


 int index = 0;
 unsigned short memory[MEMORY_SIZE];

 void pcpp(){
  //pc++
    memory[index++] = c2bin(NO, LR1, RLPC);
    memory[index++] = c2bin(NO, LR2, ONE);
    memory[index++] = c2bin(NO, aEN0, LPC);

    memory[index++] = c2bin(SKIP3, CHCK_C, NO);

    memory[index++] = c2bin(NO, LR1, RHPC);
    memory[index++] = c2bin(NO, aEN0, APC);

    memory[index++] = c2bin(NO, CHCK_C, ONE);
    memory[index++] = c2bin(NO, APC, NO);
    memory[index++] = c2bin(NO, NO, NO);

    //end pc++
 
 }
 void pcpt() {
     //pc++
     memory[index++] = c2bin(NO, LR1, RLPC);
     memory[index++] = c2bin(NO, LR2, THREE);
     memory[index++] = c2bin(NO, aEN0, LPC);

     memory[index++] = c2bin(SKIP3, CHCK_C, NO);

     memory[index++] = c2bin(NO, LR1, RHPC);
     memory[index++] = c2bin(NO, aEN0, APC);

     memory[index++] = c2bin(NO, CHCK_C, ONE);
     memory[index++] = c2bin(NO, APC, NO);
     memory[index++] = c2bin(NO, NO, NO);

     //end pc++

 }
int main(int argument_count, char* argument_list[])
{

   


    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }


#pragma region LDS
        //LDS
    index = 0*64;
    memory[index++] = c2bin(NO,  RHPC, APC);
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();
    memory[index++] = c2bin(NO, WCREG2, MV);
    pcpp();
    memory[index++] = c2bin(NO, RCREG2, LDR);
    memory[index++] = c2bin(NO, WCREG2, MV);

    memory[index++] = c2bin(NO, RCREG2, ADR);
    memory[index++] = c2bin(NO, MV, WRx);
    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion
#pragma region STS


       //STS
    index = 1 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, WCREG2, MV);

    pcpp();
    
    memory[index++] = c2bin(NO, RCREG2,LDR);
    memory[index++] = c2bin(NO, WCREG2,MV);
    memory[index++] = c2bin(RW, RRx,MV);
    memory[index++] = c2bin(RW, ADR,RCREG2);


    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion


#pragma region MOV

    index = 2 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1); 
    pcpp();

    memory[index++] = c2bin(NO, MV,WCREG2);
    memory[index++] = c2bin(NO, WRx2,RRx);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion

#pragma region ADD

    index = 3 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN0);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion
#pragma region ADI

    index = 4 * 64;
         //ADI
 
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx, LR1);
    memory[index++] = c2bin(NO, RCREG2, LR2);
    memory[index++] = c2bin(NO, WRx, aEN0);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion

#pragma region SUB

    index = 5 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN1);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion 
#pragma region CP

    index = 6 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, NO, aEN1);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion

#pragma region AND

    index = 7 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN2);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion
#pragma region ANDI
 //ANDI
    index = 8 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx, LR1);
    memory[index++] = c2bin(NO, RCREG2, LR2);
    memory[index++] = c2bin(NO, WRx, aEN2);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion 

#pragma region OR
 
    index = 9 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN3);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion
#pragma region ORI
  //ORI
    index = 10 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();
    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx, LR1);
    memory[index++] = c2bin(NO, RCREG2, LR2);
    memory[index++] = c2bin(NO, WRx, aEN3);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion

#pragma region XOR

     //XOR
    index = 11 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN4);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion  
#pragma region XORI

          //XORI
    index = 12 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx, LR1);
    memory[index++] = c2bin(NO, RCREG2, LR2);
    memory[index++] = c2bin(NO, WRx, aEN4);

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion

#pragma region ROL
 //ROL
    index = 13 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN5);

    pcpp();

    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion
    /*
#pragma region SWAP
 
     //SWAP
    index = 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    //pc++
    memory[index++] = c2bin(NO, LR1, WCREG1);
    memory[index++] = c2bin(NO, LR2, ONE);
    memory[index++] = c2bin(NO, aEN0, LPC);

    memory[index++] = c2bin(NO, CHCK_C, ONE);

    memory[index++] = c2bin(NO, LR1, RHPC);
    memory[index++] = c2bin(NO, aEN0, APC);

    memory[index++] = c2bin(NO, CHCK_C, NO);
    memory[index++] = c2bin(NO, APC, NO);
    //end pc++

    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, RRx2, LR1);
    memory[index++] = c2bin(NO, RRx3, LR2);
    memory[index++] = c2bin(NO, WRx, aEN6);

    //pc++
    memory[index++] = c2bin(NO, LR1, WCREG1);
    memory[index++] = c2bin(NO, LR2, ONE);
    memory[index++] = c2bin(NO, aEN0, LPC);

    memory[index++] = c2bin(NO, CHCK_C, ONE);

    memory[index++] = c2bin(NO, LR1, RHPC);
    memory[index++] = c2bin(NO, aEN0, APC);

    memory[index++] = c2bin(NO, CHCK_C, NO);
    memory[index++] = c2bin(NO, APC, NO);
    //end pc++



#pragma endregion
*/
#pragma region JMP




        //JMP
    index = 14 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(NO, MV, RLPC);
    memory[index++] = c2bin(NO, RCREG1, APC);
   

    pcpp();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion
#pragma region BRBS

    index = 15 * 64;
    memory[index++] = c2bin(NO, MV, WCREG1);
    pcpp();

    memory[index++] = c2bin(SKIP25, NO, CHCK_BRBS);
 

    pcpp();

    memory[index++] = c2bin(NO, MV, LPC);

    pcpp();
    
    memory[index++] = c2bin(NO, MV, WCREG2);
    memory[index++] = c2bin(NO, APC, RCREG2);


    memory[index++] = c2bin(NO, RST, RCREG2);

    pcpt();
    memory[index++] = c2bin(NO, RST, NO);
#pragma endregion

      


    //text based file for simulIDE
    ofstream orderedfile("C:\\test\\test.data", ios::out);
    for (int i = 0; i < MEMORY_SIZE / 16; i++) {
        for (int j = 0; j < 16; j++) {
            unsigned int temp = memory[j + i * 16];

            orderedfile << temp;
            if (j != 15) {
                orderedfile << ",    ";
            }

        }
        orderedfile << "\n";
    }
    orderedfile.close();
    cout << "Success!";
    return 0;
}

