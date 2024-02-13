#include <iostream>
#include <random>
#include <thread>
#include <iomanip>
#include <cmath>
#include <string>
#include <ctime>
#include <map>
#include <fstream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

#include "../Header Files/Stock.h"
#include "../Header Files/Market.h"
#include "../Header Files/Profile.h"
#include "/usr/local/cs/cs251/react.h"
using namespace std;

// global variables
map<string, string> texts;
bool react = false;
const int input_index = 0, prompt_index = 108;
int end_index;

const int ticker_index = 25, company_name_index = 32, stock_price_index = 45, percent_index = 56, difference_index = 65, high_index = 77, low_index = 82;
const int shares_index = 86, value_index = 93;
const int button_index = 99, button_sell_index = 103;
const int market_size = 5;
int market_company_indexs[market_size][7];
int stock_index = 309;
int holdings_company_indexs[market_size][2];

// State variables
SandP500 SP;
Profile profile(SP);
// Logic helpers
bool holdings;
bool display_user_info;
bool string_input;
bool social_tab, learning_tab, trade_tab, leaderboard_tab, profile_tab, all_tabs;
// Market iterations
bool iterate;
int iterations;
// Buying & selling
string ticker;
int shares;
// Profile creation/login
string first_name, last_name;
string user_name, password;

// Stock read/write
void read_stocks(Market &market, istream &is) {
    string line = "";
    while (getline(is, line)) {
        string ticker, comp_name;
        double ipo, curr_share, prev_share, high, low, vol;
        string temp_string = "";

        stringstream input_string(line);

        getline(input_string, ticker, ',');
        getline(input_string, comp_name, ',');

        getline(input_string, temp_string, ',');
        ipo = stod(temp_string.c_str());
        getline(input_string, temp_string, ',');
        curr_share = stod(temp_string.c_str());
        getline(input_string, temp_string, ',');
        prev_share = stod(temp_string.c_str());
        getline(input_string, temp_string, ',');
        high = stod(temp_string.c_str());
        getline(input_string, temp_string, ',');
        low = stod(temp_string.c_str());
        getline(input_string, temp_string);
        vol = stod(temp_string.c_str());

        market.update_stock(ticker, ipo, curr_share, prev_share, high, low, vol);
        line = "";
    }
}

void write_stocks(Market &market, ostream &os) {
    int num_companies = market.get_market_size();
    // for each company, do this
    Stock **companies = market.get_public_companies();
    for (int i = 0; i < num_companies; i++) {
        Stock *company = companies[i];
        string ticker, company_name, temp;
        string ipo, price, prev_price, high, low, vol;

        // String info
        ticker = company->get_ticker();
        company_name = company->get_company_name();

        // Double info
        ipo = format_output_string(company->get_ipo_price());
        price = format_output_string(company->get_share_price());
        prev_price = format_output_string(company->get_previous_share_price());
        high = format_output_string(company->get_high_price());
        low = format_output_string(company->get_low_price());
        vol = format_output_string(company->get_volatility());

        os << ticker << "," << company_name << "," << ipo << "," << price << "," << prev_price << "," << high << "," << low << "," << vol << endl;
    }
}

