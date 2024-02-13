#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
#include "../Header Files/Stock.h"

mt19937 mt(std::random_device{}());

string format_output_string(double input) {
    string output;
    string input_str = to_string(input);
    int i;
    int len = input_str.length();
        
    for (i = 0; i < len; i++) {
        output += input_str[i];
        if (input_str[i] == '.')
            break;
    }
    i++;
    if (i < len - 1) {
        output += input_str[i];
        output += input_str[i+1];
    }
    else if (i < len) {
        output += input_str[i];
        output += '0';
    }
    else
        output += "00";
    return output;
}

Stock::Stock(string stock_ticker, string comp_name, double initial_public_offering, double stock_vol) : ticker(stock_ticker), company_name(comp_name), IPO(initial_public_offering), volatility(stock_vol) {
    current_share_price = previous_share_price = high_price = low_price = IPO;
}

Stock::Stock(string stock_ticker, string comp_name, double initial_public_offering, double current, double previous, double high, double low, double stock_vol) : ticker(stock_ticker), company_name(comp_name), IPO(initial_public_offering), current_share_price(current), previous_share_price(previous), high_price(high), low_price(low), volatility(stock_vol) { }

double Stock::get_ipo_price() const {
    return IPO;
}

double Stock::get_share_price() const {
    return current_share_price;
}

double Stock::get_previous_share_price() const {
    return previous_share_price;
}

double Stock::get_high_price() const {
    return high_price;
}

double Stock::get_low_price() const {
    return low_price;
}

double Stock::get_volatility() const {
    return volatility;
}

double Stock::get_price_difference_IPO() const {
    return current_share_price - IPO;
}

double Stock::get_percent_difference_IPO() const {
    return ((current_share_price - IPO) / IPO) * 100;
}

string Stock::get_ticker() const {
    return ticker;
}

string Stock::get_company_name() const {
    return company_name;
}

std::string Stock::get_string_share_price() const {
    return format_output_string(current_share_price);
}

std::string Stock::get_string_percent_difference_IPO() const {
    return format_output_string(get_percent_difference_IPO());
}

std::string Stock::get_string_price_difference_IPO() const {
    return format_output_string(get_price_difference_IPO());
}

std::string Stock::get_string_high_price() const {
    return format_output_string(high_price);
}

std::string Stock::get_string_low_price() const {
    return format_output_string(low_price);
}

void Stock::set_ipo_price(double price) {
    IPO = price;
}

void Stock::set_share_price(double price) {
    current_share_price = price;
}

void Stock::set_previous_share_price(double price) {
    previous_share_price = price;
}

void Stock::set_high_price(double price) {
    high_price = price;
}

void Stock::set_low_price(double price) {
    low_price = price;
}

void Stock::set_volatility(double price) {
    volatility = price;
}

void Stock::simulate_market_movement() {
    int sign;
    previous_share_price = current_share_price;
    uniform_int_distribution<int> positive_or_negative(0, 1);
    sign = (positive_or_negative(mt) == 0) ? 1 : -1;
    uniform_int_distribution<int> fluctuation_range(1, volatility);
    double percent_movement = (fluctuation_range(mt) / 100.0) * sign;
    current_share_price += percent_movement;

    // Check if price is negative
    if (current_share_price < 0)
        current_share_price = 0;

    // Updates high and low prices
    if (current_share_price > high_price)
        high_price = current_share_price;
    if (current_share_price < low_price)
        low_price = current_share_price;
}

void Stock::display_stock_info(ostream &os) {
    string current_body_str, percent_body_str, price_body_str, high_body_str, low_body_str;
    string current_share_price_str, percent_difference_str, price_difference_str, high_str, low_str;
    double price_difference = get_price_difference_IPO();
    double percent_difference = get_percent_difference_IPO();
    string arrow, color_arrow;

    // Determine color (red/green)
    if (price_difference > 0) {
        arrow = "\u2191";
        color_arrow = "\033[1;32m" + arrow + "\033[0m ";
    }
    else if (price_difference < 0) {
        arrow = "\u2193";
        color_arrow = "\033[1;31m" + arrow + "\033[0m ";   
    }
    else {
        arrow = "= ";
        color_arrow = arrow;
    }

    // Format strings numbers
    current_body_str = format_output_string(current_share_price);
    percent_body_str = format_output_string(percent_difference);
    price_body_str = format_output_string(price_difference);
    high_body_str = format_output_string(high_price);
    low_body_str = format_output_string(low_price);

    // Format outputs
    current_share_price_str = "$ " + current_body_str;
    percent_difference_str = "% " + percent_body_str;
    price_difference_str = "$ " + price_body_str;
    high_str = "$ " + high_body_str;
    low_str = "$ " + low_body_str;

    os << "----------------------------------------------------------------------------------------------------------\n";
    os << "| " << left << setw(10) << ticker << " | " << left << setw(20) << company_name << " | " << left << setw(10) << current_share_price_str << " | " << color_arrow << right << setw(10) << percent_difference_str << " | " << color_arrow << right << setw(10) << price_difference_str << " | " << right << setw(10) << high_str << " | " << right << setw(10) << low_str << " |" << endl;
}
