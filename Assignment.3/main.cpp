#include "simachine.h"

using namespace std;

int main(int argc, char* argv[]) {

    string sourcename1;
    string sourcename2;
    string sourcename3;
    string sourcename4;

    if (argc < 5) {
        printf("Please note at least one file has to be valid.\n");

        printf("Please input first filename then press enter: ");
        getline (std::cin, sourcename1);

        printf("Please input second filename then press enter: ");
        getline (std::cin, sourcename2);

        printf("Please input third filename then press enter: ");
        getline (std::cin, sourcename3);

        printf("Please input fourth filename then press enter: ");
        getline (std::cin, sourcename4);
    } else {
        sourcename1 = argv[1];
        sourcename2 = argv[2];
        sourcename3 = argv[3];
        sourcename4 = argv[4];
    }

    fstream source1(sourcename1.c_str(), std::ifstream::in);
    fstream source2(sourcename2.c_str(), std::ifstream::in);
    fstream source3(sourcename3.c_str(), std::ifstream::in);
    fstream source4(sourcename4.c_str(), std::ifstream::in);

    simachine sim1;

    std::thread th1([&] {
        sim1.m_num = 0;
        sim1.loader(source1);
        sim1.machine();
    });


    std::thread th2([&] {
            sim1.m_num = 1;
            sim1.loader(source2);
            sim1.machine();
    });

    std::thread th3([&] {
        sim1.m_num = 2;
        sim1.loader(source3);
        sim1.machine();
    });

    std::thread th4([&] {
        sim1.m_num = 3;
        sim1.loader(source4);
        sim1.machine();
    });
    printf("\nFirst machine:\n");
        th1.join();
    printf("\nSecond machine:\n");
        th2.join();

    printf("\nThird machine:\n");
        th3.join();
    printf("\nFourth machine:\n");
        th4.join();
    return 0;
}
