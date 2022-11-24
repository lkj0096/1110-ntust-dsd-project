#include "kiss_file_t.h"

#include "table_element_t.h"

#include <iostream>
#include <map>

void kiss_file_t::read_in(std::fstream& input) {

    string prefix;

    while(true){
        input >> prefix;
        if(prefix == ".start_kiss") {

        }else if (prefix == ".i") {
            input >> this->input_cnt;
        } else if (prefix == ".o") {
            input >> this->output_cnt;
        } else if (prefix == ".p") {
            input >> this->part_cnt;
        } else if (prefix == ".s") {
            input >> this->state_cnt;
        } else if (prefix == ".r") {
            input >> this->init_state;
            for(int i = 0 ; i < part_cnt; i++) {
                static string output_tmp;
                static string nows_tmp, nexts_tmp;
                static uint8_t tmp = 0;
                static string input_tmp;
                input >> input_tmp >> nows_tmp >> nexts_tmp >> output_tmp;
                this->states.push_back(state_t(input_tmp, nows_tmp, nexts_tmp, output_tmp));
            }
        } else if(prefix == ".end_kiss"){
            break;
        } else {
            std::cerr << "please check your kiss file" << std::endl;
            break;
        }
    }

}

void kiss_file_t::minimization() {

    std::map<string, int> mymap;
    vector<string> demap;

    int index = 0;
    for(auto& i : this->states){
        bool flag = 1;
        for(auto& j : mymap){
            if(i.this_state == j.first) { flag = 0; }
        }
        if(flag){
            mymap.insert(std::make_pair(i.this_state, index++));
            demap.push_back(i.this_state);
        }
    }

    vector<vector<table_element_t>> table
        (mymap.size(), vector<table_element_t>(mymap.size(), table_element_t()));

    for(int i = 0; i < mymap.size(); i++){
        for(int j = 0; j < mymap.size(); j++){
            if(i - j < 1){
                table[i][j].enable = 0;
                continue;
            }
            vector<state_t> A_lists, B_lists;
            for(auto k : this->states){
                if(k.this_state == demap[i]){
                    A_lists.push_back(k);
                }
                if(k.this_state == demap[j]){
                    B_lists.push_back(k);
                }
            }

            vector<list_element_t> tmp_lists;
            for(auto m : A_lists){
                for(auto n : B_lists){
                    if(m.input == n.input){
                        if(m.output != n.output){
                            table[i][j].enable = false;
                        }else{
                            table[i][j].A = demap[i];
                            table[i][j].B = demap[j];
                            tmp_lists.push_back(
                                list_element_t(m.input, m.next_state, n.next_state));
                        }
                    }
                }
            }
            table[i][j].nstate_lists = tmp_lists;
        }
    }

    // find implicate
    bool imp_flag = 1;
    while (imp_flag){
        imp_flag = 0;
        for(int i = 0; i < table.size(); i++){
            for(int j = 0; j < table[i].size(); j++){
                if(table[i][j].enable == false){
                    continue;
                }
                for(auto k : table[i][j].nstate_lists){
                    int ind_A = mymap[k.A_nstate];
                    int ind_B = mymap[k.B_nstate];
                    if(ind_A == ind_B){
                        continue;
                    }else if(table[std::max(ind_A, ind_B)][std::min(ind_A, ind_B)].enable == false){
                        imp_flag = 1;
                        table[i][j].enable = 0;
                    }
                }
            }
        }
    }

    // reduction
    imp_flag = 1;
    while (imp_flag){
        imp_flag = 0;
        for(int i = 0; i < table.size(); i++){
            for(int j = 0; j < table[i].size(); j++){
                if(table[i][j].enable == false){
                    continue;
                } else{
                    for(int k = 0; k < this->states.size() ; k++){
                        if(this->states[k].next_state == table[i][j].A){
                            this->states[k].next_state = table[i][j].B;
                        }
                        if(this->states[k].this_state == table[i][j].A){
                            this->states.erase(this->states.begin() + k);
                            k--;
                        }
                    }
                    int ind = mymap[table[i][j].A];
                    for(int k = 0; k < table.size(); k++){
                        table[ind][k].enable = false;
                        table[k][ind].enable = false;
                    }
                }
            }
        }
    }

    this->init_state = this->states[0].this_state;

    this->part_cnt = this->states.size();

    vector<string> states_counting;

    for(auto& i : this->states){
        bool flag = 1;
        for(auto& j : states_counting){
            if(i.this_state == j) { flag = 0; }
        }
        if(flag) {
            states_counting.push_back(i.this_state);
        }
    }

    this->state_cnt = states_counting.size();

}

void kiss_file_t::write_out(std::fstream& ff) const {

    ff << ".start_kiss" << std::endl;
    ff << ".i " << this->input_cnt << std::endl;
    ff << ".o " << this->output_cnt << std::endl;
    ff << ".p " << this->part_cnt << std::endl;
    ff << ".s " << this->state_cnt << std::endl;
    ff << ".r " << this->init_state << std::endl;

    for(auto& i : states){
        for(auto& j : i.input){
            ff << j;
        }
        ff << " " << i.this_state + " " + i.next_state + " " << i.output << std::endl;
    }
    ff << ".end_kiss" << std::endl;
}

