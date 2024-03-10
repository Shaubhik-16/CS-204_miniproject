#include <iostream>
#include <fstream> 
#include <bitset>
#include <unordered_map>
using namespace std;

// main class for all the instructions
class instructions
{
public:
    bitset<32> instructionIn32bit; // to store instuction code in bin
    string instructionInHex;       // to store instuction code in hex
    bitset<7> opcode;              // opcode for the instructions

    // function to convert 32 bit bin to hex
    void binToHex()
    {
        instructionInHex = "0x"; // initialising with 0x
        // converting 4 bits at a time to hex
        for (int i = 31; i >= 0;)
        {
            int num = 0;
            for (int j = 8; j >= 1; j /= 2)
            {
                num += instructionIn32bit[i] * j;
                i--;
            }
            if (num < 10)
            {
                instructionInHex += num + '0';
            }
            else
            {
                instructionInHex += num - 10 + 'A';
            }
        }
    }
};

// class label for label in instructions and labels
class label
{
public:
    string name; // label name
    int pc = 0;  // goto pc of label
};

// classes for type of instructions

// class for R type instructions inheriting instuctions class publicly
class R : public instructions
{
public:
    bitset<5> rd;    // to store destination register value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<5> rs2;   // to store source register 2 value
    bitset<7> func7; // to store func7 value

    // function to fetch rd,rs1,rs2 fields of the R type instruction
    void fetchInstructionDetails(string line, int i)
    {
        while (line[i] != 'x')
        {
            i++;
        }
        i++;

        // fetching rd
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue); // string to int conversion
        rd = registerValueInDecimal;

        // fetching rs1
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        registerValueInDecimal = stoi(registerValue);
        rs1 = registerValueInDecimal;

        // fetching rs2
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        registerValue = "";
        while (i < line.size() and line[i] != ' ' and line[i] != '#')
        {
            registerValue += line[i];
            i++;
        }
        registerValueInDecimal = stoi(registerValue);
        rs2 = registerValueInDecimal;

        return;
    }

    // function to make instruction to bin 32 bit format joining all different fields
    void convertTo32bit()
    {
        int idx = 0;
        for (; idx < 7; idx++)
        {
            instructionIn32bit[idx] = opcode[idx];
        }
        for (; idx < 12; idx++)
        {
            instructionIn32bit[idx] = rd[idx - 7];
        }
        for (; idx < 15; idx++)
        {
            instructionIn32bit[idx] = func3[idx - 12];
        }
        for (; idx < 20; idx++)
        {
            instructionIn32bit[idx] = rs1[idx - 15];
        }
        for (; idx < 25; idx++)
        {
            instructionIn32bit[idx] = rs2[idx - 20];
        }
        for (; idx < 32; idx++)
        {
            instructionIn32bit[idx] = func7[idx - 25];
        }
        return;
    }
};

// class for I type instructions inheriting instuctions class publicly
class I : public instructions
{
public:
    bitset<5> rd;    // to store destination register value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<12> imm;  // to store immediate field value

    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,rs1,imm
    void fetchInstructionDetailsType1(string line, int i)
    {

        // fetching rd
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue);
        rd = registerValueInDecimal;

        // fetching rs1
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        registerValueInDecimal = stoi(registerValue);
        rs1 = registerValueInDecimal;

        // fetching imm
        while (!isdigit(line[i]))
        {
            i++;
        }
        string immValue = "";
        while (i < line.size() and line[i] != ' ' and line[i] != '#')
        {
            immValue += line[i];
            i++;
        }
        int immValueInDecimal = stoi(immValue);
        imm = immValueInDecimal;

        return;
    }

    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,imm(rs1)
    void fetchInstructionDetailsType2(string line, int i)
    {

        // fetching rd
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue);
        rd = registerValueInDecimal;

        // fetching imm
        while (!isdigit(line[i]))
        {
            i++;
        }
        string immValue = "";
        while (line[i] != ' ' and line[i] != '(')
        {
            immValue += line[i];
            i++;
        }
        int immValueInDecimal = stoi(immValue);
        imm = immValueInDecimal;

        // fetching rs1
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        registerValue = "";
        while (line[i] != ' ' and line[i] != ')')
        {
            registerValue += line[i];
            i++;
        }
        registerValueInDecimal = stoi(registerValue);
        rs1 = registerValueInDecimal;

        return;
    }

    // function to make instruction to bin 32 bit format joining all different fields
    void convertTo32bit()
    {
        int idx = 0;
        for (; idx < 7; idx++)
        {
            instructionIn32bit[idx] = opcode[idx];
        }
        for (; idx < 12; idx++)
        {
            instructionIn32bit[idx] = rd[idx - 7];
        }
        for (; idx < 15; idx++)
        {
            instructionIn32bit[idx] = func3[idx - 12];
        }
        for (; idx < 20; idx++)
        {
            instructionIn32bit[idx] = rs1[idx - 15];
        }
        for (; idx < 32; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 20];
        }
        return;
    }
};

