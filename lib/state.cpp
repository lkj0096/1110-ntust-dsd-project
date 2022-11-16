#include "state_t.h"

bool state_t::operator> (const state_t& R_val){
    return (this->this_state != R_val.this_state)? this_state < R_val.this_state:
           (this->input[0] != R_val.input[0]) ? this->input[0] > R_val.input[0]:
           (this->input[1] > R_val.input[1]);
}
bool state_t::operator< (const state_t& R_val){
    return (this->this_state != R_val.this_state)? this_state > R_val.this_state:
           (this->input[0] != R_val.input[0]) ? (this->input[0] < R_val.input[0]):
           (this->input[1] < R_val.input[1]);
}