#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>
#include "History.hpp"
#include "Transaction.hpp"
#include "project3.hpp"
#include "History_Transaction_definitions.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor

Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date,
                         bool buy_sell_trans, unsigned int number_shares, double trans_amount) {
    this->symbol = ticker_symbol;
    this->day = day_date;
    this->month = month_date;
    this->year = year_date;

    if (buy_sell_trans == false) {
        this->trans_type = "Sell";
    } else {
        this->trans_type = "Buy";
    }

    this->shares = number_shares;
    this->amount = trans_amount;
    this->trans_id = assigned_trans_id;
    assigned_trans_id++;
    this->set_acb_per_share(0);
    this->set_acb(0);
    this->set_share_balance(0);
    this->set_cgl(0);
    this->set_next(nullptr);

}


// Destructor
Transaction::~Transaction() {}


// Overloaded < operator.
//

bool Transaction::operator<(Transaction const &other) { //Sees if the instance's date is less than teh passed in date
    //std::cout << "Hi" << this->year << " " << other.year << " " << this-> month << " " << other.month << " " << this->day << " " << other.day << std::endl;

    bool isLessThan = false;
    if (this->year < other.year) {
        isLessThan = true;
    } else if (this->month < other.month && this->year == other.year) {
        isLessThan = true;
    } else if (this->day < other.day && this->month == other.month && this->year == other.year) {
        isLessThan = true;
    }
    return isLessThan;

}


// Member functions to get values.
std::string Transaction::get_symbol() const { return symbol; }

unsigned int Transaction::get_day() const { return day; }

unsigned int Transaction::get_month() const { return month; }

unsigned int Transaction::get_year() const { return year; }

unsigned int Transaction::get_shares() const { return shares; }

double Transaction::get_amount() const { return amount; }

double Transaction::get_acb() const { return acb; }

double Transaction::get_acb_per_share() const { return acb_per_share; }

unsigned int Transaction::get_share_balance() const { return share_balance; }

double Transaction::get_cgl() const { return cgl; }

bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }

unsigned int Transaction::get_trans_id() const { return trans_id; }

Transaction *Transaction::get_next() { return p_next; }

// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }

void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }

void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }

void Transaction::set_cgl(double value) { cgl = value; }

void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }


// Print the transaction.
//
void Transaction::print() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(4) << get_trans_id() << " "
              << std::setw(4) << get_symbol() << " "
              << std::setw(4) << get_day() << " "
              << std::setw(4) << get_month() << " "
              << std::setw(4) << get_year() << " ";


    if (get_trans_type()) {
        std::cout << "  Buy  ";
    } else { std::cout << "  Sell "; }

    std::cout << std::setw(4) << get_shares() << " "
              << std::setw(10) << get_amount() << " "
              << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
              << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
              << std::setw(10) << std::setprecision(3) << get_cgl()
              << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
History::History() {
    this->p_head = nullptr;
}


// Destructor
History::~History() {
    Transaction *Current = this->p_head;
    Transaction *goNext = this->p_head->get_next();
    //std::cout << "Deleting Head: " << Current->get_day() << std::endl;
    delete Current;
    this->p_head = nullptr;
    while (goNext != nullptr) {
        Current = goNext;
        //std::cout << "Deleting Next: " << Current->get_day() << std::endl;
        goNext = Current->get_next();

        delete Current;
        Current = nullptr;
    }
}


// read_transaction(...): Read the transaction history from file.
//
void History::read_history() {
    ece150::open_file();
    //std::cout << "Header: " << p_head->get_day() << std::endl;
    while (ece150::next_trans_entry() != false) {
        Transaction* temp = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(),ece150::get_trans_type(), ece150::get_trans_shares(),ece150::get_trans_amount());

        //std::cout << "First Element: " << p_first->get_day() << std::endl;
        insert(temp);
        temp = nullptr;
    }
    //What if there arent enough inputs
  
}


// insert(...): Insert transaction into linked list.
//

