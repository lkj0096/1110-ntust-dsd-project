#ifndef PROJECT03_TABLE_ELEMENT_T_H
#define PROJECT03_TABLE_ELEMENT_T_H

#include <vector>
#include <string>

using std::string;
using std::vector;

class list_element_t{
public:
    list_element_t(const string &input, const string &aNstate, const string &bNstate)
        : input(input), A_nstate(aNstate), B_nstate(bNstate) {}

public:

    string input;
    string A_nstate, B_nstate;
};

class table_element_t {
public:
    table_element_t() = default;
    table_element_t(const string& A, const string& B, vector<list_element_t> lists , const bool en = 1)
        : A(A), B(B), nstate_lists(lists), enable(en) {};

    bool enable = 1;
    string A, B;
    vector<list_element_t> nstate_lists;
    // store
    // I=0 A+1 B+1
    // I=1 A+1 B+1
};


#endif //PROJECT03_TABLE_ELEMENT_T_H
