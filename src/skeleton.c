
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/***
  Name : Hamza Khan

Submitted as  project for AKEANA training
  ***/

#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255
#define MAX_LINE_LENGTH 255

const char *registerAliases[] = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3",
    "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

/*****************global data ******************/
typedef struct
{
    int address;
    char label[MAX_LABEL_LEN + 1];
} symboltable;

enum
{
    DONE,
    OK,
    EMPTY_LINE
};

int bin_instruction[32];          /*a binary array of integers to hold the instruction.*/
symboltable mytable[MAX_SYMBOLS]; /*** the array of symbol table ***/

FILE *infile = NULL;
FILE *outfile = NULL;

int binaryToDecimal(char *binary)
{
    int decimal = 0;
    int length = strlen(binary);
    int sign = 1; // Initialize the sign as positive by default

    if (binary[0] == '1')
    {
        // If the first bit is '1', set the sign to negative and convert to 2's complement.
        sign = -1;
        int flagInv = 0;

        for (int i = length - 1; i >= 0; i--)
        {
            if (flagInv)
            {
                binary[i] = (int)binary[i] - 48 ? '0' : '1';
            }
            else if (binary[i] == '1')
            {
                flagInv = 1;
            }
        }
    }

    for (int i = 0; i < length; i++)
    {
        if (binary[i] == '1')
        {
            // If the current character is '1', add 2^(length - i - 1) to the decimal value.
            decimal += 1 << (length - i - 1);
        }
        else if (binary[i] != '0')
        {
            // If the character is not '0' or '1', return an error or handle it as needed.
            printf("Invalid character in binary string: %c\n", binary[i]);
            return -1;
        }
    }

    return decimal * sign;
}

int toNum(char *pStr)
{
    char lBuf[10];
    char *t_ptr;
    char *orig_pStr;
    int t_length, k;
    int lNum, lNeg = 0;
    orig_pStr = pStr;
    if (*pStr == '-') /* hex is negative */
    {
        lNeg = 1;
        pStr++;
    }
    if (*(pStr + 1) == 'x') /* Hex */
    {
        pStr++;
        pStr++;
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for (k = 0; k < t_length; k++)
        {
            if (!isxdigit(*t_ptr))
            {
                printf("Error: invalid hex operand, %s\n", orig_pStr);
                exit(4);
            }
            t_ptr++;
        }
        sprintf(lBuf, "0x%s", pStr); /* convert hex string into integer */
        sscanf(lBuf, "%i", &lNum);
        if (lNeg)
            lNum = -lNum;
        return lNum;
    }
    else
    {

        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for (k = 0; k < t_length; k++)
        {
            if (!isdigit(*t_ptr))
            {
                printf("Error: invalid decimal operand, %s\n", orig_pStr);
                exit(4);
            }
            t_ptr++;
        }
        lNum = atoi(pStr);
        if (lNeg)
            lNum = -lNum;

        return lNum;
    }
}


int findRegisterIndex(const char *targetRegister, const char *registerNames[], int numRegisters)
{
    int registerIndex = -1; // Initialize to -1 (not found)

    if (targetRegister[0] != 'x')
    {
        for (int i = 0; i < numRegisters; i++)
        {
            if (strcmp(targetRegister, registerNames[i]) == 0)
            {
                // Register name found, store its index
                registerIndex = i;
                break; // No need to continue searching
            }
        }
    }
    else
    {
        registerIndex = atoi(targetRegister + 1);
    }

    return registerIndex;
}

char *substring(const char *str, int start, int end)
{
    if (str == NULL || start < 0 || end < start)
    {
        return NULL;
    }

    int length = end - start + 1;
    char *result = (char *)malloc(length + 1); // +1 for the null terminator
    if (result == NULL)
    {
        return NULL; // Memory allocation failed
    }

    strncpy(result, str + start, length);
    result[length] = '\0'; // Null-terminate the result string

    return result;
}

char *decToBin(int num, int length)
{
    // This function will convert a decimal number to binary decimal immediates
    if (length < 1)
    {
        return NULL; // Invalid input
    }

    // Allocate memory for the binary string, including room for the null terminator
    char *binary = (char *)malloc(length + 1);
    if (binary == NULL)
    {
        return NULL; // Memory allocation failed
    }

    // Initialize the binary string with zeros
    memset(binary, '0', length);
    binary[length] = '\0'; // Null-terminate the string

    int tempnum = num;
    // Convert decimal to binary
    int index = length - 1;
    while (index >= 0)
    {
        binary[index] = abs((num % 2)) + '0'; // Convert the remainder to '0' or '1'
        num /= 2;                             // Divide by 2 for the next binary digit
        index--;
    }

    // Handle negative values
    if (tempnum < 0)
    {
        length = strlen(binary);
        int flagInv = 0;

        for (int i = length - 1; i >= 0; i--)
        {
            if (flagInv)
            {
                binary[i] = (int)binary[i] - 48 ? '0' : '1';
            }
            else if (binary[i] == '1')
            {
                flagInv = 1;
            }
        }
    }

    return binary;
}

char *hexToBin(int num, int length)
{
    // This function will convert a Hex number to binary if you ar eusing hex immediates
    if (length < 1)
    {
        return NULL; // Invalid input
    }

    // Allocate memory for the binary string, including room for the null terminator
    char *binary = (char *)malloc(length * 4 + 1);
    if (binary == NULL)
    {
        return NULL; // Memory allocation failed
    }

    // Initialize the binary string with zeros
    memset(binary, '0', length * 4);
    binary[length * 4] = '\0'; // Null-terminate the string

    // Convert hexadecimal to binary
    int index = length * 4 - 1;
    while (num > 0 && index >= 0)
    {
        int hexDigit = num % 16; // Extract the lowest 4 bits
        for (int i = 0; i < 4; i++)
        {
            binary[index] = (hexDigit % 2) + '0'; // Convert the remainder to '0' or '1'
            hexDigit /= 2;                        // Divide by 2 for the next binary digit
            index--;
        }
        num /= 16; // Shift to the next hex digit
    }

    return binary;
}

int calc_offset(int curr_pc, char *curr_label)
{
    // Add yoru code here to caluclate the jump for PC
    int flag = 0;
    int i = 0;
    for (i = 0; i < MAX_SYMBOLS; i++)
    {
        if (strcmp(curr_label, mytable[i].label) == 0)
        {
            flag = 1; // Label found
            break;
        }
    }

    if (flag)
    {
    }
    else
    {
        return 0;
    }

    // Calculate the offset
    int offset = mytable[i].address - curr_pc;
    return offset;
}

