#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Table.h"
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

enum Command{
	OPEN, CLOSE,CLOSEALL, ORDER, STATUS, LOG,MOVE,MENU,RESTORE,BACKUP
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
	virtual BaseAction * clone() const = 0;
    virtual ~BaseAction();
	BaseAction(const BaseAction &rest);
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    std::string convertAction(const ActionStatus action)const;
	std::string getActionArgs() const;

	void setActionArgs(std::string toSet);

protected:
    std::string getErrorMsg() const;
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

    OpenTable(int id, std::vector<Customer *> &customersList);
    virtual ~OpenTable();
    std::vector<Customer*>  cloneCustomers() const;

    OpenTable(const OpenTable &rest);

    OpenTable(OpenTable &&rest);


    void clear();
    void act(Restaurant &restaurant);
	void setArgs(int id, std::vector<Customer *> &customersList);
    std::string toString() const override;

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
    void moveOrder(Table & src,Table & dst ,int customerId)const;
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


class RestoreRestaurant : public BaseAction {
public:
	virtual RestoreRestaurant * clone() const;
    RestoreRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;


};


#endif