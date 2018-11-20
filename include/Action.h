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
//----------------------------------BaseAction-------------------------------------------
class BaseAction{
public:
    //Constructor
    BaseAction();

    //Copy Constructor
    BaseAction(const BaseAction &other);

    //Destructor
    virtual ~BaseAction();


	virtual BaseAction * clone() const = 0;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    std::string convertAction(const ActionStatus action)const;


	//-------------Getters\Setters-----------------------
	ActionStatus getStatus() const;
protected:
    std::string getErrorMsg() const;
	std::string getActionArgs() const;
    void setActionArgs(std::string toSet);
    //-----------------------------------------------

    bool validTable(Table* table)const;
    void complete();
    void error(std::string errorMsg);
//------------------Members------------------------------
private:
	std::string errorMsg;
    std::string ActionArgs;
    ActionStatus status;
};

//------------------------------------OpenTable----------------------------------------
class OpenTable : public BaseAction {
public:

	//Constructor
    OpenTable(int id, std::vector<Customer *> &customersList);

	//Copy Constructor
	OpenTable(const OpenTable &rest);

	//Move Assignment
	OpenTable(OpenTable &&rest);

    //Destructor
    virtual ~OpenTable();





//---------------------------Methods-------------------------------
    virtual OpenTable* clone() const;
    std::vector<Customer*>  cloneCustomers() const;
    void clear();
    void act(Restaurant &restaurant);
	void setArgs(int id, std::vector<Customer *> &customersList);
    std::string toString() const override;
//------------------Members--------------------------------
private:
	const int tableId;
	std::vector<Customer *> customers;
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

//-----------------------------------MoveCustomer-----------------------------------------
class MoveCustomer : public BaseAction {
public:
    //Constructor
    MoveCustomer(int src, int dst, int customerId);

    //---------------------Methods------------------------------------------------
    void moveOrder(Table & src,Table & dst ,int customerId)const;
	virtual MoveCustomer * clone() const;
    bool validCustomer(Customer *customer, Table* src_T)const;
    void act(Restaurant &restaurant);
    std::string toString() const;
    //---------------Members------------------------------------------------------------------
private:
    const int srcTable;
    const int dstTable;
    const int id;
};

//-------------------------------Close--------------------------------
class Close : public BaseAction {
public:
    //Constructor
    Close(int id);

    //---------------------Methods----------------------------------
	virtual Close * clone() const;
    void act(Restaurant &restaurant);
    std::string toString() const;
    //-----------Members-------------------------
private:
    const int tableId;
};

//--------------------------CloseAll----------------------
class CloseAll : public BaseAction {
public:
	//Constructor
    CloseAll();

	//-----------------Methods------------------
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual CloseAll * clone()const;
//---------------Members----------------------
private:
};

//-----------------------------PrintMenu-------------------------------------
class PrintMenu : public BaseAction {
public:
	//Constructor
    PrintMenu();
	//-------------Methods-----------------------------
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual PrintMenu * clone()const;
//----------------Members-------------------------------
private:
};

//------------------------PrintTableStatus-----------------------------------
class PrintTableStatus : public BaseAction {
public:
	//Constructor
    PrintTableStatus(int id);
	//----------Methods-----------------------
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual PrintTableStatus * clone()const;
    //-------------Members------------------------
private:
    const int tableId;
};

//--------------------PrintActionsLog-----------------------
class PrintActionsLog : public BaseAction {
public:
	//Constructor
    PrintActionsLog();

	//--------Methods---------------------
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual PrintActionsLog * clone()const;
//--------------Members--------------------------
private:
};

//---------------------------------BackupRestaurant-----------------------------
class BackupRestaurant : public BaseAction {
public:
	//Constructor
    BackupRestaurant();

	//----------Methods----------------------
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BackupRestaurant * clone() const;
//-----------Members--------------------------
private:
};

//---------------------RestoreRestaurant--------------------
class RestoreRestaurant : public BaseAction {
public:
	//Constructor
    RestoreRestaurant();
	//----------Methods---------------------------------
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual RestoreRestaurant * clone() const;

};


#endif