#include <string>
#include <vector>
#include "Volunteer.h"
#include "Order.h"
#include "Customer.h"
#include <WareHouse.h>
#include "Action.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

WareHouse::WareHouse(const string &configFilePath):
isOpen(true),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(0),volunteerCounter(0),orderCounter(0){
    readConfigFile(configFilePath);
}

        void WareHouse::addCustomer(Customer* customer){
            customerCounter++;
            customers.push_back(customer);
        }



        void WareHouse::addOrder(Order* order){
            orderCounter++;
            pendingOrders.push_back(order);

        }
        void WareHouse::start(){
             open();
             std::cout << "Warehouse is open!" << std::endl;

             // Wait for user input and execute actions in a loop
             while (isOpen) {
                 std::string userInput;
                 std::getline(std::cin, userInput);
                  std::istringstream iss(userInput);
                  std::string firstWord;
                  iss >> firstWord;
                 

                 if (firstWord == "order") {//add order 
                     int id;
                     iss>>id;
                    BaseAction *current= new AddOrder(id);
                    current->act(*this);

                 }
                 if(firstWord=="orderStatus"){
                    int id;
                    iss>>id;
                    BaseAction *curr=new  PrintOrderStatus(id);
                   curr->act(*this);
                 }
                 if(firstWord=="step"){
                    
                    int numOfSteps;
                    iss>>numOfSteps;
                    BaseAction *curr= new SimulateStep(numOfSteps);
                   curr-> act(*this);
                 }
                 if(firstWord=="volunteerStatus"){
                    int id;
                    iss>>id;
                   BaseAction *curr=new PrintVolunteerStatus(id);
                    curr->act(*this);

                 }
                  if (firstWord == "customer") {
            std::string customerType;//change the order name is before the type
             std::string customerName;
             iss>> customerName;
            iss >> customerType;
            if (customerType == "soldier") {
             
               int locationDistance, maxOrders;

               iss >> locationDistance >> maxOrders;

       
        customers.push_back(new SoldierCustomer(customerCounter++, customerName, locationDistance, maxOrders));
         }
         else{
            int locationDistance, maxOrders;

               iss >> locationDistance >> maxOrders;

       
        customers.push_back(new CivilianCustomer(customerCounter++, customerName, locationDistance, maxOrders));
         }
       
        } 
                 if (firstWord=="customerStatus")
                 {
                   int id;
                    iss>>id;
                    BaseAction *curr=new PrintCustomerStatus(id);
                    curr->act(*this);
                 }
                 if (firstWord=="log")
                 {
                    BaseAction *curr=new PrintActionsLog();
                   curr->act(*this);
                 }
                 if (firstWord=="backup")
                 {
                    BaseAction *curr=new BackupWareHouse();
                    curr->act(*this);
                 }
                 
                 if(firstWord=="restore"){
                    BaseAction *curr=new RestoreWareHouse();
                    curr->act(*this);
                 }
                 if (firstWord=="close")
                 {
                    close();
                 }
                 
                 
                 
            
            
        }
        }
        void WareHouse::addAction(BaseAction* action){
            actionsLog.push_back(action);
        }
      Customer& WareHouse::getCustomer(int customerId) const
         {
         
          return *customers[customerId];
         }
         Volunteer & WareHouse::getVolunteer(int volunteerId) const{
          Volunteer*v=nullptr;
          
          for(Volunteer* volunteer :volunteers)
          {
           
            
            if(volunteer->getId()==volunteerId)
              v=volunteer;
          }
          return *v;
         }
         Order & WareHouse::getOrder(int orderId) const{
          Order*o=nullptr;
             for ( Order* order : pendingOrders) {
               if (order->getId() == orderId) {
                o=order;
        }
        
    }

            for ( Order* order : inProcessOrders) {
               if (order->getId() == orderId) {
                 o=order;
        }
    }

            for ( Order* order : completedOrders) {
              if (order->getId() == orderId) {
                o=order;
        }
    }
          
         return *o;
         
         }
        const vector<BaseAction*> &WareHouse:: getActions() const{
            return actionsLog;
        }
        void WareHouse:: close(){
          for ( Order* order : pendingOrders){
              cout<<"OrderID: " + to_string (order->getId()) + ", " +"CustomerID: " +to_string(order->getCustomerId()) +", " +"Status: " + order->statusprosses(order->getStatus())+"\n";
          }
          for ( Order* order : inProcessOrders){
              cout<<"OrderID: " + to_string (order->getId()) + ", " +"CustomerID: " +to_string(order->getCustomerId()) +", " +"Status: " + order->statusprosses(order->getStatus())+"\n";
          }
          for ( Order* order : completedOrders){
              cout<<"OrderID: " + to_string (order->getId()) + ", " +"CustomerID: " +to_string(order->getCustomerId()) +", " +"Status: " + order->statusprosses(order->getStatus())+"\n";
          }
          isOpen=false;

         }
        void WareHouse:: open(){
             isOpen = true;
        }
       
       
      static bool  compareOrders(const Order* order1,const  Order* order2) {
            return order1->getId() < order2->getId();
        }

        static void sortOrders(std::vector<Order*>& orders) {
            std::sort(orders.begin(), orders.end(), compareOrders);
            }
       
       
       
       
        void WareHouse::step(){
            sortOrders(pendingOrders);

            auto iter=pendingOrders.begin();
            
            while (iter!=pendingOrders.end())
            {
                 
                bool moved=false;
                Order *order=*iter;
               if (order->getStatus()==OrderStatus::PENDING)
               {
               
                    for(Volunteer* vol:volunteers){
                        
                        if (vol->VolType()=="Collector" ||vol->VolType()=="LimitedCollector")
                        {
                          if(vol->canTakeOrder(*order))
                          {
                           order->setStatus(OrderStatus::COLLECTING);
                            vol->acceptOrder(*order);//remove from pending and add it to inprogress
                            order->setCollectorId(vol->getId());
                           
                             pendingOrders.erase(iter);
                              inProcessOrders.push_back(order);
                              
                              moved=true;

                            break;
                          }

               }
               
            }
               }
                
            else
            {
               

               
                    for(Volunteer* vol:volunteers){
                        
                        if (vol->VolType()=="Driver" ||vol->VolType()=="LimitedDriver")
                        {
                            
                          if(vol->canTakeOrder(*order))
                          {
                           order->setStatus(OrderStatus::DELIVERING);
                            vol->acceptOrder(*order);//remove from pending and add it to inprogress
                            order->setDriverId(vol->getId());
                            
                              pendingOrders.erase(iter);
                              inProcessOrders.push_back(order);
                               moved=true;


                            break;
                          }

               }
               
            }
              

            }  
            if (!moved)
            {
                iter++;
            }
            
          



         }
         
        
        auto iterVol=volunteers.begin();
           while(iterVol!=volunteers.end()){//תיקון!!!!
           Volunteer *vol=*iterVol;
              if(vol->isBusy()){
                vol->step();
                
                  
                if (vol->VolType()=="Collector" ||vol->VolType()=="LimitedCollector"){//ADD LIMITED COLLECTOR 
                    
                    if (vol->getActiveOrderId()==-1)
                    {
                       
                       
                        int completed=vol->getCompletedOrderId();
                        // Order* order = &getOrder(completed);
                        auto iterInProcess=inProcessOrders.begin();
                        bool founded=false;
                        
                           while ((iterInProcess!=inProcessOrders.end())& (!founded)){
                            Order *addto=*iterInProcess;
                            if(completed==addto->getId()){
                            inProcessOrders.erase(iterInProcess);
                            
                            pendingOrders.push_back(addto);
                            founded=true;
                            }
                            else{
                           iterInProcess++;
                           }

                           }
                     }
                    
                }
                else if (vol->VolType()=="Driver" ||vol->VolType()=="LimitedDriver"){
                    if (vol->getActiveOrderId()==-1)
                    {
                        int completed=vol->getCompletedOrderId();
                         auto iterInProcess=inProcessOrders.begin();
                         bool founded=false;
                           while ((iterInProcess!=pendingOrders.end())& (!founded)){
                            Order *addto=*iterInProcess;
                            if(completed==addto->getId()){
                            inProcessOrders.erase(iterInProcess);
                            addto->setStatus(OrderStatus::COMPLETED);
                            pendingOrders.push_back(addto);
                            founded=true;
                            }
                            else{
                           iterInProcess++;
                           }

                           }
                     }
                    
                }
              
                if (!vol->hasOrdersLeft())
                {
                   
                   
                    volunteers.erase(iterVol);
                    
                    delete vol;
                    vol=nullptr;
                    
                }
                else{
                    iterVol++;
                }
              } else{
                iterVol++;
              }
               

          }
        
        }


         void WareHouse:: readConfigFile(const string &filePath) {
        std::ifstream configFile(filePath);

        if (configFile.is_open()) {
            std::string line;
            while (std::getline(configFile, line)) {
                // Process each line of the file as needed
                processConfigLine(line);
            }

            configFile.close();
        } 
    }

    void WareHouse:: processConfigLine(const std::string &line) {
        std::istringstream iss(line);
        std::string firstWord;
        iss >> firstWord;
        

        if (firstWord == "customer") {
            std::string customerType;//change the order name is before the type
             std::string customerName;
             iss>> customerName;
            iss >> customerType;
            if (customerType == "soldier") {
             
               int locationDistance, maxOrders;

               iss >> locationDistance >> maxOrders;

       
        customers.push_back(new SoldierCustomer(customerCounter++, customerName, locationDistance, maxOrders));
         }
         else{
            int locationDistance, maxOrders;

               iss >> locationDistance >> maxOrders;

       
        customers.push_back(new CivilianCustomer(customerCounter++, customerName, locationDistance, maxOrders));
         }
       
        } 
        
        else if (firstWord == "volunteer") {
             std::string VolunteerName;
              std::string VolunteerType;
            iss >> VolunteerName>>VolunteerType;
            
            if (VolunteerType=="collector")
            {
                
                int coolDown;
                iss>>coolDown;
                volunteers.push_back(new CollectorVolunteer(volunteerCounter++,VolunteerName,coolDown));// change to add customer
            }
             if (VolunteerType=="limited_collector")
            {
                int coolDown,maxOrders;
                iss>>coolDown>>maxOrders;
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter++,VolunteerName,coolDown,maxOrders));// change to add customer
            }
              if (VolunteerType=="driver")
            {
                int maxDistance,distancePerStep;
                iss>>maxDistance>>distancePerStep;
                volunteers.push_back(new DriverVolunteer(volunteerCounter++,VolunteerName,maxDistance,distancePerStep));// change to add customer
            }
             if (VolunteerType=="limited_driver")
            {
                int maxDistance,distancePerStep,maxOrders;
                iss>>maxDistance>>distancePerStep>>maxOrders;
               
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter++,VolunteerName,maxDistance,distancePerStep,maxOrders));// change to add customer
            }
            
        }
        }
       
    
    int WareHouse::getVolunteerCounter(){
        return volunteerCounter;
    }

