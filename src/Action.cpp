
#include <sstream>
#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"



//----------------------BaseAction------------------------------------
//Empty Constructor
BaseAction::BaseAction(): status(PENDING),errorMsg(nullptr),ActionArgs(""){}

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
OpenTable ::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers(customersList){


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
//OpenTable Destructor
OpenTable:: ~OpenTable(){
    for(int i=0;i<customers.size();++i){
       delete customers[i];
    }
}

//Copy Constructor
OpenTable::OpenTable(const OpenTable &open):tableId(open.tableId){
    for(int i=0;i<open.customers.size();i++){
        customers[i]=open.customers[i].clone();
    }

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
            BaseAction *close = new Close(srcTable);
            (*close).act(restaurant);//activate close
            restaurant.addBaseAction(close);//add Close action to actionLog

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


//-----------------------------CloseAll------------------------------------------------------
//Constructor Order
CloseAll ::CloseAll(): BaseAction(){

}

void CloseAll ::act(Restaurant &restaurant) {
    std::vector<Table *> Table_vec = restaurant.getTables();
//each table that is open will be closed and the bill will be printed in increasing order
    for(int i = 0; i < Table_vec.size(); i++) {
        if (Table_vec[i]->isOpen())
            restaurant.addBaseAction(new Close(i));
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