// Global variable read/write
void read_global_variables(istream &is) {
    string line = "";
    getline(is, line);
    stringstream input_string(line);
    string temp_string = "";

    // Set holdings, display_user_info, string_input
    getline(input_string, temp_string, ',');
    holdings = stoi(temp_string);
    getline(input_string, temp_string, ',');
    display_user_info = stoi(temp_string);
    getline(input_string, temp_string, ',');
    string_input = stoi(temp_string);

    // Set tabs
    getline(input_string, temp_string, ',');
    social_tab = stoi(temp_string);
    getline(input_string, temp_string, ',');
    learning_tab = stoi(temp_string);
    getline(input_string, temp_string, ',');
    trade_tab = stoi(temp_string);
    getline(input_string, temp_string, ',');
    leaderboard_tab = stoi(temp_string);
    getline(input_string, temp_string, ',');
    profile_tab = stoi(temp_string);
    getline(input_string, temp_string, ',');
    all_tabs = stoi(temp_string);

    // Set iterations
    getline(input_string, temp_string, ',');
    iterate = stoi(temp_string);
    getline(input_string, temp_string, ',');
    iterations = stoi(temp_string);

    // Set ticker and shares
    getline(input_string, temp_string, ',');
    ticker = temp_string;
    getline(input_string, temp_string, ',');
    shares = stoi(temp_string);

    // Set user info
    getline(input_string, temp_string, ',');
    first_name = temp_string;
    getline(input_string, temp_string, ',');
    last_name = temp_string;
    getline(input_string, temp_string, ',');
    user_name = temp_string;
    getline(input_string, temp_string, ',');
    password = temp_string;
}

void write_global_variables(ostream &os) {
    os << holdings  << "," << display_user_info << "," << string_input << "," << social_tab << "," << learning_tab << "," << trade_tab << "," << leaderboard_tab << "," << profile_tab << "," << all_tabs << "," << iterate << "," << iterations << "," << ticker << "," << shares << "," << first_name << "," << last_name << "," << user_name << "," << password << endl;
}

// Profile read/write
void read_profile_variables(Profile &p1, istream &is) {
    string input;
    getline(is, input);
    p1.set_funds(stod(input));

    while (getline(is, input)) {
        string stock_ticker;
        int stock_shares;
        stringstream vals(input);
        string temp;

        getline(vals, stock_ticker, ',');
        getline(vals, temp, ',');
        stock_shares = stoi(temp);

        if (p1.get_shares(stock_ticker) == 0)
            p1.add(stock_ticker, stock_shares);
    }

    // Set profile attributes
    p1.set_username(user_name);
    p1.set_first_name(first_name);
    p1.set_last_name(last_name);
    p1.set_password(password);
}

void write_profile_variables(Profile &p1, ofstream &os) {
    os << p1.get_funds() << endl;
    Market &m = p1.get_market();
    Stock **companies = m.get_public_companies();
    for (int i = 0; i < m.get_market_size(); i++) {
        string ticker = companies[i]->get_ticker();
        if (p1.is_holding(ticker)) {
            int shares = p1.get_shares(ticker);
            os << ticker << ',' << shares << endl;
        }
    }
}

// Database functions
bool user_exists(const string& username) {
    ifstream database("user_database.txt");
    string account;
    while (getline(database, account)) {
        if (account.find(username) == 0) {
            database.close();
            return true;
        }
    }
    database.close();
    return false;
}

bool correct_password(const string& username, const string& pwd) {
    int max_data = 21;
    string account_data[max_data];
    ifstream database("user_database.txt");
    string account;
    string data_piece;
    while (getline(database, account)) {
        if (account.find(username) == 0) {
            int data_count = 0;
            stringstream ss(account);
            while (getline(ss, data_piece, ',') && data_count < max_data) {
                account_data[data_count++] = data_piece;
            }
            if (pwd == account_data[1]) {
                database.close();
                return true;
            }
            else {
                database.close();
                return false;
            }
        }
    }
    database.close();
    return false;
}

std::vector<std::string> user_info_vector(const string& username) {
    int max_data = 22;
    std::vector<std::string> account_data;
    ifstream database("user_database.txt");
    string account;
    string data_piece;
    while (getline(database, account)) {
        if (account.find(username) == 0) {
            int data_count = 0;
            stringstream ss(account);
            while (getline(ss, data_piece, ',') && data_count < max_data) {
                account_data.push_back(data_piece);
                data_count++;
            }
            database.close();
            return account_data;
        }
    }
    database.close();
    return account_data;
}

void add_to_user_database(Profile &p) {
    ofstream database("user_database.txt", std::ios::app);
    if (!database.is_open()) {
        cerr << "file not opening" << endl;
        return;
    }
    string data_string = p.profile_data_string(p.get_market());
    database << data_string << "\n";
    database.close();
}

void update_user_database(Profile &p) {
    string usnm = p.get_username();
    if (!user_exists(usnm)) {
        add_to_user_database(p);
    }
    else {
        ifstream readfile("user_database.txt");
        ofstream tempfile("temp.txt", std::ofstream::trunc);
        string account;
        while (getline(readfile, account)) {
            if (account.find(usnm) == 0) {
                tempfile << p.profile_data_string(p.get_market()) << "\n";
            } else {
                tempfile << account << "\n";
            }
        }
        readfile.close();
        tempfile.close();
        remove("user_database.txt");
        rename("temp.txt","user_database.txt");
    }
}

// Sorting function for leaderboard
bool sorting_function(const pair<string, double> &pair1, const pair<string, double> &pair2) {
    return pair1.second > pair2.second;
}

// Leaderboard function
void print_leaderboard(ostream &os) {
    string usnm = profile.get_username();
    std::vector<pair<std::string, double>> net_worths;
    const int max_data = 22;
    ifstream user_database("user_database.txt");
    if (!user_database.is_open()) {
        os << "Database file not opening" << endl;
        return;
    }
    string account;
    bool found_self = false;
    while (getline(user_database, account)) {
        stringstream ss(account);
        std::vector<std::string> account_data;
        string data_piece;
        int data_count = 0;
        while (getline(ss, data_piece, ',') && data_count < max_data) {
                account_data.push_back(data_piece);
                data_count++;
            }
        if (data_count < max_data) {
            continue;
        }
        double value = 0;
        try {
            value = stod(account_data[21]);
        } catch (const std::exception &e) {
            continue;
        }
        if (account_data[0] == usnm) {
            net_worths.push_back(std::make_pair(usnm, profile.get_net_worth()));
            found_self = true;
        }
        else {
            net_worths.push_back(std::make_pair(account_data[0], value));
        }
    }
    if (!found_self) {
        net_worths.push_back(std::make_pair(usnm, profile.get_net_worth()));
    }
    sort(net_worths.begin(), net_worths.end(), sorting_function);
    int num_users_to_display = min(static_cast<int>(net_worths.size()), 5);
    if (num_users_to_display == 0) {
        os << "No users to display" << endl;
    }
    else {
        if (num_users_to_display == 1) {
            os << "The highest profiting user is:" << endl;
        }
        else {
            os << "The top " << num_users_to_display << " highest profiting users are:" << endl;
        }
        for (int i=0; i<num_users_to_display; i++) {
            os << i+1 << ". " << net_worths[i].first << ": $" << net_worths[i].second-1000 << endl;
        }
        os << endl;
    }
}

// Helper functions
// Handles actions of a buy button
void handle_button_buy(Profile &p, string buy_ticker) {
    if (p.buy(buy_ticker, 1)) {
            std::cerr << "Successfully bought one share of " << buy_ticker << "!\n";
            this_thread::sleep_for(chrono::milliseconds(1000));
    }
    else {
        std::cerr << "Failed to buy one share of " << buy_ticker << "...\n";
        this_thread::sleep_for(chrono::milliseconds(1000));      
    }
}

// Handles actions of a sell button
void handle_button_sell(Profile &p, string sell_ticker) {
    if (p.sell(sell_ticker, 1)) {
            std::cerr << "Successfully bought one share of " << sell_ticker << "!\n";
            this_thread::sleep_for(chrono::milliseconds(1000));
    }
    else {
        std::cerr << "Failed to buy one share of " << sell_ticker << "...\n";
        this_thread::sleep_for(chrono::milliseconds(1000));      
    }
}

// Prints filler text after prompt is written to mem
void print_filler(int length, ostream &os) {
    for (int i = 0; i < length; i++) { os << '~'; }
}

