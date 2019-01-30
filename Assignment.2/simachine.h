#ifndef SIMACHINE_H
#define SIMACHINE_H

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>

using namespace std;

class simachine
{
    public:
        simachine();
        ~simachine();
        void loader(fstream& source);

    private:

        string commands[20] = {"ADD", "add", "MUL", "mul", "LE", "le", "NEG",
        "neg", "JMP0", "jmp0", "ASS", "ass", "JMP", "jmp", "READ", "read",
        "WRITE", "write", "HALT", "halt"};

        struct instruction {
            int opcode;
            int arg1;
            int arg2;
            int arg3;
            bool arg1_a;
            bool arg2_a;
        };

        int DM[1024];
        instruction IM[1024];
        int pc;
        int last_instr = 0;
        int last_data = 1;
        int linenum = 0;
        int inum = -1;

        void data_loader(fstream &source);
        void instr_loader(fstream &source);

        string trim(string x);
        void adr_error(string com);

        void machine();
        void cat1(string com, string instruction, int space);
        void cat2(string com, string instruction, int space);
        void cat3(string com, string instruction, int space);

        void add(int index);
        void neg(int index);
        void mul(int index);
        void jmp(int index);
        void jmp0(int index);
        void ass(int index);
        void le(int index);
        void read(int index);
        void write(int index);
        void halt(int index);

};

#endif // SIMACHINE_H
