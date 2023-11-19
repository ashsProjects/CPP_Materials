#ifndef WORKDAY_H_INCLUDED
#define WORKDAY_H_INCLUDED

#include "Event.h"
#include "Priority_Event.h"
#include <string>
#include <iostream>

class Workday: public Event {
    std::string job_name;
    double hourly_pay = 15.00;
  public:
    //Big 5
    Workday(const Workday &rhs) = default;
    Workday &operator=(const Workday &rhs) = default;
    Workday &operator=(Workday &&rhs) = default;
    Workday(Workday &&rhs) = default;
    virtual ~Workday() = default;
    //delete default constructor
    Workday() = delete;
    //constructors
    Workday(const std::string &name): job_name(name), Event() {}
    Workday(const std::string &name, double pay): job_name(name), hourly_pay(pay), Event() {}
    Workday(const std::string &name, const std::string &date): job_name(name), Event(date) {}
    Workday(const std::string &name, const std::string &date, double pay): job_name(name), hourly_pay(pay), Event(date) {}
    Workday(const std::string &name, int y, int m, int d): job_name(name) {set(y,m,d);}
    
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Event &rhs): A reference to an event object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current object
     * to the object passed in as the parameter to this method. The comparison is
     * done on the year, month, and day of each of the objects.
    */
    bool operator<(const Event &rhs) const;
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const string &rhs): A reference to a string containing a date
     * 
     * Returns: A booelan if this object is earlier than date in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current object
     * to the date passed in as the parameter to this method. The method will first
     * create an Event object and use the above and call the above method. The comparison is
     * done on the year, month, and day of each of the objects.
    */
    bool operator<(const std::string &rhs) const;
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Priority_Event &rhs): A reference to a priority_event object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current object
     * to the object passed in as the parameter to this method. Compared to the other mehtods,
     * this method compares a Workday object to a priority event object. The comparison is
     * done on the year, month, and day of each of the objects. In case they are the same dates, 
     * the priority of the rhs object is checked to see if it is less than 10.
    */
    bool operator<(const Priority_Event &rhs) const;
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Event &rhs): A reference to an workday object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current object
     * to the object passed in as the parameter to this method. Compared to the other mehtods,
     * this method compares a workday object to another workday object. The comparison is
     * done on the year, month, and day of each of the objects. In case they are the same dates,
     * the hourly_pay is compared.
    */
    bool operator<(const Workday &rhs) const;
    /**
     * GetName
     * 
     * Params: None
     * Returns: A string value of the name
     * 
     * This methods is a getter that returns the string name of the object.
     * 
    */
    std::string GetName() const {return this->job_name;}
    /**
     * GetPay
     * 
     * Params: None
     * Returns: A double value of the hourly pay
     * 
     * This methods is a getter that returns the double hourly pay of the object.
     * 
    */
    double GetPay() const {return this->hourly_pay;}
};

/**
 * Method: operator<<
 * 
 * Params:
 *  1. (ostream &os): reference to the output stream used for printing
 *  2. (const Workday &rhs): a reference to a Workday object
 * 
 * Returns: An ostream for printing the string to screen
 * 
 * This is an overloaded << operator. It is used to print the string from
 * the ToString() method, which outputs the string representation of the
 * current generation of the object.
*/
std::ostream& operator<<(std::ostream &out, const Workday &rhs);

#endif/*WORKDAY_H_INCLUDED*/