// Resets all tab booleans
void reset_tabs() {
    social_tab = false;
    learning_tab = false;
    trade_tab = false;
    leaderboard_tab = false;
    profile_tab = false;
    all_tabs = false;
}

// Checks if string can be converted to an int
bool check_input(const string &input) {
    try {
        stoi(input);
    } catch (const std::invalid_argument & e) {
        std::cerr << "Invalid input... try again.\n";
        this_thread::sleep_for(chrono::milliseconds(1000));
        return 0;
    }
    return 1;
}

// Main functions and State class
// Current state of the program class
class State {
    string file_name;
    string curr_screen;
public:
    State(istream &is) { 
        // get screen
        getline(is, file_name);
        getline(is, curr_screen);

        // get market info
        ifstream in("stocks_curr.csv");
        read_stocks(SP, in);
        if (curr_screen == "None") {
            SP.update_all_stock_ipo();
        }

        // Init global variables
        bool just_starting = (file_name == "initial_state");
        ifstream in_global(just_starting ? "global_init.csv" : "global_curr.csv");
        read_global_variables(in_global);

        // Init profile variables
        ifstream in_profile(just_starting ? "profile_init.csv" : "profile_curr.csv");
        read_profile_variables(profile, in_profile);
    }

    void write_to(ostream &os) const { 
        // update curr screen
        os << "curr_state" << endl << curr_screen << endl;

        // update market info
        ofstream out("stocks_curr.csv");
        write_stocks(SP, out);

        // update global variables
        ofstream out_global("global_curr.csv");
        write_global_variables(out_global);

        // update profile info
        ofstream out_profile("profile_curr.csv");
        write_profile_variables(profile, out_profile);
    }

    string get_curr_screen() const { return curr_screen; }
    void update(string input, istream &is);
};

// Initialize and read in menu screens
void read_text_data() {
    ifstream fs("menu.txt");
    string key;
    for (int i = 0; i < 24; ++i) {
	    getline(fs, texts[key], '#');
	    texts[key].pop_back();
	    getline(fs, key);
    }
}