int isOpcode(char *lptr)
{
    // printf("here :%s\n",lptr);
 

    if (
        (strcmp(lptr, "add") == 0) ||
        (strcmp(lptr, "sub") == 0) ||
        (strcmp(lptr, "xor") == 0) ||
        (strcmp(lptr, "or") == 0) ||
        (strcmp(lptr, "and") == 0) ||
        (strcmp(lptr, "sll") == 0) ||
        (strcmp(lptr, "srl") == 0) ||
        (strcmp(lptr, "sra") == 0) ||
        (strcmp(lptr, "slt") == 0) ||
        (strcmp(lptr, "sltu") == 0) ||
        (strcmp(lptr, "addi") == 0) ||
        (strcmp(lptr, "xori") == 0) ||
        (strcmp(lptr, "ori") == 0) ||
        (strcmp(lptr, "andi") == 0) ||
        (strcmp(lptr, "slli") == 0) ||
        (strcmp(lptr, "srli") == 0) ||
        (strcmp(lptr, "srai") == 0) ||
        (strcmp(lptr, "slti") == 0) ||
        (strcmp(lptr, "sltiu") == 0) ||
        (strcmp(lptr, "lb") == 0) ||
        (strcmp(lptr, "lh") == 0) ||
        (strcmp(lptr, "lw") == 0) ||
        (strcmp(lptr, "lbu") == 0) ||
        (strcmp(lptr, "lhu") == 0) ||
        (strcmp(lptr, "sb") == 0) ||
        (strcmp(lptr, "sh") == 0) ||
        (strcmp(lptr, "sw") == 0) ||
        (strcmp(lptr, "beq") == 0) ||
        (strcmp(lptr, "bne") == 0) ||
        (strcmp(lptr, "blt") == 0) ||
        (strcmp(lptr, "bge") == 0) ||
        (strcmp(lptr, "bltu") == 0) ||
        (strcmp(lptr, "bgeu") == 0) ||
        (strcmp(lptr, "jal") == 0) ||
        (strcmp(lptr, "jr") == 0) ||
        (strcmp(lptr, "jalr") == 0) ||
        (strcmp(lptr, "lui") == 0) ||
        (strcmp(lptr, "auipc") == 0) ||
        (strcmp(lptr, "ecall") == 0) ||
        (strcmp(lptr, "ebreak") == 0) ||
        (strcmp(lptr, "nop") == 0) ||
        (strcmp(lptr, "mv") == 0) ||
        (strcmp(lptr, "not") == 0) ||
        (strcmp(lptr, "neg") == 0) ||
        (strcmp(lptr, "j") == 0) ||
        (strcmp(lptr, "jal") == 0) ||
        (strcmp(lptr, "li") == 0) ||
        (strcmp(lptr, "la") == 0) ||
        (strcmp(lptr, "call") == 0) ||
        (strcmp(lptr, "ret") == 0) ||
        (strcmp(lptr, "beqz") == 0) ||
        (strcmp(lptr, "bgez") == 0) ||
        (strcmp(lptr, "bltz") == 0) ||
        (strcmp(lptr, "bgt") == 0) ||
        (strcmp(lptr, "ble") == 0))
    {
        // to be filled further
        
        return (1);
    }
    else
    {

        return 0;
    }
}



int sType(char *instruction)
{
    char opcode[7];
    char rs1[4];
    char offset[16];
    char rs2[5];

    // printf("%s\n",originalInstruction);
    // Use sscanf to parse the instruction format directly
    if (sscanf(instruction, " %6s %5[^ ,], %15[^ (](%5[^)])", opcode, rs2, offset, rs1) != 4)
    {
        printf("Invalid instruction format: %s\n", instruction);
        exit(4);
    }
    char *funct3_bin;

    if (strcmp(opcode, "sw") == 0)
    {
        funct3_bin = "010";
    }
    else if (strcmp(opcode, "sb") == 0)
    {
        funct3_bin = "000";
    }
    else if (strcmp(opcode, "sh") == 0)
    {
        funct3_bin = "001";
    }
    else
    {
        printf("Invalid opcode: %s\n", opcode);
        exit(2);
    }

    // // Check if the registers are valid
    // int rs1_num = atoi(rs1 + 1);
    // int rs2_num = atoi(rs2 + 1);

    int rs1_num = findRegisterIndex(rs1, registerAliases, 32);
    int rs2_num = findRegisterIndex(rs2, registerAliases, 32);

    int offset_num = atoi(offset);

    if (rs1_num < 0 || rs1_num > 31 || rs2_num < 0 || rs2_num > 31)
    {
        printf("Invalid register index.\n");
        exit(4);
    }

    // Check that the offset value fits in the 12-bit field
    if (offset_num < -2048 || offset_num > 2047)
    {
        printf("Invalid constant in : \t %s\n", instruction);
        exit(3);
    }

    // // Create a string for the binary representation
    char *binary = (char *)malloc(33); // 32 bits + '\0'
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(4);
    }

    // Convert offset value to binary (12 bits)
    char *offset_bin;
    offset_bin = decToBin(offset_num, 12);
    char *rs1_bin = decToBin(rs1_num, 5);
    char *rs2_bin = decToBin(rs2_num, 5);

    // Create the binary string
    snprintf(binary, 33, "%s%s%s%s%s%s", substring(offset_bin, 0, 6), rs2_bin, rs1_bin, funct3_bin, substring(offset_bin, 7, 12), "0100011");

    for (int i = 0; i < 32; i++)
    {
        bin_instruction[i] = (int)binary[i] - 48;
    }
    free(binary);

    return 0;
}

