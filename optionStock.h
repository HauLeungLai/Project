#ifndef OPTIONSTOCK_H
#define OPTIONSTOCK_H

#include "stock.h"
#include <string>

// OptionStock class, derived from Stock class
class OptionStock : public Stock {
private:
    double strikePrice;
    std::string expirationDate;

public:
    // Constructor
    OptionStock(const std::string& ticker, const std::string& name, double price, double strikePrice, const std::string& expirationDate);

    // Getter for the strike price
    double getStrikePrice() const;

    // Getter for the expiration date
    std::string getExpirationDate() const;

    // Overriding the getAssetType() method
    std::string getAssetType() const override;
};

#endif // OPTIONSTOCK_H
