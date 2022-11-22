#include "STG_dot_file_t.h"

void STG_dot_file_t::input_from_kiss(const kiss_file_t & kiss_file) {
    this->init_state = kiss_file.init_state;
    this->state_names.push_back(this->init_state);
    for(auto& i : kiss_file.states){
        bool flag = 1;
        for(auto& j : this->state_names){
            if(i.this_state == j){ flag = 0; }
        }
        if(flag){ this->state_names.push_back(i.this_state); }
    }


    for(auto& i : kiss_file.states){
        bool flag = 1;
        dot_label_t dot_label;
        dot_label.this_state = i.this_state;
        dot_label.next_state = i.next_state;
        std::pair<string, string> tmp = std::make_pair(i.input, i.output);
        dot_label.label.push_back(tmp);
        for(auto& j : this->labels){
            if(i.this_state == j.this_state && i.next_state == j.next_state){
                j.label.push_back(tmp);
                flag = 0;
            }
        }
        if(flag){
            this->labels.push_back(dot_label);
        }
    }
}

void STG_dot_file_t::write_out(std::fstream & output) const {
    output << "digraph STG {" << std::endl;
    output << "\trankdir=LR;" << std::endl;

    output << "\tINIT [shape=point];" << std::endl;

    for(auto& i : this->state_names){
        output << "\t" << i << " [label=\"" << i << "\"];\n";
    }

    output << "\tINIT -> " << this->init_state << ";" << std::endl;

    for(auto& i : this->labels){
        bool first = 1;
        output << "\t" << i.this_state << " -> " << i.next_state << " [label=\"";
        for(auto& j : i.label){
            if(first){
                first = 0;
            }else{
                output << ",";
            }
            for(auto& m : j.first){
                output << m;
            }
            output << "/" << (j.second);
        }
        output << "\"];\n";
    }

    output << "}" << std::endl;
}
