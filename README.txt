To compile:
-cd into the directory containing the extracted files (schedMain.cpp, schedulers.h, schedulers.cpp)
-enter: g++ schedMain.cpp schedulers.cpp -o sched

To run:
-enter: sched <input file>
-if an input file is not specified, the program will try to read in from "./procList.txt". Otherwise
 it will attempt to read the file specified