// class for S type instructions inheriting instuctions class publicly
class S : public instructions
{
public:
    bitset<12> imm;  // to store imm value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<5> rs2;   // to store source register 2 value

    // function to fetch rs1,imm,rs2 fields of the S type instruction
    void fetchInstructionDetails(string line, int i)
    {
        // fetching rs2
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue);
        rs2 = registerValueInDecimal;

        // fetching imm
        while (!isdigit(line[i]))
        {
            i++;
        }
        string immValue = "";
        while (line[i] != ' ' and line[i] != '(')
        {
            immValue += line[i];
            i++;
        }
        int immValueInDecimal = stoi(immValue);
        imm = immValueInDecimal;

        // fetching rs1
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        registerValue = "";
        while (line[i] != ' ' and line[i] != ')')
        {
            registerValue += line[i];
            i++;
        }
        registerValueInDecimal = stoi(registerValue);
        rs1 = registerValueInDecimal;

        return;
    }

    // function to make instruction to bin 32 bit format joining all different fields
    void convertTo32bit()
    {
        int idx = 0;
        for (; idx < 7; idx++)
        {
            instructionIn32bit[idx] = opcode[idx];
        }
        for (; idx < 12; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 7]; // imm[4:0]
        }
        for (; idx < 15; idx++)
        {
            instructionIn32bit[idx] = func3[idx - 12];
        }
        for (; idx < 20; idx++)
        {
            instructionIn32bit[idx] = rs1[idx - 15];
        }
        for (; idx < 25; idx++)
        {
            instructionIn32bit[idx] = rs2[idx - 20];
        }
        for (; idx < 32; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 20]; // imm[11:5]
        }
        return;
    }
};

// class for SB type instructions inheriting instuctions class publicly
class SB : public instructions
{
public:
    bitset<12> imm;  // to store imm value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<5> rs2;   // to store source register 2 value

    // function to fetch rs1,imm,rs2 fields of the SB type instruction
    void fetchInstructionDetails(string line, int i, unordered_map<string, int> labelMap, int currPC)
    {

        // fetching rs1
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue);
        rs1 = registerValueInDecimal;

        // fetching rs2
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        registerValueInDecimal = stoi(registerValue);
        rs2 = registerValueInDecimal;

        // fetching imm
        while (line[i] != ',')
        {
            i++;
        }
        i++;
        while (line[i] == ' ')
            i++;
        string Label = "";
        while (i < line.size() and line[i] != ' ' and line[i] != '#')
        {
            Label += line[i];
            i++;
        }
        auto it = labelMap.find(Label);
        if (it != labelMap.end())
        {
            int labelPc = it->second;
            imm = (labelPc - currPC) / 2;
        }
        return;
    }

    // function to make instruction to bin 32 bit format joining all different fields
    void convertTo32bit()
    {
        int idx = 0;
        for (; idx < 7; idx++)
        {
            instructionIn32bit[idx] = opcode[idx];
        }
        instructionIn32bit[idx] = imm[10]; // imm[11]
        idx++;
        for (; idx < 12; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 8]; // imm[4:1]
        }
        for (; idx < 15; idx++)
        {
            instructionIn32bit[idx] = func3[idx - 12];
        }
        for (; idx < 20; idx++)
        {
            instructionIn32bit[idx] = rs1[idx - 15];
        }
        for (; idx < 25; idx++)
        {
            instructionIn32bit[idx] = rs2[idx - 20];
        }
        for (; idx < 31; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 21]; // imm[10:5]
        }
        instructionIn32bit[idx] = imm[11]; // imm[12]
        return;
    }
};

// class for U type instructions inheriting instuctions class publicly
class U : public instructions
{
public:
    bitset<5> rd;   // to store destination register value
    bitset<20> imm; // to store immediate field value

    // function to fetch rd,imm fields of the I type instruction of type rd,rs1,imm
    void fetchInstructionDetails(string line, int i)
    {

        // fetching rd
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue);
        rd = registerValueInDecimal;

        // fetching imm
        while (!isdigit(line[i]))
        {
            i++;
        }
        string immValue = "";
        while (i < line.size() and line[i] != ' ' and line[i] != '#')
        {
            immValue += line[i];
            i++;
        }
        int immValueInDecimal = stoi(immValue);
        imm = immValueInDecimal;

