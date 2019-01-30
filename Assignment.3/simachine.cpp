#include "simachine.h"

simachine::simachine() {
    for (int i = 0; i < 1024; i++) {
        for (int m = 0; m < 4; m++) {
            IM[m][i].opcode = -1;
            IM[m][i].arg1 = -1;
            IM[m][i].arg2 = -1;
            IM[m][i].arg3 = -1;
            IM[m][i].arg1_a = -1;
            IM[m][i].arg2_a = -1;
        }
        DM[i] = -1;
    }
}

simachine::~simachine() {
    //delete[] IM;
    //delete[] DM;
}

string simachine::trim(string line) {
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '\t' || line[i] == '\n' || line[i] == '\s'
                || line[i] == '\r') {
            try{
                line.erase(i, 1);
            } catch(const std::out_of_range& err) {}
        }
    }
    return line;
}

void simachine::adr_error (string com) {
    printf("\nIncorrect %s instruction at line %i, out must be an address preceded by %", com.c_str(), linenum);
}

void simachine::data_loader (fstream& source) {

    string line;
    bool data = false;
    int mark, mark2, content, address;

    source.clear();
    source.seekg(0, source.beg);
    linenum = 0;

    while(!source.eof() && line.find(".text") == std::string::npos) {
        getline(source, line);
        linenum += 1;

        if (line.find(".data") != std::string::npos) {
            data = true;
            continue;
        }
        if (line.find(".text") != std::string::npos) {
            data = false;
            return;
        }
        if (data) {
            line = trim(line);
            try{
                if (line.find("%") != std::string::npos) {
                    mark = line.find("%") +1;
                    mark2 = line.find(":=") + 2;
                    address = stoi(line.substr(mark, line.find(":=") - mark), nullptr, 10);
                    content = stoi(line.substr(mark2, line.length() - mark2), nullptr, 10);
                }
            } catch(const std::invalid_argument& err) {
            } catch(const std::out_of_range& ai) {}
        }
        DM[address] = content;
    }
}

int simachine::loader (fstream& source) {


    lock_guard<mutex> lck(mtx);
    string line;
    bool dcorrect = false;
    bool tcorrect = false;

    source.clear();
    source.seekg(0, source.beg);
    linenum = 0;
    inum = -1;

    while (!source.eof()) {
        getline(source, line);
        linenum += 1;
        if (line.find(".data") != std::string::npos) {
            dcorrect = true;
            continue;
        } else if (line.find(".text") != std::string::npos) {
            tcorrect = true;
            continue;
        }
    }
    if (source.eof() && !dcorrect && !tcorrect) {
        printf("\nIncorrect file structure.");
    }

    source.clear();
    source.seekg(0, source.beg);
    linenum = 0;

    while (!source.eof()) {
        getline(source, line);
        linenum += 1;
        if (line.find(".data") != std::string::npos) {
            data_loader(source);
            break;
        }
    }
    source.clear();
    source.seekg(0, source.beg);
    linenum = 0;

    while(!source.eof()) {
        getline(source,line);
        linenum += 1;
        if (line.find(".text") != std::string::npos) {
            instr_loader(source);
        }
    }
    return 0;
}

void simachine::instr_loader (fstream& source){

    string instruction;
    string com;

    while (!source.eof()) {

        instruction = "";

        getline(source, instruction);
        linenum += 1;

        bool exist = false;

        for (int i = 0; i < 20; i++) {
            if (instruction.find(commands[i]) != std::string::npos) {
                exist = true;
                inum += 1;
                break;
            }
        }

        if (!exist) {
            break;
        }

        int start = instruction.find_first_not_of("\n\r\s\t");
        int space = int(instruction.find(" ", start));

        try {
            com = instruction.substr(start, space-1);
        } catch(const std::out_of_range& err) {}

        com = trim(com);

        if (com == "ADD" || com == "add" || com == "MUL" ||
            com == "mul" || com == "LE" || com == "le") {

            cat1 (com, instruction, space);

        } else if (com == "NEG" || com == "neg" || com == "JMP0" ||
                com == "jmp0" || com == "ASS" || com == "ass") {
            cat2 (com, instruction, space);
        }
        else if (com == "JMP" || com == "jmp" || com == "READ" ||
            com == "read" || com == "WRITE" || com == "write" ||
            com == "HALT" || com == "halt") {

            cat3 (com, instruction, space);

        }
    }
}

