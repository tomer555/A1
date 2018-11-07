//
// Created by ofirx on 01/11/18.
//

#include "../include/Table.h"

// #include <shared_ptr>   why is not it working?
//std::shared_ptr


//-------------------------Constructors-------------------------------

    // a constructor+member initializer lists

    Table::Table (int t_capacity) : capacity(t_capacity),orderList(),customersList(),open(){
        openTable();
          //table_C++;
        }

    // a copy constructor

    Table::Table (const Table & other){
      copy(other);

        }

    //Move constructor
    Table::Table(Table &&other) {
        steal(other);
    }

    //steal function
    void Table::steal(Table &other) {
        open=other.open;
        capacity=other.capacity;
        customersList = std::move(other.customersList);
        orderList = std::move(other.orderList);

        other.closeTable();
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


//---------------------------Destructor and Cleaners------------------

    // a destructor
    Table:: ~Table(){
        clean();
    }

    //Cleans all Restaurant fields
    void Table::clean()  {
        for(int i=0;i<customersList.size();i++){ // Clean customers
            delete customersList[i];
            customersList[i]= nullptr;
        }
        customersList.clear();

        orderList.clear(); // clean orders
    }


//----------------------------Copy---------------------------------------
//y the arrow is necessary

    //Copy 'rest' fields into 'this'
    void Table::copy(const Table & other)  {
        this->capacity=other.capacity;
        this->open=other.open;
        this->customersList.resize(other.customersList.size()); // Copy orders
        for(int i=0;i<other.customersList.size();i++){
            this->customersList[i]=other.customersList[i];
        }
        for (int j = 0; j < customersList.size(); ++j) {
            OrderPair p(other.orderList[j].first,other.orderList[j].second);
            orderList.push_back(p);

        }

        //this->orderList.resize(other.orderList.size()); // Copy customers
        //this->orderList=other.orderList;

    }

//************************functions****************************************

        //function that indicates whether a table is close/open

        bool Table::isOpen(){

            return open;
        }

        void Table::closeTable(){

           this->clean();

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
            for (int i = 0; i <orderList.size() ; ++i) {
               count=count+orderList.at(i).second.getPrice();
            }
            return count;
        }

        //i want to add a custumer to the end of a vector
        // customersList.insert(*customer);

        void Table::addCustomer(Customer* customer){

            customersList.push_back(customer);
        }


        void Table:: removeCustomer(int id){

            for (int i = 0; i < customersList.size(); ++i) {
                if (customersList[i]->getId()==id)
                    customersList.erase(customersList.begin()+i);
            }

        }

        Customer*  Table::getCustomer(int id) {

            for (int i = 0; i < customersList.size(); ++i) {
                if (customersList[i]->getId() == id) return customersList[i];

            }
        }
        bool Table::isExisted(int id) {

            for (int i = 0; i < customersList.size(); ++i) {
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


            //a clarification is needed
            void Table::order(const std::vector<Dish> &menu){
                //when you do table.order() it can call customer.order(menu) for all the customers at the table
                for (int i = 0; i <customersList.size() ; ++i) {
                    for (int j = 0; j < customersList[i]->order(menu).size(); ++j) {
                        OrderPair p(i,menu[customersList[i]->order(menu)[j]]);
                        orderList.push_back(p);
                    }
                }
                }





