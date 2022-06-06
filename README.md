# CMSC-411-Project
Zee Khuzaima Mustanzir, Devon Slonaker, Bryan Nguyen

1. Run "make" to create the simulator object file
2. Run "./simulator sample_test.txt data.txt output.txt" to run the simulator file with the assembly code, memory binary, and desired output file destination name respectively.

If you encounter a combined number on line 1 under the memory cycle (such as 89) with nothing in the write back cycle, please rerun the program or repoen the output file as this is a visual error regarding the tab escape character.
using cat to view output.txt seems to give the best results in typesetting

This project follows the directions provided in lecture 18. 
The stalls that occur during a stage are diaplayed under that stage rather than being added to the cycles of the next stage
There is a bit of branch prediction happening in our simulator. We always start the next instruction after the branch and if the branch is taken, we simply mark that instruction as having completed without having done anything and then jump. This results in the instruction after branch being always displayed in the output, but it does not affect the actual cycles and, therefore, the number of cycles taken by each stage of each instruction remains correct.
