#ifndef __MARKET_H__
#define __MARKET_H__

#include <map>
#include "Stock.h"

// Class for market structure
class Market {
    int num_public_companies;
    int market_max_size;
    Stock **public_companies;
    std::map<std::string, Stock*> company_lookup;

    public:
    Market();
    
    Market(int size);

    int get_market_size() const;

    int get_max_market_size() const;

    void print_market(std::ostream &os);

    bool lookup_stock(std::string st);

    Stock *get_stock(std::string st);

    void emulate_market_iteration(int iterations, bool print, std::ostream &os);

    bool add_stock(std::string st, std::string cn, double ipo, double vol);

    bool add_stock(std::string st, std::string cn, double ipo, double curr, double prev, double high, double low, double vol);

    bool update_stock(std::string st, double ipo, double curr, double prev, double high, double low, double vol);

    bool update_stock_ipo(std::string st, double ipo);

    void update_all_stock_ipo();

    Stock **get_public_companies();

    ~Market();
};

class SandP500 : public Market {
    public:
    SandP500();
};

#endif