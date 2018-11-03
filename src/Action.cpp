
#include <sstream>
#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"



//----------------------BaseAction------------------------------------
//Empty Constructor
BaseAction::BaseAction(): status(PENDING),errorMsg(nullptr){}

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
//-----------------------------OpenTable------------------------------

//Constructor OpenTable
OpenTable ::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers(customersList){}

void OpenTable ::act(Restaurant &restaurant){
    Table *temp=restaurant.getTable(tableId);
    if(temp!= nullptr && dynamic_cast<Table*>(temp)) { //Pointer check and Type check
        if(!(*temp).isOpen()) {
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
Order ::Order(int id):BaseAction(),tableId(id){}

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
std::string Order :: toString() const{
    std::stringstream s1;
    s1 <<"Taking Order from Table number: "<<tableId;
    return s1.str();
}

//---------------------------------MoveCustomer-----------------------------------

MoveCustomer ::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId){}

void MoveCustomer ::act(Restaurant &restaurant){
    Table *temp_src=restaurant.getTable(srcTable);
    Table *temp_dst=restaurant.getTable(dstTable);
    Customer *temp_c = (*temp_src).getCustomer(id);
   if(validTables(temp_src,temp_dst) && validCustomer(temp_c,temp_src)) {//valid checks
       (*temp_dst).addCustomer(temp_c);//add customer to destination Table
       (*temp_src).removeCustomer(id);
       if((*temp_src).getCustomers().empty()) // check if src Table is empty
           (*temp_src).closeTable();




   }
   else
       this->error("Cannot move Customer");


}

//Prints the OpenTable action with all it's details
std::string MoveCustomer :: toString() const{

}

//checks if both src and des tables are valid
bool MoveCustomer ::validTables(Table *src_T, Table *des_T) const {
    bool output= true;
    if(src_T== nullptr || !dynamic_cast<Table*>(src_T) ||//Pointer check and Type check Table
            des_T== nullptr || !dynamic_cast<Table*>(des_T) ||
       !(*src_T).isOpen()||!(*des_T).isOpen() || //src Table and dst Table open check
       (*des_T).getCapacity()<=(*des_T).getCustomers().size()){//check des Table is not full
        output= false;
    }
    return output;
}

//checks if customer is valid and exists in src Table
bool MoveCustomer :: validCustomer(Customer *customer,Table* src_T) const {
    bool output= true;
    if(customer == nullptr || !dynamic_cast<Customer*>(customer) || !(*src_T).isExisted())
        return false;
    return output;
}
//------------------------