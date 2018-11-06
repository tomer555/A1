
#include "../include/Restaurant.h"
#include <regex>
//-------------------------Constructors-------------------------------



//Empty constructor
Restaurant::Restaurant():open(true),tables(),actionsLog(),menu(){}


//constructor
Restaurant::Restaurant(const std::string &configFilePath):open(true),actionsLog() {
    std::vector<std::string> initialze;
    std::vector<std::string> vec = splitString(configFilePath, "[\\r\\n]+");
    for(int i=0;i<vec.size();i++){
        if(vec[i]!="\n" && vec[i]!=" \n" && vec[i].find('#') == std::string::npos){
            std::vector<std::string> split=splitStringComma(vec[i]);
            for(int j=0;j<split.size();j++){
                initialze.push_back(split[j]);
            }
        }
    }
    if(!initialze.empty()) {
        int numOfTables = atoi(initialze[0].c_str());

        for (int i = 1; i < numOfTables + 1; i++) {
            tables.push_back(new Table(atoi(initialze[i].c_str())));
        }
        for (int j = numOfTables + 1;j< initialze.size(); j=j+3) {
            Dish d(atoi(initialze[j].c_str()),initialze[j+1],atoi(initialze[j+2].c_str()),convert(initialze[j+3]));
            menu.push_back(d);
        }
    }


}

//Copy Constructor
Restaurant::Restaurant(const Restaurant &rest){
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

    //this->menu.resize(rest.menu.size()); //Maybe not necessary
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


std::vector<std::string> Restaurant:: splitString(const std::string& stringToSplit, const std::string& regexPattern)
{
    std::vector<std::string> result;
    const std::regex rgx(regexPattern);
    std::sregex_token_iterator iter(stringToSplit.begin(), stringToSplit.end(), rgx, -1);

    for (std::sregex_token_iterator end; iter != end; ++iter)
    {
        result.push_back(iter->str());
    }

    return result;
}

std::vector<std::string> Restaurant:: splitStringComma(const std::string& stringToSplit){
    std::stringstream ss(stringToSplit);
    std::vector<std::string> result;
    while( ss.good() )
    {
        std::string substr;
        getline( ss, substr, ',' );
        result.push_back( substr );
    }
    return result;
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
void Restaurant :: start() {
    std::cout << "Restaurant is now open!";
    std::string userInput;
    std::cin >> userInput;
    while (userInput != "closeall") {


        std::cin >> userInput;
    }
}


DishType Restaurant ::convert(const std::string& str)const {

    if (str == "BVG")
        return BVG;
    else if (str == "VEG")
        return VEG;
    else if (str == "ALC")
        return ALC;
    else if (str == "SPC")
        return SPC;

    return SPC;
}