int bType(char *instruction, int curr_pc)
{
    // This function will convert all B type instruction into binary string
    char opcode[7];
    char rs1[5];
    char rs2[5];
    char label[16];

    // Use sscanf to parse the instruction format directly
    if (sscanf(instruction, "%6s %4[^,], %4[^,], %15s", opcode, rs1, rs2, label) != 4)
    {
        printf("Invalid instruction format: %s\n", instruction);
        exit(4);
    }

    // Ensure the opcode is a branch instruction
    if (strcmp(opcode, "beq") != 0 && strcmp(opcode, "bne") != 0 &&
        strcmp(opcode, "blt") != 0 && strcmp(opcode, "bge") != 0 &&
        strcmp(opcode, "bltu") != 0 && strcmp(opcode, "bgeu") != 0)
    {
        printf("Invalid opcode: %s\n", opcode);
        exit(2);
    }

    // Check if the registers are valid
    // int rs1_num = atoi(rs1 + 1);
    // int rs2_num = atoi(rs2 + 1);

    int rs1_num = findRegisterIndex(rs1, registerAliases, 32);
    int rs2_num = findRegisterIndex(rs2, registerAliases, 32);

    if (rs1_num < 0 || rs1_num > 31 || rs2_num < 0 || rs2_num > 31)
    {
        printf("Invalid register index.\n");
        exit(4);
    }

    // Create a string for the binary representation
    char *binary = (char *)malloc(33); // 32 bits + '\0'
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(4);
    }

    // Determine the funct3 value based on the opcode
    char *funct3_bin;
    if (strcmp(opcode, "beq") == 0)
    {
        funct3_bin = "000";
    }
    else if (strcmp(opcode, "bne") == 0)
    {
        funct3_bin = "001";
    }
    else if (strcmp(opcode, "blt") == 0)
    {
        funct3_bin = "100";
    }
    else if (strcmp(opcode, "bge") == 0)
    {
        funct3_bin = "101";
    }
    else if (strcmp(opcode, "bltu") == 0)
    {
        funct3_bin = "110";
    }
    else if (strcmp(opcode, "bgeu") == 0)
    {
        funct3_bin = "111";
    }

    // Convert label to a signed offset
    int label_offset = calc_offset(curr_pc - 4, label); // Subtract 4 to account for the PC increment

    // Check that the offset value fits in the 12-bit field
    if (label_offset < -2048 || label_offset > 2047)
    {
        printf("Label offset out of range for B-type instruction.\n");
        exit(1);
    }

    // printf("%d", label_offset);
    // Convert offset value to binary (13 bits)
    char *offset_bin;
    offset_bin = decToBin(label_offset, 13);
    char *rs1_bin = decToBin(rs1_num, 5);
    char *rs2_bin = decToBin(rs2_num, 5);

    // Create the binary string
    snprintf(binary, 33, "%s%s%s%s%s%s%s%s", substring(offset_bin, 0, 0), substring(offset_bin, 2, 7), rs2_bin, rs1_bin, funct3_bin, substring(offset_bin, 8, 11), substring(offset_bin, 1, 1), "1100011");
    // snprintf(binary, 33, "%s%s%s%s%s%s%s%s",substring(offset_bin,0,0),substring(offset_bin,2,7),  rs2_bin,rs1_bin, funct3_bin,substring(offset_bin,8,11), substring(offset_bin,1,1), "1100011");

    for (int i = 0; i < 32; i++)
    {
        bin_instruction[i] = (int)binary[i] - 48;
    }
    free(binary);

    return 0;
}

