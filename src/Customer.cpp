#include "../include/Customer.h"

//--------------------------------Customer-------------------------------------------------
//Constructor Memory initialization
Customer::Customer(std::string c_name, int c_id):name(std::move(c_name)),id(c_id){}

//Returns the Customer's name
std::string Customer:: getName() const{
    return this->name;
}

//Returns the Customer's ID number
int Customer:: getId() const {
    this->id;
}

//--------------------------------VegetarianCustomer-------------------------------------------------

VegetarianCustomer ::VegetarianCustomer(std::string name, int id) : Customer(std::move(name),id){}

std::vector<int> VegetarianCustomer:: order(const std::vector<Dish> &menu){
    std::vector<int> output;
    if(menu.empty())//check if dish menu is empty - if so return empty vector
        return output;
    bool bvg= false;
    bool veg= false;
    int low_id=menu[0].getId();
    int exp_bvg=0;
    int bvg_id=-1;
    for(int i=0;i<menu.size();i++){
        int dishPrice=menu[i].getPrice();
        int dishID=menu[i].getId();
        DishType dishType=menu[i].getType();
        switch (dishType){
            case BVG:
                bvg= true;
                if(dishPrice==exp_bvg && dishID<bvg_id)//if prices equal - check by lower id
                    bvg_id=dishID;
                else if(dishPrice>exp_bvg) {//check if current BVG price is higher than the exp_bvg
                    exp_bvg = dishPrice;
                    bvg_id = dishID;
                }
                break;
            case VEG:
                if(dishID<=low_id){//check if current VEG ID number is lower than the low_id
                    veg= true;
                    low_id=dishID;
                }
                break;
            case SPC:break;
            case ALC:break;
        }
    }
    // check if VEG dish existed AND BVG existed
    if(bvg & veg) {
        output.push_back(low_id);
        output.push_back(bvg_id);
    }
    return output;
}


//--------------------------------CheapCustomer-------------------------------------------------
CheapCustomer::CheapCustomer(std::string name, int id): Customer(std::move(name),id){}

std::vector<int> CheapCustomer:: order(const std::vector<Dish> &menu){
    std::vector<int> output;
    if(menu.empty())//check if dish menu is empty - if so return empty vector
        return output;
    int low_price=menu[0].getPrice();
    int low_price_id=menu[0].getId();
    for(int j=1;j<menu.size();j++){
        int dishPrice=menu[j].getPrice();
        int dishID=menu[j].getId();
        if(dishPrice==low_price && dishID<low_price_id)//if prices equal - check by lower id
            low_price_id=dishID;
        else if(dishPrice<low_price) //checks if the current Dish price is lower than low_price
            low_price=dishPrice;
    }
    output.push_back(low_price_id);
    return output;
}


//--------------------------------SpicyCustomer-------------------------------------------------

SpicyCustomer ::SpicyCustomer(std::string name, int id): Customer(std::move(name),id) {}


//--------------------------------AlchoholicCustomer-------------------------------------------------

AlchoholicCustomer ::AlchoholicCustomer(std::string name, int id):Customer(std::move(name),id){}