int WareHouse::getOrderCounter(){
    return orderCounter;
}
int WareHouse::getCustomerCounter(){
    return customerCounter;
}
WareHouse:: ~WareHouse(){ 
    for (Order* order : pendingOrders) {
            delete order;
        }
        pendingOrders.clear();
        for (Order* order : inProcessOrders) {
            delete order;
        }
        inProcessOrders.clear();
        for (Order* order : completedOrders) {
            delete order;
        }
        completedOrders.clear();
        for (Customer* customer : customers) {
            delete customer;
        }
        customers.clear();
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();
        // Release memory allocated for actions
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();
        }
    
   

   WareHouse::WareHouse(const WareHouse &other):isOpen(other.isOpen),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers(),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter)
 {
    // Copy constructor implementation
    // Copy all the elements in vectors, deep copy if necessary
    // For simplicity, let's assume deep copy is required
    for (const BaseAction* action : other.actionsLog) {
        
        actionsLog.push_back(action->clone());
    }

    for (const Volunteer* volunteer : other.volunteers) {
       
        volunteers.push_back(volunteer->clone());
    }

    for(const Customer* cus:other.customers){
        customers.push_back(cus->clone());
    }
    for(const Order * order :other.pendingOrders){
        pendingOrders.push_back(new Order(*order));

    }
    for(const Order* order :other.inProcessOrders){
        inProcessOrders.push_back(new Order(*order));
    }
    for(const Order * order : other.completedOrders){
        completedOrders.push_back(new Order(*order));
    }

   }