int iType(char *instruction, int curr_pc)
{
    char opcode[7];
    char rd[4];
    char rs1[4];
    char imm[12];
    char label[255];
    char *immediate_bin;
    char *offset_bin;
    // Use sscanf to parse the instruction format directly
    // if (sscanf(instruction, "%6s %3[^,], %3[^,], %12s", opcode, rd, rs1, imm) != 4) {
    //     printf("Invalid instruction format: %s\n", instruction);
    //     return NULL;
    // }

    while (isspace(instruction[0]))
    {
        instruction++;
    }

    if (instruction[0] == 'l')
    {
        if (sscanf(instruction, "%6s %3[^,], %10[^()](%3[^)])", opcode, rd, imm, rs1) != 4)
        {
            printf("Invalid instruction format: %s\n", instruction);
            exit(4);
        }

        // Convert immediate value to an integer
        int immediate_value = atoi(imm);
        // Convert immediate value to binary (12 bits)

        immediate_bin = decToBin(immediate_value, 12);

        // Check that the immediate value fits in the 12-bit field
        if (immediate_value < -2048 || immediate_value > 2047)
        {
            printf("Immediate value out of range for I-type instruction.\n");
            exit(3);
        }
    }
    else if (instruction[0] == 'j')
    {
        if (sscanf(instruction, "%6s %3[^,], %3[^,], %12s", opcode, rd, rs1, label) != 4)
        {
            printf("Invalid instruction format: %s\n", instruction);
            exit(4);
        }
        int len = strlen(label);
        int labelisNum = 1;

        if (label[0] == '-')
        {
            for (int i = 1; i < len; i++)
            {
                if (!isdigit(label[i]))
                {
                    labelisNum = 0;
                    break;
                }
            }
        }
        else
        {
            for (int i = 0; i < len; i++)
            {
                if (!isdigit(label[i]))
                {
                    labelisNum = 0;
                    break;
                }
            }
        }

        // Convert label to a signed offset
        int label_offset = atoi(label);
        if (!labelisNum)
        {
            label_offset = calc_offset(curr_pc, label);
        }

        // Convert immediate value to an integer

        // Check that the immediate value fits in the 12-bit field
        if (label_offset < -2048 || label_offset > 2047)
        {
            printf("Immediate value out of range for I-type instruction.\n");
            exit(3);
        }

        offset_bin = decToBin(label_offset, 12);
    }
    else if (instruction[0] == 'e')
    {
        if (sscanf(instruction, "%s", opcode) != 1)
        {
            printf("Invalid instruction format: %s\n", instruction);
            exit(4);
        }
        strcpy(rd, "x0");
        strcpy(rs1, "x0");
    }
    else
    {
        if (sscanf(instruction, "%6s %3[^,], %3[^,], %12s", opcode, rd, rs1, imm) != 4)
        {
            printf("Invalid instruction format: %s\n", instruction);
            exit(4);
        }

        int immediate_value ;
        if (imm[1] == 'x'){
            immediate_value = (toNum(imm));
        }
        else {
            immediate_value = atoi(imm);
        }
        immediate_bin = decToBin(immediate_value, 12);


        // Check that the immediate value fits in the 12-bit field
        if (immediate_value < -2048 || immediate_value > 2047)
        {
            // printf("%x\n", instruction);

            printf("Immediate value out of range for I-type instruction.\n");
            exit(3);
        }
    }

    // // Ensure the opcode is valid for I-type instructions
    // if (strcmp(opcode, "addi") != 0 && strcmp(opcode, "andi") != 0 &&
    //     strcmp(opcode, "ori") != 0 && strcmp(opcode, "xori") != 0) {
    //     printf("Invalid opcode: %s\n", opcode);
    //     return NULL;
    // }

    // Check if the registers are valid
    // int rd_num = atoi(rd + 1);
    // int rs1_num = atoi(rs1 + 1);

    int rd_num = findRegisterIndex(rd, registerAliases, 32);
    int rs1_num = findRegisterIndex(rs1, registerAliases, 32);

    if (rd_num < 0 || rd_num > 31 || rs1_num < 0 || rs1_num > 31)
    {
        printf("Invalid register index.\n");
        exit(4);
    }

    // Create a string for the binary representation
    char *binary = (char *)malloc(33); // 32 bits + '\0'
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(4);
    }

    // Determine the funct3 value based on the opcode
    char *funct3_bin;
    if (strcmp(opcode, "addi") == 0)
    {
        funct3_bin = "000";
    }
    else if (strcmp(opcode, "xori") == 0)
    {
        funct3_bin = "100";
    }
    else if (strcmp(opcode, "ori") == 0)
    {
        funct3_bin = "110";
    }
    else if (strcmp(opcode, "andi") == 0)
    {
        funct3_bin = "111";
    }
    else if (strcmp(opcode, "slli") == 0)
    {
        funct3_bin = "001";
        strncpy(immediate_bin, "0000000", 7);
    }
    else if (strcmp(opcode, "srli") == 0)
    {
        funct3_bin = "101";
        strncpy(immediate_bin, "0000000", 7);
    }
    else if (strcmp(opcode, "srai") == 0)
    {
        funct3_bin = "101";
        strncpy(immediate_bin, "0100000", 7);
    }
    else if (strcmp(opcode, "slti") == 0)
    {
        funct3_bin = "010";
    }
    else if (strcmp(opcode, "sltiu") == 0)
    {
        funct3_bin = "011";
    }
    else if (strcmp(opcode, "lw") == 0)
    {
        funct3_bin = "010";
    }
    else if (strcmp(opcode, "lh") == 0)
    {
        funct3_bin = "001";
    }
    else if (strcmp(opcode, "lhu") == 0)
    {
        funct3_bin = "101";
    }
    else if (strcmp(opcode, "lb") == 0)
    {
        funct3_bin = "000";
    }
    else if (strcmp(opcode, "lbu") == 0)
    {
        funct3_bin = "100";
    }
    else if (strcmp(opcode, "jalr") == 0)
    {
        funct3_bin = "000";
    }

    char *rs1_bin = decToBin(rs1_num, 5);
    char *rd_bin = decToBin(rd_num, 5);

    
    // Create the binary string
    if (instruction[0] == 'l')
    {
        snprintf(binary, 33, "%s%s%s%s%s", immediate_bin, rs1_bin, funct3_bin, rd_bin, "0000011");
    }
    else if (instruction[0] == 'j')
    {
        snprintf(binary, 33, "%s%s%s%s%s", offset_bin, rs1_bin, funct3_bin, rd_bin, "1100111");
    }
    else if (strcmp(opcode, "ecall") == 0)
    {

        snprintf(binary, 33, "%s%s%s%s%s", "000000000000", "00000", "000", "00000", "1110011");
    }
    else if (strcmp(opcode, "ebreak") == 0)
    {

        snprintf(binary, 33, "%s%s%s%s%s", "000000000001", "00000", "000", "00000", "1110011");
    }
    else
    {

        snprintf(binary, 33, "%s%s%s%s%s", immediate_bin, rs1_bin, funct3_bin, rd_bin, "0010011");
    }

    for (int i = 0; i < 32; i++)
    {
        bin_instruction[i] = (int)binary[i] - 48;
    }

    free(binary);
    return 0;
}

int RType(char *instruction)
{
    // This function will convert all R type instruction into binary string
    char opcode[7];
    char rd[4];
    char rs1[4];
    char rs2[4];

    // Use sscanf to parse the instruction format directly
    if (sscanf(instruction, "%6s %3[^,], %3[^,], %3s", opcode, rd, rs1, rs2) != 4)
    {
        printf("Invalid instruction format: %s\n", instruction);
        exit(4);
    }

    // Ensure the opcode is valid for R-type instructions
    if (strcmp(opcode, "add") != 0 && strcmp(opcode, "sub") != 0 &&
        strcmp(opcode, "and") != 0 && strcmp(opcode, "or") != 0 &&
        strcmp(opcode, "xor") != 0 && strcmp(opcode, "sll") != 0 &&
        strcmp(opcode, "srl") != 0 && strcmp(opcode, "sra") != 0 &&
        strcmp(opcode, "slt") != 0 && strcmp(opcode, "sltu") != 0)
    {
        printf("Invalid opcode: %s\n", opcode);
        exit(2);
    }

    // Check if the registers are valid
    // int rd_num = atoi(rd + 1);
    // int rs1_num = atoi(rs1 + 1);
    // int rs2_num = atoi(rs2 + 1);

    int rd_num = findRegisterIndex(rd, registerAliases, 32);
    int rs1_num = findRegisterIndex(rs1, registerAliases, 32);
    int rs2_num = findRegisterIndex(rs2, registerAliases, 32);

    if (rd_num < 0 || rd_num > 31 || rs1_num < 0 || rs1_num > 31 || rs2_num < 0 || rs2_num > 31)
    {
        printf("Invalid register index.\n");
        exit(4);
    }

    // Create a string for the binary representation
    char *binary = (char *)malloc(33); // 32 bits + '\0'
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(4);
    }

    // Determine the funct3 value based on the opcode
    char *funct3_bin;
    char *funct7_bin = "0000000";
    if (strcmp(opcode, "add") == 0)
    {
        funct3_bin = "000";
    }
    else if (strcmp(opcode, "sub") == 0)
    {
        funct3_bin = "000";
        funct7_bin = "0100000";
    }
    else if (strcmp(opcode, "and") == 0)
    {
        funct3_bin = "111";
    }
    else if (strcmp(opcode, "or") == 0)
    {
        funct3_bin = "110";
    }
    else if (strcmp(opcode, "xor") == 0)
    {
        funct3_bin = "100";
    }
    else if (strcmp(opcode, "sll") == 0)
    {
        funct3_bin = "001";
    }
    else if (strcmp(opcode, "srl") == 0)
    {
        funct3_bin = "101";
    }
    else if (strcmp(opcode, "sra") == 0)
    {
        funct3_bin = "101";
        funct7_bin = "0100000";
    }
    else if (strcmp(opcode, "slt") == 0)
    {
        funct3_bin = "010";
    }
    else if (strcmp(opcode, "sltu") == 0)
    {
        funct3_bin = "011";
    }

    // Convert register numbers to binary (5 bits each)
    char *rd_bin = decToBin(rd_num, 5);
    char *rs1_bin = decToBin(rs1_num, 5);
    char *rs2_bin = decToBin(rs2_num, 5);

    // Create the binary string
    snprintf(binary, 33, "%s%s%s%s%s%s", funct7_bin, rs2_bin, rs1_bin, funct3_bin, rd_bin, "0110011");
    for (int i = 0; i < 32; i++)
    {
        bin_instruction[i] = (int)binary[i] - 48;
    }
    free(binary);

    return 0;
}

