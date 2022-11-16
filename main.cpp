#include <iostream>
#include <string>
#include <fstream>

#include "lib/kiss_file_t.h"
#include "lib/STG_dot_file_t.h"

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;





int main(int argc, char* argv[]) {

    if(argc == 3){
        kiss_file_t kiss_file;
        std::fstream ff(argv[1], std::ios::in);
        cout << argv[1] << endl;
        kiss_file.read_in(ff);

        STG_dot_file_t input_STG_dot_file;
        input_STG_dot_file.input_from_kiss(kiss_file);

        std::fstream ff2(argv[2], std::ios::out);
        cout << argv[2] << endl;
        input_STG_dot_file.write_out(ff2);

        return 3;
    }

    if(argc != 5 && argc != 4 && argc != 1){
        std::cerr << "usage: ./<BINARY FILE> <INPUT KISS> <OUTPUT KISS> <OUTPUT DOT>" << endl;
        return 1;
    }

    kiss_file_t kiss_file;

    if (argc == 4) {
        std::fstream ff(argv[1], std::ios::in);
        kiss_file.read_in(ff);
    }
    else if (argc == 5) {
        std::fstream ff(argv[1], std::ios::in);
        kiss_file.read_in(ff);
    }
    else {
        kiss_file.read_in(static_cast<std::fstream&>(cin));
    }

    STG_dot_file_t input_STG_dot_file;

    input_STG_dot_file.input_from_kiss(kiss_file);

    if(argc == 5){
        std::fstream ff(argv[2], std::ios::out | std::ios::trunc);
        input_STG_dot_file.write_out(ff);
    }else{
        input_STG_dot_file.write_out(static_cast<std::fstream&>(cout));
    }

    kiss_file.minimization();

    if (argc == 4) {
        std::fstream ff(argv[2], std::ios::out | std::ios::trunc);
        kiss_file.write_out(ff);
    } else if (argc == 5) {
        std::fstream ff(argv[3], std::ios::out | std::ios::trunc);
        kiss_file.write_out(ff);
    } else{
        kiss_file.write_out(static_cast<std::fstream&>(cout));
    }

    STG_dot_file_t output_STG_dot_file;

    output_STG_dot_file.input_from_kiss(kiss_file);

    if (argc == 4) {
        std::fstream ff(argv[3], std::ios::out | std::ios::trunc);
        kiss_file.write_out(ff);
    } else if (argc == 5) {
        std::fstream ff(argv[4], std::ios::out | std::ios::trunc);
        output_STG_dot_file.write_out(ff);
    } else{
        output_STG_dot_file.write_out(static_cast<std::fstream&>(cout));
    }

    return 0;
}
