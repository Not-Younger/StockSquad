#include <iostream>
#include <iomanip>
#include <string>
#include "../Header Files/Profile.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;

string hash_password(string pass) {
    string hashed_pass;
    long unsigned int i;
    for (i = 0; i < pass.length(); i++) {
        if (i % 2 == 0)
            hashed_pass += (pass[i] + 47 - 33) % (127 - 33) + 33;
        else
            hashed_pass += (pass[i] + 24 - 33) % (127 - 33) + 33;
    }
    return hashed_pass;
}

Profile::Profile(Market &m) : market(m) {
    funds = 0;
    total_posts = 0;
    num_profiles++;
}

Profile::Profile(string u_name, double money, Market &m) : username(u_name), funds(money), market(m) {
    num_profiles++;
}
Profile::Profile(string u_name, string pwd, double money, Market &m) : username(u_name), funds(money), market(m) {
    password = hash_password(pwd);
    num_profiles++;
}
Profile::Profile(std::string u_name, std::string pwd, std::string f_name, std::string l_name, double money, Market &m) : username(u_name), first_name(f_name), last_name(l_name), funds(money), market(m) {
    password = hash_password(pwd);
    num_profiles++;
}

Profile::Profile(std::vector<std::string> vector_info, Market &m) : market(m) {
    username = vector_info[0];
    password = vector_info[1];
    first_name = vector_info[2];
    last_name = vector_info[3];
    funds = stod(vector_info[4]);
    int num_companies = market.get_market_size();
    for (int i = 5; i < (num_companies*3)+5; i+=3) {
        string ticker = vector_info[i];
        holdings[ticker] = stoi(vector_info[i+1]);
        market.get_stock(ticker)->set_share_price(stod(vector_info[i+2]));
    }
    total_posts = stoi(vector_info[20]);
    num_profiles++;
    ofstream os("profile_curr.csv");
    os << get_funds() << endl;
    Stock **companies = m.get_public_companies();
    for (int i = 0; i < m.get_market_size(); i++) {
        string ticker = companies[i]->get_ticker();
        if (is_holding(ticker)) {
            int shares = get_shares(ticker);
            os << ticker << ',' << shares << endl;
        }
    }
    os.close();
}

Profile &Profile::operator=(const Profile &x) {
    username = x.get_username();
    password = x.get_password();
    first_name = x.get_first_name();
    last_name = x.get_last_name();
    funds = x.get_funds();
    market = x.get_market();
    total_posts = x.get_total_posts();
    holdings = x.get_holdings();
    return *this;
}

bool Profile::buy(string st, int shares) {
    Stock *stock;
    if (market.lookup_stock(st)) {
        stock = market.get_stock(st);
        double purchase_cost = stock->get_share_price() * shares;
        if (purchase_cost <= funds) {
            funds -= purchase_cost;
            if (is_holding(st)) {
                holdings[st] += shares;
            }
            else {
                holdings[st] = shares;
            }
            return 1;
        }
    }
    return 0;
}

bool Profile::sell(string st, int shares) {
    Stock *stock;
    if (market.lookup_stock(st) && is_holding(st)) {
        if (get_shares(st) >= shares) {
            stock = market.get_stock(st);
            double sell_cost = stock->get_share_price() * shares;
            funds += sell_cost;
            holdings[st] -= shares;
            return 1;
        }
    }
    return 0;
}

bool Profile::add(std::string st, int shares) {
    if (market.lookup_stock(st)) {
        if (is_holding(st))
            holdings[st] += shares;
        else
            holdings[st] = shares;
        return 1;
    }
    return 0;
}

bool Profile::remove(std::string st, int shares) {
    if (market.lookup_stock(st) && is_holding(st)) {
        if (get_shares(st) >= shares) {
            holdings[st] -= shares;
            return 1;
        }
    }
    return 0;
}

void Profile::print_holdings(ostream &os) {
    os << left << setw(10) << "| Ticker " << left << setw(20) << "| Company Name " << left << setw(15) << "| Shares " << left << setw(15) << "| Value " << "|" << endl;
    os << "-------------------------------------------------------------\n";
    Stock **companies = market.get_public_companies();
    int num_companies = market.get_market_size();
    
    for (int i = 0; i < num_companies; i++) {
        string ticker = companies[i]->get_ticker();
        os << left << setw(10) << "| " + ticker + " " << left << setw(20) << "| " + companies[i]->get_company_name() + " " << left << setw(15) << "| " + to_string(get_shares(ticker)) + " " << left << setw(15) << "| $" + format_output_string((get_share_value(ticker))) << "|" << endl;
    }
    os << "-------------------------------------------------------------\n";
    os << "Net worth: $" << get_net_worth() << endl;
}

bool Profile::is_holding(string st) {
    if (holdings[st])
        return 1;
    return 0;
}

std::map<std::string, int> Profile::get_holdings() const {
    return holdings;
}

int Profile::get_shares(string st) {
    if (is_holding(st)) {
        return holdings[st];
    }
    return 0;
}

double Profile::get_share_value(string st) {
    if (is_holding(st)) {
        double curr_price = market.get_stock(st)->get_share_price();
        return curr_price * get_shares(st);
    }
    return 0;
}

Market &Profile::get_market() const {
    return market;
}

double Profile::get_funds() const {
    return funds;
}

double Profile::get_net_worth() {
    Stock **companies = market.get_public_companies();
    int num_companies = market.get_market_size();
    double total_net_worth = get_funds();

    for (int i = 0; i < num_companies; i++) {
        string ticker = companies[i]->get_ticker();
        total_net_worth += get_share_value(ticker);
    }
    return total_net_worth;
}

string Profile::get_username() const {
    return username;
}

string Profile::get_password() const {
    return password;
}

string Profile::get_first_name() const {
    return first_name;
}

string Profile::get_last_name() const {
    return last_name;
}

int Profile::get_total_posts() const {
    return total_posts;
}

void Profile::set_funds(double money) {
    funds = money;
}

void Profile::set_username(std::string u_name) {
    username = u_name;
}

void Profile::set_password(std::string pwd) {
    password = pwd;
}

void Profile::set_first_name(std::string f_name) {
    first_name = f_name;
}

void Profile::set_last_name(std::string l_name) {
    last_name = l_name;
}

void Profile::print_profile(ostream &os) {
    os << "You are currently viewing " << get_username() << "'s profile" << endl;
    os << "Net worth: $" << get_net_worth() << endl;
    os << "Total posts: " << get_total_posts() << endl; // Create section below that actually lists out a few of the most recent posts
    os << "Portfolio: " << endl;
    print_holdings(os);
}

string Profile::profile_data_string(Market &m) {
    ostringstream oss;
    Stock **companies = m.get_public_companies();
    int num_companies = m.get_market_size();
    oss << get_username() << "," << get_password() << "," << get_first_name() << "," << get_last_name() << "," << to_string(get_funds());
    for (int i = 0; i < num_companies; i++) {
        string ticker = companies[i]->get_ticker();
        oss << "," << ticker << "," << to_string(get_shares(ticker)) << "," << to_string((companies[i]->get_share_price()));
    }
    oss << "," << to_string(get_total_posts()) << "," << to_string(get_net_worth());
    string string_oss = oss.str();
    return string_oss;
}

int Profile::num_profiles = 0;