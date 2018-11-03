#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);
    Restaurant(const Restaurant &rest);
    Restaurant(Restaurant &&rest);
    Restaurant& operator=(const Restaurant& rest);
    Restaurant& operator=(Restaurant&& rest);
    void start();
    void clean();
    void steal(Restaurant &rest);

    void copy(const Restaurant& rest);
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    virtual ~Restaurant();

    int Parse(std::string &configFilePath);

private:
    bool open;
    int table_c;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif