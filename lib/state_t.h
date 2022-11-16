#ifndef PROJECT03_STATE_T_H
#define PROJECT03_STATE_T_H
#pragma once

#include <string>
#include <vector>
using std::vector;
using std::string;


class state_t{
public:
    state_t(const vector<uint8_t>& input, const string &thisState, const string &nextState, const bool& output)
    : input(input), output(output), this_state(thisState), next_state(nextState) {}

    vector<uint8_t> input;
    string this_state, next_state;
    bool output;

    bool operator> (const state_t& R_val);
    bool operator< (const state_t& R_val);
};



#endif //PROJECT03_STATE_T_H
