#pragma once

#include <string>
#include <iostream>
#include <sstream>

using namespace std;
//#ifndef INSTRUCTION_H
//#define INSTRUCTION_H
int regs[32];//I printed it and it looks like it is filled with 0's already
int memory[32];

//takes in a string and returns the value stored in athe appropriate location 
//corresponding that string in regs[]
bool isNotBranch(string opcode){
    if (opcode == "bne")
        return false;
    if (opcode == "beq")
        return false;
    if (opcode == "j")
        return false;
    return true;
}
bool isBranch(string opcode){
    if (opcode == "bne")
        return true;
    if (opcode == "beq")
        return true;
    if (opcode == "j")
        return true;
    return false;
}
int getRegValue(string regName){
    //erase the first char, e.g. "r32" becomes "32"
    regName.erase(0, 1);
    //r1 will correspond to regs[0]
    int index = atoi(regName.c_str());
    return regs[index];
    //char rName[regName.length()];
    //strcpy(rName, regName);
    //printf("The string after removing the first char is: %s\n", regName);
}
void setRegValue(string regName, int value){
    regName.erase(0,1);
    int index = atoi(regName.c_str());
    regs[index] = value;
}
/*
int getMemory(string address){
    int index = (stoi(address, nullptr, 16)-256)/4;
    return memory[index];
}
void setMemory(string addressAndOffset, int value){
    int index = (stoi(addressAndOffset, nullptr, 16)-256)/4;
    memory[index] = value;
}
*/
class Intermediate_register{
    public:
        int inst_index;
        string opcode, rs, rt, rd;
        Intermediate_register(){
            inst_index = -1;
            opcode = rs = rt = rd = "";
        }
};
class Instruction{
    public:
        string label, opcode, rs, rt, rd;
        bool has_started, has_completed, has_data_hazard;
        int state, nearest_data_hazard;
        //keep track of the cycle at which each stage was completed
        int cycles[7];
        //used to store the offset of load and store instructions
        int offset;
        Instruction(){
            label = opcode = rs = rt = rd = "";
            has_started = has_completed = has_data_hazard = 0;
            //-1 means not started
            state = -1;
            nearest_data_hazard = -1;
            for (int i=0; i<7; i++)
                cycles[i] = 0;
            offset = -1;

        }
        void execute(){
            if(opcode == "lw"){
                //rs = "0(r2)"
                int address = getRegValue(rs)+offset;
                int index = (address-256)/4;
                int value = memory[index];
                setRegValue(rd, value);
            }
            if(opcode == "sw"){
                int address = getRegValue(rs)+offset;
                int index = (address-256)/4;
                memory[index] = getRegValue(rd);
            }
            if (opcode == "li"){
                int immValue = stoi(rs, nullptr, 16);
                setRegValue(rd, immValue);
            }
            //////////////////////////////////////////////////
            //Arithmetic Instruction:
            ///////////////////////////////////////////////////
            if (opcode == "add"){
                int p = getRegValue(rs);
                int q = getRegValue(rt);
                int result = p+q;
                setRegValue(rd, result);
            }
            if(opcode == "addi"){
                int p = getRegValue(rs);
                int q = stoi(rt, nullptr, 16);
                int result = p+q;
                setRegValue(rd, result);
            }
            if (opcode == "mult"){
                int p = getRegValue(rs);
                int q = getRegValue(rt);
                int result = p*q;
                setRegValue(rd, result);
            }
            if (opcode == "multi"){
                int p = getRegValue(rs);
                int q = stoi(rt, nullptr, 16);
                int result = p*q;
                setRegValue(rd, result);               
            }
            if (opcode == "sub"){
                int p = getRegValue(rs);
                int q = getRegValue(rt);
                int result = p-q;
                setRegValue(rd, result);               
            }
            if (opcode == "subi"){
                int p = getRegValue(rs);
                int q = stoi(rt, nullptr, 16);
                int result = p-q;
                setRegValue(rd, result);               
            }
            if(opcode == "hlt"){
                //do nothing
            }
        }
        bool executeBranch(){
            //return false;
            if(opcode == "beq"){
                int p = getRegValue(rd);
                int q = getRegValue(rs);
                if (p==q)
                    return true;
                else
                    return false;
            }
            if(opcode == "bne"){
                int p = getRegValue(rd);
                int q = getRegValue(rs);
                if (p!=q)
                    return true;
                else
                    return false;
            }
            //otherwise/in-case-of-j
            return true;
        }
        string printCycles(){
        string output = "";

         output.append(label + "\t");
                cout << label << "\t";
                output.append(opcode+ "\t" );
                cout << opcode << "\t";
                output.append(rd+ "\t");
                cout << rd << "\t";
                output.append(rs+ "\t");
                cout << rs << "\t";
                output.append(rt+ "\t");
                cout << rt << "\t";
                for (int i=0; i<7; i++)
                {
                    if (cycles[i] != 0)
                    {
                    	output.append(to_string(cycles[i])+"\t");
                    	
                        cout << cycles[i] << "\t";
                        }
                
                    else
                    {
                    	output.append("\t");
                        cout << "\t";
                    }
                 }
                output.append("\n");
                cout << endl;
                
                //cout << output;
                return output;
        }
        //for debugging
        void print(){
            cout << label << "\t"
                << opcode << "\t"
                << rd << "\t"
                << rs << "\t"
                << rt << "\t"
                << endl;
        }
};

//#endif
