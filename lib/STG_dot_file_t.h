#ifndef PROJECT03_STG_DOT_FILE_T_H
#define PROJECT03_STG_DOT_FILE_T_H
#pragma once

#include "state_t.h"
#include "kiss_file_t.h"

#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;

class dot_label_t{
public:
    string this_state, next_state;
    vector<std::pair<vector<uint8_t>, bool>> label;
};

class STG_dot_file_t{
private:
    string init_state;
    vector<string> state_names;
    vector<dot_label_t> labels;
public:
    void input_from_kiss(const kiss_file_t&);
    void write_out(std::fstream&) const;
};


#endif //PROJECT03_STG_DOT_FILE_T_H
