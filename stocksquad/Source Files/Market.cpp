#include <iostream>
#include <thread>
#include <iomanip>
#include <string>
#include <ctime>

#include "../Header Files/Market.h"

using namespace std;

int num_public_companies;
int market_max_size;
Stock **public_companies;
map<string, Stock*> company_lookup;

Market::Market() : num_public_companies(0), market_max_size(5) {
    public_companies = new Stock*[5];
    for (int i=0; i<5; i++) {
        public_companies[i] = nullptr;
    }
}

Market::Market(int size) : num_public_companies(0), market_max_size(size) {
    public_companies = new Stock*[size];
    for (int i=0; i<size; i++) {
        public_companies[i] = nullptr;
    }
}

int Market::get_market_size() const {
    return num_public_companies;
}

int Market::get_max_market_size() const {
    return market_max_size;
}

void Market::print_market(ostream &os) {
    os << "| " << left << setw(10) << "Ticker" << " | " << left << setw(20) << "Company Name" << " | " << left << setw(10) << "Curr Price" << " | " << left << setw(10) << "\% Change" << "  " << " | " << left << setw(10) << "Price/Share" << " " << " | " << left << setw(10) << "High" << " | " << left << setw(10) << "Low" << " |" << endl;
    os << "----------------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < num_public_companies; i++) {
        public_companies[i]->display_stock_info(os);
    }
    os << "----------------------------------------------------------------------------------------------------------\n";
}

bool Market::lookup_stock(string st) {
    if (company_lookup[st])
        return 1;
    return 0;
}

Stock *Market::get_stock(string st) {
    if (lookup_stock(st))
        return company_lookup[st];
    cout << "Error: Doesn't exist\n";
    return nullptr;
}

void Market::emulate_market_iteration(int iterations, bool print, ostream &os) {
    // Max iterations
    if (iterations > 50)
        iterations = 50;

    if (print) {
        cout << "\033c";
        while (iterations > 0) {  
            print_market(os);
            for (int i = 0; i < num_public_companies; i++) {
                public_companies[i]->simulate_market_movement();
            }
            this_thread::sleep_for(chrono::milliseconds(250));         // One second delay before next loop
            cout << "\033c";
            iterations--;
        }
        print_market(os);
    }
    else {
        while (iterations > 0) {
            for (int i = 0; i < num_public_companies; i++) {
                public_companies[i]->simulate_market_movement();
            }
            iterations--;
        }
    }
}

bool Market::add_stock(string st, string cn, double ipo, double vol) {
    
    if (num_public_companies >= market_max_size) {
        cout << "Max number of companies\n";
        return 0;
    }
    else if (company_lookup[st]) {
        cout << "Company already exists\n";
        return 0;
    }
    public_companies[num_public_companies] = new Stock(st, cn, ipo, vol);
    company_lookup[st] = public_companies[num_public_companies];
    num_public_companies++;
    return 1;
}

bool Market::add_stock(string st, string cn, double ipo, double curr, double prev, double high, double low, double vol) {
    if (num_public_companies >= market_max_size) {
        cout << "Max number of companies\n";
        return 0;
    }
    else if (company_lookup[st]) {
        cout << "Company already exists\n";
        return 0;
    }
    public_companies[num_public_companies] = new Stock(st, cn, ipo, curr, prev, high, low, vol);
    company_lookup[st] = public_companies[num_public_companies];
    num_public_companies++;
    return 1;
}

bool Market::update_stock(std::string st, double ipo, double curr, double prev, double high, double low, double vol) {
    if (!company_lookup[st]) {
        cout << "Company does not exist\n";
        return 0;
    }
    Stock *company = company_lookup[st];
    
    company->set_ipo_price(ipo);
    company->set_share_price(curr);
    company->set_previous_share_price(prev);
    company->set_high_price(high);
    company->set_low_price(low);
    company->set_volatility(vol);
    return 1;
}

bool Market::update_stock_ipo(std::string st, double ipo) {
    if (!company_lookup[st]) {
        cout << "Company does not exist\n";
        return 0;
    }
    Stock *company = company_lookup[st];
    company->set_ipo_price(ipo);
    return 1;
}

void Market::update_all_stock_ipo() {
    for (int i = 0; i < num_public_companies; i++) {
        Stock *curr_company = public_companies[i];
        double curr_share_price = curr_company->get_share_price();
        public_companies[i]->set_ipo_price(curr_share_price);
    }
}

Stock **Market::get_public_companies() {
    return public_companies;
}

Market::~Market() {
    for (int i = 0; i < market_max_size; i++) {
        if (public_companies[i] != nullptr) {
            delete public_companies[i];
        }
    }
    delete[] public_companies;
}

SandP500::SandP500() : Market() {
    add_stock("APPL", "Apple", 200, 100);
    add_stock("TSLA", "Tesla", 170, 100);
    add_stock("AMZN", "Amazon", 80, 100);
    add_stock("XCEL", "Excell Energy", 67, 100);
    add_stock("GOOG", "Google", 1, 100);
}
