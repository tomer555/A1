
#include <iostream>
#include "../include/Table.h"

//-------------------------Constructors-------------------------------



    // a constructor+member initializer lists

    Table::Table (int t_capacity) : capacity(t_capacity),open(false),customersList(),orderList(){

        }

    // a copy constructor

    Table::Table (const Table & other):capacity(),open(),customersList(),orderList(){
      copy(other);
        }

//Move constructor
    Table::Table(Table &&other):capacity(),open(),customersList(),orderList() {
        steal(other);
    }

//Copy Assignment Operator
Table & Table:: operator=(const Table& other)  {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    }
    clean();
    copy(other);
    return *this;
}

//Move Assignment Operator
    Table & Table:: operator=(Table&& other) {
        clean();
        steal(other);
        return *this;
}


//steal function
    void Table::steal(Table &other) {
        open=other.open;
        capacity=other.capacity;
        customersList = std::move(other.customersList);
        orderList = std::move(other.orderList);

    }





//---------------------------Destructor and Cleaners------------------

    // a destructor
    Table:: ~Table(){
        clean();
    }

    //Cleans all Restaurant fields
    void Table::clean()  {
        for(unsigned int i=0;i<customersList.size();i++){ // Clean customers
            delete customersList[i];
            customersList[i]= nullptr;
        }
        customersList.clear();


        orderList.clear(); // clean orders
    }


//----------------------------Copy---------------------------------------


    //Copy 'rest' fields into 'this'
    void Table::copy(const Table & other)  {
        this->capacity=other.capacity;
        this->open=other.open;
                                                       // Copy orders
        for(unsigned int i=0;i<other.customersList.size();i++){
            this->customersList.push_back(other.customersList[i]->clone());
        }
        for (unsigned int j = 0; j < other.orderList.size(); ++j) {
            OrderPair p(other.orderList[j].first,other.orderList[j].second);
            orderList.push_back(p);

        }

    }

//************************functions****************************************

        //function that indicates whether a table is close/open

        bool Table::isOpen(){

            return open;
        }

        void Table::closeTable(){
           clean();
           open=false;
        }

        void Table::openTable(){

        open=true;
        }

        int Table::getCapacity() const{

            return capacity;
        }

        //data or at? i just want an access to the pair, then dish, then the dish's price

        int Table::getBill(){
            //is there a need for =0 ?
            int count=0;
            for (unsigned int i = 0; i <orderList.size() ; ++i) {
               count=count+orderList.at(i).second.getPrice();
            }
            return count;
        }

        //i want to add a custumer to the end of a vector
        // customersList.insert(*customer);

        void Table::addCustomer(Customer* customer){

            customersList.push_back(customer->clone());
        }


        void Table:: removeCustomer(int id){

            for (unsigned int i = 0; i < customersList.size(); ++i) {
                if (customersList[i]->getId()==id){
                        delete customersList[i];
                        customersList.erase(customersList.begin()+i);
                }
             }
        }

        Customer*  Table::getCustomer(int id) {

            for (unsigned int i = 0; i < customersList.size(); i++) {
                if (customersList[i]->getId() == id)
                    return customersList[i];

            }
            return nullptr;
        }
        bool Table::isExisted(int id) {

            for (unsigned int i = 0; i < customersList.size(); ++i) {
                if (customersList[i]->getId() == id) return true;
    }
        return false;
}

            std::vector<Customer*>& Table:: getCustomers(){

                return customersList;

            }

            std::vector<OrderPair>& Table::getOrders(){

                return orderList;

            }



            void Table::order(const std::vector<Dish> &menu) {
                //when you do table.order() it can call customer.order(menu) for all the customers at the table
                for (unsigned int i = 0; i < customersList.size(); ++i) {
                    std::vector<int> customerOrder = customersList[i]->order(menu);
                    for (unsigned int j = 0; j < customerOrder.size(); ++j) {
                        for (unsigned int k = 0; k < menu.size(); ++k) {
                            if (menu[k].getId() == customerOrder[j]) {
                                orderList.push_back(makeOrder(customersList[i]->getId(), menu[k]));
                                std::cout <<customersList[i]->getName()<<" ordered "<<menu[k].getName()<<"\n";
                            }
                        }
                    }
                }
            }

OrderPair Table:: makeOrder(int customerId,Dish dish)const{
    OrderPair p(customerId,dish);
    return p;

}


void Table:: setOrderList(std::vector<OrderPair> toset){
    orderList=std::move(toset);
}