void History::insert(Transaction *p_new_trans) {

    if (this->get_p_head() == nullptr) {
        this->p_head = p_new_trans;
    } else {
        Transaction *p_old = p_head;

        while (p_old->get_next() != nullptr) {
            p_old = p_old->get_next();
        }

        p_old->set_next(p_new_trans);
        //std::cout << "New Element: " << p_new->get_day() << std::endl;

    }

}

// sort_by_date(): Sort the linked list by trade date.
//

void History::sort_by_date() {
    bool hasPlaced{false};
    bool isFirst{true};
    Transaction *p_move = p_head->get_next();//computation for putting in sorted array
    Transaction *p_compare = this->get_p_head(); //value in sorted array that you are comparing with
    Transaction *p_goNext = p_head->get_next(); //next val in sorted array
    Transaction *p_before = nullptr; //For case where you are squishing values, you need to have node before
    this->p_head->set_next(nullptr);
    while (p_goNext != nullptr) {
        hasPlaced = false;
        p_compare = this->p_head;
        p_goNext = p_move->get_next();
        isFirst = true;
        p_before = nullptr;

        while (hasPlaced == false) {
            if (*p_compare < *p_move) { // Add to the end of the linked list

                if (p_compare->get_next() == nullptr) {
                    hasPlaced = true;
                    p_compare->set_next(p_move);
                    p_move->set_next(nullptr);
                } else { //Check the next value in the linked list
                    isFirst = false;
                    p_before = p_compare;
                    p_compare = p_compare->get_next();
                }

            } else { //p_move is smaller so put it in front of p_compare

                if (isFirst == true) {
                    p_move->set_next(p_compare);
                    this->p_head = p_move;
                    hasPlaced = true;
                } else { //squish
                    hasPlaced = true;
                    p_move->set_next(p_before->get_next());
                    p_before->set_next(p_move);
                }
            }
        }

        p_move = p_goNext;
    }

    p_move = this->p_head;
    do {
        p_move = p_move->get_next();
    } while (p_move != nullptr);
    p_move = nullptr;
     p_compare = nullptr; //value in sorted array that you are comparing with
     p_goNext = nullptr; //next val in sorted array
     p_before = nullptr;

}

// update_acb_cgl(): Updates the ACB and CGL values.
//
void History::update_acb_cgl() {

    double running_acb{0};
    unsigned int running_share_balance{0};
    double last_acb_per_share {0};
    Transaction* passThrough = this->get_p_head();

    while (passThrough != nullptr){
        if(passThrough->get_trans_type() == true){ // Buy
            running_acb += passThrough->get_amount();
           running_share_balance += passThrough->get_shares();
           passThrough->set_acb(running_acb);
           passThrough->set_share_balance(running_share_balance);
            passThrough -> set_acb_per_share(running_acb/ running_share_balance);
            last_acb_per_share = passThrough->get_acb_per_share();
        } else if (passThrough -> get_trans_type() == false){ // Sell
            passThrough->set_acb_per_share(last_acb_per_share);
            passThrough->set_cgl(passThrough->get_amount() -(passThrough->get_shares()*passThrough->get_acb_per_share()));
            running_acb -= last_acb_per_share*passThrough->get_shares();
            running_share_balance -= passThrough->get_shares();
            passThrough->set_share_balance(running_share_balance);

            passThrough->set_acb(running_acb);
        }
        passThrough = passThrough->get_next();
    }

}

// compute_cgl(): )Compute the ACB, and CGL.
//

double History::compute_cgl(unsigned int year) {
    double year_cgl{0};
    Transaction *iterate = this->get_p_head();

    while (iterate->get_year() <= year) {
        if (iterate->get_year() == year) {
            year_cgl += iterate->get_cgl();
        }
        if (iterate->get_next() != nullptr) {
            iterate = iterate->get_next();
        } else {
            year--;
        }
    }
    return year_cgl;
}


// print() Print the transaction history.
//

void History::print() {
    std::cout << "============ BEGIN TRANSACTION HISTORY ============\n";
    Transaction *pass = this->get_p_head();

    while (pass != nullptr) {
        pass->print();
        pass = pass->get_next();
    }

    std::cout << "============ END TRANSACTION HISTORY ============\n";
}


// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }


#endif