void simachine::cat1 (string com, string instruction, int space) {
    string arg1, arg2, arg3;
    int argu1, argu2, argu3;
    int comma, bcomma;
    bool arg1_a, arg2_a;

    comma = int(instruction.find(",", space));
    try {
        arg1 = instruction.substr(space+1, comma-space-1);
    } catch(const std::out_of_range& err) {}
    bcomma = comma +1;

    comma = int(instruction.find(",", bcomma));
    try {
        arg2 = instruction.substr(bcomma+1, comma-bcomma-1);
    } catch(const std::out_of_range& err) {}

    bcomma = comma +1;

    comma = int(instruction.find(",", bcomma));
    try {
        arg3 = instruction.substr(bcomma+1, comma-bcomma-1);
    } catch(const std::out_of_range& err) {}

    try {
        if (arg1.find("%") == std::string::npos) {
            arg1_a = false;
        } else {
            arg1_a = true;
            arg1.erase(arg1.find("%"), 1);
        }
        if (arg2.find("%") == std::string::npos) {
            arg2_a = false;
        } else {
            arg2_a = true;
            arg2.erase(arg2.find("%"), 1);
        }
        if (arg3.find("%") != std::string::npos) {
            arg3.erase(arg3.find("%"), 1);
        } else {
            printf("\nIncorrect %s instruction at line %i. out must be address.", com.c_str(), linenum);
            exit(1);
        }
    } catch(const std::out_of_range& err) {}

    try {
        argu1 = stoi(arg1); argu2 = stoi(arg2); argu3 = stoi(arg3);
    } catch (const exception& err) {}

    IM[m_num][inum].arg1 = argu1;
    IM[m_num][inum].arg2 = argu2;
    IM[m_num][inum].arg3 = argu3;
    IM[m_num][inum].arg1_a = arg1_a;
    IM[m_num][inum].arg2_a = arg2_a;

    if (com == "ADD" || com == "add") {
        IM[m_num][inum].opcode = 1;
    } else if (com == "MUL" || com == "mul") {
        IM[m_num][inum].opcode = 2;
    } else if (com == "LE" || com == "le") {
        IM[m_num][inum].opcode = 3;
    }
}

void simachine::cat2 (string com, string instruction, int space) {
    string arg1, arg2;
    int argu1, argu2;
    int comma, bcomma;
    bool arg1_a;

    comma = int(instruction.find(",", space));
    try {
        arg1 = instruction.substr(space+1, comma-space-1);
    } catch(const std::out_of_range& err) {}

    bcomma = comma +1;

    comma = int(instruction.find(",", bcomma));
    try {
        arg2 = instruction.substr(bcomma+1, comma-bcomma-1);
    } catch(const std::out_of_range& err) {}


    try {
        if (arg1.find("%") == std::string::npos) {
            arg1_a = false;
        } else {
            arg1_a = true;
            arg1.erase(arg1.find("%"), 1);
        }
        if (arg2.find("%") != std::string::npos) {
            arg2.erase(arg2.find("%"), 1);
        } else {
            printf("\nIncorrect %s instruction at line %i. out must be address.", com.c_str(), linenum);
            exit(1);
        }
    } catch(const std::out_of_range& err) {}

    try {
        argu1 = stoi(arg1); argu2 = stoi(arg2);
    } catch (const invalid_argument& err) {}

    IM[m_num][inum].arg1 = argu1;
    IM[m_num][inum].arg2 = argu2;
    IM[m_num][inum].arg1_a = arg1_a;

    if (com == "NEG" || com == "neg") {
        IM[m_num][inum].opcode = 4;
    } else if (com == "JMP0" || com == "jmp0") {
        IM[m_num][inum].opcode = 5;
    } else if (com == "ASS" || com == "ass") {
        IM[m_num][inum].opcode = 6;
    }
}

void simachine::cat3 (string com, string instruction, int space) {
    string arg1;
    int argu1;
    int comma, bcomma;
    bool arg1_a = false;

    comma = int(instruction.find(",", space));
    try {
        arg1 = instruction.substr(space+1, comma-space-1);
    } catch(const std::out_of_range& err) {}

    arg1 = trim(arg1);

    try {
        if (arg1.find("%") != std::string::npos) {
            arg1_a = true;
            arg1.erase(arg1.find("%"), 1);
        } else if (com != "WRITE" && com !="write" && com != "HALT" && com != "halt"){
            printf("\nIncorrect %s instruction at line %i. out must be address.", com.c_str(), linenum);
            exit(1);
        }
    } catch(const std::out_of_range& err) {}

    try {
        argu1 = stoi(arg1);
    } catch (const invalid_argument& err) {}

    IM[m_num][inum].arg1 = argu1;

    if (com == "JMP" || com == "jmp") {
        IM[m_num][inum].opcode = 7;
    } else if (com == "READ" || com == "read") {
        IM[m_num][inum].opcode = 8;
    } else if (com == "WRITE" || com == "write") {
        IM[m_num][inum].opcode = 9;
    } else if (com == "HALT" || com == "halt") {
        IM[m_num][inum].opcode = 10;
    }
}

