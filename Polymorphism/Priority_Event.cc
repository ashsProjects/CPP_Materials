#include "Priority_Event.h"
#include <stdexcept>
#include <iomanip>
#include <string>

bool Priority_Event::operator<(const Event &rhs) const
{
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

bool Priority_Event::operator<(const std::string &rhs) const
{
    Event tmp(rhs);
    return (*this)<tmp;
}

bool Priority_Event::operator<(const Priority_Event &rhs) const
{
    if (year() == rhs.year() && month() == rhs.month() && day() == rhs.day()) {
        return GetPriority() < rhs.GetPriority();
    }
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

short Priority_Event::validateValue(short value) const
{
    if (value < 0) 
        throw(std::runtime_error(this->priority_error + value + '\n'));
    
    else return value;
}

std::ostream &operator<<(std::ostream &out, const Priority_Event &rhs)
{
    const auto save_fill = std::cout.fill('0');
    return out << rhs.GetName() << ", Priority: " << rhs.GetPriority() << ", Date: "
			   << std::setw(4) << rhs.year() << '-'
			   << std::setw(2) << rhs.month() << '-'
			   << std::setw(2) << rhs.day() << std::setfill(save_fill);
}
