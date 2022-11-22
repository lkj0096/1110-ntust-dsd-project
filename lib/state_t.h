#ifndef PROJECT03_STATE_T_H
#define PROJECT03_STATE_T_H
#pragma once

#include <string>
#include <vector>
using std::vector;
using std::string;


class state_t{
public:
    state_t(const string& input, const string &thisState, const string &nextState, const string& output)
    : input(input), output(output), this_state(thisState), next_state(nextState) {}

    string input;
    string this_state, next_state;
    string output;

    bool operator> (const state_t& R_val);
    bool operator< (const state_t& R_val);
};



#endif //PROJECT03_STATE_T_H