// Display current screen
void display(const State &state, ostream &os) {
    string screen = state.get_curr_screen();
    if (react) {
        // add first
        os << "~~~~~~~~~~~~~~~~~~~~~~~~~";  // 25 bytes of filler
        os << "Ticker" << '\0' << "Company Name" << '\0' << "Curr Price" << '\0' << "% Change" << '\0' << "Price/Share" << '\0' << "High" << '\0' << "Low" << '\0' << "Shares" << '\0' << "Value" << '\0' << "BUY" << '\0' << "SELL" << '\0';
        os << texts[screen] << '\0';

        // Filler after prompt
        int len = 0;
        while (texts[screen][len++]) {}
        print_filler(200 - len, os); // filler
        os << '\0';

        // Stock market info for yaml
        int num_companies = SP.get_market_size();
        Stock **companies = SP.get_public_companies();
        int next_add_index = stock_index;
        for (int i = 0; i < num_companies; i++) {
            string current;
            Stock *company = companies[i];

            current = company->get_ticker();
            os << current << '\0';
            market_company_indexs[i][0] = next_add_index;
            next_add_index += current.length() + 1;

            current = company->get_company_name();
            os << current << '\0';
            market_company_indexs[i][1] = next_add_index;
            next_add_index += current.length() + 1;

            current = company->get_string_share_price();
            os << current << '\0';
            market_company_indexs[i][2] = next_add_index;
            next_add_index += current.length() + 1;

            current = company->get_string_percent_difference_IPO();
            os << current << '\0';
            market_company_indexs[i][3] = next_add_index;
            next_add_index += current.length() + 1;

            current = company->get_string_price_difference_IPO();
            os << current << '\0';
            market_company_indexs[i][4] = next_add_index;
            next_add_index += current.length() + 1;

            current = company->get_string_high_price();
            os << current << '\0';
            market_company_indexs[i][5] = next_add_index;
            next_add_index += current.length() + 1;

            current = company->get_string_low_price();
            os << current << '\0';
            market_company_indexs[i][6] = next_add_index;
            next_add_index += current.length() + 1;
        }

        // Holdings info for yaml
        string names[] = {"APPL", "TSLA", "AMZN", "XCEL", "GOOG"};
        for (int i = 0; i < num_companies; i++) {
            string current;
            current = to_string(profile.get_shares(names[i]));
            os << current << '\0';
            holdings_company_indexs[i][0] = next_add_index;
            next_add_index += current.length() + 1;

            current = format_output_string(profile.get_share_value(names[i]));
            os << current << '\0';
            holdings_company_indexs[i][1] = next_add_index;
            next_add_index += current.length() + 1;
        }

        // Options
        if (display_user_info) {
            os << "Logged in as " << profile.get_username() << endl;
            os << "Funds: $" << profile.get_funds() << endl;
            os << endl;
        }
        if (iterate && iterations != 0) {
            SP.emulate_market_iteration(iterations, 0, os);
            iterate = 0;
            iterations = 0;
        }
        
        os << '\0';
        end_index = next_add_index;

        if (social_tab) {
            _add_yaml("social.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        else if (learning_tab) {
            _add_yaml("learning.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        else if (trade_tab) {
            _add_yaml("market.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"ticker_index", ticker_index},
                        {"company_name_index", company_name_index},
                        {"stock_price_index", stock_price_index},
                        {"percent_index", percent_index},
                        {"difference_index", difference_index},
                        {"high_index", high_index},
                        {"low_index", low_index},
                        {"company0_ticker", market_company_indexs[0][0]},
                        {"company0_name", market_company_indexs[0][1]},
                        {"company0_price", market_company_indexs[0][2]},
                        {"company0_percent", market_company_indexs[0][3]},
                        {"company0_difference", market_company_indexs[0][4]},
                        {"company0_high", market_company_indexs[0][5]},
                        {"company0_low", market_company_indexs[0][6]},
                        {"company1_ticker", market_company_indexs[1][0]},
                        {"company1_name", market_company_indexs[1][1]},
                        {"company1_price", market_company_indexs[1][2]},
                        {"company1_percent", market_company_indexs[1][3]},
                        {"company1_difference", market_company_indexs[1][4]},
                        {"company1_high", market_company_indexs[1][5]},
                        {"company1_low", market_company_indexs[1][6]},
                        {"company2_ticker", market_company_indexs[2][0]},
                        {"company2_name", market_company_indexs[2][1]},
                        {"company2_price", market_company_indexs[2][2]},
                        {"company2_percent", market_company_indexs[2][3]},
                        {"company2_difference", market_company_indexs[2][4]},
                        {"company2_high", market_company_indexs[2][5]},
                        {"company2_low", market_company_indexs[2][6]},
                        {"company3_ticker", market_company_indexs[3][0]},
                        {"company3_name", market_company_indexs[3][1]},
                        {"company3_price", market_company_indexs[3][2]},
                        {"company3_percent", market_company_indexs[3][3]},
                        {"company3_difference", market_company_indexs[3][4]},
                        {"company3_high", market_company_indexs[3][5]},
                        {"company3_low", market_company_indexs[3][6]},
                        {"company4_ticker", market_company_indexs[4][0]},
                        {"company4_name", market_company_indexs[4][1]},
                        {"company4_price", market_company_indexs[4][2]},
                        {"company4_percent", market_company_indexs[4][3]},
                        {"company4_difference", market_company_indexs[4][4]},
                        {"company4_high", market_company_indexs[4][5]},
                        {"company4_low", market_company_indexs[4][6]},
                        {"shares_index", shares_index},
                        {"value_index", value_index},
                        {"company0_shares", holdings_company_indexs[0][0]},
                        {"company0_value", holdings_company_indexs[0][1]},
                        {"company1_shares", holdings_company_indexs[1][0]},
                        {"company1_value", holdings_company_indexs[1][1]},
                        {"company2_shares", holdings_company_indexs[2][0]},
                        {"company2_value", holdings_company_indexs[2][1]},
                        {"company3_shares", holdings_company_indexs[3][0]},
                        {"company3_value", holdings_company_indexs[3][1]},
                        {"company4_shares", holdings_company_indexs[4][0]},
                        {"company4_value", holdings_company_indexs[4][1]},
                        {"button_index", button_index},
                        {"button_sell_index", button_sell_index},
                        {"end_index", end_index}});
        }
        else if (leaderboard_tab) {
            _add_yaml("leaderboard.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        else if (profile_tab) {
            _add_yaml("profile.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        else if (display_user_info) {
            _add_yaml("all_tabs.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        else if (all_tabs) {
            _add_yaml("all_tabs.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        else {
            _add_yaml("login_screen.yaml", {{"prompt_index", prompt_index},
                        {"input_index", input_index},
                        {"end_index", end_index}});
        }
        
    } else {
        os << "\033c";

        if (display_user_info) {
            os << "Logged in as " << profile.get_username() << endl;
            os << "Funds: $" << profile.get_funds() << endl;
            os << endl;
        }
        if (iterate && iterations != 0) {
            SP.emulate_market_iteration(iterations, 1, os);
            iterate = 0;
            iterations = 0;
        }
        else if (trade_tab) {
            SP.print_market(os);
        }
        if (holdings)
            profile.print_holdings(os);
        if (profile_tab) {
            profile.print_profile(os);
        }
        if (leaderboard_tab) {
            print_leaderboard(os);
        }

        // Print prompt
        os << texts[screen] << '\0';
        
        if (react) os << '\0';  // provide ending null-byte
        else if (screen == "end") exit(0);
    }
}

// Logic of menu system
void State::update(string input, istream &is) {
    if (react && _received_event()) {
        if (_event_id_is("b", 0)) {
            handle_button_buy(profile, "APPL");
            return;
        }
        else if (_event_id_is("b", 1)) {
            handle_button_buy(profile, "TSLA");
            return;
        }
        else if (_event_id_is("b", 2)) {
            handle_button_buy(profile, "AMZN");
            return;
        }
        else if (_event_id_is("b", 3)) {
            handle_button_buy(profile, "XCEL");
            return;
        }
        else if (_event_id_is("b", 4)) {
            handle_button_buy(profile, "GOOG");
            return;
        }
        else if (_event_id_is("s", 0)) {
            handle_button_sell(profile, "APPL");
            return;
        }
        else if (_event_id_is("s", 1)) {
            handle_button_sell(profile, "TSLA");
            return;
        }
        else if (_event_id_is("s", 2)) {
            handle_button_sell(profile, "AMZN");
            return;
        }
        else if (_event_id_is("s", 3)) {
            handle_button_sell(profile, "XCEL");
            return;
        }
        else if (_event_id_is("s", 4)) {
            handle_button_sell(profile, "GOOG");
            return;
        }
        else if (_event_id_is("t", 0)) {
            curr_screen = "social_menu";
            social_tab = true;
            return;
        }
        else if (_event_id_is("t", 1)) {
            reset_tabs();
            curr_screen = "learning_menu";
            learning_tab = true;
            return;
        }
        else if (_event_id_is("t", 2)) {
            reset_tabs();
            curr_screen = "trade_menu";
            trade_tab = true;
            return;
        }
        else if (_event_id_is("t", 3)) {
            reset_tabs();
            curr_screen = "leaderboard_menu";
            leaderboard_tab = true;
            return;
        }
        else if (_event_id_is("t", 4)) {
            reset_tabs();
            curr_screen = "profile_menu";
            profile_tab = true;
            return;
        }
        else if (_event_id_is("h", 0)) {
            reset_tabs();
            curr_screen = "all_tabs";
            all_tabs = true;
            return;
        }
        else if (_event_id_is("l", 0)) {
            reset_tabs();
            cerr << "Thank you for using StockSquad\n";
            this_thread::sleep_for(chrono::milliseconds(1000));
            curr_screen = "end";
            display_user_info = false;
            update_user_database(profile);
            return;
        }

        // get input from the stream
        else if ('~' != is.peek()) getline(is, input, '\0');
    }

    // starting sequence
    if (curr_screen == "None") curr_screen = "start_screen";
    else if (curr_screen == "start_screen") curr_screen = "login_choice";
    else if (curr_screen == "end") curr_screen = "start_screen"; // reset

    else {    
        // int input
        if (!string_input) {
            if (check_input(input)) {
                int input_num;
                input_num = stoi(input);
                if (curr_screen == "login_choice") {
                    if (input_num == 1) {
                        curr_screen = "login_exist_user";
                        string_input = true;
                    }
                    else if (input_num == 2) {
                        curr_screen = "login_create_user";
                        string_input = true;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "all_tabs") {
                    if (input_num == 1) {
                        curr_screen = "trade_menu";
                        trade_tab = true;
                        all_tabs = false;
                    }
                    else if (input_num == 2) {
                        curr_screen = "social_menu";
                        social_tab = true;
                        all_tabs = false;
                    }
                    else if (input_num == 3) {
                        curr_screen = "learning_menu";
                        learning_tab = true;
                        all_tabs = false;
                    }
                    else if (input_num == 4) {
                        curr_screen = "leaderboard_menu";
                        leaderboard_tab = true;
                        all_tabs = false;
                    }
                    else if (input_num == 5) {
                        curr_screen = "profile_menu";
                        profile_tab = true;
                        all_tabs = false;
                    }
                    else if (input_num == 6) {
                        curr_screen = "quit_screen";
                        all_tabs = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "trade_menu") {
                    if (input_num == 1) {
                        curr_screen = "trade_iterate";
                        iterate = true;
                    }
                    else if (input_num == 2) {
                        curr_screen = "trade_buy_tick";
                        string_input = true;
                    }
                    else if (input_num == 3) {
                        curr_screen = "trade_sell_tick";
                        string_input = true;
                    }
                    else if (input_num == 4) {
                        curr_screen = "trade_holdings";
                        holdings = true;
                    }
                    else if (input_num == 5) {
                        curr_screen = "all_tabs";
                        all_tabs = true;
                        trade_tab = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "trade_iterate") {
                    // SP.emulate_market_iteration(input_num, 1, cout);
                    iterations = input_num;
                    curr_screen = "trade_menu";
                }
                else if (curr_screen == "trade_buy_share") {
                    shares = input_num;
                    if (profile.buy(ticker, shares)) {
                        std::cerr << "Success!\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                    else {
                        std::cerr << "Failed...\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));      
                    }
                    curr_screen = "trade_menu";
                }
                else if (curr_screen == "trade_sell_share") {
                    shares = input_num;
                    if (profile.sell(ticker, shares)) {
                        std::cerr << "Success!\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                    else {
                        std::cerr << "Failed...\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));      
                    }
                    curr_screen = "trade_menu";
                }
                else if (curr_screen == "trade_holdings") {
                    if (input_num == 1) {
                        curr_screen = "trade_menu";
                        holdings = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "social_menu") {
                    if (input_num == 1) {
                        curr_screen = "all_tabs";
                        all_tabs = true;
                        social_tab = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "learning_menu") {
                    if (input_num == 1) {
                        curr_screen = "all_tabs";
                        all_tabs = true;
                        learning_tab = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "leaderboard_menu") {
                    if (input_num == 1) {
                        curr_screen = "all_tabs";
                        all_tabs = true;
                        leaderboard_tab = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "profile_menu") {
                    if (input_num == 1) {
                        curr_screen = "view_another_profile";
                    }
                    else if (input_num == 2) {
                        curr_screen = "all_tabs";
                        all_tabs = true;
                        profile_tab = false;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "view_another_profile") {
                    if (input_num == 1) {
                        profile_tab = true;
                        curr_screen = "profile_menu";
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else if (curr_screen == "quit_screen") {
                    if (input_num == 1) {
                        cout << "Thank you for using StockSquad\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                        curr_screen = "end";
                        display_user_info = false;
                        update_user_database(profile);
                    }
                    else if (input_num == 2) {
                        curr_screen = "all_tabs";
                        all_tabs = true;
                    }
                    else {
                        std::cerr << "Not an option... try again.\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
            }
            // Other wise stay on same page and try again
        }
        // string input
        else if (string_input) {
            if (curr_screen == "login_exist_user") {
                if (!user_exists(input)) {
                    cout << "Username does not exist... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                else {
                    user_name = input;
                    curr_screen = "login_exist_pass";
                    profile.set_username(input);
                }
            }
            else if (curr_screen == "login_exist_pass") {
                if (!correct_password(profile.get_username(),input)) {
                    cout << "Incorrect password... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                else {
                    profile = Profile(user_info_vector(profile.get_username()), SP);
                    password = input;
                    first_name = profile.get_first_name();
                    last_name = profile.get_last_name();
                    curr_screen = "all_tabs";
                    all_tabs = true;
                    display_user_info = true;
                    string_input = false;
                    ofstream out("stocks_curr.csv");
                    write_stocks(SP, out);
                    out.close();
                    ofstream out_global("global_curr.csv");
                    write_global_variables(out_global);
                    out_global.close();
                    ofstream out_profile("profile_curr.csv");
                    write_profile_variables(profile, out_profile);
                    out_profile.close();
                }
            }
            else if (curr_screen == "login_create_user") {
                if (input.length() > 16 or input.length() < 4) {
                    cout << "Username must be 4-16 characters... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                else {
                    user_name = input;
                    curr_screen = "login_create_pass";
                    profile.set_username(input);
                    
                }
            }
            else if (curr_screen == "login_create_pass") {
                if (input.length() > 16 or input.length() < 4) {
                    cout << "Password must be 4-16 characters... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                else {
                    password = input;
                    curr_screen = "login_create_pass_2";
                }
            }
            else if (curr_screen == "login_create_pass_2") {
                if (password == input) {
                    curr_screen = "login_create_fname";
                    profile.set_password(input);
                }
                else {
                    cout << "Password didn't match... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }     
            }
            else if (curr_screen == "login_create_fname") {
                if (input.length() < 1) {
                    cout << "First name must include at least one character... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                else {
                    first_name = input;
                    curr_screen = "login_create_lname";
                    profile.set_first_name(input);
                }
            }
            else if (curr_screen == "login_create_lname") {
                if (input.length() < 1) {
                    cout << "Last name must include at least one character... try again.\n";
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
                else {
                    last_name = input;
                    string_input = false;
                    display_user_info = true;
                    curr_screen = "all_tabs";
                    all_tabs = true;
                    profile.set_last_name(input);
                }
            }
            else if (curr_screen == "trade_buy_tick") {
                ticker = input;
                curr_screen = "trade_buy_share";
                string_input = false;
            }
            else if (curr_screen == "trade_sell_tick") {
                ticker = input;
                curr_screen = "trade_sell_share";
                string_input = false;
            }
        }
    }
}

int main(int argc, char **argv) {
    react = (argc > 1);
    read_text_data();

    bool just_starting = react ? _read_event_info() : true;
    while(1) {
        ifstream fs1(just_starting ? "initial_state" : "curr_state");
        State state(fs1);
        fs1.close();

        string input;
        if (just_starting) just_starting = false;
        else if (!react) getline(cin, input);

        ifstream fs2("incoming_text");
        state.update(input, fs2);

        ofstream fs3("outgoing_text"), fs4(react ? "end_state" : "curr_state");
        display(state, react ? fs3 : cout);
        state.write_to(fs4);

        if (react) {
            _write_react_yaml();
            break;
        }
    }
}