        return;
    }

    // function to make instruction to bin 32 bit format joining all different fields
    void convertTo32bit()
    {
        int idx = 0;
        for (; idx < 7; idx++)
        {
            instructionIn32bit[idx] = opcode[idx];
        }
        for (; idx < 12; idx++)
        {
            instructionIn32bit[idx] = rd[idx - 7];
        }
        for (; idx < 32; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 12];
        }
        return;
    }
};

// class for UJ type instructions inheriting instuctions class publicly
class UJ : public instructions
{
public:
    bitset<5> rd;   // to store destination register value
    bitset<20> imm; // to store imm value

    // function to fetch rd,imm fields of the UJ type instruction
    void fetchInstructionDetails(string line, int i, unordered_map<string, int> labelMap, int currPC)
    {

        // fetching rd
        while (line[i] != 'x')
        {
            i++;
        }
        i++;
        string registerValue = "";
        while (line[i] != ' ' and line[i] != ',')
        {
            registerValue += line[i];
            i++;
        }
        int registerValueInDecimal = stoi(registerValue);
        rd = registerValueInDecimal;

        // fetching imm
        while (line[i] != ',')
        {
            i++;
        }
        i++;
        while (line[i] == ' ')
            i++;
        string Label = "";
        while (i < line.size() and line[i] != ' ' and line[i] != '#')
        {
            Label += line[i];
            i++;
        }
        auto it = labelMap.find(Label);
        if (it != labelMap.end())
        {
            int labelPc = it->second;
            imm = (labelPc - currPC) / 2;
        }
        return;
    }

    // function to make instruction to bin 32 bit format joining all different fields
    void convertTo32bit()
    {
        int idx = 0;
        for (; idx < 7; idx++)
        {
            instructionIn32bit[idx] = opcode[idx];
        }
        for (; idx < 12; idx++)
        {
            instructionIn32bit[idx] = rd[idx - 7];
        }
        for (; idx < 20; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 1]; // imm[19:12]
        }
        instructionIn32bit[idx] = imm[10]; // imm[11]
        idx++;
        for (; idx < 31; idx++)
        {
            instructionIn32bit[idx] = imm[idx - 21]; // imm[10:1]
        }
        instructionIn32bit[idx] = imm[19]; // imm[20]

        return;
    }
};

// classes for individual instructions

// class for add instructions inheriting R publicly
class add : public R
{
public:
    add()
    {
        this->opcode = 51;
        this->func3 = 0;
        this->func7 = 0;
    }
};

// class for and instructions inheriting R publicly
class And : public R
{ // since and is a keyword so And name is given
public:
    And()
    {
        this->opcode = 51;
        this->func3 = 7;
        this->func7 = 0;
    }
};

// class for or instructions inheriting R publicly
class Or : public R
{ // since or is a keyword so Or name is given
public:
    Or()
    {
        this->opcode = 51;
        this->func3 = 6;
        this->func7 = 0;
    }
};

// class for sll instructions inheriting R publicly
class sll : public R
{
public:
    sll()
    {
        this->opcode = 51;
        this->func3 = 1;
        this->func7 = 0;
    }
};

// class for slt instructions inheriting R publicly
class slt : public R
{
public:
    slt()
    {
        this->opcode = 51;
        this->func3 = 2;
        this->func7 = 0;
    }
};

// class for sra instructions inheriting R publicly
class sra : public R
{
public:
    sra()
    {
        this->opcode = 51;
        this->func3 = 5;
        this->func7 = 32;
    }
};

// class for srl instructions inheriting R publicly
class srl : public R
{
public:
    srl()
    {
        this->opcode = 51;
        this->func3 = 5;
        this->func7 = 0;
    }
};

// class for sub instructions inheriting R publicly
class sub : public R
{
public:
    sub()
    {
        this->opcode = 51;
        this->func3 = 0;
        this->func7 = 32;
    }
};

// class for xor instructions inheriting R publicly
class Xor : public R
{ // since xor is a keyword so Xor name is given
public:
    Xor()
    {
        this->opcode = 51;
        this->func3 = 4;
        this->func7 = 0;
    }
};

// class for mul instructions inheriting R publicly
class mul : public R
{
public:
    mul()
    {
        this->opcode = 51;
        this->func3 = 0;
        this->func7 = 1;
    }
};

// class for div instructions inheriting R publicly
class Div : public R
{ // since div() is a predefined function so Div name is given
public:
    Div()
    {
        this->opcode = 51;
        this->func3 = 4;
        this->func7 = 1;
    }
};

// class for rem instructions inheriting R publicly
class rem : public R
{
public:
    rem()
    {
        this->opcode = 51;
        this->func3 = 6;
        this->func7 = 1;
    }
};

// class for addi instructions inheriting I publicly
class addi : public I
{
public:
    addi()
    {
        opcode = 19;
        func3 = 0;
    }
};

