#ifndef PROJECT03_KISS_FILE_T_H
#define PROJECT03_KISS_FILE_T_H
#pragma once

#include "state_t.h"

#include <fstream>
#include <string>
#include <vector>
using std::vector;
using std::string;

class kiss_file_t{
    friend class STG_dot_file_t;
private:
    int input_cnt;
    int output_cnt;
    int part_cnt;
    int state_cnt;
    string init_state = "";
    vector<state_t> states;
public:
    void read_in(std::fstream&);
    void minimization();
    void write_out(std::fstream&) const;
};

#endif //PROJECT03_KISS_FILE_T_H
