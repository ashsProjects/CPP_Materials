#ifndef PRIORITY_EVENT_H_INCLUDED
#define PRIORITY_EVENT_H_INCLUDED

#include <string>
#include <ostream>
#include "Event.h"

class Priority_Event: public Event {
    std::string priority_name = "New Event";
    short priority_value = 10;
    constexpr static const char * priority_error = "Priority_Event - Error cannot set a priority less than 0. Given Priority: ";
  public:
    //Big 5
    Priority_Event(const Priority_Event &rhs) = default;
    Priority_Event &operator=(const Priority_Event &rhs) = default;
    Priority_Event &operator=(Priority_Event &&rhs) = default;
    Priority_Event(Priority_Event &&rhs) = default;
    virtual ~Priority_Event() = default;
    //constructors
    Priority_Event(): Event() {}
    Priority_Event(const std::string &name): priority_name(name), Event() {}
    Priority_Event(const std::string &name, short priority): priority_name(name), priority_value(validateValue(priority)), Event() {}
    Priority_Event(const std::string &name, const std::string &date): priority_name(name), Event(date) {}
    Priority_Event(const std::string &name, const std::string &date, short priority): priority_name(name), priority_value(validateValue(priority)), Event(date) {}
    Priority_Event(const std::string &name, int y, int m, int d): priority_name(name) {set(y,m,d);}
    Priority_Event(const std::string &name, int y, int m, int d, short priority): priority_name(name), priority_value(validateValue(priority)) {set(y,m,d);}

    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Event &rhs): A reference to an event object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current Priority_Event 
     * object to the Event object passed in as the parameter to this method. The comparison is
     * done on the year, month, and day of each of the objects. 
    */
    virtual bool operator<(const Event &rhs) const;
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
    virtual bool operator<(const std::string &rhs) const;
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Priority_Event &rhs): A reference to an Priority_Event object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current object
     * to the object passed in as the parameter to this method. The comparison is
     * done on the year, month, and day of each of the objects. In case the dates are
     * the same, it checks the priority of both events.
    */
    virtual bool operator<(const Priority_Event &rhs) const;
    /**
     * GetName
     * 
     * Params: None
     * Returns: A string value of the name
     * 
     * This methods is a getter that returns the string name of the object.
    */
    std::string GetName() const {return this->priority_name;}
    /**
     * GetPriority
     * 
     * Params: None
     * Returns: A short value of the priority. Default is 10.
     * 
     * This methods is a getter that returns the priority of the object.
     * 
    */
    short GetPriority() const {return this->priority_value;}
    /**
     * Method: ValidateValue
     * 
     * Params:
     *  1. (short value): A short containing the priority value
     * 
     * Returns: The same value if an error is not thrown
     * 
     * This method is used to check if the given priority value is less 
     * than 0. If false, it throws an error.
    */
   short validateValue(short value) const;
};

/**
 * Method: operator<<
 * 
 * Params:
 *  1. (ostream &os): reference to the output stream used for printing
 *  2. (const Priority_Event &rhs): a reference to a Priority_Event object
 * 
 * Returns: An ostream for printing the string to screen
 * 
 * This is an overloaded << operator. It is used to print the string from
 * the ToString() method, which outputs the string representation of the
 * current generation of the object.
*/
std::ostream& operator<<(std::ostream &out, const Priority_Event &rhs);

#endif/*PRIORITY_EVENT_H_INCLUDED*/