// class for andi instructions inheriting I publicly
class andi : public I
{
public:
    andi()
    {
        opcode = 19;
        func3 = 7;
    }
};

// class for ori instructions inheriting I publicly
class ori : public I
{
public:
    ori()
    {
        opcode = 19;
        func3 = 6;
    }
};

// class for lb instructions inheriting I publicly
class lb : public I
{
public:
    lb()
    {
        opcode = 3;
        func3 = 0;
    }
};

// class for lh instructions inheriting I publicly
class lh : public I
{
public:
    lh()
    {
        opcode = 3;
        func3 = 1;
    }
};

// class for lw instructions inheriting I publicly
class lw : public I
{
public:
    lw()
    {
        opcode = 3;
        func3 = 2;
    }
};

// class for ld instructions inheriting I publicly
class ld : public I
{
public:
    ld()
    {
        opcode = 3;
        func3 = 3;
    }
};

// class for jalr instructions inheriting I publicly
class jalr : public I
{
public:
    jalr()
    {
        opcode = 103;
        func3 = 0;
    }
};

// class for sb instructions inheriting S publicly
class sb : public S
{
public:
    sb()
    {
        opcode = 35;
        func3 = 0;
    }
};

// class for sw instructions inheriting S publicly
class sw : public S
{
public:
    sw()
    {
        opcode = 35;
        func3 = 2;
    }
};

// class for sd instructions inheriting S publicly
class sd : public S
{
public:
    sd()
    {
        opcode = 35;
        func3 = 3;
    }
};

// class for sh instructions inheriting S publicly
class sh : public S
{
public:
    sh()
    {
        opcode = 35;
        func3 = 1;
    }
};

// class for beq instructions inheriting SB publicly
class beq : public SB
{
public:
    beq()
    {
        opcode = 99;
        func3 = 0;
    }
};

// class for bne instructions inheriting SB publicly
class bne : public SB
{
public:
    bne()
    {
        opcode = 99;
        func3 = 1;
    }
};

// class for bge instructions inheriting SB publicly
class bge : public SB
{
public:
    bge()
    {
        opcode = 99;
        func3 = 5;
    }
};

// class for blt instructions inheriting SB publicly
class blt : public SB
{
public:
    blt()
    {
        opcode = 99;
        func3 = 4;
    }
};

// class for auipc instructions inheriting U publicly
class auipc : public U
{
public:
    auipc()
    {
        opcode = 23;
    }
};

// class for lui instructions inheriting U publicly
class lui : public U
{
public:
    lui()
    {
        opcode = 55;
    }
};

// class for jal instructions inheriting UJ publicly
class jal : public UJ
{
public:
    jal()
    {
        opcode = 111;
    }
};

// class to make machine code for whole program
class makeMC
{
public:
    bitset<32> pc = 0;                           // to store pc
    string pcInHex;                              // to store pc in hex
    unordered_map<string, int> labelMap;         // to store all labels along with their gotopc
    bitset<32> mem = 0;                          // memory address
    string memInHex;                             // to store mem in hex
    unordered_map<string, bitset<32>> memoryMap; // to store data labels along with their memory

    makeMC()
    {
        mem[28] = 1;
    }

    // function to increment pc by 4
    void incrementPC(bitset<32> &pc)
    {
        for (int i = 2; i < 32; i++)
        {
            if (pc[i] == 0)
            {
                pc[i] = 1;
                break;
            }
            else
            {
                pc[i] = 0;
            }
        }
        return;
    }

    // function to increment mem by 1
    void incrementMem()
    {
        for (int i = 0; i < 32; i++)
        {
            if (mem[i] == 0)
            {
                mem[i] = 1;
                break;
            }
            else
            {
                mem[i] = 0;
            }
        }
        return;
    }

    // function to convert 32 bit bin to hex
    void binToHexPC()
    {
        if (pc == 0)
        {
            pcInHex = "0x0";
            return;
        }
        pcInHex = "0x"; // initialising with 0x
        bool flag = 0;
        // converting 4 bits at a time to hex
        for (int i = 31; i >= 0;)
        {
            int num = 0;
            for (int j = 8; j >= 1; j /= 2)
            {
                num += pc[i] * j;
                i--;
            }
            if (num != 0)
                flag = 1;
            if (flag)
            {
                if (num < 10)
                {
                    pcInHex += num + '0';
                }
                else
                {
                    pcInHex += num - 10 + 'A';
                }
            }
        }
        return;
    }

    // function to convert 32 bit bin to hex
    void binToHexmem()
    {
        memInHex = "0x"; // initialising with 0x
        // converting 4 bits at a time to hex
        for (int i = 31; i >= 0;)
        {
            int num = 0;
            for (int j = 8; j >= 1; j /= 2)
            {
                num += mem[i] * j;
                i--;
            }
            if (num < 10)
            {
                memInHex += num + '0';
            }
            else
            {
                memInHex += num - 10 + 'A';
            }
        }
        return;
    }