int uType(char *instruction)
{
    // This function will convert all U type instruction into binary string
    char opcode[7];
    char rd[4];
    char imm[21];

    // Use sscanf to parse the instruction format directly
    if (sscanf(instruction, "%6s %3[^,], %20s", opcode, rd, imm) != 3)
    {
        printf("Invalid instruction format: %s\n", instruction);
        exit(4);
    }

    // Ensure the opcode is valid for U-type instructions
    if (strcmp(opcode, "lui") != 0 && strcmp(opcode, "auipc") != 0)
    {
        printf("Invalid opcode: %s\n", opcode);
        exit(2);
    }

    // Check if the register is valid
    // int rd_num = atoi(rd + 1);

    int rd_num = findRegisterIndex(rd, registerAliases, 32);

    if (rd_num < 0 || rd_num > 31)
    {
        printf("Invalid register index.\n");
        exit(4);
    }

    // Convert immediate value to an integer
    int immediate_value = toNum(imm);

    // Check that the immediate value fits in the 20-bit field
    if (immediate_value < -1048576 || immediate_value > 1048575)
    {
        printf("Immediate value out of range for U-type instruction.\n");
        exit(3);
    }

    // Create a string for the binary representation
    char *binary = (char *)malloc(33); // 32 bits + '\0'
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(4);
    }

    // Determine the opcode value
    char *opcode_bin;
    if (strcmp(opcode, "lui") == 0)
    {
        opcode_bin = "0110111";
    }
    else if (strcmp(opcode, "auipc") == 0)
    {
        opcode_bin = "0010111";
    }

    // Convert register number to binary (5 bits)
    char *rd_bin = decToBin(rd_num, 5);

    // Convert immediate value to binary (20 bits)
    char *immediate_bin = decToBin(immediate_value, 20);

    // Create the binary string
    snprintf(binary, 33, "%s%s%s", immediate_bin, rd_bin, opcode_bin);
    for (int i = 0; i < 32; i++)
    {
        bin_instruction[i] = (int)binary[i] - 48;
    }
    free(binary);
    return 0;
}

int jType(char *instruction, int curr_pc)
{
    char opcode[7];
    char rd[4];
    char label[21];

    // Use sscanf to parse the instruction format directly
    if (sscanf(instruction, "%6s %3[^,], %20s", opcode, rd, label) != 3)
    {
        printf("Invalid instruction format: %s\n", instruction);
        exit(4);
    }

    // Ensure the opcode is valid for J-type instructions
    if (strcmp(opcode, "jal") != 0)
    {
        printf("Invalid opcode: %s\n", opcode);
        exit(2);
    }

    // Check if the register is valid
    // int rd_num = atoi(rd + 1);

    int rd_num = findRegisterIndex(rd, registerAliases, 32);

    if (rd_num < 0 || rd_num > 31)
    {
        printf("Invalid register index.\n");
        exit(4);
    }

    // Calculate the offset based on the label address and the current PC
    int offset = calc_offset(curr_pc - 4, label);

    // Check that the offset value fits in the 21-bit field
    if (offset < -1048576 || offset > 1048575)
    {
        printf("Offset value out of range for J-type instruction.\n");
        exit(1);
    }

    // Create a string for the binary representation
    char *binary = (char *)malloc(33); // 32 bits + '\0'
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(4);
    }

    // Determine the opcode value
    char *opcode_bin = "1101111";

    // Convert register number to binary (5 bits)
    char *rd_bin = decToBin(rd_num, 5);

    // Convert offset value to binary (21 bits)
    char *offset_bin = decToBin(offset, 21);

    // 0 0000000000 0 00000000
    // printf("%d\n", offset);
    // Create the binary string
    snprintf(binary, 33, "%s%s%s%s%s%s", substring(offset_bin, 0, 0), substring(offset_bin, 10, 19), substring(offset_bin, 9, 9), substring(offset_bin, 1, 8), rd_bin, opcode_bin);
    for (int i = 0; i < 31; i++)
    {
        bin_instruction[i] = (int)binary[i] - 48;
    }
    free(binary);
    return 0;
}


int readAndParse(FILE *pInfile, char *pLine, char **pLabel, char **pOpcode, char **pArg1, char **pArg2, char **pArg3, char **pArg4)
{
    // this function will read lines from input file one by one .

    char *lRet, *lPtr;
    int i;
    if (!fgets(pLine, MAX_LINE_LENGTH, pInfile))
        return (DONE);

    // strcpy(pLine,"addi x1,x2,x3");
    // char temp[255];
    // strcpy(temp,pLine);
    char *temp = pLine;
    pLine = strdup(pLine);

    /* convert entire line to lowercase */
    //    for( i = 0; i < strlen( pLine ); i++ )
    //     pLine[i] = tolower( pLine[i] );

    *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

    /* ignore the comments */
    lPtr = pLine;

    while (*lPtr != ';' && *lPtr != '\0' && *lPtr != '\n')
    {
        lPtr++;
    }

    *lPtr = '\0';
    if (!(lPtr = strtok(pLine, "\t\n\r ,:"))){
        return (EMPTY_LINE);
    }
    
    
    if (isOpcode(lPtr) == 0 /*&& lPtr[0] != '.'*/)
    {
        
        *pLabel = lPtr;
        if (!(lPtr = strtok(NULL, "\t\n ,")))
        return (OK);
    }

    *pOpcode = lPtr;


    if (!(lPtr = strtok(NULL, "\t\n ,")))
        return (OK);

    *pArg1 = lPtr;

    if (!(lPtr = strtok(NULL, "\t\n ,")))
        return (OK);

    *pArg2 = lPtr;
    if (!(lPtr = strtok(NULL, "\t\n ,")))
        return (OK);

    *pArg3 = lPtr;

    if (!(lPtr = strtok(NULL, "\t\n ,")))
        return (OK);

    *pArg4 = lPtr;

    // strcpy(pLine,temp);
    pLine = temp;
    return (OK);
}


