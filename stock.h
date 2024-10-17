#ifndef STOCK_H
#define STOCK_H

#include "financialAsset.h"
#include <string>

class Stock : public FinancialAsset {
protected:
    std::string companyName;

public:
    // Constructor
    Stock(const std::string& ticker, const std::string& name, double price);

    // Getters
    std::string getCompanyName() const;

    // Virtual method to get asset type
    std::string getAssetType() const override;

    // Destructor
    ~Stock() override = default;
};

#endif // STOCK_H