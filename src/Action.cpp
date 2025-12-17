#include <string>
#include <vector>
#include <iostream>
#include "Volunteer.h"
#include "WareHouse.h"
#include "Action.h"
#include <Volunteer.h>


using std::string;
using std::vector;
using namespace std;

bool isBacked=false;
BaseAction::BaseAction():errorMsg(""),status(ActionStatus::ERROR){
   
}


ActionStatus BaseAction::getStatus()const{
return status;
}
void BaseAction::complete(){
    this->status=ActionStatus::COMPLETED;
}
void BaseAction::error(string errormsg){
    this->status=ActionStatus::ERROR;
    this->errorMsg=errormsg;
}
string BaseAction::getErrorMsg() const{
    return errorMsg;
}

SimulateStep::SimulateStep(int numOfSteps):BaseAction{}, numOfSteps(numOfSteps){}
void SimulateStep::act(WareHouse& wareHouse){
    for (int i = 0; i < numOfSteps; i++)
    {
        wareHouse.step();
    }
    complete();

    wareHouse.addAction(this);
}
std::string SimulateStep::toString() const{
    return "simulateStep "+to_string(numOfSteps)+" COMPLETED";
}
SimulateStep *SimulateStep:: clone() const {
    return new SimulateStep(*this);
}


AddOrder::AddOrder(int id):customerId(id){}
 
        void AddOrder:: act(WareHouse &wareHouse) {
            if ( customerId < wareHouse.getCustomerCounter())
            {
                Customer &cus=wareHouse.getCustomer(customerId);
            if(cus.canMakeOrder()){
                Order *order=new Order(wareHouse.getOrderCounter(),customerId,cus.getCustomerDistance());
                cus.addOrder(order->getId());
                wareHouse.addOrder(order);
                complete();
            }
               else{
                
                error("Cannot place this order");
                cout<<getErrorMsg()<<endl;

            }
            }
            else{
                
                error("Cannot place this order");
                cout<<getErrorMsg()<<endl;

            }
            wareHouse.addAction(this);

            
        }
        string AddOrder:: toString() const {
            string result="";
             result="order " + to_string(customerId);
             if(BaseAction::getErrorMsg()=="")
                 result+=" COMPLETED";
             else
                 result+=" ERROR";
            return result;
        }
        AddOrder *AddOrder:: clone() const {
            return new AddOrder(*this);
        }





AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
customerName(customerName),customerType(AddCustomer::stringToActionStatus (customerType)),distance(distance),maxOrders(maxOrders){}
void AddCustomer::act(WareHouse &wareHouse){
    if(customerType== CustomerType::Soldier){
        SoldierCustomer sold(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
        wareHouse.addCustomer(&sold);
        complete();
        wareHouse.addAction(this);
    }
    else{
        CivilianCustomer civ(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
        wareHouse.addCustomer(&civ);
        complete();
        wareHouse.addAction(this);
    }


}
AddCustomer * AddCustomer:: clone() const {
    return new AddCustomer(*this);
}
 string AddCustomer:: toString() const {
    string result="customer "+ customerName +" ";
    if(customerType==CustomerType::Soldier){

        result+="soldier ";
    }
    else{
        result+="civilian ";
    }
    result+=to_string(distance) + " " +to_string(maxOrders);
    return result;        
    
 }




PrintOrderStatus:: PrintOrderStatus(int id):orderId(id){}
 void PrintOrderStatus::act(WareHouse &wareHouse){
    if(orderId<wareHouse.getOrderCounter()){ 
        
         cout << wareHouse.getOrder(orderId).toString()<< endl;
          complete();
          wareHouse.addAction(this);
 }
 else{
    error("Order doesn’t exist");
     cout << getErrorMsg() <<endl;
     wareHouse.addAction(this);

 }
 }
PrintOrderStatus * PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);

}
string PrintOrderStatus:: toString() const {
    string result="";
    result="orderStatus " + to_string(orderId);
    if(BaseAction::getErrorMsg()=="")
        result+=" COMPLETED";
    else
        result+=" ERROR";
    return result;        
}

        




