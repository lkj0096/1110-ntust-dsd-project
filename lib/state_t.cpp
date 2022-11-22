#include "state_t.h"

bool state_t::operator> (const state_t& R_val){
    return (this->this_state != R_val.this_state)? this_state < R_val.this_state:
           this->input < R_val.input;
}
bool state_t::operator< (const state_t& R_val){
    return (this->this_state != R_val.this_state)? this_state > R_val.this_state:
           this->input > R_val.input;
}