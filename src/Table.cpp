//
// Created by ofirx on 01/11/18.
//

#include "../include/Table.h"
#include <vector>
using namespace std;

class Table {

    public:

    //fields

        int T_id; //id of a table
        int capacity; //const number of seats per table, can i add a final over here if not existing on h file?
        boolean open; //indicates whether a table is close/open
        vector <Customer> customersList; //a list of customers per table
        vector <OrderPair> orderList; //a list of orders per table

    // a constructor

        Table():capacity(t_capacity){

        }

    // a copy constructor

    Table(const Table &other):capacity(other.t_capacity){

    }

    // a destructor

    ~Table(){
        }

        //function that indicates whether a table is close/open
        bool isOpen(){

            return open;
        }

    int GetTableIndex (){

      return T_id;

    }

//Table(int t_capacity);
//    int getCapacity() const;
//    void addCustomer(Customer* customer);
//    void removeCustomer(int id);
//    Customer* getCustomer(int id);
//    std::vector<Customer*>& getCustomers();
//    std::vector<OrderPair>& getOrders();
//    void order(const std::vector<Dish> &menu);
//    void openTable();
//    void closeTable();
//    int getBill();







};