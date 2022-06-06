#include "Instruction.h"
#include "parse.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]){

    Instruction instructions[20];
    Instruction running_instructions[100];
    enum State {not_started = -1, inst_fetch, inst_decode,
                exec1, exec2, exec3, mem, wr_back, complete};
    //keep track of whether a stage is in use
    bool stageFlags[7];
    
    Intermediate_register stageRegs[6];
    
    int program_counter = 0;
    //do not understand where to use this
    int running_instruction_index = 0;
    //popualte the instructions[]
    //the first cmd argument would be the instructions file
    parseInstructions(argv[1], instructions);
    parseData(argv[2]);

    //used inside the code
    Instruction cur_inst;
    int cur_state;
    string cur_opcode;
    bool halt = 0;
    bool brake = 0;
    //initialize all stageFlags to 0 (0 means available)
    for (int i=0; i<7;i++)
        stageFlags[i] = 0;
 
    for(int cycle=1; cycle<100; cycle++){
        //cout << "---------------------------------------------------------------------------------------\n";
        printf("\nCycle: %d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", cycle);
        //break after hlt's decode stage
        /*
        if(brake == 1){
            cout << "break is triggered!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            break;
        }
        */
        //start instructions that can be started, one at a time
        
        if(stageFlags[0] == 0 &&//IF stage not in use
           (stageRegs[0].opcode != "beq" || //make sure the previous instruction 
            stageRegs[0].opcode != "bne" || //is not a branch instruction
            stageRegs[0].opcode != "j")
            )// &&halt == 0)
            {//do not start any more instructions if halt = 1
                printf("inside the start instruction if\n");
                //printf("stageFlag[0] = %d\n", stageFlags[0]);
                //printf("running_instruction_index = %d\n", running_instruction_index);
                //printf("program counter = %d\n", program_counter);
                
                stageFlags[0] = 1;
                printf("instructions[program_counter].opcode = %s\n", instructions[program_counter]);
                running_instructions[running_instruction_index] = instructions[program_counter];
                //printf("running_instructions[running_instruction_index].opcode = %s\n", running_instructions[running_instruction_index].opcode);
                running_instructions[running_instruction_index].has_started = 1;
                running_instructions[running_instruction_index].state++;
                //hlt is supposed to end in decode and also wait for the last instruction to finish
                //thats why we make its nearest_data_hazard = last running instruction's index
                if (running_instructions[running_instruction_index].opcode == "hlt"){
                    running_instructions[running_instruction_index].has_data_hazard = 1;
                    running_instructions[running_instruction_index].nearest_data_hazard = running_instruction_index-1;
                }
                running_instruction_index++;
                program_counter++;
        }//endif
        //printf("After the start instruction if\ngoing into intermediary registers for loop\n");
        //Execute the running instructions, fill in intermdiate registers
        for(int inst_ptr=0; inst_ptr<100; inst_ptr++){
            //printf("cycle: %d, iteration: %d\n", cycle, inst_ptr);
            //go throught all instructions in the running_instructions[]
            //check if there is an instruction at this index
            //Instruction inilializes all string members to ""
            //so if opcode is "" that means we have yet to put an instruction here
            //also make sure the instruction is yet to be completed
            if(running_instructions[inst_ptr].opcode != "" &&
               running_instructions[inst_ptr].has_completed == 0){
                
                cur_inst = running_instructions[inst_ptr];
                cur_state = cur_inst.state;
                //check if the instruction is either not_completed or in WB
                if (cur_state <=5){
                    //populate the stageRegs
                    //stores the index in the running_intructions[]
                    stageRegs[cur_state].inst_index = inst_ptr;
                    stageRegs[cur_state].opcode = cur_inst.opcode;
                    stageRegs[cur_state].rs = cur_inst.rs;
                    stageRegs[cur_state].rt = cur_inst.rt;
                    stageRegs[cur_state].rd = cur_inst.rd;
                }//endif

            }//endif

        }//endfor--populating the intermediary registers
        cout << "printing the state registers:\n";
        for (int i=0; i<6; i++)
            cout << "stateRegs[" << i << "].opcode = " << stageRegs[i].opcode << endl;
        //printf("after the populate stationary registers for loop\n");
        //printf("going into the data hazard for loop\n");
        
        //determine data hazards in IF stage
        cout << "Before the hazard determination code block\n";
        for (int stageReg_index=6; stageReg_index>=1; stageReg_index--){
            //printf("inside data hazard for loop number: %d\n", stageReg_index);
            if(stageRegs[stageReg_index].rd != ""){
                if(stageRegs[0].rs == stageRegs[stageReg_index].rd ||
                    stageRegs[0].rt == stageRegs[stageReg_index].rd){
                        int index = stageRegs[0].inst_index;
                        int hazard_index = stageRegs[stageReg_index].inst_index;
                        running_instructions[index].has_data_hazard = 1;
                        running_instructions[index].nearest_data_hazard = hazard_index;
                        cout << "Inside the determine data hazards block\n"
                            << "running_instruction[index].opcode = " << running_instructions[index].opcode
                            << "\nhazard_index = " << hazard_index
                            << "\n nearest data hazard's opcode = " << running_instructions[hazard_index].opcode
                            <<endl;
                }//endif
                //in case of branches
                if(stageRegs[0].opcode == "bne" ||
                        stageRegs[0].opcode == "beq" ||
                        stageRegs[0].opcode == "j"){
                    if(stageRegs[0].rd == stageRegs[stageReg_index].rd ||
                        stageRegs[0].rs == stageRegs[stageReg_index].rd){
                            int index = stageRegs[0].inst_index;
                            int hazard_index = stageRegs[stageReg_index].inst_index;
                            running_instructions[index].has_data_hazard = 1;
                            running_instructions[index].nearest_data_hazard = hazard_index;
                            cout << "Inside the determine data hazards block\n"
                                << "running_instruction[index].opcode = " << running_instructions[index].opcode
                                << "\nhazard_index = " << hazard_index
                                << "\n nearest data hazard's opcode = " << running_instructions[hazard_index].opcode
                                <<endl;
                    }//endif
                }//endif--branches
            }//endif
        }//endfor--datahazards
        cout << "Entering move-to-next-stage for-loop\n";
        //move running instructions to the next stage
        for (int inst_ptr=0; inst_ptr<100; inst_ptr++){
            if(running_instructions[inst_ptr].opcode != "" &&
               running_instructions[inst_ptr].has_completed == 0){
                cur_inst = running_instructions[inst_ptr];
                cur_state = cur_inst.state;
                cur_opcode = running_instructions[inst_ptr].opcode;
                int is_blocking = 0;
                cout << "\nLoop number: " << inst_ptr << "*********************************\n";
                printf("inside the first if statement\n");
                cout << "cur_inst.opcode = " << cur_inst.opcode
                    << ", cur_state = " << cur_state << endl;
                //if in decode and the nearest_data_hazard has not been resolved
                cout << "cur_inst.nearest_data_hazard = "
                    << cur_inst.nearest_data_hazard << endl;
                if (cur_inst.nearest_data_hazard > -1){
                    if (cur_state == inst_decode &&
                        running_instructions[cur_inst.nearest_data_hazard].has_completed != 1){
                            cout << "Inside the hazard checking if statement\n";
                            is_blocking = 1;
                    }//endif
                }//endif
                //in case of hlt
                if (cur_inst.opcode == "hlt" && cur_state == inst_fetch){
                    //we set halt = 1 in instruction fetch stage so that it will stop
                    //anymore tries to start new instructions
                    halt = 1;
                }
                
                cout << "is_blocking = " << is_blocking << endl;
                if (cur_state <=5 && is_blocking == 0){
                    cout << "stageFlags [cur_state+1] = " << stageFlags[cur_state+1] << endl;
                    if(stageFlags[cur_state+1] == 0){
                        bool hlt_branch_block = 0;
                        //if we have a branch instruction
                        if (cur_opcode == "beq" || 
                            cur_opcode == "bne" || cur_opcode == "j"){
                            if(cur_state == inst_decode){
                                //if the branch is taken
                                if (running_instructions[inst_ptr].executeBranch()){
                                    //find the label
                                    int labelIndex;
                                    for(labelIndex=0; labelIndex<20; labelIndex++){
                                        if (instructions[labelIndex].label == cur_inst.rt)
                                            break;
                                    }
                                    //update the program counter
                                    running_instructions[running_instruction_index-1].has_completed = 1;
                                    stageFlags[0] = 0;
                                    //stageRegs[0]
                                    program_counter = labelIndex;
                                }
                            }//endif--ID
                            //branches end in exec1 stage
                            else if(cur_state == exec1){
                                running_instructions[inst_ptr].has_completed = 1;
                                hlt_branch_block = 1;
                            }
                        }//endif--brnach instructions
                        //in case of hlt
                        if(cur_inst.opcode == "hlt" && cur_state == inst_decode){
                            running_instructions[inst_ptr].has_completed = 1;
                            hlt_branch_block = 1;
                            brake = 1;
                        }
                        //only acquire the next stage mutex if the current instruction is
                        //not a branch in exec1 or a hlt in decode
                        if(cur_state == mem){
                            //if in mem add 2 to the cycle
                            cycle += 2;
                        }
                        if (hlt_branch_block == 0)
                            stageFlags[cur_state+1] = 1;
                        stageFlags[cur_state] = 0;
                        running_instructions[inst_ptr].state++;
                        //store the # of cycles for this stage
                        running_instructions[inst_ptr].cycles[cur_state] = cycle;
                        printf("Moving to next stateNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n");
                        cout << "running_instructions[inst_ptr].state = " <<running_instructions[inst_ptr].state <<endl;
                        cout << "cur_state = " << cur_state << endl;
                        cout << "running_instructions[inst_ptr].cycles[cur_state] = "
                            << running_instructions[inst_ptr].cycles[cur_state] << endl;
                    }//endif
                }//endif
                else if (cur_state == wr_back){
                    cout << "WR of running instruction at index : " << inst_ptr
                        << ", WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
                    stageFlags[cur_state] = 0;
                    running_instructions[inst_ptr].has_completed = 1;
                    running_instructions[inst_ptr].cycles[cur_state] = cycle;
                    cout << "calling the execute function\n";
                    running_instructions[inst_ptr].execute();
                }
            }//endif--
        }//endfor--moving to next stage
    cout << "-------------------------------------------------------------------------------------------\n";
    }//endfor--cycles

    /*
    if (instructions[0].label == "")
        cout << "label is \"\"\n";
    //cout << state << endl;
    //prints out the contents of instructions[]
    */
    /*
    for (int i=0; i<20; i++){
        instructions[i].printCycles();
    }
    */
    ofstream myFile;
    myFile.open(argv[3]);
    myFile << "Cycle Number for each state\t\tIF\tID\tEX1\tEX2\tEX3\tMEM\tWB\n";
    cout << "Cycle Number for each state\t\tIF\tID\tEX1\tEX2\tEX3\tMEM\tWB\n";
    for (int i=0; i<100; i++){
        if (running_instructions[i].opcode != "")
            myFile<< running_instructions[i].printCycles();
        //if (isBranch(running_instructions[i].opcode))
            //i++;
    }
    

    /*
    cout << stageRegs[0].opcode << endl;
    int n = getRegValue("r32");
    string s = "r32";
    s.erase(0,1);
    cout << s << endl;
    cout << "The integer converted from r32 is: " <<  n << endl;
    for(int i=0; i<32; i++)
        cout << regs[i]<<endl;
    setRegValue("r32", 20);
    cout << "Should be 20: "<<getRegValue("r32")<<endl;
    */
   /*
   for(int i=0; i<32; i++)
        cout << memory[i] << endl;
    cout << "converting 100h to int: " << stoi("100h", nullptr, 16) << endl;
    setMemory("100h", 200);
    cout << "accessing memory at 100h, should be 200: "
        << getMemory("100h") << endl;
        */
    cout << "instructions[1].rs = " << instructions[1].rs
        << " instructions[1].offset = " << instructions[1].offset << endl;
    return 1;
}