PrintCustomerStatus::PrintCustomerStatus(int customerId):customerId(customerId){}
void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(customerId<wareHouse.getCustomerCounter()){

    
    std::string result = "CustomerID: " + std::to_string(customerId) + "\n";
            for (const auto& orderId : wareHouse.getCustomer(customerId).getOrdersIds()) {
            result += "OrderID: " + std::to_string(orderId) + "\n";
            result += "OrderStatus: ";
            Order o=wareHouse.getOrder(orderId);
            if (o.getStatus() == OrderStatus::PENDING) {
                result += "Pending";
            } else if (o.getStatus() == OrderStatus::COLLECTING) {
                result += "Collecting";
            } else if (o.getStatus() == OrderStatus::DELIVERING) {
                result += "Delivering";
            } else if (o.getStatus() == OrderStatus::COMPLETED) {
                result += "Completed";
            }//wrong

            result += "\n";
        }

        result += "numOrdersLeft: " + std::to_string( wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getOrdersIds().size())  ;
        cout << result<<endl;
        wareHouse.addAction(this);
        complete();
    }
    else
    {
        error("Customer doesn’t exist");
             cout<< getErrorMsg() << endl;
             wareHouse.addAction(this);
    }
}
PrintCustomerStatus * PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
 }

string PrintCustomerStatus:: toString() const {
    string result="";
    result="customerStatus " + to_string(customerId);
    if(BaseAction::getErrorMsg()=="")
        result+=" COMPLETED";
    else
        result+=" ERROR";
    return result;   
    
}



PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id){}
void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(volunteerId<=wareHouse.getVolunteerCounter()){ 
        //Volunteer* v=wareHouse.getVolunteer(volunteerId).clone();
         cout<< wareHouse.getVolunteer(volunteerId).toString()<<endl;
         
          complete();
          wareHouse.addAction(this);
 }
 else{
    error("Volunteer doesn’t exist");
     cout<<getErrorMsg() << endl;
      wareHouse.addAction(this);

 }
 }
 PrintVolunteerStatus * PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
 }
string PrintVolunteerStatus:: toString() const {
    string result="";
    result="volunteerStatus " + to_string(volunteerId);
    if(BaseAction::getErrorMsg()=="")
        result+=" COMPLETED";
    else
        result+=" ERROR";
    return result;   
    
}


PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog:: act(WareHouse &wareHouse) {
    for ( BaseAction* a : wareHouse.getActions())
    {
        cout << a->toString() <<endl;
    }
    wareHouse.addAction(this);
    complete();
}
PrintActionsLog * PrintActionsLog:: clone() const {
    return new PrintActionsLog(*this);
}
string PrintActionsLog:: toString() const  {
    return "log COMPLETED";
}



Close::Close(){}
void Close:: act(WareHouse &wareHouse){
    wareHouse.close();
    complete();
    wareHouse.addAction(this);
}
Close * Close::clone() const {
    return new Close(*this);
}
string Close:: toString() const {
 return "";
}



 BackupWareHouse::BackupWareHouse(){}
 void BackupWareHouse:: act(WareHouse &wareHouse) {
    if(!isBacked){

    
    backup=new WareHouse(wareHouse);
    }
    else{
        delete(backup);
         backup=new WareHouse(wareHouse);

    }
    isBacked=true;
     wareHouse.addAction(this);
        complete();
 }
        BackupWareHouse *BackupWareHouse:: clone() const {
            return new BackupWareHouse(*this);
        }
        string BackupWareHouse:: toString() const {
            return "backup COMPLETED";
        }





 RestoreWareHouse :: RestoreWareHouse(){

 }
        void RestoreWareHouse:: act(WareHouse &wareHouse) {
            if(!isBacked){
                error("No backup available");
                std::cout<<getErrorMsg()<<endl;
            }
            else{
                wareHouse.operator=(*backup);
                complete();
            }
            wareHouse.addAction(this);

        }
        RestoreWareHouse *RestoreWareHouse:: clone() const {
            return new RestoreWareHouse(*this);

        }
        string RestoreWareHouse:: toString() const {
            string result="restore";
            if(BaseAction::getErrorMsg()=="")
                result+=" COMPLETED";
            else
              result+=" ERROR";
         return result;
        }


CustomerType AddCustomer:: stringToActionStatus(const std::string& str) const {
    if (str == "Soldier") {
        return  CustomerType::Soldier;
    } else   {
        return CustomerType::Civilian;
    }
}
 

