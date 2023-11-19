#include "Holiday.h"
#include <iomanip>
#include <string>

bool Holiday::operator<(const Event &rhs) const
{
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

bool Holiday::operator<(const std::string &rhs) const
{
    Event tmp(rhs);
    return (*this)<tmp;
}

bool Holiday::operator<(const Priority_Event &rhs) const
{
    if (year() == rhs.year() && month() == rhs.month() && day() == rhs.day()) {
        return GetPriority() < rhs.GetPriority();
    }
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

bool Holiday::operator<(const Holiday &rhs) const
{
    if (year() == rhs.year() && month() == rhs.month() && day() == rhs.day()) {
        return GetPriority() < rhs.GetPriority();
    }
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

std::ostream &operator<<(std::ostream &out, const Holiday &rhs)
{
    const auto save_fill = std::cout.fill('0');
    std::string is_holiday = rhs.GetPriority() == 0 ? "Is " : "Is Not ";
    return out << rhs.GetName() << ", " << is_holiday << "a Govenment Holiday, Date: "
			   << std::setw(4) << rhs.year() << '-'
			   << std::setw(2) << rhs.month() << '-'
			   << std::setw(2) << rhs.day() << std::setfill(save_fill);
}
