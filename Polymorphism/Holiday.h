#ifndef HOLIDAY_H_INCLUDED
#define HOLIDAY_H_INCLUDED

#include <string>
#include <ostream>
#include "Priority_Event.h"

class Holiday: public Priority_Event {
  public:
    //Big 5
    Holiday(const Holiday &rhs) = default;
    Holiday &operator=(const Holiday &rhs) = default;
    Holiday &operator=(Holiday &&rhs) = default;
    Holiday(Holiday &&rhs) = default;
    virtual ~Holiday() = default;
    //delete default constructor
    Holiday() = delete;
    //constructors
    Holiday(const std::string &name): Priority_Event(name, 15) {}
    Holiday(const std::string &name, bool is_gov): Priority_Event(name, 0) {}
    Holiday(const std::string &name, const std::string &date): Priority_Event(name, date, 15) {}
    Holiday(const std::string &name, const std::string &date, bool is_gov): Priority_Event(name, date, 0) {}
    Holiday(const std::string &name, int y, int m, int d): Priority_Event(name,y,m,d,15) {}
    Holiday(const std::string &name, int y, int m, int d, bool is_gov): Priority_Event(name,y,m,d,0) {}

    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Event &rhs): A reference to an event object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current Holiday 
     * object to the Event object passed in as the parameter to this method. The comparison is
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
     * This is an overloaded < (less than) operator. It will compare the current Holiday object
     * to the date passed in as the parameter to this method. The method will first
     * create an Event object and use the above and call the above method. The comparison is
     * done on the year, month, and day of each of the objects.
    */
    bool operator<(const std::string &rhs) const;
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Priority_Event &rhs): A reference to an Priority_Event object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current Priority_Event 
     * object to the Priority_Event object passed in as the parameter to this method. The comparison is
     * done on the year, month, and day of each of the objects. In case they are the same date, 
     * the priority of the obejcts are compared.
    */
    bool operator<(const Priority_Event &rhs) const;
    /**
     * Method: operator<
     * 
     * Params:
     *  1. (const Holiday &rhs): A reference to an Holiday object
     * 
     * Returns: A booelan if this object is earlier than object in parameter
     * 
     * This is an overloaded < (less than) operator. It will compare the current Holiday 
     * object to another Holiday object passed in as the parameter to this method. The comparison is
     * done on the year, month, and day of each of the objects. In case they are the same date,
     * the priority of the objects are checked.
    */
    bool operator<(const Holiday &rhs) const;
};

/**
 * Method: operator<<
 * 
 * Params:
 *  1. (ostream &os): reference to the output stream used for printing
 *  2. (const Holiday &rhs): a reference to a Holiday object
 * 
 * Returns: An ostream for printing the string to screen
 * 
 * This is an overloaded << operator. It is used to print the string from
 * the ToString() method, which outputs the string representation of the
 * current generation of the object.
*/
std::ostream& operator<<(std::ostream &out, const Holiday &rhs);

#endif/*HOLIDAY_H_INCLUDED*/