// Copy Assignment Operator
//copy Assignment operator= : 
 WareHouse& WareHouse :: operator=(const WareHouse& other){
    if (&other != this){
      this->isOpen = other.isOpen;

    for (BaseAction* action : this->actionsLog ){
        delete action;
    }

    for (Volunteer* volunteer : this->volunteers ){
        delete volunteer;
    }

    for (Order* ordertodel : this->pendingOrders ){
        delete ordertodel;
    }

    for (Order* ordertodel : this->inProcessOrders ){
        delete ordertodel;
    }

    for (Order* ordertodel : this->completedOrders ){
        delete ordertodel;
    }

    for (Customer* custtodel : this->customers ){
        delete custtodel;
    }


    actionsLog.clear();
    for(BaseAction* action : other.actionsLog){
        this->addAction(action->clone());
    }
    volunteers.clear();
    for(Volunteer* volunteer : other.volunteers){
        this->volunteers.push_back(volunteer->clone());
    }
    pendingOrders.clear();
    for (Order* ordertopush : other.pendingOrders){
        this->pendingOrders.push_back(new Order(*ordertopush));
    }
    inProcessOrders.clear();
    for (Order* ordertopush : other.inProcessOrders){
        this->inProcessOrders.push_back(new Order(*ordertopush));
    }
    completedOrders.clear();
    for (Order* ordertopush : other.completedOrders){
        this->completedOrders.push_back(new Order(*ordertopush));
    }
    customers.clear();
    for (Customer* custtopush : other.customers){
        this->customers.push_back(custtopush->clone());
    }

    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;

    }
    return *this;
}
// Move Constructor
//Move Constructor :
WareHouse::WareHouse(WareHouse&& other)

    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter)
{}


//Move assignment operator=
WareHouse &WareHouse::operator=(WareHouse &&other)
{
    if (this != &other)
    {
        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();
        for (Volunteer *volunteer : volunteers)
        {
            delete volunteer;
        }
        volunteers.clear();
        for (Order *order : pendingOrders)
        {
            delete order;
        }
        pendingOrders.clear();
        for (Order *order : inProcessOrders)
        {
            delete order;
        }
        inProcessOrders.clear();
        for (Order *order : completedOrders)
        {
            delete order;
        }
        completedOrders.clear();

        for (Customer *customer : customers)
        {
            delete customer;
        }
        customers.clear();

        isOpen = other.isOpen;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
    }
    return *this;
}
