#include "Workday.h"
#include <iomanip>
#include <string>
#include <ostream>

bool Workday::operator<(const Event &rhs) const
{
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

bool Workday::operator<(const std::string &rhs) const
{
    Event tmp(rhs);
    return (*this)<tmp;
}

bool Workday::operator<(const Priority_Event &rhs) const
{
    if (year() == rhs.year() && month() == rhs.month() && day() == rhs.day()) {
        return rhs.GetPriority() < 10;
    }
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

bool Workday::operator<(const Workday &rhs) const
{
    if (year() == rhs.year() && month() == rhs.month() && day() == rhs.day()) {
        return this->hourly_pay < rhs.GetPay();
    }
    return year() < rhs.year() ||
        (year() == rhs.year() && (month() < rhs.month() || (month() == rhs.month() && day() < rhs.day())));
}

std::ostream &operator<<(std::ostream &out, const Workday &rhs)
{
    const auto save_fill = std::cout.fill('0');
    return out << rhs.GetName() << ", Pay: $" << rhs.GetPay() << ", Date: "
			   << std::setw(4) << rhs.year() << '-'
			   << std::setw(2) << rhs.month() << '-'
			   << std::setw(2) << rhs.day() << std::setfill(save_fill);
}
