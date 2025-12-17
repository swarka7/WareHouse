#include <string>
#include <vector>
#include <iostream>
#include <Order.h> 

using std::string;
using std::vector;


Order::Order(int id, int customerId, int distance):
id(id),customerId(customerId),distance(distance) ,status( OrderStatus::PENDING),collectorId(NO_VOLUNTEER),driverId(NO_VOLUNTEER){}

 int Order::getId() const{
    return this->id;
 }
 int Order::getDistance() const{
    return distance;
 }

 int Order::getCustomerId() const{
            return customerId;
        }
void Order::setStatus(OrderStatus status){
            this->status=status;
        }

void Order::setCollectorId(int collectorId){
            this->collectorId=collectorId;
        }
void Order::setDriverId(int driverId){
            this->driverId=driverId;
        }
int Order::getCollectorId() const{
            return collectorId;
        }
int Order::getDriverId() const{
            return driverId;
        }
 OrderStatus Order::getStatus() const{
            return status;
        }
         string Order::statusprosses(OrderStatus status) const{
            switch (status) {
        case OrderStatus::PENDING:
        {
            return "Pending";
            break;
        }
        case OrderStatus::COLLECTING:
        {
            return "Collecting";
            break;
        }
        case OrderStatus::DELIVERING:
        {
            return "Delivering";
            break;
        }
        case OrderStatus::COMPLETED:
        {
            return "Completed";
            break;
        }
        }
        return "";
        }
          const string Order::toString() const{
            std:: string s1="OrderId:";
            s1 = s1 + std::to_string(id);
            std::string s2="OrderStatus:"+ statusprosses(status);
            std:: string s3="CustomerID:"+ std::to_string(customerId);
            std:: string s4;
            if (collectorId!=NO_VOLUNTEER)
            {
                 s4="Collector:"+ std::to_string(collectorId);
            }
            else{
                s4="Collector:None";
            }
            std:: string s5;
            if (driverId!=NO_VOLUNTEER)
            {
                 s5="Driver:"+ std::to_string(driverId);
            }
            else{
                 s5="Driver:None";
            }
            string line="\n";
            string rst=s1+ line +s2+line+s3+line+s4+line +s5;
            
            
            return rst;

          }