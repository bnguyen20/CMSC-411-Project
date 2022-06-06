#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bits/stdc++.h>
#include <sstream>
#include "Instruction.h"

using namespace std;

string remove_spaces(string input)
{
  string newString;
  int i = 0;

  // add each character to the newString
  while(input[i])
  {
    if(input[i] != ' ')
    {
      newString += input[i];
    }
    i++;
  }
  return newString;
}

string remove_colon(string input)
{
  string newString;
  int i = 0;

  // add each character to the newString
  while(input[i])
  {
    if(input[i] != ':')
    {
      newString += input[i];
    }
    i++;
  }
  return newString;
}

void parseInstructions (char *fileName, Instruction instructions[])
{
  // open sample_test.txt
  ifstream myFile;
  myFile.open(fileName);

  if(myFile){
    string label;
    string inst;
    // all three inputs
    string inp;
    // 3 strings below are parsed arguments
    string inp1;
    string inp2;
    string inp3;

    // file will be stored into buffer and we will parse it
    string buffer;

    // max number of arguments we can have in an instruction
    string inputs[3];

    //iterator for instructions[]
    int inst_num = 0;
    while(getline(myFile, buffer, '|')){
        label = "";
        inst = "";
        inp = "";
        for(int i = 0; i < 3; i++)
        {
            inputs[i] = "";
        }

        // get the label
        label = buffer;
        label = remove_spaces(label);
        label = remove_colon(label);

        // get the instruction
        getline(myFile, buffer, '|');
        inst = buffer;
        inst = remove_spaces(inst);

        // get the inputs
        getline(myFile, buffer);
        inp = buffer;
        inp = remove_spaces(inp);


        int i = 0;
        stringstream ss(inp);
        while(getline(ss, inputs[i], ','))
        {
            i++;
        }
        inp1 = inputs[0];
        inp2 = inputs[1];
        inp3 = inputs[2];
        
        //transform all to lower case
        transform(label.begin(), label.end(), label.begin(), ::tolower);
        transform(inst.begin(), inst.end(), inst.begin(), ::tolower);
        transform(inp1.begin(), inp1.end(), inp1.begin(), ::tolower);
        transform(inp2.begin(), inp2.end(), inp2.begin(), ::tolower);
        transform(inp3.begin(), inp3.end(), inp3.begin(), ::tolower);
        //put the parsed intruction into the array of instructions
        int ofs = -1;
        if (inst == "lw" || inst == "sw"){
                stringstream ss(inp2);
                string buffer;
                getline(ss, buffer, '(');
                ofs = stoi(buffer);
                getline(ss, buffer, ')');
                inp2 = buffer;
        }
        instructions[inst_num].label = label;
        instructions[inst_num].opcode = inst;
        instructions[inst_num].rd = inp1;
        instructions[inst_num].rs = inp2;
        instructions[inst_num].rt = inp3;
        instructions[inst_num].offset = ofs;
        inst_num++;
        
        }//end while
    }//end if
    else
    {
        cout << "poop" << endl;
    }


}//end parseInstruction

void parseData(string fileName){
  ifstream myFile;
  myFile.open(fileName);
  string buffer;
  int counter = 0;
  while(getline(myFile, buffer))
  {
  //cout << buffer << endl;
  memory[counter] = stoi(buffer, nullptr, 2);
  counter++;
  }
}//end parseData
