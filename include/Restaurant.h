#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include <memory>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{

    //-----------------Constructors-----------------------------------
public:
    //Empty Constructor
	Restaurant();

    // Constructor
    Restaurant(const std::string &configFilePath);

//--------------------Rule of 5----------------------------------
    //Destructor
    virtual ~Restaurant();

    // Copy Constructor
    Restaurant(const Restaurant &rest);

    // Copy Assignment
    Restaurant& operator=(const Restaurant& rest);

    // Move Constructor
     Restaurant(Restaurant &&rest) noexcept;

    // Move Assignment
    Restaurant& operator=(Restaurant&& rest) noexcept;


    //-----------------------Getters-----------------------------------------

    int getNumOfTables() const;
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    const std::vector<Table*>& getTables() const; // Return a reference to the Tables


    //--------------------Methods----------------------------------

    void start();
    void clear();
    void steal(Restaurant &rest);
    void copy(const Restaurant& rest);
    Table* getTable(int ind);
    std::vector<Dish>& getMenu();
    std::vector<std::string> splitString(const std::string& stringToSplit, const std::string& regexPattern);
    std::vector<std::string> splitStringComma(const std::string& stringToSplit);
    DishType convertDish(const std::string& str)const;
    Command convertCommand(const std::string& str)const;
    std::string convetAction(ActionStatus action)const;
    CustomerType convertCustomer(const std::string& str)const;



    //------------------Members--------------------------------------
private:
    int *index;
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif