
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

        if(vec[i]!="\n" && vec[i].at(0) !=' ' && vec[i].find('#') == std::string::npos){
            std::vector<std::string> split=splitStringComma(vec[i]);
            for(int j=0;j<split.size();j++){
                std::cout<<split[j]<<"\n";
                initialze.push_back(split[j]);
            }
        }
    }
    if(!initialze.empty()) {
        int numOfTables = atoi(initialze[0].c_str());

        for (int i = 1; i < numOfTables + 1; i++) {
            tables.push_back(new Table(atoi(initialze[i].c_str())));
        }
        int dishid=0;
        for (int j = numOfTables + 1;j< initialze.size(); j=j+3) {
            Dish d(dishid,initialze[j],atoi(initialze[j+2].c_str()),convertDish(initialze[j+1]));
            menu.push_back(d);
            dishid++;
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
        this->tables.push_back(new Table(*rest.tables[i]));//will activate Table's copy constructor
    }

    //this->menu.resize(rest.menu.size()); //Maybe not necessary
    for(int i=0;i<rest.menu.size();i++){// Copy Dishes
        Dish d(rest.menu[i].getId(),rest.menu[i].getName(),rest.menu[i].getPrice(),rest.menu[i].getType());
        menu.push_back(d);
    }

    this->actionsLog.resize(rest.actionsLog.size()); //Copy actionLog
    for(int i=0;i<rest.actionsLog.size();i++){
        this->actionsLog.push_back(rest.actionsLog[i]->clone());
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
    //std::stringstream ss(stringToSplit);
    std::vector<std::string> result;



        std::regex words_regex(",|:|-|\\s+");
        auto words_begin = std::sregex_iterator(stringToSplit.begin(), stringToSplit.end(), words_regex);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            std::cout << (*i).str() << '\n';

    /*
    while( ss.good() )
    {
        std::string substr1;
        std::string substr2;
        std::stringstream s1(ss.str());
        getline( ss, substr1, ',');
        getline( s1, substr2, ' ');
        if(substr1.size()<substr2.size())
            result.push_back( substr1 );
        else
            ss<<s1.str();
            result.push_back( substr2 );


    }
    return result;
     */
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
    std::cout << "Restaurant is now open!";
    std::string userInput;
    std::cin >> userInput;

    while (userInput != "closeall") {
        std::vector<std::string> words= splitStringComma(userInput);
        Command command=convertCommand(words[0]);
        switch (command) {

            case OPEN: {
                int tableId = atoi(words[1].c_str());
                std::vector<Customer *> customers_temp;
                int index = 0;
                for (int i = 2; i < words.size(); i = i + 2) {
                    Customer *temp = nullptr;
                    switch (convertCustomer(words[i + 1])) {
                        case CHP :
                            temp = new CheapCustomer(words[i], index);
                            break;
                        case VEGT :
                            temp = new VegetarianCustomer(words[i], index);
                            break;
                        case SPCY :
                            temp = new SpicyCustomer(words[i], index);
                            break;
                        case ALCO :
                            temp = new AlchoholicCustomer(words[i], index);
                            break;
                    }
                    customers_temp.push_back(temp);
                }
                OpenTable *temp_open = new OpenTable(tableId, customers_temp);
                actionsLog.push_back(temp_open);
                (*temp_open).act(*this);
                break;
            }

            case ORDER : {
                int tableId = atoi(words[1].c_str());
                actionsLog.push_back(new Order(tableId));
                break;
            }


            case CLOSE: {
                int tableId = atoi(words[1].c_str());
                actionsLog.push_back(new Close(tableId));
                break;
            }

            case MOVE: {
                int srcTableId = atoi(words[1].c_str());
                int dstTableId = atoi(words[2].c_str());
                int customerId = atoi(words[3].c_str());


                break;
            }

            case STATUS:
                break;

            case LOG:
                break;
            case BACKUP:
                break;
            case RESTORE:
                break;
            case MENU:
                break;

            case CLOSEALL:
                break;
        }




        std::cin >> userInput;
        }
    }






