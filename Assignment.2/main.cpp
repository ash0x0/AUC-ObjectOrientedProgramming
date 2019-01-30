#include "simachine.h"

using namespace std;

int main(int argc, char* argv[]) {

    string sourcename;

    if (argc < 2) {
        printf("Please input filename then press enter: ");
        getline (std::cin, sourcename);
    } else {
        sourcename = argv[1];
    }

    fstream source(sourcename.c_str(), std::ifstream::in);

    if (!source) {
        cerr << "Source file could not be opened! Check permissions or specify different file.\n";
        exit(1);
    }

    simachine sim1;
    sim1.loader(source);

    return 0;
}
