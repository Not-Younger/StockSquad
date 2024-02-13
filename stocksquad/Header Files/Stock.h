#ifndef __STOCK_H__
#define __STOCK_H__

#include <random>
#include <fstream>

// Random function
extern std::mt19937 mt;

// String formating function for displaing stocks
std::string format_output_string(double input);

// Class for stock structure
class Stock {
    std::string ticker;
    std::string company_name;
    double IPO;
    double current_share_price;
    double previous_share_price;
    double high_price, low_price;
    double volatility;

    public:
    Stock(std::string stock_ticker, std::string comp_name, double initial_public_offering, double stock_vol);
    Stock(std::string stock_ticker, std::string comp_name, double initial_public_offering, double current, double previous, double high, double low, double stock_vol);

    double get_ipo_price() const;

    double get_share_price() const;

    double get_previous_share_price() const;

    double get_high_price() const;

    double get_low_price() const;

    double get_volatility() const;

    double get_price_difference_IPO() const;

    double get_percent_difference_IPO() const;

    std::string get_ticker() const;

    std::string get_company_name() const;

    std::string get_string_share_price() const;

    std::string get_string_percent_difference_IPO() const;

    std::string get_string_price_difference_IPO() const;

    std::string get_string_high_price() const;

    std::string get_string_low_price() const;

    void set_ipo_price(double price);

    void set_share_price(double price);

    void set_previous_share_price(double price);

    void set_high_price(double price);

    void set_low_price(double price);

    void set_volatility(double price);

    void simulate_market_movement();
    
    void display_stock_info(std::ostream &os);
};

#endif