    // function to convert 8 bit bin to hex
    string binToHex(bitset<8> byte)
    {
        string hexa = "0x";
        // converting 4 bits at a time to hex
        for (int i = 7; i >= 0;)
        {
            int num = 0;
            for (int j = 8; j >= 1; j /= 2)
            {
                num += byte[i] * j;
                i--;
            }
            if (num < 10)
            {
                hexa += num + '0';
            }
            else
            {
                hexa += num - 10 + 'A';
            }
        }
        return hexa;
    }

    // to check if the label is correctly made in input.asm
    bool checkLabel(string line)
    {
        int i = line.size();
        while (line[i] != ':')
        {
            i--;
            if (i < 0)
                return false;
        }
        return true;
    }

    // to store label name and its corresponding gotopc in labelmap
    void makeLabel(string line)
    {
        int i = 0;
        string labelName = "";
        while (line[i] != ' ' && line[i] != ':')
        {
            labelName += line[i];
            i++;
        }
        while (line[i] != ':')
        {
            i++;
        }
        label *newLabel = new label();
        newLabel->pc = this->pc.to_ulong();
        newLabel->name = labelName;
        labelMap[labelName] = newLabel->pc;
        for (int j = i + 1; j < line.size(); j++)
        {
            if (line[j] == '#')
                break;
            if (line[j] != ' ')
            {
                string subLine = line.substr(j);
                if (checkDirectives(subLine))
                {
                    workOnDirectives(subLine);
                    break;
                }
                else
                {
                    if (checkInstruction(subLine) != "comment" and checkInstruction(subLine) != "error")
                        ;
                    incrementPC(pc);
                    break;
                }
            }
        }
        return;
    }

    bool checkDirectives(string line)
    {
        int i = 0;
        while (line[i] == ' ')
            i++;
        string directive = "";
        while (line[i] != ' ')
        {
            directive += line[i];
            i++;
        }
        if (directive == ".byte" || directive == ".half" || directive == ".word" || directive == ".dword" || directive == ".asciz")
            return true;
        else
            return false;
    }

    void makeDataLabel(string line)
    {
        int i = 0;
        string labelName = "";
        while (line[i] != ' ' && line[i] != ':')
        {
            labelName += line[i];
            i++;
        }
        while (line[i] != ':')
        {
            i++;
        }
        for (int j = i + 1; j < line.size(); j++)
        {
            if (line[j] == '#')
                break;

            if (line[j] != ' ')
            {
                string subLine = line.substr(j);
                if (!checkDirectives(subLine))
                {
                    cout << "error";
                    return;
                }
                else
                {
                    workOnDirectives(subLine);
                    break;
                }
            }
        }
        memoryMap[labelName] = mem;
        return;
    }