char bin2hex(int highbit)
{
    char retval;
    int i;
    unsigned char dec; /* declared char to be compatible with retval*/
    dec = 0;

    /* this is an iterative algo working from msb to lsb */
    for (i = 0; i < 4; i++)
    {
        dec = 2 * dec; /* add to the power of 2 */
        dec = dec + bin_instruction[highbit + i];
    }

    if (dec > 9)
    { /* the hex value must be an alphabet */
        dec = dec - 10;
        retval = dec + 'A';
    }
    else
    { /* the hex value is a number 0--9 */
        retval = dec + '0';
    }

    return retval;
}

/********************write_instruction**********************/
/* writes the instruction into the output file as a hex val*/
void write_instruction()
{
    char a;
    /* first put the '0x' in the beginning of the line*/
    fputc('0', outfile);
    fputc('x', outfile);

    //   a = bin2hex(31);
    //   fputc(a, outfile);
    // Add code for further nibbles here

    for (int i = 0; i < 32; i += 4)
    {
        char hex_digit = bin2hex(i); // Convert 4 bits at a time to a hex digit
        fputc(hex_digit, outfile);
    }

    /** now put a newline at the end*/
    fputs("\r\n", outfile);
}

int instructionType(char *lptr)
{

    if ((strcmp(lptr, "add") == 0) ||
        (strcmp(lptr, "sub") == 0) ||
        (strcmp(lptr, "xor") == 0) ||
        (strcmp(lptr, "or") == 0) ||
        (strcmp(lptr, "and") == 0) ||
        (strcmp(lptr, "sll") == 0) ||
        (strcmp(lptr, "srl") == 0) ||
        (strcmp(lptr, "sra") == 0) ||
        (strcmp(lptr, "slt") == 0) ||
        (strcmp(lptr, "sltu") == 0))
        return 1;

    if ((strcmp(lptr, "addi") == 0) ||
        (strcmp(lptr, "xori") == 0) ||
        (strcmp(lptr, "ori") == 0) ||
        (strcmp(lptr, "andi") == 0) ||
        (strcmp(lptr, "slli") == 0) ||
        (strcmp(lptr, "srli") == 0) ||
        (strcmp(lptr, "srai") == 0) ||
        (strcmp(lptr, "slti") == 0) ||
        (strcmp(lptr, "sltiu") == 0) ||
        (strcmp(lptr, "lb") == 0) ||
        (strcmp(lptr, "lh") == 0) ||
        (strcmp(lptr, "lw") == 0) ||
        (strcmp(lptr, "lbu") == 0) ||
        (strcmp(lptr, "lhu") == 0) ||
        (strcmp(lptr, "jalr") == 0) ||
        (strcmp(lptr, "ecall") == 0) ||
        (strcmp(lptr, "ebreak") == 0))
        return 2;

    if ((strcmp(lptr, "sb") == 0) ||
        (strcmp(lptr, "sh") == 0) ||
        (strcmp(lptr, "sw") == 0))
        return 3;

    if ((strcmp(lptr, "beq") == 0) ||
        (strcmp(lptr, "bne") == 0) ||
        (strcmp(lptr, "blt") == 0) ||
        (strcmp(lptr, "bge") == 0) ||
        (strcmp(lptr, "bltu") == 0) ||
        (strcmp(lptr, "bgeu") == 0))
        return 4;

    if ((strcmp(lptr, "jal") == 0))
        return 5;

    if ((strcmp(lptr, "lui") == 0) ||
        (strcmp(lptr, "auipc") == 0))
        return 6;
    if ((strcmp(lptr, "nop") == 0) ||
        (strcmp(lptr, "mv") == 0) ||
        (strcmp(lptr, "not") == 0) ||
        (strcmp(lptr, "neg") == 0) ||
        (strcmp(lptr, "j") == 0)  ||
        (strcmp(lptr, "jr") == 0)||
        (strcmp(lptr, "jal") == 0) ||
        (strcmp(lptr, "li") == 0) ||
        (strcmp(lptr, "la") == 0) ||
        (strcmp(lptr, "call") == 0) ||
        (strcmp(lptr, "ret") == 0) ||
        (strcmp(lptr, "beqz") == 0) ||
        (strcmp(lptr, "bgez") == 0) ||
        (strcmp(lptr, "bltz") == 0) ||
        (strcmp(lptr, "bgt") == 0) ||
        (strcmp(lptr, "ble") == 0))
        return (7);

    else
    {

        return 0;
    }
}

