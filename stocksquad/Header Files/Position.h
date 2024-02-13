#ifndef __POSITION_H__
#define __POSITION_H__

#include<iostream>
#include"Stock.h"
#include<unordered_map>

// The "position" class will be used to keep track of all the relevant info about all interactions made by a user with a stock
// The goal is to have 1 position for every stock bought or sold for each user

class Position {/*: protected Stock {
  FROM THE STOCK CLASS 

  std::string ticker;
  std::string company_name;
  double IPO;
  double current_share_price;
  double previous_share_price;
  double high_price, low_price;
  double volatility;  */
  Stock* stock;
  int num_shares_owned;
  double cost_basis; // total $ amount invested into the stock

public:
  bool buy(int howMany);
  bool sell(int howMany);
  // double get_share_price(); FROM STOCK CLASS or
  double getPrice(); // stock->get_share_price();
  double getPL(); // PL: Profit and Loss
  double getPLPerShare();
  double getCost();
  double getCostPerShare();
};

#endif