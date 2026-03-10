/******************************************************************************
Prac 2 - AVR ASM OpCode Decoder
*******************************************************************************/

#include <stdio.h>
#include <inttypes.h>

const uint8_t flash_mem[] ={ 
    0x00, 0x24, 0xA0, 0xE0, 0xB2, 0xE0, 0x0D, 0x91, 0x00, 0x30, 0xE9, 0xF7, 0x11, 0x97, 0xC0, 0xE0, 0xC4, 
    0xD2, 0xE0, 0x09, 0x91, 0x1E, 0x91, 0x01, 0x17, 0x51, 0xF4, 0x0A, 0x2F, 0x0A, 0x95, 0x1C, 0x2F, 0x65, 
    0x01, 0x17, 0xB9, 0xF7, 0x0B, 0x2F, 0x1D, 0x2F, 0x01, 0x17, 0x99, 0xF7, 0x03, 0x94, 0x00, 0x00 };

const uint16_t inst16_table[] = {
  {0x0}, //NOP
};

enum{
    e_NOP,
};


// Op Code struct
typedef union {
    uint16_t op16; // e.g.: watchdog, nop
    struct{
        uint16_t op4:4;
        uint16_t d5:5;
        uint16_t op7:7;
    }type1; // e.g: LSR
    struct{
        uint16_t r4:4;
        uint16_t d5:5;
        uint16_t r1:1;
        uint16_t op6:6;
    }type2; // e.g.: MOV,MUL,ADC,ADD,AND,
    struct{
        uint16_t d10:10;
        uint16_t op6:6;
    }type3;
    struct{
        uint16_t k41:4; //para diferencas los k's altos y bajso
        uint16_t d4:4;
        uint16_t k42:4;
        uint16_t op4:4;
    }type4;
    struct{
        uint16_t s3:3;
        uint16_t k7:7;
        uint16_t op6:6;
    }type5;
    struct{
        uint16_t k4:4;
        uint16_t d2:2;
        uint16_t k2:2;
        uint16_t op8:8;
    }type6;
    struct{
        uint16_t k12:12;
        uint16_t op4:4;
    }type7;
    struct{
        uint16_t r4:4;
        uint16_t d4:4;
        uint16_t op8:8;
    }type8;
} Op_Code_t;


int main()
{
    Op_Code_t *instruction;
    printf("- Practica 2: AVR OpCode -\n");
    // Decode the instructions by cycling through the array
    for (uint8_t idx = 0; idx < sizeof(flash_mem); idx+=2)
    {
        instruction = (Op_Code_t*) &flash_mem[idx];
        if (instruction->op16 == inst16_table[e_NOP])
        {
            printf("NOP\n");
        }
        //inicio 
        //type 1
        else if (instruction->type1.op7 == 0x4A && instruction->type1.op4 == 0x03){ //INC
            uint8_t registro = instruction->type1.d5;
            printf("INC R%d \n", registro);
        }
        else if (instruction->type1.op7 == 0x48 && instruction->type1.op4 == 0x0D){ //LD
            uint8_t registro = instruction->type1.d5;
            printf("LD R%d \n", registro);
        }
        //type2
        else if (instruction->type2.op6 == 0x02){ // SBC
            uint8_t reg_destino = instruction->type2.d5;
            uint8_t reg_fuente = (instruction->type2.r1 <<4 | instruction->type2.r4);
            printf("SBC R%d R%d \n", reg_destino, reg_fuente);
        }
        else if (instruction->type2.op6 == 0x07){ // ADC
            uint8_t reg_destino = instruction->type2.d5;
            uint8_t reg_fuente = (instruction->type2.r1 <<4 | instruction->type2.r4);
            printf("ADC R%d R%d \n", reg_destino, reg_fuente);
        }
        else if (instruction->type2.op6 == 0x05){ // CP
            uint8_t reg_destino = instruction->type2.d5;
            uint8_t reg_fuente = (instruction->type2.r1 <<4 | instruction->type2.r4);
            printf("CP R%d R%d \n", reg_destino, reg_fuente);
        }
        else if (instruction->type2.op6 == 0x0B){ // MOV
            uint8_t reg_destino = instruction->type2.d5;
            uint8_t reg_fuente = (instruction->type2.r1 <<4 | instruction->type2.r4);
            printf("MOV R%d R%d \n", reg_destino, reg_fuente);
        }
        //type 3
        else if (instruction->type3.op6 == 0x09){ //CLR
            uint8_t registro = instruction->type3.d10;
            printf("CLR R%d \n", registro);
        }
        //type 4
        else if (instruction->type4.op4 == 0x0E){ // LDI
            uint8_t reg_destino = 16 + instruction->type4.d4; // + 16 par el desfase
            uint8_t constante = (instruction->type4.k42 << 4 | instruction->type4.k41);
            printf("LDI R%d %d \n", reg_destino, constante); 
        }
        else if (instruction->type4.op4 == 0x03){ // CPI
            uint8_t reg_destino = 16 + instruction->type4.d4; // + 16 par el desfase
            uint8_t constante = (instruction->type4.k42 << 4 | instruction->type4.k41);
            printf("CPI R%d %d \n", reg_destino, constante);
        }
        else if (instruction->type4.op4 == 0x05){ // SUBI
            uint8_t reg_destino = 16 + instruction->type4.d4; // + 16 par el desfase
            uint8_t constante = (instruction->type4.k42 << 4 | instruction->type4.k41);
            printf("SUBI R%d %d \n", reg_destino, constante);
        }
        else if (instruction->type4.op4 == 0x06){ // ORI
            uint8_t reg_destino = 16 + instruction->type4.d4; // + 16 par el desfase
            uint8_t constante = (instruction->type4.k42 << 4 | instruction->type4.k41);
            printf("ORI R%d %d \n", reg_destino, constante);
        }
        //type 5
        else if (instruction->type5.op6 == 0x3D){ //BRBC
            uint8_t constante = instruction->type5.k7;
            uint8_t reg_fuente = instruction->type5.s3;
            printf("BRBC R%d %d \n", reg_fuente, constante);
        }
        //type 6
        else if (instruction->type6.op8 == 0x97){ // SBIW
            uint8_t constante = (instruction->type6.k2 << 4 | instruction->type6.k4);
            uint8_t reg_destino = 24 + (instruction->type6.d2 * 2);
            printf("SBIW R%d %d \n", reg_destino, constante);
        }
        //type 7
        else if (instruction->type7.op4 == 0x0D){ // RCAL
            uint8_t constante = instruction->type7.k12;
            printf("RCAL %d \n", constante);
        }
        //type 8
        else if (instruction->type8.op8 == 0x01){ //MOVW
            uint8_t reg_destino = instruction->type8.d4 *2;
            uint8_t reg_fuente = instruction->type8.r4 *2;
            printf("MOVW R%d R%d \n", reg_destino, reg_fuente);
        }
        //final
        else
        {
            printf("unknown\n");
        }
    }
    return 0;
}
