//
// Created by ofirx on 01/11/18.
//

#include "../include/Table.h"
#include <vector>
// #include <shared_ptr>   why is not it working?
//std::shared_ptr
using namespace std;

//-------------------------Constructors-------------------------------

    // a constructor+member initializer lists

    Table::Table (int t_capacity) : capacity(t_capacity),orderList(),customersList(),open(){
        openTable();
          //table_C++;
        }

    // a copy constructor

    Table::Table (const Table & other):capacity(other.capacity){
        orderList=other.orderList;
        customersList=other.customersList;
        open=other.open;

        }

//why is the '&&' for?

    //Move constructor
    Table::Table(Table &&other) {
        steal(other);
    }

    //steal function
    void Table::steal(Table &other) {
        open=other.open;
        customersList = std::move(other.customersList);
        orderList=other.orderList;
    // missing delete rest vectors

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
        orderList.clear(); // clean orders
    }


//----------------------------Copy---------------------------------------

    //Copy 'rest' fields into 'this'
    void Table::copy(const Table & other)  {

        this->orderList.resize(other.orderList.size()); // Copy orders
        for(int i=0;i<other.orderList.size();i++){

    //something about the pair type
            this->orderList[i]=other.orderList[i];
        }

        this->customersList.resize(other.customersList.size()); // Copy customers
        this->customersList=other.customersList;

    }

    //************************functions*************************************************

        //function that indicates whether a table is close/open

        bool Table::isOpen(){

            return open;
        }

        /*returns a T_id value. i should hold a counter so i can count the tables and giving them an ID
        i need a global variable in restorant class*/

       int Table::getTableIndex() const {

           return T_id;
       }

        void Table::closeTable(){

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


        }


