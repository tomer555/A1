
#include <sstream>
#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"



//----------------------BaseAction--------------------------------------------
//Empty Constructor
BaseAction::BaseAction():errorMsg(std::__cxx11::string()) ,ActionArgs(std::__cxx11::string()),status(PENDING){};

//copy constructor
BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg),ActionArgs(other.getActionArgs()),status(other.status){}

//Empty Destructor
BaseAction::~BaseAction() {};

//Return the Action's status
ActionStatus BaseAction:: getStatus() const{
    return this->status;
}

//Change the Action's status to completed
void BaseAction:: complete(){
    this->status=COMPLETED;
}

//Return the error message
std::string BaseAction::  getErrorMsg() const{
    return this->errorMsg;
}


//print,update,save Error status
void BaseAction:: error(std::string errorMsg){
    this->status=ERROR;
    this->errorMsg=std::move(errorMsg);
}

//setting the action's args
void BaseAction::setActionArgs(std::string toSet) {
    this->ActionArgs=std::move(toSet);

}

//return the action's args
std::string BaseAction::getActionArgs()const {
    return  this->ActionArgs;

}

bool BaseAction ::validTable(Table *table) const {
    bool output= true;
    if(table== nullptr || !dynamic_cast<Table*>(table) ){//Pointer check and Type check Table
        output= false;
    }
    return output;
}

std::string BaseAction :: convertAction(const ActionStatus action)const{
    switch (action){
        case PENDING:
            return "Pending";
        case COMPLETED:
            return "Completed";
        case ERROR:
            return "Error";

    }
    return std::__cxx11::string();
}

//-----------------------------OpenTable------------------------------

//Constructor OpenTable
OpenTable ::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers(std::move(customersList)){
    setArgs(id,customersList);
}
//Destructor
OpenTable:: ~OpenTable(){
    clear();
}

//clear
void OpenTable:: clear(){
    for(unsigned int i=0;i<customers.size();i++){
        delete customers[i];
    }
}
//copy constructor
OpenTable ::OpenTable(const OpenTable &other):BaseAction(other), tableId(other.tableId),customers(other.cloneCustomers()){}


//move constructor
OpenTable ::OpenTable(OpenTable &&other):BaseAction(),tableId(other.tableId),customers(other.customers){
}



void OpenTable::setArgs (int id, std::vector<Customer *> &customersList){
    std:: stringstream s1;
    std:: string  output;
    s1 <<id;
    for(unsigned int i =0 ; i<customers.size();i++){
        output.append(" "+customers[i]->toString());
    }
    setActionArgs(s1.str()+output);
}


std::vector<Customer*> OpenTable:: cloneCustomers() const{
    std::vector<Customer*> output;
    for( unsigned int i=0;i<customers.size();i++){
        output.push_back(customers[i]->clone());
    }
    return output;
}

void OpenTable ::act(Restaurant &restaurant){
    Table *temp=restaurant.getTable(tableId);

        if(validTable(temp) && !(*temp).isOpen()) {

                for (unsigned int i = 0; i < customers.size(); i++)
                    (*temp).addCustomer(customers[i]);//Add Customer to the Table
                (*temp).openTable();
                this->complete(); //without any exceptions change to complete

        } else{
            std::string output="Error: Table does not exist or is already open";
            this->error(output);
            std::cout<<output+"\n";

        }
}

//Prints the OpenTable action with all it's details

std::string OpenTable :: toString() const {
    std::stringstream ss;
    if (getStatus() == ERROR) {
        ss << "open " << getActionArgs() <<" "<< getErrorMsg();
        return ss.str();
    } else {
        ss << "open " << getActionArgs() <<" "<< convertAction(getStatus());
        return ss.str();

    }
}


OpenTable *OpenTable::clone() const {
    return new OpenTable(*this);// activates copy constructor
}









//---------------------------------Order-----------------------------------

//Constructor Order
Order ::Order(int id):BaseAction(),tableId(id){
    setActionArgs(std::to_string(id));
}

void Order ::act(Restaurant &restaurant) {
    Table *temp = restaurant.getTable(tableId);
    if (temp != nullptr && dynamic_cast<Table *>(temp) &&
        (*temp).isOpen()) { //Pointer check and Type check TableOpen check
        (*temp).order(restaurant.getMenu()); // * here need to come the print of the order
        this->complete();
    } else {
        std::string output = "Table doesn't exist or is not open";
        this->error(output);
        std::cout << output + "\n";
    }
}

