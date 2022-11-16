#include "kiss_file_t.h"

#include <iostream>

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
                static bool output_tmp;
                static string nows_tmp, nexts_tmp;
                static uint8_t tmp = 0;
                vector<uint8_t> vec_input_tmp;
                for(int j = 0 ; j < input_cnt ; j++){
                    input >> tmp;
                    vec_input_tmp.push_back(tmp);
                }
                input >> nows_tmp >> nexts_tmp >> output_tmp;
                this->states.push_back(state_t(vec_input_tmp, nows_tmp, nexts_tmp, output_tmp));
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
        ff << " " << i.this_state + " " + i.next_state + " " << (i.output ? '1' : '0') << std::endl;
    }
    ff << ".end_kiss" << std::endl;
}

