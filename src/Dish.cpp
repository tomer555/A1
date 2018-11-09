
#include <sstream>
#include "../include/Dish.h"

//Constructor- Memory  initialization
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):
id(d_id),
name(std::move(d_name)),
price(d_price),
type(d_type){}



//Returns the Dish Name
std::string Dish :: getName() const{
    return this->name;
}

//Returns the Dish ID number
int Dish ::getId() const {
    return this->id;
}

//Returns the Dish ID number
int Dish :: getPrice() const{
    return this->price;
}

DishType Dish:: getType() const{
    return this->type;
}

std::string Dish :: toString() const{
    std::stringstream s1;
    s1 <<name<<" "<<price<<"NIS";
    return s1.str();
}

Dish::~Dish() {}
