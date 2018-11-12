#include <sstream>
#include "../include/Customer.h"


//--------------------------------Customer-------------------------------------------------


//Constructor Memory initialization
Customer::Customer(std::string c_name, int c_id):name(std::move(c_name)),id(c_id){}

//Destructor
Customer::~Customer(){}

//Returns the Customer's name
std::string Customer:: getName() const{
    return this->name;
}

//Returns the Customer's ID number
int Customer:: getId() const {
    return this->id;
}



bool Customer::orderCheapest(std::vector<int>&output,const std::vector<Dish> &menu, DishType type) {
    int low_price=0;
    int type_index=-1;
    bool type_flag= false;
    for(unsigned int i=0;i<menu.size();i++) {
        if (menu[i].getType() == type) {
            if (!type_flag) {
                type_index = menu[i].getId();
                low_price = menu[i].getPrice();
                type_flag = true;
            }
            if (menu[i].getPrice() == low_price && menu[i].getId() < type_index)
                type_index = menu[i].getId();
            if (menu[i].getPrice() < low_price) {
                low_price = menu[i].getPrice();
                type_index = menu[i].getId();
            }
        }
    }
    if(type_flag) {
        output.push_back(type_index);
        return true;
    }
    return false;
}

bool Customer::orderExpensive(std::vector<int>&output,const std::vector<Dish> &menu, DishType type) {
    int high_price = 0;
    int type_index = -1;
    bool type_flag = false;
    for (unsigned int i = 0; i < menu.size(); i++) {
        if (menu[i].getType() == type) {
            if (!type_flag) {
                type_index = menu[i].getId();
                type_flag = true;
            }
            if (menu[i].getPrice() == high_price && menu[i].getId() < type_index)
                type_index = menu[i].getId();
            if (menu[i].getPrice() > high_price) {
                high_price = menu[i].getPrice();
                type_index = menu[i].getId();
            }
        }
    }
    if (type_flag) {
        output.push_back(type_index);
        return true;
    }
    return false;
}

//--------------------------------VegetarianCustomer-------------------------------------------------

VegetarianCustomer ::VegetarianCustomer(std::string name, int id) : Customer(std::move(name),id){}

std::vector<int> VegetarianCustomer:: order(const std::vector<Dish> &menu){
    std::vector<int> output;
    if(menu.empty())//check if dish menu is empty - if so return empty vector
        return output;


    bool veg= false;
    int low_id=-1;
    for(unsigned int i=0;i<menu.size();i++) {
        if (menu[i].getType() == VEG) {
            if (!veg) {
                low_id = menu[i].getId();
                veg = true;
            }
            if (menu[i].getId() < low_id) //check if current VEG ID number is lower than the low_id
                low_id = menu[i].getId();
        }
    }
    if(veg && orderExpensive(output,menu,BVG)) //will add to output most expensive BVG is exists
         output.push_back(low_id); // will add the lowest id veg Dish
    return output;
}



VegetarianCustomer* VegetarianCustomer:: clone() const{
    return new VegetarianCustomer(*this);
}

std::string VegetarianCustomer::toString() const {
    std::stringstream ss;
    ss <<this->getName()<<",veg";
    return ss.str();
}

//--------------------------------CheapCustomer-------------------------------------------------
CheapCustomer::CheapCustomer(std::string name, int id): Customer(std::move(name),id) , firstOrder(false){}

std::vector<int> CheapCustomer:: order(const std::vector<Dish> &menu){
    std::vector<int> output;
    if(!firstOrder) { //CheapCustomer can only order one time
        if (menu.empty())//check if dish menu is empty - if so return empty vector
            return output;
        int low_price = menu[0].getPrice();
        int low_price_id = menu[0].getId();
        for (unsigned int j = 1; j < menu.size(); j++) {
            int dishPrice = menu[j].getPrice();
            int dishID = menu[j].getId();
            if (dishPrice == low_price && dishID < low_price_id)//if prices equal - check by lower id
                low_price_id = dishID;
            else if (dishPrice < low_price) { //checks if the current Dish price is lower than low_price
                low_price = dishPrice;
                low_price_id=dishID;
            }
        }
        output.push_back(low_price_id);
        firstOrder=true;
    }
    return output;
}


//clone the CheapCustomer object
CheapCustomer* CheapCustomer:: clone() const{
    return new CheapCustomer(*this);
}



std::string CheapCustomer::toString() const {
    std::stringstream ss;
    ss <<this->getName()<<",chp";
    return ss.str();
}


//--------------------------------SpicyCustomer-------------------------------------------------
//Constructor
SpicyCustomer ::SpicyCustomer(std::string name, int id): Customer(std::move(name),id),firstOrder(false) {}

//clone the SpicyCustomer object
SpicyCustomer* SpicyCustomer:: clone() const{
    return new SpicyCustomer(*this);
}


std::vector<int> SpicyCustomer:: order(const std::vector<Dish> &menu){
    std::vector<int> output;
    if (menu.empty())//check if dish menu is empty - if so return empty vector
        return output;
    if(!firstOrder) { // SpicyCustomer first order
        orderExpensive(output, menu, SPC);//will order the most expensive spicy dish if exist
        firstOrder = true;
    }
    else
        orderCheapest(output,menu,BVG);//will order the cheapest non Alc  if exist

    return output;
}

std::string SpicyCustomer::toString() const {
    std::stringstream ss;
    ss <<this->getName()<<",spc";
    return ss.str();
}


//--------------------------------AlchoholicCustomer-----------------------------------------------------------------------------------------------------

//AlchoholicCustomer Constructor
AlcoholicCustomer ::AlcoholicCustomer(std::string name, int id):Customer(std::move(name),id),firstorder(false),flag(false),lastOrderPrice(0),lastOrderId(-1){}


//clone AlchoholicCustomer object
AlcoholicCustomer* AlcoholicCustomer:: clone() const{
    return new AlcoholicCustomer(*this);
}

std::vector<int> AlcoholicCustomer:: order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if (menu.empty())
        return output;
    if (!firstorder && orderCheapest(output, menu, ALC)) {
        lastOrderId = output[0];
        for (unsigned int j = 0; j < menu.size(); j++) {
            if (lastOrderId == menu[j].getId())
                lastOrderPrice = menu[j].getPrice();
        }
        firstorder=true;
    } else {
        bool alc= false;
        int tempMin = lastOrderPrice;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == ALC) {
                if (!flag && tempMin < menu[i].getPrice()) {
                        tempMin = menu[i].getPrice();
                        lastOrderId = menu[i].getId();
                        alc=true;
                        flag = true;
                    }

                if (menu[i].getPrice() > lastOrderPrice && menu[i].getPrice() == tempMin &&
                    menu[i].getId() < lastOrderId) {
                    tempMin = menu[i].getPrice();
                    lastOrderId = menu[i].getId();
                    alc=true;
                }
                if (menu[i].getPrice() > lastOrderPrice && menu[i].getPrice() < tempMin) {
                    tempMin = menu[i].getPrice();
                    lastOrderId = menu[i].getId();
                    alc=true;
                }
            }
        }
        if (alc) {
            lastOrderPrice = tempMin;
            output.push_back(lastOrderId);
        }
    }
    return output;
}



std::string AlcoholicCustomer::toString() const {
    std::stringstream ss;
    ss <<this->getName()<<",alc";
    return ss.str();
}
//-------------------------------------------------------------------------------------------------------------------------------


