
#include <sstream>
#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"



//----------------------BaseAction--------------------------------------------
//Empty Constructor
BaseAction::BaseAction(): status(PENDING),errorMsg(""),ActionArgs(""){}


//Empty Destructor
BaseAction::~BaseAction() {}

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

void BaseAction:: error(std::string errorMsg){
    this->status=ERROR;
    this->errorMsg=std::move(errorMsg);
}

void BaseAction::setActionArgs(std::string toSet) {
    this->ActionArgs=std::move(toSet);

}

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
//-----------------------------OpenTable------------------------------

//Constructor OpenTable
OpenTable ::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers(std::move(customersList)){
}

void OpenTable::setArgs (int id, std::vector<Customer *> &customersList){
    std:: stringstream s1;
    std:: string  output;
    s1 <<id;
    output=s1.str();
    for(int i =0 ; i<customersList.size();i++){
        output+=customersList[i]->toString();
    }
    setActionArgs(output);
}




void OpenTable ::act(Restaurant &restaurant){
    Table *temp=restaurant.getTable(tableId);

        if(validTable(temp)) {
            if(!(*temp).isOpen()){

            try {
                for (int i = 0; i < customers.size(); i++)
                    (*temp).addCustomer(customers[i]);//Add Customer to the Table
                (*temp).openTable();
                this->complete(); //without any exceptions change to complete
            }
            catch (std::exception &e) { //Catch exception thrown from addCustomer
                this->error("Failed to add a customer");
            }
        } else{
            std::stringstream myError;
            myError <<"Table no: "<<tableId<<" is Already open";
            this->error(myError.str());
        }

    } else {
        std::stringstream myError;
        myError << "Table no: " << tableId << " doesn't exist Or invalid Table id";
        this->error(myError.str());
    }
}

//Prints the OpenTable action with all it's details
std::string OpenTable :: toString() const{
    std::stringstream s1;
    std::stringstream s2;
    s1 <<"Open Table number: "<<tableId<< ", with the following customers:\n";
    std::string output =s1.str();
    for (int i = 0; i <customers.size() ; ++i) {
       s2<<customers[i]->toString()<< "\n";
       output+=s2.str();
    }

    return output;
}

OpenTable *OpenTable::clone() const {
    return nullptr;// new OpenTable(*this);
}



//---------------------------------Order-----------------------------------

//Constructor Order
Order ::Order(int id):BaseAction(),tableId(id){
    setActionArgs(std::to_string(id));
}

void Order ::act(Restaurant &restaurant){
    Table *temp=restaurant.getTable(tableId);
    if(temp!= nullptr && dynamic_cast<Table*>(temp) && (*temp).isOpen()) { //Pointer check and Type check TableOpen check
        (*temp).order(restaurant.getMenu()); // * here need to come the print of the order
        this->complete();
    }
    else
        this->error("Table doesn't exist or is not open");
}

//Prints the Order action with the Table number
std::string Order :: toString() const {
    std::stringstream s1;
    s1 << "Taking Order from Table number: " << tableId;
    return s1.str();
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
    if (validTable(temp_src)&&validTable(temp_dst) && //valid checks
            (*temp_src).isOpen() &&(*temp_dst).isOpen()&&//open checks
            validCustomer(temp_c, temp_src)){ //costumer check
        (*temp_dst).addCustomer(temp_c);//add customer to destination Table
        (*temp_src).removeCustomer(id);
        if ((*temp_src).getCustomers().empty()) {// check if src Table is empty
            (*temp_src).closeTable();

        } else
            this->error("Cannot move Customer");
    }

}


