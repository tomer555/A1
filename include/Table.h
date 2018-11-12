#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;



class Table{
public:
    //-------------------Rule of 5----------------------------------------
    //Constructor
    Table(int t_capacity);

    //Copy Constructor
    Table(const Table &other);

    //Destructor
    virtual ~Table();

    //Move Constructor
    Table(Table &&other);


    //Copy Assignment
    Table & operator=(const Table& other);

    //Move Assignment
    Table & operator=(Table&& other);
    void copy(const Table & rest);
    void clean();
    void steal(Table &other);
//---------------------------Getters/Setters-------------------------------------------------
    int getCapacity() const;
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void setOrderList(std::vector<OrderPair> toset);

    //------------------------------Methods--------------------------------------------------
    OrderPair makeOrder(int customerId,Dish dish)const;
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    bool isExisted(int id);
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    void addCustomer(Customer* customer);
//-----------------------------------Members----------------------------------------
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif