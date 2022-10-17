#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>

using std::cout, std::cin, std::endl;
using std::map, std::set, std::pair;
using std::vector;
using std::fstream;
using std::string;

string to_bin_str(size_t bin, size_t size){
    string str = "";

    while(size){
        if(bin & 1){
            str = '1' + str;
        }else{
            str = '0' + str;
        }
        bin >>= 1;
        size--;
    }
    return str;
}

size_t number_of_1(string str){
    size_t cnt = 0;
    for(auto& i : str){
        if(i == '1'){ cnt++; }
    }
    return cnt;
}

class str_bool_t{
public:
    str_bool_t() : part(""), out(0) {};
    str_bool_t(string part = "", uint8_t out = 0): part(part), out(out) {};
    string part;
    uint8_t out;

    bool operator== (const str_bool_t& match) const{
        return (match.part == this->part && match.out == this->out);
    }

    bool operator== (const string& match) const{// 0X00 == 0100 , 0xxx == 0111 != 1111
        if(this->part.size() != match.size()){
            std::cerr << __LINE__ << " size diff" << endl;
            return 0;
        }
        for(int i = 0 ; i < this->part.size(); i++){
            if(this->part[i] != match[i] ){
                if (!(this->part[i] == '-' || match[i] == '-')){
                    return false;
                }
            }

        }
        return  1;
    }

    str_bool_t mix(const str_bool_t& mixed) const{ // 0100 0000 -> 0x00    x,x->x  1,x->1
        str_bool_t ret(string(this->part.size(), '0'), (this->out == 2 && mixed.out == 2) ? 2 : 1);
        for(int i = 0 ; i < this->part.size() ; i++){
            if(this->part[i] == mixed.part[i]){
                ret.part[i] = this->part[i];
            } else if(this->part[i] == '1' && mixed.part[i] == '0'){
                ret.part[i] = '-';
            } else if(this->part[i] == '0' && mixed.part[i] == '1'){
                ret.part[i] = '-';
            }
        }
        return ret;
    }

    size_t repeat_digit(const str_bool_t& mixed){
        size_t ret = 0;
        for(int i = 0 ; i < this->part.size() ; i++){
            if(this->part[i] == mixed.part[i]){
                ret++;
            }
        }
        return ret;
    }
};


void print_ess_table(vector<vector<uint8_t>> esspi_table, vector<str_bool_t> prime_imp){
    cout << endl << string(prime_imp[0].part.size(), ' ');

    for(int i = 0 ; i < esspi_table[0].size(); i++){
        cout << std::setw(3) << i;
    }
    cout << endl;

    for(int i = 0 ; i < esspi_table.size(); i++){
        cout << prime_imp[i].part;
        for(int j = 0 ; j < esspi_table[i].size(); j++){

            cout << std::setw(3) << (esspi_table[i][j] ? '1' : '0');
        }
        cout << endl;
    }
}


class pla_file_t{
public:
//    pla_file_t() {};
//    pla_file_t(const pla_file_t&);
//    pla_file_t operator= (const pla_file_t&);
    uint32_t input_cnt = 0;
    uint32_t output_cnt = 0;
    vector<string> var_list = {};
    string fn_name = "f";
    uint32_t str_bool_cnt = 0;
    vector<str_bool_t> str_bool_list = {};
    void prime_implicant();
    pla_file_t to_output();
private:
    vector<str_bool_t> _ess_pi = {};
};

void pla_file_t::prime_implicant() {

    vector<pair<size_t, uint8_t>> index_vec;

    for(auto& str_bool : this->str_bool_list){
        vector<pair<size_t, uint8_t>> indexs({{0, 0}});
        for(char& chr : str_bool.part){
            size_t size = indexs.size();
            for(int i = 0 ; i < size; i++){
                indexs[i].first <<= 1;
                indexs[i].second = str_bool.out;
                if(chr == '1'){
                    indexs[i].first += 1;
                    indexs[i].second = str_bool.out;
                }else if(chr == '-'){
                    indexs.push_back(indexs[i]);
                    indexs[i].first += 0;
                    indexs[i].second = str_bool.out;
                    indexs[indexs.size() - 1].first += 1;
                    indexs[indexs.size() - 1].second = str_bool.out;
                }
            }
        }
        index_vec.insert(index_vec.end(), indexs.begin(), indexs.end());
    }

    set<pair<size_t, uint8_t>> index_set;
    for(auto& i : index_vec){ index_set.insert(i); }

    vector<str_bool_t> extend_str_bool;
    for(auto& i : index_set){
        extend_str_bool.push_back(str_bool_t(
            to_bin_str(i.first, this->input_cnt),
            i.second));
    }

    vector<vector<vector<pair<str_bool_t, bool>>>> nliteral
        (this->input_cnt + 1, vector<vector<pair<str_bool_t, bool>>>(
            this->input_cnt + 1, vector<pair<str_bool_t, bool>>()));
    for(auto& i : extend_str_bool){
        nliteral[0][number_of_1(i.part)].push_back(std::make_pair(i, 0));
    }

    for(int nlit = 1; nlit < this->input_cnt + 1; nlit++){
        for(int gp_cnt = 0 ; gp_cnt < this->input_cnt + 1 - nlit; gp_cnt++){
            for(auto &lower_gp : nliteral[nlit - 1][gp_cnt]){
                for(auto& bigger_gp : nliteral[nlit - 1][gp_cnt + 1]){
                    if(lower_gp.first.repeat_digit(bigger_gp.first) == lower_gp.first.part.size() - 1){
                        lower_gp.second = 1;
                        bigger_gp.second = 1;
                        str_bool_t literal = lower_gp.first.mix(bigger_gp.first);
                        nliteral[nlit][gp_cnt].push_back({literal, 0});
                    }
                }
            }
        }
    }

    vector<str_bool_t> prime_imp;
    for(int nlit = 0; nlit < this->input_cnt + 1; nlit++){
        for(int gp_cnt = 0 ; gp_cnt< this->input_cnt + 1- nlit; gp_cnt++){
            for(auto& literal : nliteral[nlit][gp_cnt]){
                if(literal.second == false){
                    prime_imp.push_back(literal.first);
                }
            }
        }
    }

    for(int i = 0 ; i < prime_imp.size() ; i++){
        for(int j = 0 ; j < prime_imp.size(); j++){
            if(i == j) { continue; }
            if(prime_imp[i] == prime_imp[j]){
                prime_imp.erase(prime_imp.begin()+j);
            }
        }
    }

    vector<vector<uint8_t>> esspi_table(prime_imp.size(), vector<uint8_t>(pow(2, this->input_cnt), 0));

    for(int i = 0 ; i < esspi_table.size(); i++){
        for(int j = 0 ; j < esspi_table[i].size(); j++){
            if  (index_set.find({j, 1}) != index_set.end()){
                if(prime_imp[i] == to_bin_str(j, this->input_cnt)){
                    esspi_table[i][j] = 1;
                }
            }
        }
    }

    vector<str_bool_t> ess_pi;
    bool flag = true;
    while(flag) {
        flag = false;

        vector<size_t> cnt_table(esspi_table[0].size(), 0);
        vector<pair<size_t, size_t>> essential_point;
        for(int row_ind = 0 ; row_ind < esspi_table[0].size(); row_ind++){
            for(int col_ind = 0; col_ind < esspi_table.size(); col_ind++){
                if(esspi_table[col_ind][row_ind]){
                    cnt_table[row_ind]++;
                }
            }
            if(cnt_table[row_ind] == 1){
                flag = true;
                essential_point.push_back({row_ind, -1});
            }
        }
        if(flag == false){ break; }

        for(int i = 0; i < essential_point.size(); i++){
            for(int col_ind = 0 ; col_ind < esspi_table.size(); col_ind++){
                if(esspi_table[col_ind][essential_point[i].first]){
                    essential_point[i].second = col_ind;
                    ess_pi.push_back(prime_imp[col_ind]);
                }
            }
        }

        for(int i = 0; i < essential_point.size(); i++){
            for(int row_ind = 0 ; row_ind < esspi_table[0].size(); row_ind++) {
                if(esspi_table[essential_point[i].second][row_ind] == 1){
                    for(int col_ind = 0 ; col_ind < esspi_table.size(); col_ind++){
                        esspi_table[col_ind][row_ind] = 0;
                    }
                }
            }
        }
    }

    flag = true;

    while (flag){
        vector<size_t> cols;
//        vector<size_t> cnt_table(esspi_table.size(), 0);
        size_t row;
        flag = false;
        for(int col_ind = 0 ; col_ind < esspi_table.size(); col_ind++){
            for(int row_ind = 0 ; row_ind < esspi_table[0].size(); row_ind++) {
                if(esspi_table[col_ind][row_ind] == 1){
                    ess_pi.push_back(prime_imp[col_ind]);
                    row = row_ind;
                    flag = 1;
                    for(int _col = 0 ; _col < esspi_table.size(); _col++){
                        if(esspi_table[_col][row_ind] == 1){
                            cols.push_back(_col);
                        }
                    }
                }
                if(flag){ break;}
            }
            if(flag){ break;}
        }
        if(flag == false) { break; }

        for(auto& rm_col : cols){
            for(int row_ind = 0 ; row_ind < esspi_table[0].size(); row_ind++) {
                if(row_ind == row) { continue; }
                if(esspi_table[rm_col][row_ind] == 1){
                    for(int col_ind = 0 ; col_ind < esspi_table.size(); col_ind++){
                        esspi_table[col_ind][row_ind] = 0;
                    }
                }
            }
            esspi_table[rm_col][row] = 0;
        }
    }

    for(int i = 0 ; i < ess_pi.size() ; i++){
        for(int j = 0 ; j < ess_pi.size(); j++){
            if(i == j) { continue; }
            if(ess_pi[i] == ess_pi[j]){
                ess_pi.erase(ess_pi.begin()+j);
            }
        }
    }

    this->_ess_pi = ess_pi;

    cout << "Total number of terms: " << _ess_pi.size() << endl;
    cout << "Total number of literals: " ;
    size_t lit = 0;
    for(const auto& i : _ess_pi){
        for(const auto& j : i.part){
            if(j != '-') lit++;
        }

    }
    cout << lit << endl;
}

