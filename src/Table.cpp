//
// Created by ofirx on 01/11/18.
//

#include "../include/Table.h"
#include <vector>
// #include <shared_ptr>   why is not it working?
//std::shared_ptr
using namespace std;

    // a constructor+member initializer lists

    Table::Table (int t_capacity) : capacity(t_capacity){
          //table_C++;
          //adding stuff
        }

    // a copy constructor

    Table::Table (const Table & other){
        //adding stuff
    capacity=other.capacity;

        }

    // a destructor

    Table:: ~Table(){
            //
            //adding stuff
        }

    //functions

        //function that indicates whether a table is close/open

        bool Table::isOpen(){

            return open;
        }

        //returns a T_id value. i should hold a counter so i can count the tables and giving them an ID
        //i need a global variable in restorant class

       /* int Table::GetTableIndex (){

            return table_C;

        }
*/

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


