#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Table.h"
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
	virtual BaseAction * clone() const = 0;
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
	std::string getErrorMsg() const; //**********originally was under Protected but it doesn't make sense
	std::string getActionArgs() const;

	void setActionArgs(std::string toSet);

protected:
    bool validTable(Table* table)const;
    void complete();
    void error(std::string errorMsg);

private:
	std::string errorMsg;
    std::string ActionArgs;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
	virtual OpenTable* clone() const;
	void clear();
	OpenTable& operator=(const OpenTable &other);
    OpenTable(int id, std::vector<Customer *> &customersList);
	virtual ~OpenTable();
	OpenTable(const OpenTable &other);
	OpenTable(OpenTable &&other);
	OpenTable& operator=(OpenTable &&other);
	void copy(const std::vector<Customer*>& customerList);
    void act(Restaurant &restaurant);
	void setArgs(int id, std::vector<Customer *> &customersList);
    std::string toString() const;

private:
	const int tableId;
	const std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
	virtual Order * clone() const;
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
	virtual MoveCustomer * clone() const;
    bool validCustomer(Customer *customer, Table* src_T)const;
    void act(Restaurant &restaurant);
    std::string toString() const;

private:

    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
	virtual Close * clone() const;
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
	virtual CloseAll * clone()const;
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
};


class PrintMenu : public BaseAction {
public:
	virtual PrintMenu * clone()const;
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
};


class PrintTableStatus : public BaseAction {
public:
	virtual PrintTableStatus * clone()const;
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
	virtual PrintActionsLog * clone()const;
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
};


class BackupRestaurant : public BaseAction {
public:
	virtual BackupRestaurant * clone() const;
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;

private:
};


class RestoreResturant : public BaseAction {
public:
	virtual RestoreResturant * clone() const;
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;


};


#endif