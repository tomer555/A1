#include <sstream>
#include <algorithm>
#include "../include/Customer.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"



//--------------------------------Customer-------------------------------------------------


//Constructor Memory initialization
Customer::Customer(std::string c_name, int c_id):firstOrder(false),name(std::move(c_name)),id(c_id){}

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

//--------------------------------VegetarianCustomer-------------------------------------------------

VegetarianCustomer ::VegetarianCustomer(std::string name, int id) : Customer(std::move(name),id),vegDishId(-1),bvgDishId(-1),okToOrder(
        false){}

std::vector<int> VegetarianCustomer:: order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    std::vector<std::pair<int, int>> veg_pairs; // first is id, second is price
    std::vector<std::pair<int, int>> bvg_pairs;
    if (!firstOrder && !menu.empty()) {
        veg_pairs = std::move(typeSort(menu, VEG, true));
        bvg_pairs = std::move(typeSort(menu, BVG, false));
        firstOrder = true;
        if (!veg_pairs.empty() && !bvg_pairs.empty()) {
            okToOrder = true;
            vegDishId = veg_pairs[0].first;
            bvgDishId = bvg_pairs[bvg_pairs.size() - 1].first;
        }
    }
    if (okToOrder) {
        output.push_back(vegDishId);
        output.push_back(bvgDishId);
    }
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
CheapCustomer::CheapCustomer(std::string name, int id): Customer(std::move(name),id){}

std::vector<int> CheapCustomer:: order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if(!firstOrder &&!menu.empty()) { //CheapCustomer can only order one time
        std::vector<std::pair<int,int>> pairs; // first is id, second is price
        for(unsigned int i=0; i<menu.size();i++)
            pairs.emplace_back(menu[i].getId(), menu[i].getPrice());
        std::sort(pairs.begin(),pairs.end(),compCheapToExp);
        output.push_back(pairs[0].first);
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
SpicyCustomer ::SpicyCustomer(std::string name, int id): Customer(std::move(name),id),bvgDishId(-1) {}

//clone the SpicyCustomer object
SpicyCustomer* SpicyCustomer:: clone() const{
    return new SpicyCustomer(*this);
}


std::vector<int> SpicyCustomer:: order(const std::vector<Dish> &menu){
    std::vector<int> output;
    if(!firstOrder) {// SpicyCustomer first order
        std::vector<std::pair<int, int>> spc_pairs = std::move(typeSort(menu, SPC, false));
        if (!spc_pairs.empty()) {
            output.push_back(spc_pairs[spc_pairs.size() - 1].first);
            firstOrder = true;
        }
    } else if(bvgDishId==-1){
        std::vector<std::pair<int, int>> spc_pairs = std::move(typeSort(menu, BVG, false));
        if(!spc_pairs.empty())
            bvgDishId = spc_pairs[0].first;
    }
    if(bvgDishId!=-1)
        output.push_back(bvgDishId);

    return output;
}

std::string SpicyCustomer::toString() const {
    std::stringstream ss;
    ss <<this->getName()<<",spc";
    return ss.str();
}


//--------------------------------AlchoholicCustomer-----------------------------------------------------------------------------------------------------

//AlchoholicCustomer Constructor
AlcoholicCustomer ::AlcoholicCustomer(std::string name, int id):Customer(std::move(name),id),currentIndex(0),alc_menu(){}


//clone AlchoholicCustomer object
AlcoholicCustomer* AlcoholicCustomer:: clone() const{
    return new AlcoholicCustomer(*this);
}

std::vector<int> AlcoholicCustomer:: order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if(!firstOrder&&!menu.empty()) { // alc menu is not created yet
        alc_menu=std::move(typeSort(menu,ALC, false));
    }
    if(currentIndex<alc_menu.size()) { //check is we didn't reach the most expensive dish
        output.push_back(alc_menu[currentIndex].first);
        currentIndex++;
        firstOrder=true;
    }
    return output;
}





std::string AlcoholicCustomer::toString() const {
    std::stringstream ss;
    ss <<this->getName()<<",alc";
    return ss.str();
}

std::vector<std::pair<int,int>> typeSort(const std::vector<Dish> &menu, DishType type,bool idOnly) {
    std::vector<std::pair<int,int>> output;
    std::vector<std::pair<int,int>> pairs; // first is id, second is price
    int lastPrice=-1;
    for(unsigned int i=0; i<menu.size();i++){
        if(menu[i].getType()==type)
            pairs.emplace_back(menu[i].getId(), menu[i].getPrice());
    }
    if(idOnly) {
        std::sort(pairs.begin(), pairs.end(), compId);
        return pairs;
    }
    else {
        std::sort(pairs.begin(), pairs.end(), compCheapToExp);
        for (unsigned int i = 0; i < pairs.size(); i++) {
            if (pairs[i].second > lastPrice) {
                output.push_back(pairs[i]);
                lastPrice = pairs[i].second;
            }
        }
        return output;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------

bool compCheapToExp (std::pair<int,int> i, std::pair<int,int> j){
    if(i.second==j.second && i.first<j.first)
        return true;
    else if(i.second<j.second)
        return true;
    return false;
}

bool compId (std::pair<int,int> i, std::pair<int,int> j){
    return (i.first<j.first);
}