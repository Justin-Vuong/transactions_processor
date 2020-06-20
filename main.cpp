#include <iostream>
#include "project3.hpp"
#include "History_Transaction_definitions.hpp"
#ifndef MARMOSET_TESTING

unsigned int Transaction::assigned_trans_id = 0;
int main() {
    History trans_history;
    trans_history.read_history();
    std::cout << "[Starting history]:" << std::endl;
    trans_history.print();
    trans_history.sort_by_date();
    std::cout << "[Sorted          ]:"  << std::endl;
    trans_history.print();
    trans_history.update_acb_cgl();
    trans_history.print();
    std::cout << "[CGL for 2018    ]:" << trans_history.compute_cgl(2018) << std::endl;
    std::cout << "[CGL for 2019    ]:" << trans_history.compute_cgl(2019) << std::endl;
    //std::cout << "Get amount:" << trans_history.get_p_head()->get_next()->get_acb() << " get cgl:"<<trans_history.get_p_head()->get_next()->get_acb();
    return 0;
}
#endif