//Prints the Order action with the Table number
std::string Order :: toString() const {
    std::stringstream ss;
    if (getStatus() == ERROR) {
        ss << "order " << getActionArgs()<<" "<<getErrorMsg();
        return ss.str();
    } else {
        ss << "order " << getActionArgs() <<" "<< convertAction(getStatus());
        return ss.str();
    }
}
Order* Order::clone() const{
    return new Order(*this);
}



//---------------------------------MoveCustomer-----------------------------------

MoveCustomer ::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId){
    std ::string output=std::to_string(src)+" "+std::to_string(dst)+" "+std::to_string(customerId);
    setActionArgs(output);
}

void MoveCustomer ::act(Restaurant &restaurant) {
    Table *temp_src = restaurant.getTable(srcTable);
    Table *temp_dst = restaurant.getTable(dstTable);
    Customer *temp_c = (*temp_src).getCustomer(id);
    if (validTable(temp_src) && validTable(temp_dst) && //valid checks
        (*temp_src).isOpen() && (*temp_dst).isOpen() &&//open checks
        (unsigned)(*temp_dst).getCapacity()>(*temp_dst).getCustomers().size()&&//capacity check
        validCustomer(temp_c, temp_src)) { //customer check
        (*temp_dst).addCustomer(temp_c);//add customer to destination Table
        moveOrder(*temp_src, *temp_dst, id);
        (*temp_src).removeCustomer(id);
        if ((*temp_src).getCustomers().empty()) {// check if src Table is empty
            (*temp_src).closeTable();
        }
        this->complete();
    } else {
        std::string output = "Cannot move customer";
        this->error(output);
        std::cout << output + "\n";
    }
}



void MoveCustomer:: moveOrder(Table & src,Table & dst ,int customerId)const {
    std::vector<OrderPair> orders = src.getOrders();
    std::vector<OrderPair> new_src;
    for (unsigned int i = 0; i < src.getOrders().size(); i++) {
        if (orders[i].first == customerId) {
            dst.getOrders().push_back(orders[i]);
        } else{
            new_src.push_back(orders[i]);
        }
    }
    orders.clear();
    src.setOrderList(new_src);
}


//Prints the OpenTable action with all it's details
std::string MoveCustomer :: toString() const {
    std::stringstream ss;
    if (getStatus() == ERROR) {
        ss << "move " << getActionArgs() <<" "<< getErrorMsg();
        return ss.str();
    } else {
        ss << "move " << getActionArgs() <<" "<< convertAction(getStatus());
        return ss.str();
    }
}



//checks if customer is valid and exists in src Table
bool MoveCustomer :: validCustomer(Customer *customer,Table* src_T) const {
    bool output= true;
    if(customer == nullptr || !dynamic_cast<Customer*>(customer) || !(*src_T).isExisted(id))
        return false;
    return output;
}

MoveCustomer * MoveCustomer:: clone() const{
    return new MoveCustomer(*this);
}
//------------------------------------------Close--------------------------------------------

//Constructor Order
Close ::Close(int id): BaseAction(),tableId(id){
    setActionArgs(std::to_string(id));
}

void Close ::act(Restaurant &restaurant) {
    Table *temp = restaurant.getTable(tableId);
    if (validTable(temp) && (*temp).isOpen()) { //valid check and open check
        std::cout << "Table " << tableId << " was closed. Bill " << (*temp).getBill() << "NIS\n";
        (*temp).closeTable();
        this->complete();
    } else {
        std::string output = "Table doesn't exist or is not open";
        this->error(output);
        std::cout << output + "\n";
    }
}

//Prints the Close action with the Table number
std::string Close :: toString() const {
    std::stringstream ss;
    if (getStatus() == ERROR) {
        ss << "close " << getActionArgs() <<" "<< getErrorMsg();
        return ss.str();
    } else {
        ss << "close " << getActionArgs() <<" "<< convertAction(getStatus());
        return ss.str();
    }
}

Close * Close:: clone() const{
    return new Close(*this);
}

//-----------------------------CloseAll------------------------------------------------------
//Constructor Order
CloseAll ::CloseAll(): BaseAction(){}

void CloseAll ::act(Restaurant &restaurant) {
    std::vector<Table *> Table_vec = restaurant.getTables();
//each table that is open will be closed and the bill will be printed in increasing order
    for (unsigned int i = 0; i < Table_vec.size(); i++) {
        if (Table_vec[i]->isOpen()) {
            std::cout << "Table " << i << " was closed. Bill " << Table_vec[i]->getBill() << "NIS\n";
            Table_vec[i]->closeTable();
        }
        this->complete();
    }
}

