#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
	virtual Customer* clone() const = 0;
	virtual ~Customer();
    bool orderCheapest(std::vector<int>&output,const std::vector<Dish> &menu, DishType type);
    bool orderExpensive(std::vector<int>&output,const std::vector<Dish> &menu, DishType type);
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;

private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
	virtual VegetarianCustomer* clone() const;
	VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
};


class CheapCustomer : public Customer {
public:

	virtual CheapCustomer* clone() const;
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
    bool firstOrder;
};


class SpicyCustomer : public Customer {
public:
	virtual SpicyCustomer* clone() const;
	SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
    bool firstOrder;
};


class AlchoholicCustomer : public Customer {
public:
	virtual AlchoholicCustomer* clone() const;
	AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
    bool firstOrder;
    int lastOrderPrice;
    int lastOrderId;

};


#endif