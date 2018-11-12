#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
bool compAlc (std::pair<int,int> i, std::pair<int,int> j);


enum CustomerType{
	VEGT, CHP,SPCY, ALCO,
};


//----------------------------Customer------------------------------------------
class Customer{

public:
    //Constructor
    Customer(std::string c_name, int c_id);

    //Destructor
	virtual ~Customer();


	//-----------------------Methods------------------------------
    bool orderCheapest(std::vector<int>&output,const std::vector<Dish> &menu, DishType type);
    bool orderExpensive(std::vector<int>&output,const std::vector<Dish> &menu, DishType type);

    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone() const = 0;
//------------------Members-----------------------
private:
    const std::string name;
    const int id;
};

//-----------------VegetarianCustomer--------------------------
class VegetarianCustomer : public Customer {
public:
	//Constructor
	VegetarianCustomer(std::string name, int id);

	//------------Methods--------------------------
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual VegetarianCustomer* clone() const;

    //--------Members-----------------
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

//---------------------SpicyCustomer----------------------------
class SpicyCustomer : public Customer {
public:
	//Constructor
	SpicyCustomer(std::string name, int id);

	//---------------Methods-----------------------------
    virtual SpicyCustomer* clone() const;
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    //-------------Members--------------------
private:
    bool firstOrder;
};

//--------------------AlcoholicCustomer-------------------------------------
class AlcoholicCustomer : public Customer {
public:
	//Constructor
	AlcoholicCustomer(std::string name, int id);

	//----------------Methods------------------------------
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual AlcoholicCustomer* clone() const;

    //-------------Members-----------------------------------
private:
	unsigned int currentIndex;
};


#endif