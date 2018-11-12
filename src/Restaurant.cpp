
#include "../include/Restaurant.h"
#include <regex>
#include <iostream>
#include <fstream>
//-------------------------Constructors-------------------------------



//Empty constructor
Restaurant::Restaurant():index(new int(0)),open(true),tables(),menu(),actionsLog(){}


//constructor
Restaurant::Restaurant(const std::string &configFilePath):index(new int(0)),open(true),tables(),menu(),actionsLog() {

    std::vector<std::string> vec;
    std::string line;
    std::ifstream myfile (configFilePath);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            vec.push_back(line);
        }
        myfile.close();
    }
    std::vector<std::string> initialze;
    for(unsigned int i=0;i<vec.size();i++){
        if(vec[i]!="\n" && vec[i].at(0) !=' ' && vec[i].find('#') == std::string::npos){
            std::vector<std::string> split=splitString(vec[i], "[,]+");
            for(unsigned int j=0;j<split.size();j++){

                initialze.push_back(split[j]);
            }
        }
    }
    if(!initialze.empty()) {
        unsigned int numOfTables = atoi(initialze[0].c_str());

        for (unsigned int i = 1; i < numOfTables + 1; i++) {
            tables.push_back(new Table(atoi(initialze[i].c_str())));
        }
        int dishid=0;
        for (unsigned int j = numOfTables + 1;j< initialze.size(); j=j+3) {
            Dish d(dishid,initialze[j],atoi(initialze[j+2].c_str()),convertDish(initialze[j+1]));
            menu.push_back(d);
            dishid++;
        }
    }


}

//Copy Constructor
Restaurant::Restaurant(const Restaurant &rest):index(),open(),tables(),menu(),actionsLog(){
    copy(rest);
}

//Move constructor
Restaurant::Restaurant(Restaurant &&rest)noexcept :index(),open(),tables(),menu(),actionsLog(){
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

    for(unsigned int i=0;i<tables.size();i++){ // Clean Tables
        delete tables[i];
        tables[i]= nullptr;
    }
    tables.clear();

    for(unsigned int i=0;i<actionsLog.size();i++){ // Clean actionLog
        delete actionsLog[i];
        actionsLog[i]= nullptr;
    }
    actionsLog.clear();


    //menu.clear(); // clean Dishes

    delete index;

}


//----------------------------Copy---------------------------------------

//Copy 'rest' fields into 'this'
void Restaurant::copy(const Restaurant & rest)  {
    open=rest.open;
    index=new int(*(rest.index));

    //clone the BaseAction and will return a pointer to the new BaseAction
    for(unsigned int i=0;i<rest.actionsLog.size();i++){//Copy actionLog
        this->actionsLog.push_back(rest.actionsLog[i]->clone());
    }



    for(unsigned int i=0;i<rest.tables.size();i++){//restaurant
        this->tables.push_back(new Table(*rest.tables[i]));//will activate Table's copy constructor
    }


    for(unsigned int i=0;i<rest.menu.size();i++){// Copy Dishes
        Dish d(rest.menu[i]);
        menu.push_back(d);
    }



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


//Returns the number of tables in the Restaurant
int Restaurant:: getNumOfTables() const{
    return static_cast<int>(tables.size());
}


//iterates over Table vector and returns pointer to table that has 'ind' index
Table* Restaurant:: getTable(int ind){
   if(ind<0 || ind>getNumOfTables())
       return nullptr;

    return tables[ind];

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

DishType Restaurant ::convertDish(const std::string& str)const {

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

CustomerType Restaurant ::convertCustomer(const std::string& str)const {

    if (str == "chp")
        return CHP;
    else if (str == "veg")
        return VEGT;
    else if (str == "alc")
        return ALCO;
    else if (str == "spc")
        return SPCY;

    return CHP;
}


Command Restaurant ::convertCommand(const std::string& str)const {

    if (str == "open")
        return OPEN;
    else if (str == "order")
        return ORDER;
    else if (str == "close")
        return CLOSE;
    else if (str == "move")
        return MOVE;
    else if (str == "status")
        return STATUS;
    else if (str == "log")
        return LOG;
    else if (str == "closeall")
        return CLOSEALL;
    else if (str == "restore")
        return RESTORE;
    else if (str == "menu")
        return MENU;
    else if (str == "backup")
        return BACKUP;
    return LOG;
}
// Main function of Restaurant: Open the restaurant and execute commands.
void Restaurant :: start() {
    std::cout << "Restaurant is now open!\n";
    std::string userInput = "start";


    while (userInput != "closeall") {
        getline(std::cin, userInput);
        std::vector<std::string> words = splitString(userInput, "[, \\s]+");
        Command command = convertCommand(words[0]);

        switch (command) {

            case OPEN: {
                int tableId = atoi(words[1].c_str());
                std::vector<Customer *> customers_temp;
                for (unsigned int i = 2; i < words.size(); i = i + 2) {
                    Customer *temp = nullptr;
                    switch (convertCustomer(words[i + 1])) {
                        case CHP :
                            temp = new CheapCustomer(words[i], *index);
                            break;
                        case VEGT :
                            temp = new VegetarianCustomer(words[i], *index);
                            break;
                        case SPCY :
                            temp = new SpicyCustomer(words[i], *index);
                            break;
                        case ALCO :
                            temp = new AlcoholicCustomer(words[i], *index);
                            break;
                    }
                    customers_temp.push_back(temp);
                    (*index)++;
                }
                OpenTable *temp_open = new OpenTable(tableId, customers_temp);
                actionsLog.push_back(temp_open);
                (*temp_open).act(*this);
                break;
            }

            case ORDER : {
                int tableId = atoi(words[1].c_str());
                Order *temp = new Order(tableId);
                (*temp).act(*this);
                actionsLog.push_back(temp);
                break;
            }


            case CLOSE: {
                int tableId = atoi(words[1].c_str());
                Close *close_temp = new Close(tableId);
                (*close_temp).act(*this);
                actionsLog.push_back(close_temp);

                break;
            }

            case MOVE: {
                int srcTableId = atoi(words[1].c_str());
                int dstTableId = atoi(words[2].c_str());
                int customerId = atoi(words[3].c_str());
                MoveCustomer *move = new MoveCustomer(srcTableId, dstTableId, customerId);
                move->act(*this);
                actionsLog.push_back(move);
                break;
            }

            case STATUS: {
                int tableId = atoi(words[1].c_str());
                PrintTableStatus *status = new PrintTableStatus(tableId);
                status->act(*this);
                actionsLog.push_back(status);
                break;

            }

            case LOG: {
                PrintActionsLog *log = new PrintActionsLog();
                log->act(*this);
                actionsLog.push_back(log);
            }
                break;
            case BACKUP: {
                BackupRestaurant *temp_backup = new BackupRestaurant();
                temp_backup->act(*this);
                actionsLog.push_back(temp_backup);
                break;
            }
            case RESTORE: {
                RestoreRestaurant *restore = new RestoreRestaurant();
                restore->act(*this);
                actionsLog.push_back(restore);
                break;
            }
            case MENU: {
                PrintMenu *menu = new PrintMenu();
                menu->act(*this);
                actionsLog.push_back(menu);
                break;
            }
            case CLOSEALL: {
                CloseAll *closeAll = new CloseAll();
                closeAll->act(*this);
                actionsLog.push_back(closeAll);
                break;
            }
        }

    }
}