void simachine::machine () {

    lock_guard<mutex> lck(mtx);
    int code;

    for (pc = 0; IM[m_num][pc].opcode != -1; pc +=1) {
        code = IM[m_num][pc].opcode;
        switch (code) {
            case 1:
                add (pc);
                break;
            case 2:
                mul (pc);
                break;
            case 3:
                le (pc);
                break;
            case 4:
                neg (pc);
                break;
            case 5:
                jmp0 (pc);
                break;
            case 6:
                ass (pc);
                break;
            case 7:
                jmp (pc);
                break;
            case 8:
                read (pc);
                break;
            case 9:
                write (pc);
                break;
            case 10 :
                halt (pc);
                return;
                break;
            default:
                cerr << "\nOperation code not listed";
                exit(1);
                break;
        }
    }
}

void simachine::add(int index) {
    int in1, in2;
    if (IM[m_num][index].arg1_a) in1 = DM[IM[m_num][index].arg1];
    else in1 = IM[m_num][index].arg1;
    if (IM[m_num][index].arg2_a) in2 = DM[IM[m_num][index].arg2];
    else in2 = IM[m_num][index].arg2;
    DM[IM[m_num][index].arg3] = in1 + in2;
}

void simachine::mul(int index) {
    int in1, in2;
    if (IM[m_num][index].arg1_a) in1 = DM[IM[m_num][index].arg1];
    else in1 = IM[m_num][index].arg1;
    if (IM[m_num][index].arg2_a) in2 = DM[IM[m_num][index].arg1];
    else in2 = IM[m_num][index].arg2;
    DM[IM[m_num][index].arg3] = in1 * in2;
}

void simachine::le(int index) {
    int in1, in2;
    if (IM[m_num][index].arg1_a) in1 = DM[IM[m_num][index].arg1];
    else in1 = IM[m_num][index].arg1;
    if (IM[m_num][index].arg2_a) in2 = DM[IM[m_num][index].arg1];
    else in2 = IM[m_num][index].arg2;
    (in1 <= in2) ? (DM[IM[m_num][index].arg3] = 1) : (DM[IM[m_num][index].arg3] = 0);
}

void simachine::neg(int index) {
    int in1;
    if (IM[m_num][index].arg1_a) in1 = DM[IM[m_num][index].arg1];
    else in1 = IM[m_num][index].arg1;
    DM[IM[m_num][index].arg2] = -1 * in1;
}

void simachine::jmp0(int index){
    int in1;
    if (IM[m_num][index].arg1_a) in1 = DM[IM[m_num][index].arg1];
    else in1 = IM[m_num][index].arg1;
    if (IM[m_num][IM[m_num][index].arg2].opcode != -1) {
        (in1 == 0) ? (pc = IM[m_num][index].arg2) : (pc = index);
    }
}

void simachine::ass(int index) {
    int in1;
    if (IM[m_num][index].arg1_a) in1 = DM[IM[m_num][index].arg1];
    else in1 = IM[m_num][index].arg1;
    DM[IM[m_num][index].arg2] = in1;
}

void simachine::jmp(int index) {
    if (IM[m_num][IM[m_num][index].arg1].opcode != -1) {
        pc = IM[m_num][index].arg1;
    }
}

void simachine::read(int index) {
    int number;
    string input;
    printf("\nInput an integer then press enter: ");
    getline (std::cin, input);
    if (input.find_first_not_of("\n\s\t\r") == std::string::npos) {
        printf("INVALID INPUT");
        exit(1);
    }
    try {
        number = stoi (input, nullptr);
    } catch (const std::invalid_argument&) {}
    DM[IM[m_num][index].arg1] = number;
}

void simachine::write(int index){
    int number;
    if (IM[m_num][index].arg1_a) number = DM[IM[m_num][index].arg1];
    else number = IM[m_num][index].arg1;
    printf("%i\n", number);
}

void simachine::halt(int index){
    printf("\nMachine received HALT signal\n");
}