pla_file_t pla_file_t::to_output(){
    pla_file_t output;
    output.input_cnt = this->input_cnt;
    output.output_cnt = this->output_cnt;
    output.var_list = this->var_list;
    output.fn_name = this->fn_name;
    output.str_bool_cnt = this->_ess_pi.size();
    output.str_bool_list = this->_ess_pi;
    return  output;
}

pla_file_t input_pla(std::fstream &input) {
    pla_file_t pla_file;
    string prefix;


    while(true){
        input >> prefix;
        if (prefix == ".i") {
            input >> pla_file.input_cnt;
        } else if (prefix == ".o") {
            input >> pla_file.output_cnt;
        } else if (prefix == ".ilb") {
            for(int i = 0 ; i < pla_file.input_cnt; i++){
                string var;
                input >> var;
                pla_file.var_list.push_back(var);
            }
        } else if (prefix == ".ob") {
            input >> pla_file.fn_name;
        } else if (prefix == ".p"){
            input >> pla_file.str_bool_cnt;
            for(int i = 0 ; i < pla_file.str_bool_cnt ; i++){
                string part;
                char out = '-';
                input >> part;
                input >> out;
                switch (out) {
                case '0':
                    pla_file.str_bool_list.push_back(str_bool_t(part, 0));
                    break;
                case '1':
                    pla_file.str_bool_list.push_back(str_bool_t(part, 1));
                    break;
                case '-':
                    pla_file.str_bool_list.push_back(str_bool_t(part, 2));
                    break;
                default:
                    break;
                }
            }
        } else if(prefix == ".e"){
            break;
        } else {
            std::cerr << "please check your pla file" << endl;
            break;
        }
    }
    return pla_file;
}

void output_pla(pla_file_t& plaFile, std::fstream &output){
    output << ".i " << plaFile.input_cnt << endl;
    output << ".o " << plaFile.output_cnt << endl;
    output << ".ilb";
    for(const auto& i : plaFile.var_list){
        output << ' ' << i;
    }
    output << endl;
    output << ".ob " << plaFile.fn_name << endl;
    output << ".p " << plaFile.str_bool_cnt << endl;
    for(const auto& i : plaFile.str_bool_list){
        output << i.part << ' ' << ((i.out == 1)? '1' : (i.out == 0)? '0' : '-') << endl;
    }
    output << ".e" << endl;
}

int main(int argc, char *argv[]) {
    pla_file_t pla_file;
    if (argc == 3) {
        std::fstream ff(argv[1]);
        pla_file = input_pla(ff);
    } else {
        pla_file = input_pla(static_cast<std::fstream&>(cin));
    }

    cout << endl;
    pla_file.prime_implicant();
    cout << endl;

    pla_file_t output(pla_file.to_output());

    if (argc == 3) {
        std::fstream ff(argv[2]);
        output_pla(output, ff);
    } else{
        output_pla(output, static_cast<std::fstream&>(cout));
    }

}