int pseudoToOriginal(char *pseudoInstruction, char *PsOpcode, int current_pc)
{
    char *originalInstruction = malloc(sizeof(char) * 255); // Adjust size as needed

    // Initialize the binary string
    int binary ;

    // Source and target registers
    char sourceRegister[4];
    char targetRegister[4];
    for (int i = 0; i < 3; i++)
    {
        sourceRegister[i] = '\0';
        targetRegister[i] = '\0';
    }

    // pseudoInstruction=strtok(pseudoInstruction,":");
    // if(pseudoInstruction!=NULL)
    // {

    // }
    // Immediate value (for instructions like "li" and "addi")
    int immediateValue;

    // Determine the pseudo-instruction and convert to the corresponding original instruction
    if (strcmp(PsOpcode, "nop") == 0)
    {
        // Original instruction: addi x0, x0, 0 (NOP)
        snprintf(originalInstruction, 255, "addi x0, x0, 0");
    }
    else if (strcmp(PsOpcode, "li") == 0)
    {
        // Example: "li x3, 42"
        char number[255];
        if (sscanf(pseudoInstruction, " li %3[^,], %s", targetRegister, number) == 2)
        {
            immediateValue = toNum(number);

            if (immediateValue > -2048 && immediateValue < 2047)
                snprintf(originalInstruction, 255, "addi %s, x0, %d", targetRegister, immediateValue);
            else
            {
                char *immediateValue_bin = decToBin(immediateValue + 0x800, 32);
                char *immediateValue_up20bit = substring(immediateValue_bin, 0, 19);
                int upper20 = binaryToDecimal(immediateValue_up20bit);
                int lower12 = immediateValue - (upper20 << 12);

                snprintf(originalInstruction, 255,"lui %s, %d\n",targetRegister,upper20);
                
                if (lower12 != 0){
                    snprintf(originalInstruction, 255,"addi %s, %s, %d",targetRegister,targetRegister,lower12);

                }
            }
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "mv") == 0)
    {
        // Example: "mv x3, x2"
        if (sscanf(pseudoInstruction, " mv %3[^,] , %3s", targetRegister, sourceRegister) == 2)
        {
            snprintf(originalInstruction, 255, "addi %s, %s, 0", targetRegister, sourceRegister);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "not") == 0)
    {
        // Example: "not x4, x3"
        if (sscanf(pseudoInstruction, " not %3[^,], %3s", targetRegister, sourceRegister) == 2)
        {
            snprintf(originalInstruction, 255, "xori %s, %s, -1", targetRegister, sourceRegister);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "neg") == 0)
    {
        // Example: "neg x2, x1"
        if (sscanf(pseudoInstruction, "neg %3[^,], %3s", targetRegister, sourceRegister) == 2)
        {
            snprintf(originalInstruction, 255, "sub %s, x0, %s", targetRegister, sourceRegister);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "j") == 0)
    {
        // Example: "j my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " j %s", label) == 1)
        {
            snprintf(originalInstruction, 255, "jal x0, %s", label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "jal") == 0)
    {
        // Example: "jal my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " jal %s", label) == 1)
        {
            snprintf(originalInstruction, 255, "jal x1, %s", label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "jr") == 0)
    {
        // Example: "jal my_label"
        char targetregister[3];
        if (sscanf(pseudoInstruction, " jr %s", targetregister) == 1)
        {
            snprintf(originalInstruction, 255, "jalr x0, %s, 0", targetregister);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "la") == 0)
    {
        // Example: "la x2, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " la %3[^,], %s", targetRegister, label) == 2)
        {
            int label_off = calc_offset(current_pc - 8, label);
            char *label_off_bin = decToBin(label_off, 32);
            snprintf(originalInstruction, 255, "auipc %s, %d\naddi %s, %s, %d", targetRegister, binaryToDecimal(substring(label_off_bin, 0, 19)), targetRegister, targetRegister, binaryToDecimal(substring(label_off_bin, 20, 31)));
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "call") == 0)
    {
        // Example: "call my_function"
        char label[64];
        if (sscanf(pseudoInstruction, " call %s", label) == 1)
        {
            int label_off = calc_offset(current_pc - 8, label);
            char *label_off_bin = decToBin(label_off, 32);
            if (label_off >= -2048 && label_off <= 2047)
            {
                snprintf(originalInstruction, 255, "auipc x6, %d\n jalr x1,x6,%d", 0, binaryToDecimal(substring(label_off_bin, 20, 31)));
            }
            else
            {
                snprintf(originalInstruction, 255, "auipc x6, %d\n jalr x1,x6,%d", binaryToDecimal(substring(label_off_bin, 0, 19)), binaryToDecimal(substring(label_off_bin, 20, 31)));
            }
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "ret") == 0)
    {
        // Original instruction: "jalr x0, x1, 0"
        snprintf(originalInstruction, 255, "jalr x0, x1, 0");
    }
    else if (strcmp(PsOpcode, "beqz") == 0)
    {
        // Example: "beqz x2, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " beqz %3[^,], %20s[^,]", sourceRegister, label) == 2)
        {
            snprintf(originalInstruction, 255, "beq %s, x0, %s", sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "bgez") == 0)
    {
        // Example: "bgez x2, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " bgez %3[^,], %s", sourceRegister, label) == 2)
        {
            snprintf(originalInstruction, 255, "bge %s, x0, %s", sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "bltz") == 0)
    {
        // Example: "bltz x2, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " bltz %3[^,], %s", sourceRegister, label) == 2)
        {
            snprintf(originalInstruction, 255, "blt %s, x0, %s", sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }
    else if (strcmp(PsOpcode, "bgt") == 0)
    {
        // Example: "bgt x3, x4, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " bgt %3[^,], %3[^,], %s", sourceRegister, targetRegister, label) == 3)
        {
            snprintf(originalInstruction, 255, "blt %2s, %2s, %s", targetRegister, sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }

    else if (strcmp(PsOpcode, "ble") == 0)
    {
        // Example: "ble x3, x4, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " ble %3[^,], %3[^,], %s", sourceRegister, targetRegister, label) == 3)
        {
            snprintf(originalInstruction, 255, "bge %3s, %3s, %s", targetRegister, sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }

    else if (strcmp(PsOpcode, "bgtu") == 0)
    {
        // Example: "bgtu x3, x4, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " bgtu %3[^,], %3[^,], %s", sourceRegister, targetRegister, label) == 3)
        {
            snprintf(originalInstruction, 255, "bltu %3s, %3s, %s", targetRegister, sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }

    else if (strcmp(PsOpcode, "bleu") == 0)
    {
        // Example: "bleu x3, x4, my_label"
        char label[64];
        if (sscanf(pseudoInstruction, " bleu %3[^,], %3[^,], %s", sourceRegister, targetRegister, label) == 3)
        {
            snprintf(originalInstruction, 255, "bgeu %3s, %3s, %s", targetRegister, sourceRegister, label);
        }
        else
        {
            printf("Invalid format for pseudo-instruction: %s\n", pseudoInstruction);
            return -1;
        }
    }

again:
    char *dup = strdup(originalInstruction);
    char *lOpcode = strtok(dup, " \t\n");
    int type = instructionType(lOpcode);

    switch (type)
    {
    case 1:
    {
        originalInstruction = strtok(originalInstruction, "\n");
        binary = RType(originalInstruction);
        write_instruction();
        originalInstruction = strtok(NULL, "\n");
        if (originalInstruction != NULL)
        {
            goto again;
        }
        break;
    }
    case 2:
    {
        originalInstruction = strtok(originalInstruction, "\n");
        binary = iType(originalInstruction, current_pc);
        write_instruction();
        originalInstruction = strtok(NULL, "\n");
        if (originalInstruction != NULL)
        {
            goto again;
        }
        break;
    }
    case 3:
    {
        originalInstruction = strtok(originalInstruction, "\n");
        binary = sType(originalInstruction);
        write_instruction();
        originalInstruction = strtok(NULL, "\n");
        if (originalInstruction != NULL)
        {
            goto again;
        }
        break;
    }
    case 4:
    {
        originalInstruction = strtok(originalInstruction, "\n");
        binary = bType(originalInstruction, current_pc);
        write_instruction();
        originalInstruction = strtok(NULL, "\n");
        if (originalInstruction != NULL)
        {
            goto again;
        }
        break;
    }
    case 5:
    {
        originalInstruction = strtok(originalInstruction, "\n");
        binary = jType(originalInstruction, current_pc);
        write_instruction();
        originalInstruction = strtok(NULL, "\n");
        if (originalInstruction != NULL)
        {
            goto again;
        }
        break;
    }
    case 6:
    {
        originalInstruction = strtok(originalInstruction, "\n");
        binary = uType(originalInstruction);
        write_instruction();
        originalInstruction = strtok(NULL, "\n");
        if (originalInstruction != NULL)
        {
            goto again;
        }
        break;
    }

        return binary;
    }
}

int main(int argc, char *argv[])
{

    /* open the source file */
    infile = fopen(argv[1], "r");  // First file for reading
    outfile = fopen(argv[2], "w"); // second file for writing machine language
    char c;
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
        *lArg2, *lArg3, *lArg4;

    // sType("sw    zero,-44(s0)");

    int lRet;
    int lRet1;
    int immediate_op;
    if (!infile)
    {
        printf("Error: Cannot open input file %s\n", argv[1]);
        exit(4);
    }
    if (!outfile)
    {
        printf("Error: Cannot open output file %s\n", argv[2]);
        exit(4);
    }

    /*look for the .orig statement*/
    int start_address = 0;
    //    mytable[1].address=16;
    // strcpy(mytable[1].label,"loop");

    /****************************************************************/
    FILE *lInfile;
    FILE *lInfile2;

    lInfile = infile; /* open the input file */
    lInfile2 = infile;
    int current_pc = 0;
    char *templabel;
    int index_table = 0;

    // jType("jal x6,loop",current_pc);
    /* *templabel = *lLabel;*/
    do
    {
        lRet1 = readAndParse(lInfile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);

        // Add your code here
        if (strcmp(lOpcode, "la") == 0 || strcmp(lOpcode, "call") == 0)
            current_pc = current_pc + 8;
        else if (isOpcode(lOpcode))
            current_pc = current_pc + 4;
        if (strcmp(lLabel, "") != 0 /*&& lLabel[strlen(lLabel)-1]==':'*/)
        {
            // lLabel[strlen(lLabel)-1]='\0';
            mytable[index_table].address = current_pc;
            strcpy(mytable[index_table].label, lLabel);
            index_table++;
        }

    } while (lRet1 != DONE);

    // Second pass starts here
    current_pc = start_address; // Initialize the program counter with the start address
    fseek(lInfile2, 0, SEEK_SET);
    while ((lRet = readAndParse(lInfile2, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4)) != DONE)
    {
        // printf("%s\n", lOpcode);
        if (lRet == EMPTY_LINE || /*strcmp(lLabel,"")!=0*/ lLabel[0] == '#')
        {
            continue; // Skip empty lines
        }

        // // Check for label and update the symbol table
        // if (lLabel[0] != '\0') {
        //     strcpy(mytable[index_table].label, lLabel);
        //     mytable[index_table].address = current_pc;
        //     index_table++;
        // }

        // Check the instruction type
        int type = instructionType(lOpcode);

        if (strcmp(lOpcode, "jal") == 0 && strcmp(lArg2, "") == 0)
        {
            type = 7;
        }
        if (strcmp(lOpcode, "la") == 0 || strcmp(lOpcode, "call") == 0)
            current_pc = current_pc + 8;
        else if (isOpcode(lOpcode))
            current_pc = current_pc + 4;
        switch (type)
        {
        case 1: // R-Type
        {
            // Generate R-Type binary instruction
            int binary = RType(lLine);
            // Write the binary instruction to the output file
            // char * hex=bin2hex(binary);
            // fprintf(outfile, "%s\n", hex);
            write_instruction();
            break;
        }
        case 2: // I-Type
        {
            // Generate I-Type binary instruction
            int binary = iType(lLine, current_pc);
            // Write the binary instruction to the output file
            // char * hex=bin2hex(binary);
            // fprintf(outfile, "%s\n", hex);
            write_instruction();
            break;
        }
        case 3: // S-Type
        {
            // Generate S-Type binary instruction
            int binary = sType(lLine);
            // Write the binary instruction to the output file
            // char * hex=bin2hex(binary);
            // fprintf(outfile, "%s\n", hex);
            write_instruction();
            break;
        }
        case 4: // B-Type
        {
            // Generate B-Type binary instruction
            int binary = bType(lLine, current_pc);
            // Write the binary instruction to the output file
            // char * hex=bin2hex(binary);
            // fprintf(outfile, "%s\n", hex);
            write_instruction();
            break;
        }
        case 5: // J-Type
        {
            // Generate J-Type binary instruction
            int binary = jType(lLine, current_pc);
            // Write the binary instruction to the output file
            // char * hex=bin2hex(binary);
            // fprintf(outfile, "%s\n", hex);
            write_instruction();
            break;
        }
        case 6: // U-Type
        {
            // Generate U-Type binary instruction
            int binary = uType(lLine);
            // Write the binary instruction to the output file
            // char * hex=bin2hex(binary);
            // fprintf(outfile, "%s\n", hex);
            write_instruction();
            break;
        }
        case 7: // Pseudo Instructions (e.g., NOP)
        {
            pseudoToOriginal(lLine, lOpcode, current_pc);
            break;
        }
            // default:
            //     printf("Error: Unknown instruction type.\n");
            //     fclose(infile);
            //     fclose(outfile);
            //     exit(4);
        }
    }

    fclose(infile); /* Now close files */
    fclose(outfile);
}
