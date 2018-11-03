//
// Created by ofirx on 01/11/18.
//

#include "../include/Table.h"
#include <vector>
// #include <shared_ptr>   why is not it working?
//std::shared_ptr
using namespace std;

class Table {

    public:

    //fields

        int* T_id; //id of a table
        int capacity; //const number of seats per table, can i add a final over here if not existing on h file?
        boolean open; //indicates whether a table is close/open
        vector <Customer> customersList; //a list of customers per table
        vector <OrderPair> orderList; //a list of orders per table

    // a constructor

        Table():capacity(t_capacity){
            (*T_id)++;
        }

    // a copy constructor

        Table(const Table &other):capacity(other.t_capacity){

        }

    // a destructor

        ~Table(){
        }

    //functions

        //function that indicates whether a table is close/open

        bool isOpen(){

            return open;
        }

        //returns a T_id value. i should hold a counter so i can count the tables and giving them an ID

        int GetTableIndex (){

            return *T_id;

        }


        void closeTable(){

            open=false;
        }

        void openTable(){

        open=true;
        }

        int getCapacity() const{

            return capacity;
        }

        //data or at? i just want an access to the pair, then dish, then the dish's price

        int getBill(){
            //is there a need for =0 ?
            int count=0;
            for (int i = 0; i <orderList.size() ; ++i) {
               count=count+orderList.at(i).getPrice();
            }
            return count;
        }

        //i want to add a custumer to the end of a vector
        // customersList.insert(*customer);

        void addCustomer(Customer* customer){

            auto ilist = {*customer};

            customersList.insert(customersList.begin() +customersList.size()+1, ilist);
        }


        void removeCustomer(int id){

            for (int i = 0; i < customersList.size(); ++i) {
                if (customersList[i].getId()==id)
                    customersList.erase(i);
            }

        }

        Customer* getCustomer(int id) {

            for (int i = 0; i < customersList.size(); ++i) {
                if (customersList[i] == id) return customersList[i];

            }
        }

            std::vector<Customer*>& getCustomers(){

                return &customersList;

            }

            std::vector<OrderPair>& getOrders(){

                return &orderList;

            }


            //a clarification is needed
            void order(const std::vector<Dish> &menu){



        }


};