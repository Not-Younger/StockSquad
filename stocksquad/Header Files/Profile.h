#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <map>
#include "Stock.h"
#include "Market.h"

using namespace std;

std::string hash_password(std::string pass);

// Class for profile structure
class Profile {
    std::string username;
    std::string password;
    std::string first_name;
    std::string last_name;
    double funds;
    Market &market;
    std::map<std::string, int> holdings;
    int total_posts;
    double net_worth;

    static int num_profiles;

    public:
    Profile(Market &m);
    Profile(std::string u_name, double money, Market &m);
    Profile(std::string u_name, std::string pwd, double money, Market &m);
    Profile(std::string u_name, std::string pwd, std::string f_name, std::string l_name, double money, Market &m);
    Profile(std::vector<std::string> vector_info, Market &m);

    Profile &operator=(const Profile &x);

    bool buy(std::string st, int shares);

    bool sell(std::string st, int shares);

    bool add(std::string st, int shares);

    bool remove(std::string st, int shares);

    void print_holdings(std::ostream &os);

    bool is_holding(std::string st);

    std::map<std::string, int> get_holdings() const;

    int get_shares(std::string st);

    double get_share_value(std::string st);

    Market &get_market() const;

    double get_funds() const;

    double get_net_worth();

    std::string get_username() const;

    std::string get_password() const;

    std::string get_first_name() const;

    std::string get_last_name() const;

    int get_total_posts() const;

    void set_funds(double money);

    void set_username(std::string u_name);

    void set_password(std::string pwd);

    void set_first_name(std::string f_name);

    void set_last_name(std::string l_name);

    void print_profile(std::ostream &os);

    std::string profile_data_string(Market &m);
};
#endif