//Prints the Close action with the Table number
std::string CloseAll :: toString() const{
    return "closeall";
}

CloseAll * CloseAll:: clone() const{
    return new CloseAll(*this);
}
//-----------------------------PrintMenu------------------------------------------------------
PrintMenu ::PrintMenu(): BaseAction(){}

void PrintMenu ::act(Restaurant &restaurant) {
    std::vector<Dish> dishes = restaurant.getMenu();
//prints each dish in the menu
    for(unsigned int i = 0; i < dishes.size(); i++) {
        std ::string output =dishes[i].toString();
        std::cout<<output<<"\n";
    }
    this->complete();
}

//Prints the Restaurant Menu
std::string PrintMenu :: toString() const{
    return "print "+convertAction(getStatus());
}

PrintMenu * PrintMenu:: clone()const{
    return new PrintMenu(*this);
}

//--------------------------printTableStatus-------------------------------------------
PrintTableStatus ::PrintTableStatus(int id): BaseAction() ,tableId(id){
    setActionArgs(std::to_string(id));
}

void PrintTableStatus ::act(Restaurant &restaurant) {
    Table *temp = restaurant.getTable(tableId);
    std::stringstream s1;

    if ((*temp).isOpen()) {
        s1 << "Table " << tableId << " status: open\n";
        std::cout << s1.str();
        std::cout << "Customers:\n";
        std::vector<Customer *> customers = (*temp).getCustomers();
        for (unsigned int i = 0; i < customers.size(); i++) {
            std::cout << customers[i]->getId()<<" "<<customers[i]->getName()<<"\n";
        }
        std::cout << "Orders:\n";
        std::vector<OrderPair> orders = (*temp).getOrders();
        for ( unsigned int i = 0; i < orders.size(); i++) {
            std::stringstream s2;
            s2 << orders[i].second.toString() << " " << orders[i].first;
            std::cout << s2.str() << "\n";
        }
        std::cout << "Current Bill: " << (*temp).getBill()<<"NIS\n";
    } else {
        s1 << "Table " << tableId << " status: closed\n";
        std::cout << s1.str();
    }

    this->complete();
}


//Prints the Table status, customers list, orders and bill
std::string PrintTableStatus :: toString() const{
    std::stringstream ss;
    ss<< "status "<<getActionArgs()<<" "<<convertAction(getStatus());
    return ss.str();
}

PrintTableStatus * PrintTableStatus:: clone()const{
    return new PrintTableStatus(*this);
}



//--------------------PrintActionsLog---------------------------------

PrintActionsLog ::PrintActionsLog(): BaseAction(){}

void PrintActionsLog ::act(Restaurant &restaurant) {
    std::vector<BaseAction*> actions = restaurant.getActionsLog();
    for (unsigned int i = 0; i < actions.size(); i++)
        std::cout <<actions[i]->toString()<<"\n";
    this->complete();
}


//Prints the Table status, customers list, orders and bill
std::string PrintActionsLog :: toString() const{
    return "log";
}

PrintActionsLog * PrintActionsLog:: clone()const{
    return new PrintActionsLog(*this);
}
//----------------------------------BackupRestaurant----------------------------------------------------------


BackupRestaurant::BackupRestaurant():BaseAction() {
}

void BackupRestaurant::act(Restaurant &restaurant) {
    this->complete();
    if(backup== nullptr){
        backup=new Restaurant (restaurant); //copy constructor
    } else {
        *backup = restaurant;// activating copy assignment
    }
}


std::string BackupRestaurant::toString() const {
    std::stringstream ss;
    if (getStatus() == ERROR) {
        ss << "backup " << getErrorMsg();
        return ss.str();
    } else {
        ss << "backup "<< convertAction(getStatus());
        return ss.str();
    }
}


BackupRestaurant *BackupRestaurant::clone() const {
    return new BackupRestaurant(*this);
}
//-----------------------------RestoreRestaurant-------------------------------



RestoreRestaurant::RestoreRestaurant():BaseAction() {
}

void RestoreRestaurant::act(Restaurant &restaurant) {
if(backup== nullptr){
    std::string output="There is no backup available";
    error(output);
    std::cout<<output+"\n";
} else{
    restaurant=*backup; //activating copy assignment
    this->complete();
}
}

std::string RestoreRestaurant::toString() const {
    return "restore "+convertAction(this->getStatus());
}
RestoreRestaurant *RestoreRestaurant::clone() const {
    return new RestoreRestaurant(*this);
}
