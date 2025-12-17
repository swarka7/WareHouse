#include <string>
#include <vector>
#include "Order.h"
#include "Volunteer.h"
#include <iostream>

// #include "WareHouse.h"
using std::string;
using std::vector;


#define NO_ORDER -1


         Volunteer ::Volunteer (int id, const string &name):completedOrderId(NO_ORDER),activeOrderId(NO_ORDER), id(id),name(name){}
        int  Volunteer::getId() const
        {
            return id;
        }
        const string & Volunteer::getName() const{
            return name;
        }
        int Volunteer:: getActiveOrderId() const{
          return  activeOrderId;
            
        }
        int Volunteer:: getCompletedOrderId() const{
            return completedOrderId;
        }

        bool Volunteer::isBusy() const{
           return activeOrderId!=-1;
        } // Signal whether the volunteer is currently processing an order    
       

    



       CollectorVolunteer:: CollectorVolunteer(int id, const string &name, int coolDown):Volunteer(id,name),coolDown(coolDown),timeLeft(coolDown)
        {}
        CollectorVolunteer * CollectorVolunteer::clone() const {
             return new CollectorVolunteer(*this);
        }
        void CollectorVolunteer::step() 
        {
             if (timeLeft > 0) {
                timeLeft--;  
               
              if (timeLeft == 0) {
                  completedOrderId = activeOrderId;  
                 activeOrderId = -1;  
        }
    }
        }
        int CollectorVolunteer::getCoolDown() const
        {
            return coolDown;
        }
        
        
        int CollectorVolunteer::getTimeLeft() const{
            return timeLeft;
        }
        
        bool CollectorVolunteer::decreaseCoolDown() //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        { 
           timeLeft--;
           return timeLeft==0;
        }
        bool CollectorVolunteer::hasOrdersLeft() const {
            // CollectorVolunteer can always take more orders
              return true;
        }
        bool CollectorVolunteer::canTakeOrder(const Order &order) const {
           if(!(isBusy())){
            return true;
           }
           else 
           return false;
        }
        void CollectorVolunteer::acceptOrder(const Order &order) {
             if (canTakeOrder(order)) {
                activeOrderId = order.getId();
                
              timeLeft = coolDown;  // Set the initial value of timeLeft when accepting a new order
         }
        }
        void CollectorVolunteer::setTimeLeft(int newTime) {
          CollectorVolunteer::  timeLeft=newTime;
        }
        string CollectorVolunteer::toString() const {//
         
            std:: string s1="VolunteerID:";
            s1 = s1 + std::to_string(getId());
            std::string s2="isBusy:";
            if(isBusy()){
                s2=s2+"True";
            }
            else{
                s2=s2+"False";
            }
            std:: string s3="OrderID:";
             if(!isBusy()){
                s3=s3+"None";
             }
             else{
                
                s3=s3+std::to_string( activeOrderId);
             }
          std::string s4="TimeLeft:";
          if (getTimeLeft()==0)
          {
            s4=s4+"None";
          }
          else{
            s4=s4+std::to_string(getTimeLeft());
          }
            std:: string s5="OrdersLeft:No Limit";
            
           
            string line="\n";
            string rst=s1+ line +s2+line+s3+line+s4+line+s5;
            
            
            return rst;
            
       
        }
        string CollectorVolunteer:: VolType(){
            return "Collector";
        }
       
    
    


        LimitedCollectorVolunteer::  LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders){}
        LimitedCollectorVolunteer * LimitedCollectorVolunteer::clone() const {
             return new LimitedCollectorVolunteer(*this);
        }
        bool LimitedCollectorVolunteer::hasOrdersLeft() const {
             return ordersLeft > 0;
        }
        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
            return !(isBusy()) && ordersLeft > 0;
        }
        void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
           if (canTakeOrder(order)) {
             activeOrderId = order.getId();
             setTimeLeft(getCoolDown());  
             ordersLeft--;
         }
        }

        int LimitedCollectorVolunteer::getMaxOrders() const{
            return maxOrders;
        }
        int LimitedCollectorVolunteer:: getNumOrdersLeft() const{
            return ordersLeft;
        }
        string LimitedCollectorVolunteer::toString() const {
           
            std:: string s1="VolunteerID:";
            s1 = s1 + std::to_string(getId());
            std::string s2="isBusy:";
            if(isBusy()){
                s2=s2+"True";
            }
            else{
                s2=s2+"False";
            }
            std:: string s3="OrderID:";
             if(!isBusy()){
                s3=s3+"None";
             }
             else{
                
                s3=s3+std::to_string( activeOrderId);
             }
          std::string s4="TimeLeft:";
          if (getTimeLeft()==0)
          {
            s4=s4+"None";
          }
          else{
            s4=s4+std::to_string(getTimeLeft());
          }
            std:: string s5="OrdersLeft:";
            s5=s5+std::to_string(ordersLeft);

            
           
            string line="\n";
            string rst=s1+ line +s2+line+s3+line+s4+line+s5;
            
            
            return rst;
        }
        string LimitedCollectorVolunteer:: VolType(){
            return "LimitedCollector";
        }
    
   

     DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
       Volunteer(id,name),maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(0){}
       
        DriverVolunteer * DriverVolunteer::clone() const {
        return new  DriverVolunteer(*this);  
        }
        void DriverVolunteer:: setDistanceLeft(int distance){
             distanceLeft=distance;
        }

        int DriverVolunteer:: getDistanceLeft() const{
            return distanceLeft;
        }
        int DriverVolunteer ::getMaxDistance() const{
            return maxDistance;
        }
        int DriverVolunteer:: getDistancePerStep() const{
            return distancePerStep;
        }  
        bool DriverVolunteer:: decreaseDistanceLeft(){
            distanceLeft=distanceLeft-distancePerStep;
            if (distanceLeft<=0)
            {
                distanceLeft=0;//added
                completedOrderId=activeOrderId;
                activeOrderId=NO_ORDER;
                return true;
            }
            else
            {
                return false;
            }
            
        }
        bool DriverVolunteer:: hasOrdersLeft() const {
            return true;
        }
        bool DriverVolunteer:: canTakeOrder(const Order &order) const {

          if (!(isBusy())&& (order.getDistance()<=maxDistance))
          {
            return true;
          }
          else{
         return false;
          }
            
        }// Signal if the volunteer is not busy and the order is within the maxDistance
        void DriverVolunteer ::acceptOrder(const Order &order) {
            if (canTakeOrder(order))
            {
                distanceLeft=order.getDistance();
                 activeOrderId = order.getId();
            }
            
        } // Assign distanceLeft to order's distance
        void DriverVolunteer:: step() {
            decreaseDistanceLeft();
            
        } // Decrease distanceLeft by distancePerStep
        string  DriverVolunteer:: toString() const {
            std:: string s1="VolunteerID:";
            s1 = s1 + std::to_string(getId());
            std::string s2="isBusy:";
            if(isBusy()){
                s2=s2+"False";
            }
            else{
                s2=s2+"True";
            }
            std:: string s3="OrderID:";
             if(isBusy()){
                s3=s3+std::to_string( activeOrderId);
             }
             else{
                s3=s3+"None";
             }
          std::string s4="TimeLeft:";
          if (getDistanceLeft()==0)
          {
            s4=s4+"None";
          }
          else{
            s4=s4+std::to_string(getDistanceLeft());
          }
            std:: string s5="OrdersLeft:No Limit";
            
           
            string line="\n";
            string rst=s1+ line +s2+line+s3+line+s4+line+s5;
            
            
            return rst;
            

        }
        string DriverVolunteer:: VolType(){
            return "Driver";
        }

   
    LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
        DriverVolunteer(id,name,maxDistance,distancePerStep) ,maxOrders(maxOrders),ordersLeft(maxOrders)
        {}
          LimitedDriverVolunteer * LimitedDriverVolunteer::clone() const {
             return new LimitedDriverVolunteer(*this);
        }


        bool LimitedDriverVolunteer::hasOrdersLeft() const {
            return ordersLeft>0;
        }
        bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const {
      
            return DriverVolunteer::canTakeOrder(order) & hasOrdersLeft();
        }
        void LimitedDriverVolunteer:: acceptOrder(const Order &order) {
            if (canTakeOrder(order))
            {
              DriverVolunteer::setDistanceLeft( order.getDistance());
               activeOrderId = order.getId();
                ordersLeft--;
            }
            
        }

        int LimitedDriverVolunteer:: getMaxOrders() const{
            return maxOrders;
        }
        int LimitedDriverVolunteer:: getNumOrdersLeft() const{
            return ordersLeft;
        }
        string LimitedDriverVolunteer::toString() const {
             std:: string s1="VolunteerID:";
            s1 = s1 + std::to_string(getId());

            std::string s2="isBusy:";
            if(isBusy()){
                s2=s2+"False";
            }
            else{
                s2=s2+"True";
            }
            std:: string s3="OrderID:";
             if(isBusy()){
                s3=s3+std::to_string( activeOrderId);
             }
             else{
                s3=s3+"None";
             }
          std::string s4="TimeLeft:";
          if (getDistanceLeft()==0)
          {
            s4=s4+"None";
          }
          else{
            s4=s4+std::to_string(getDistanceLeft());
          }
          
            std:: string s5="OrdersLeft:";
            s5=s5+std::to_string(ordersLeft);
            
           
            string line="\n";
            string rst=s1+ line +s2+line+s3+line+s4+line+s5;
            
            
            return rst;
            


        }
         string LimitedDriverVolunteer:: VolType(){
            return "LimitedDriver";
        }

   