    void workOnDirectives(string line)
    {
        int i = 0;
        while (line[i] == ' ')
            i++;
        string directive = "";
        while (line[i] != ' ')
        {
            directive += line[i];
            i++;
        }
        if (directive == ".byte")
        {
            ofstream fout;                         // to write to a file
            fout.open("output.mc", std::ios::app); // open output.mc to write
            while (i < line.size())
            {
                bitset<8> byte;
                string t = "";
                while (i < line.size() and (line[i] == ' ' or line[i] == ','))
                {
                    i++;
                }
                if (line[i] == '#')
                    break;
                while (line[i] != ' ' and line[i] != ',')
                {
                    t += line[i];
                    i++;
                }
                byte = stoi(t);
                binToHexmem();
                fout << memInHex << " " << binToHex(byte) << endl;
                incrementMem();
            }
            fout.close();
        }
        else if (directive == ".half")
        {
            ofstream fout;                         // to write to a file
            fout.open("output.mc", std::ios::app); // open output.mc to write
            while (i < line.size())
            {
                bitset<8> byte;
                bitset<16> half;
                string t = "";
                while (i < line.size() and (line[i] == ' ' or line[i] == ','))
                {
                    i++;
                }
                if (line[i] == '#')
                    break;
                while (line[i] != ' ' and line[i] != ',')
                {
                    t += line[i];
                    i++;
                }
                half = stoi(t);
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        byte[k] = half[8 * j + k];
                    }
                    binToHexmem();
                    fout << memInHex << " " << binToHex(byte) << endl;
                    incrementMem();
                }
            }
            fout.close();
        }
        else if (directive == ".word")
        {
            ofstream fout;                         // to write to a file
            fout.open("output.mc", std::ios::app); // open output.mc to write
            while (i < line.size())
            {
                bitset<8> byte;
                bitset<32> word;
                string t = "";
                while (i < line.size() and (line[i] == ' ' or line[i] == ','))
                {
                    i++;
                }
                if (line[i] == '#')
                    break;
                while (line[i] != ' ' and line[i] != ',')
                {
                    t += line[i];
                    i++;
                }
                word = stoi(t);
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        byte[k] = word[8 * j + k];
                    }
                    binToHexmem();
                    fout << memInHex << " " << binToHex(byte) << endl;
                    incrementMem();
                }
            }
            fout.close();
        }
        else if (directive == ".dword")
        {
            ofstream fout;                         // to write to a file
            fout.open("output.mc", std::ios::app); // open output.mc to write
            while (i < line.size())
            {
                bitset<8> byte;
                bitset<64> dword;
                string t = "";
                while (i < line.size() and (line[i] == ' ' or line[i] == ','))
                {
                    i++;
                }
                if (line[i] == '#')
                    break;
                while (line[i] != ' ' and line[i] != ',')
                {
                    t += line[i];
                    i++;
                }
                dword = stoll(t);
                for (int j = 0; j < 8; j++)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        byte[k] = dword[8 * j + k];
                    }
                    binToHexmem();
                    fout << memInHex << " " << binToHex(byte) << endl;
                    incrementMem();
                }
            }
            fout.close();
        }
        else if (directive == ".asciz")
        {
            ofstream fout;                         // to write to a file
            fout.open("output.mc", std::ios::app); // open output.mc to write
            while (i < line.size())
            {
                bitset<8> byte;
                // bitset<16> half;
                string t = "";
                while (i < line.size() and line[i] != '\"')
                {
                    i++;
                }
                i++;
                while (i < line.size() and line[i] != '\"')
                {
                    t += line[i];
                    i++;
                }
                char ch = t[0];
                int z = 0;
                while (ch != '\0')
                {
                    z++;
                    byte = ch;
                    binToHexmem();
                    fout << memInHex << " " << binToHex(byte) << endl;
                    incrementMem();
                    ch = t[z];
                }
            }
            fout.close();
        }
    }

    bool checkDataDirective(string line)
    {
        int i = 0;
        while (line[i] == ' ')
            i++;
        string instruct = "";
        while (i < line.size() and line[i] != ' ')
        {
            instruct += line[i];
            i++;
        }
        if (instruct == ".data")
        {
            return true;
        }
        return false;
    }

    bool checkTextDirective(string line)
    {
        int i = 0;
        while (line[i] == ' ')
            i++;
        string instruct = "";
        while (i < line.size() and line[i] != ' ')
        {
            instruct += line[i];
            i++;
        }
        if (instruct == ".text")
        {
            return true;
        }
        return false;
    }

    // function to check if the instruction is right or not and which instruction is it
    string checkInstruction(string line)
    {
        int i = 0;
        while (line[i] == ' ')
            i++;
        // fetching instruction name in instruct
        string instruct = "";
        while (i < line.size() and line[i] != ' ')
        {
            instruct += line[i];
            i++;
        }

        if (instruct[0] == '#' or instruct == "\0")
        {
            return "comment";
        }

        else if (instruct == "add")
        {
            add *addInstruction = new add();                  // making an object of class add
            addInstruction->fetchInstructionDetails(line, i); // fetching fields
            addInstruction->convertTo32bit();                 // converting to 32 bit code
            addInstruction->binToHex();                       // converting to hex
            return addInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "and")
        {
            And *andInstruction = new And();                  // making an object of class And
            andInstruction->fetchInstructionDetails(line, i); // fetching fields
            andInstruction->convertTo32bit();                 // converting to 32 bit code
            andInstruction->binToHex();                       // converting to hex
            return andInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "or")
        {
            Or *newInstruction = new Or();                    // making an object of class Or
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "or")
        {
            Or *newInstruction = new Or();                    // making an object of class And
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "sll")
        {
            sll *newInstruction = new sll();                  // making an object of class sll
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "slt")
        {
            slt *newInstruction = new slt();                  // making an object of class slt
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "sra")
        {
            sra *newInstruction = new sra();                  // making an object of class sra
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "srl")
        {
            srl *newInstruction = new srl();                  // making an object of class srl
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "sub")
        {
            sub *newInstruction = new sub();                  // making an object of class sub
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "xor")
        {
            Xor *newInstruction = new Xor();                  // making an object of class Xor
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "mul")
        {
            mul *newInstruction = new mul();                  // making an object of class mul
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "div")
        {
            Div *newInstruction = new Div();                  // making an object of class Div
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "rem")
        {
            rem *newInstruction = new rem();                  // making an object of class rem
            newInstruction->fetchInstructionDetails(line, i); // fetching fields
            newInstruction->convertTo32bit();                 // converting to 32 bit code
            newInstruction->binToHex();                       // converting to hex
            return newInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "addi")
        {
            addi *addiInstruction = new addi();                     // making an object of class addi
            addiInstruction->fetchInstructionDetailsType1(line, i); // fetching fields
            addiInstruction->convertTo32bit();                      // converting to 32-bit code
            addiInstruction->binToHex();                            // converting to hex
            return addiInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "andi")
        {
            andi *andiInstruction = new andi();                     // making an object of class andi
            andiInstruction->fetchInstructionDetailsType1(line, i); // fetching fields
            andiInstruction->convertTo32bit();                      // converting to 32 bit code
            andiInstruction->binToHex();                            // converting to hex
            return andiInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "ori")
        {
            ori *oriInstruction = new ori();                       // making an object of class ori
            oriInstruction->fetchInstructionDetailsType1(line, i); // fetching fields
            oriInstruction->convertTo32bit();                      // converting to 32-bit code
            oriInstruction->binToHex();                            // converting to hex
            return oriInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "lb")
        {
            lb *lbInstruction = new lb();                         // making an object of class lb
            lbInstruction->fetchInstructionDetailsType2(line, i); // fetching fields
            lbInstruction->convertTo32bit();                      // converting to 32-bit code
            lbInstruction->binToHex();                            // converting to hex
            return lbInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "ld")
        {
            ld *ldInstruction = new ld();                         // making an object of class ld
            ldInstruction->fetchInstructionDetailsType2(line, i); // fetching fields
            ldInstruction->convertTo32bit();                      // converting to 32-bit code
            ldInstruction->binToHex();                            // converting to hex
            return ldInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "lh")
        {
            lh *lhInstruction = new lh();                         // making an object of class lh
            lhInstruction->fetchInstructionDetailsType2(line, i); // fetching fields
            lhInstruction->convertTo32bit();                      // converting to 32-bit code
            lhInstruction->binToHex();                            // converting to hex
            return lhInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "lw")
        {
            lw *lwInstruction = new lw();                         // making an object of class lw
            lwInstruction->fetchInstructionDetailsType2(line, i); // fetching fields
            lwInstruction->convertTo32bit();                      // converting to 32-bit code
            lwInstruction->binToHex();                            // converting to hex
            return lwInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "jalr")
        {
            jalr *jalrInstruction = new jalr();                     // making an object of class jalr
            jalrInstruction->fetchInstructionDetailsType2(line, i); // fetching fields
            jalrInstruction->convertTo32bit();                      // converting to 32-bit code
            jalrInstruction->binToHex();                            // converting to hex
            return jalrInstruction->instructionInHex;               // returning mc in hex
        }

        else if (instruct == "sb")
        {
            sb *sbInstruction = new sb();                    // making an object of class sb
            sbInstruction->fetchInstructionDetails(line, i); // fetching fields
            sbInstruction->convertTo32bit();                 // converting to 32-bit code
            sbInstruction->binToHex();                       // converting to hex
            return sbInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "sw")
        {
            sw *swInstruction = new sw();                    // making an object of class sw
            swInstruction->fetchInstructionDetails(line, i); // fetching fields
            swInstruction->convertTo32bit();                 // converting to 32-bit code
            swInstruction->binToHex();                       // converting to hex
            return swInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "sd")
        {
            sd *sdInstruction = new sd();                    // making an object of class sd
            sdInstruction->fetchInstructionDetails(line, i); // fetching fields
            sdInstruction->convertTo32bit();                 // converting to 32-bit code
            sdInstruction->binToHex();                       // converting to hex
            return sdInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "sh")
        {
            sh *shInstruction = new sh();                    // making an object of class sh
            shInstruction->fetchInstructionDetails(line, i); // fetching fields
            shInstruction->convertTo32bit();                 // converting to 32-bit code
            shInstruction->binToHex();                       // converting to hex
            return shInstruction->instructionInHex;          // returning mc in hex
        }

        else if (instruct == "beq")
        {
            beq *beqInstruction = new beq();                                                 // making an object of class beq
            beqInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // fetching fields
            beqInstruction->convertTo32bit();                                                // converting to 32-bit code
            beqInstruction->binToHex();                                                      // converting to hex
            return beqInstruction->instructionInHex;                                         // returning mc in hex
        }

        else if (instruct == "bne")
        {
            bne *bneInstruction = new bne();                                                 // Creating a new object of type bne
            bneInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            bneInstruction->convertTo32bit();                                                // Converting instruction to 32-bit code
            bneInstruction->binToHex();                                                      // Converting bin code to hex
            return bneInstruction->instructionInHex;                                         // Returning the instruction in hex format
        }

        else if (instruct == "bge")
        {
            bge *bgeInstruction = new bge();                                                 // Creating a new object of type bge
            bgeInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            bgeInstruction->convertTo32bit();                                                // Converting instruction to 32-bit code
            bgeInstruction->binToHex();                                                      // Converting bin code to hex
            return bgeInstruction->instructionInHex;                                         // Returning the instruction in hex format
        }

        else if (instruct == "blt")
        {
            blt *bltInstruction = new blt();                                                 // Creating a new object of type blt
            bltInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            bltInstruction->convertTo32bit();                                                // Converting instruction to 32-bit code
            bltInstruction->binToHex();                                                      // Converting bin code to hex
            return bltInstruction->instructionInHex;                                         // Returning the instruction in hex format
        }

        else if (instruct == "auipc")
        {
            auipc *auipcInstruction = new auipc();              // Creating a new object of type auipc
            auipcInstruction->fetchInstructionDetails(line, i); // Fetching instruction details
            auipcInstruction->convertTo32bit();                 // Converting instruction to 32-bit code
            auipcInstruction->binToHex();                       // Converting bin code to hex
            return auipcInstruction->instructionInHex;          // Returning the instruction in hex format
        }

        else if (instruct == "lui")
        {
            lui *luiInstruction = new lui();                  // Creating a new object of type lui
            luiInstruction->fetchInstructionDetails(line, i); // Fetching instruction details
            luiInstruction->convertTo32bit();                 // Converting instruction to 32-bit code
            luiInstruction->binToHex();                       // Converting bin code to hex
            return luiInstruction->instructionInHex;          // Returning the instruction in hex format
        }

        else if (instruct == "jal")
        {
            jal *jalInstruction = new jal();                                                 // Creating a new object of type jal
            jalInstruction->fetchInstructionDetails(line, i, labelMap, this->pc.to_ulong()); // Fetching instruction details
            jalInstruction->convertTo32bit();                                                // Converting instruction to 32-bit code
            jalInstruction->binToHex();                                                      // Converting bin code to hex
            return jalInstruction->instructionInHex;                                         // Returning the instruction in hex format
        }

        else
            return "error"; // return error if instruction doesnot match to any name
    }

    // function to read from .asm file and write to .mc file
    void readAndWrite()
    {
        ifstream fin;  // to read a file
        ofstream fout; // to write to a file
        string line;

        fout.open("output.mc");
        fout.close();

        fin.open("input.asm"); // open input.asm to read
        // checking all labels only and storing them
        while (getline(fin, line))
        {
            if (checkDataDirective(line))
            {
                while (!checkTextDirective(line) and !EOF)
                {
                    getline(fin, line);
                    if (checkDirectives(line))
                        workOnDirectives(line);
                    else if (checkLabel(line))
                        makeDataLabel(line);
                }
            }
            else if (checkDirectives(line))
            {
                workOnDirectives(line);
            }
            else if (checkTextDirective(line))
            {
                continue;
            }
            else if (checkLabel(line))
            {
                makeLabel(line);
            }
            else if (checkInstruction(line) == "error" or checkInstruction(line) == "comment")
            {
                continue;
            }
            else
                incrementPC(this->pc);
        }
        fin.close(); // close input.asm

        this->pc = 0;

        fin.open("input.asm");                 // open input.asm to read
        fout.open("output.mc", std::ios::app); // open output.mc to write
        fout << endl;
        if (!fout)
        {
            cout << "error";
        }
        // checking instructions only (not labels)
        while (getline(fin, line))
        {
            if (checkInstruction(line) == "comment")
            {
                continue;
            }
            else if (checkDataDirective(line))
            {
                while (!checkTextDirective(line) and !EOF)
                {
                    getline(fin, line);
                }
            }
            else if (checkDirectives(line))
            {
                continue;
            }
            else if (checkTextDirective(line))
            {
                continue;
            }
            else if (checkLabel(line))
            {
                int i = 0;
                while (line[i] != ':')
                    i++;
                i++;
                string subLine = line.substr(i);

                if (checkInstruction(subLine) != "error" and checkInstruction(subLine) != "comment")
                {
                    binToHexPC();
                    fout << pcInHex << " " << checkInstruction(subLine) << endl; // writing to output.mc
                    incrementPC(this->pc);
                }
            }
            else if (!checkLabel(line))
            {
                binToHexPC();
                fout << pcInHex << " " << checkInstruction(line) << endl; // writing to output.mc
                incrementPC(this->pc);
            }
        }
        fin.close();  // close input.asm
        fout.close(); // close output.mc
    }
};

int main()
{
    makeMC *newMC = new makeMC(); // making object for makeMC class
    newMC->readAndWrite();        // makes output.mc taking inputs from input.asm
    return 0;
}

