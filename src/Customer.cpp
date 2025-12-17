#include <string>
#include <vector>
#include "Customer.h"
#include "Order.h"
#include "WareHouse.h"
#include "Action.h"
#include <iostream>
using std::string;
using std::vector;




        Customer::Customer(int id, const string &name, int locationDistance, int maxOrders):
        id(id),name(name),locationDistance(locationDistance),maxOrders(maxOrders),ordersId()
    {}
        
        const string& Customer:: getName() const{
            return name;
        }
        int Customer:: getId() const{
            return id;
        }

        int Customer:: getCustomerDistance() const{
            return locationDistance;
        }
        int Customer:: getMaxOrders() const{
            return maxOrders;
        } //Returns maxOrders
        int  Customer:: getNumOrders() const{
            return ordersId.size();
        } //Returns num of orders the customer has made so far
        bool  Customer::canMakeOrder() const{
          return  maxOrders- ordersId.size()!=0;
        } //Returns true if the customer didn't reach max orders
        const vector<int> & Customer:: getOrdersIds() const{
            return ordersId;
        }
        
        int  Customer:: addOrder(int orderId){
             if (canMakeOrder()) {
                
                  ordersId.push_back(orderId);
                  
                 return orderId;  // Return orderId if the order was added successfully
    }        
            else {
                 return -1;  // Return -1 if the customer has reached the maximum orders
    }
 
        }    
    




        SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders):Customer(id,name,locationDistance,maxOrders)
        {}
        
        SoldierCustomer * SoldierCustomer::clone() const {
            return new SoldierCustomer(*this);
        }


    
   
       
    
    


        CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders):Customer(id,name,locationDistance,maxOrders)
        {}
        CivilianCustomer * CivilianCustomer::clone() const {
            return new CivilianCustomer(*this);
        }