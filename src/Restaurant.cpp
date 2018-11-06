
#include "../include/Restaurant.h"
//-------------------------Constructors-------------------------------



//Empty constructor
Restaurant::Restaurant():open(true),tables(),actionsLog(),menu(){}


//constructor
Restaurant::Restaurant(const std::string &configFilePath):open(true),actionsLog() {




}

//Copy Constructor
Restaurant::Restaurant(const Restaurant &rest):{
    copy(rest);
}

//Move constructor
Restaurant::Restaurant(Restaurant &&rest) noexcept {
    steal(rest);
}

//Copy Assignment Operator
Restaurant & Restaurant:: operator=(const Restaurant& rest)  {
    // check for "self assignment" and do nothing in that case
    if (this != &rest) {
        clear();
        copy(rest);
    }
    return *this;
}


//Move Assignment Operator
Restaurant & Restaurant:: operator=(Restaurant&& rest) noexcept {
    clear();
    steal(rest);
    return *this;
}

//steals 'rest' resources into 'this'
void Restaurant::steal(Restaurant &rest) {
    open=rest.open;
    tables = std::move(rest.tables);
    menu=std::move(rest.menu);
    actionsLog=std::move(rest.actionsLog);

    rest.open= false;
}

//---------------------------Destructor and Cleaners------------------
//Destructor
Restaurant::~Restaurant(){
clear();
}

//Cleans all Restaurant fields
void Restaurant::clear()  {

    for(int i=0;i<tables.size();i++){ // Clean Tables
        delete tables[i];
        tables[i]= nullptr;
    }
    tables.clear();

    for(int i=0;i<actionsLog.size();i++){ // Clean actionLog
        delete actionsLog[i];
        actionsLog[i]= nullptr;
    }
    actionsLog.clear();


    menu.clear(); // clean Dishes

}


//----------------------------Copy---------------------------------------

//Copy 'rest' fields into 'this'
void Restaurant::copy(const Restaurant & rest)  {
    open=rest.open;

    this->tables.resize(rest.tables.size()); // Copy Tables
    for(int i=0;i<rest.tables.size();i++){
        this->tables[i]=rest.tables[i];//will activate Table's copy constructor
    }

    this->menu.resize(rest.menu.size()); //Maybe not necessary
    for(int i=0;i<rest.menu.size();i++){// Copy Dishes
        Dish d(rest.menu[i].getId(),rest.menu[i].getName(),rest.menu[i].getPrice(),rest.menu[i].getType());
        menu.push_back(d);
    }

    this->actionsLog.resize(rest.actionsLog.size()); //Copy actionLog
    for(int i=0;i<rest.actionsLog.size();i++){
        this->actionsLog[i]=rest.actionsLog[i]->clone();
    }
//clone the BaseAction and will return a pointer to the new BaseAction
}

//-----------------------Methods----------------------------------------------------


int Restaurant:: Parse(std::string &configFilePath){
    std::size_t index3=configFilePath.find("Menu");
    std::size_t index1=configFilePath.find("#number of tables");
    std::size_t index2=configFilePath.find("#tables description");

    std::string para1=configFilePath.substr(index1,index2);
    std::string para2=configFilePath.substr(index2,index3);
    std::string para3=configFilePath.substr(index3);
    std::size_t para1_lspace=para1.find('\n');
    std::size_t para1_rspace=para1.rfind('\n');
    std::size_t para2_lspace=para2.find('\n');
    std::size_t para2_rspace=para2.rfind('\n');
    std::size_t para3_lspace=para3.find('\n');

    std::string final1=para1.substr(para1_lspace+1,para1_rspace);
    std::string final2=para2.substr(para2_lspace+1,para2_rspace);
    std::string final3=para3.substr(para3_lspace+1);
    std ::cout<< final1;
    std ::cout<< final2;
    std ::cout<< final3;

}
//Returns the number of tables in the Restaurant
int Restaurant:: getNumOfTables() const{
    return static_cast<int>(tables.size());
}


//iterates over Table vector and returns pointer to table that has 'ind' index
Table* Restaurant:: getTable(int ind){
   if(ind<0 || ind>getNumOfTables())
       return nullptr;

    return tables.at(static_cast<unsigned int>(ind));

}
//Returns a reference to the Menu vector
std::vector<Dish>& Restaurant:: getMenu(){
    return menu;
}

//Returns a reference to the Menu vector
const std::vector<Table*>& Restaurant:: getTables() const{
    return tables;
}


// Return a reference to the history of actions
const std::vector<BaseAction*>& Restaurant :: getActionsLog() const{
    return actionsLog;
}


// Main function of Restaurant: Open the restaurant and execute commands.
void Restaurant :: start(){
std::cout <<"Restaurant is now open!";
std:: string userInput;
std :: cin>>userInput;
while(userInput!="closeall"){




    std :: cin>>userInput;
}

}