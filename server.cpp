#include <iostream>
#include "neuron.h"
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <fstream>
#include <time.h>
#include <unistd.h>

using namespace std;

void print_help_message() {
    cout << "Server" << endl;
    cout << "------" << endl;
}

int main(int argc, char *argv[]) {
    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool out = false;
    bool developer = false;
    float p = 0.5;
    int bound = 0;
    int size = 1;
    int seed = -1;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:b:n:p:d")) != -1) {
        switch (arg) {
            case 'h':
                print_help_message();
                return 1;
            case 's':
                seed = stoi(optarg);
                break;
            case 'o':
                fileout = optarg;
                out = true;
                break;
            case 'f':
                filename = optarg;
                random = false;
                break;
            case 'n':
                size = stoi(optarg);
                break;
            case 'b':
                bound = stoi(optarg);
                break;
            case 'p':
                p = stof(optarg);
                break;
            case 'd':
                developer = true;
                break;
            case '?':
                if (optopt == 'c')
                    cerr << "la opcion " << optopt << " necesita un argumento" << endl;
                else if (isprint (optopt))
                    cerr << "Opcion " << optopt << " desconocida" << endl;
                print_help_message();
            default:
                break;
        }
    }

    
    std::tuple<double, double, int,  int> result;

    if (developer) {
        result = run_test_random(seed, p, size, bound);
        if (out) {
            write_to_file(fileout, result);
        }
        return 0;
    }
        
    if (random) {
        for (int i = 1; i <= size; i++) {
            result = run_test_random(seed, p, i, bound);
            if (out) {
                write_to_file(fileout, result);
            }
        }
    }
    else {
        result = run_test_from_file(filename, bound);
        if (out) {
            write_to_file(fileout, result);
        }
    }
    
}