//Prints the OpenTable action with all it's details
std::string MoveCustomer :: toString() const{
    return "move";
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

void Close ::act(Restaurant &restaurant){
    Table *temp=restaurant.getTable(tableId);
    if(validTable(temp)&& (*temp).isOpen() ) { //valid check and open check
        std::cout<<"Table "<<tableId<<" was closed. Bill "<<(*temp).getBill()<<"NIS";
        (*temp).closeTable();
        (*temp).openTable();
        this->complete();
    }
    else
        this->error("Table doesn't exist or is not open");
}

//Prints the Close action with the Table number
std::string Close :: toString() const {
    std::stringstream s1;
    s1 << "Closing Table number:" << tableId;
    return s1.str();
}

Close * Close:: clone() const{
    return new Close(*this);
}

//-----------------------------CloseAll------------------------------------------------------
//Constructor Order
CloseAll ::CloseAll(): BaseAction(){

}

void CloseAll ::act(Restaurant &restaurant) {
    std::vector<Table *> Table_vec = restaurant.getTables();
//each table that is open will be closed and the bill will be printed in increasing order
    for(int i = 0; i < Table_vec.size(); i++) {
        if (Table_vec[i]->isOpen())
            Table_vec[i]->closeTable();
    }
this->complete();
    //restaurant need to be close now? or when we go back from the function

}

//Prints the Close action with the Table number
std::string CloseAll :: toString() const{
    std::stringstream s1;
    s1 <<"Closing All Tables and closing the restaurant";
    return s1.str();
}

CloseAll * CloseAll:: clone() const{
    return new CloseAll(*this);
}
//-----------------------------PrintMenu------------------------------------------------------
PrintMenu ::PrintMenu(): BaseAction(){}

void PrintMenu ::act(Restaurant &restaurant) {
    std::vector<Dish> dishes = restaurant.getMenu();
//prints each dish in the menu
    for(int i = 0; i < dishes.size(); i++) {
        std ::string output =dishes[i].toString();
        std::cout<<output<<"\n";
    }
    this->complete();
}

//Prints the Restaurant Menu
std::string PrintMenu :: toString() const{
    std::stringstream s1;
    s1 <<"Prints the Restaurant Menu";
    return s1.str();
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
    std::stringstream s2;
    if ((*temp).isOpen()) {
        s1 << "Table " << tableId << " status: open\n";
        std::cout << s1.str();
    } else {
        s1 << "Table " << tableId << " status: close\n";
        std::cout << s1.str();
    }
    std::cout << "Customers:\n";
    std::vector<Customer *> customers = (*temp).getCustomers();
    for (int i = 0; i < customers.size(); i++) {
        std::string output = customers[i]->toString();
        std::cout << output << "\n";
    }
    std::cout << "Orders:\n";
    std::vector<OrderPair> orders = (*temp).getOrders();
    for (int i = 0; i < orders.size(); i++) {
        s2 << orders[i].second.toString() << " " << orders[i].first;
        std::cout << s2.str() << "\n";
    }

    std::cout << "Current bill:\n" << (*temp).getBill();
    this->complete();
}


//Prints the Table status, customers list, orders and bill
std::string PrintTableStatus :: toString() const{
    std::stringstream s1;
    s1 <<"Prints the Table status, customers list, orders and bill";
    return s1.str();
}

PrintTableStatus * PrintTableStatus:: clone()const{
    return new PrintTableStatus(*this);
}



//--------------------PrintActionsLog---------------------------------

PrintActionsLog ::PrintActionsLog(): BaseAction(){}

void PrintActionsLog ::act(Restaurant &restaurant) {

    std::vector<BaseAction*> actions = restaurant.getActionsLog();
    for (int i = 0; i < actions.size(); i++) {
        if(actions[i]->getStatus()==ERROR){
            std::cout <<actions[i]->toString()<<" "<<actions[i]->getActionArgs()<<" "<< actions[i]->getErrorMsg()<<"\n";
        } else
            std::cout <<actions[i]->toString()<<" "<<actions[i]->getActionArgs()<<" "<< actions[i]->getStatus()<<"\n";
    }
    this->complete();
}


//Prints the Table status, customers list, orders and bill
std::string PrintActionsLog :: toString() const{
    return "log";
}

PrintActionsLog * PrintActionsLog:: clone()const{
    return new PrintActionsLog(*this);
}
//--------------------------------------------------------------------------------------------
BackupRestaurant *BackupRestaurant::clone() const {
    return nullptr;
}

BackupRestaurant::BackupRestaurant() {

}

void BackupRestaurant::act(Restaurant &restaurant) {

}

std::string BackupRestaurant::toString() const {
    return std::__cxx11::string();
}

RestoreResturant *RestoreResturant::clone() const {
    return nullptr;
}

RestoreResturant::RestoreResturant() {

}

void RestoreResturant::act(Restaurant &restaurant) {

}

std::string RestoreResturant::toString() const {
    return std::__cxx11::string();
}
