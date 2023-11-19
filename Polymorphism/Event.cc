#include "Event.h"
#include <string.h>
#include <stdexcept>
#include <ctime>
#include <iomanip>

Event::Event(const std::string &input) {
    if (!translate_date(input, yyyy, mm, dd))
       throw std::runtime_error("Bad date “" + input + "”");
}

Event::Event() {
    translate_date("today", yyyy, mm, dd);
}

void Event::set(int year, int month, int day) {
    if (year<1 || year > 9999)
	throw std::runtime_error("Year "+ std::to_string(year)
			    + " not in range 1≤year≤9999");
    // Are these values valid?
    struct tm tm = {};				// Zero-initialize it all.
    tm.tm_hour = 12;			// Avoid DST problems.
    tm.tm_mon = month-1;		// Jan=0, Feb=1, …
    tm.tm_mday = day;
    tm.tm_year = year-1900;		// 1900-based year
    tm.tm_isdst = -1;			// You figure it out, std::mktime.
    
    // We succeed if nothing changed!
    if (std::mktime(&tm) == -1
	|| tm.tm_mon!=month-1
	|| tm.tm_mday!=day
	|| tm.tm_year!=year-1900)
	throw(std::runtime_error("Bad date " + std::to_string(year) + "-"
	      + std::to_string(month) + "-" + std::to_string(day)));

    yyyy = year;
    mm = month;
    dd = day;
}

int Event::year() const {
    return yyyy;
}

int Event::month() const {
    return mm;
}

int Event::day() const {
    return dd;
}

bool Event::operator<(const Event &rhs) const {
    return yyyy < rhs.yyyy ||
        (yyyy == rhs.yyyy && (mm < rhs.mm || (mm == rhs.mm && dd < rhs.dd)));
}

bool Event::operator<(const std::string &rhs) const{
    Event tmp(rhs);
    return (*this)<tmp;
}

std::ostream &operator<<(std::ostream &os, const Event &e) {
    const auto save_fill = os.fill('0');
    return os << std::setw(4) << e.year() << '-'
	      << std::setw(2) << e.month() << '-'
	      << std::setw(2) << e.day() << std::setfill(save_fill);
}

static bool yyyy_ddd(const std::string &line, struct tm &tm) {
    const char digits[] = "0123456789";
    const auto dot = line.find_first_not_of(digits);
    if (dot==0 || dot==line.npos || line[dot] != '.')
	return false;
    const auto end = line.find_first_not_of(digits, dot+1);
    if (end == dot+1 || end != line.npos)
	return false;

    const int year = stoi(line.substr(0, dot)),
	      day = stoi(line.substr(dot+1));

    if (year < 1 || year > 9999)
	return false;

    tm = {};				// Zero-initialize it all.
    tm.tm_hour = 12;			// Avoid DST problems.
    tm.tm_mday = day;			// std::mktime will normalize Jan 123
    tm.tm_year = year-1900;		// 1900-based year
    tm.tm_isdst = -1;			// You figure it out, std::mktime.
    if (std::mktime(&tm) == -1)		// Normalize date (Jan 33⇒Feb 2)
	return false;
    // If the year changed, then the day number was out of range.
    if (tm.tm_year != year-1900)	// Jan 0 or Jan 367?
	return false;

    return true;
}

static bool yyyy_mm_dd(const std::string &line, struct tm &tm) {
    if (line.size() != 10) return false;
    if (!isdigit(line[0])) return false;
    if (!isdigit(line[1])) return false;
    if (!isdigit(line[2])) return false;
    if (!isdigit(line[3])) return false;
    if (line[4] != '-')    return false;
    if (!isdigit(line[5])) return false;
    if (!isdigit(line[6])) return false;
    if (line[7] != '-')    return false;
    if (!isdigit(line[8])) return false;
    if (!isdigit(line[9])) return false;

    const int year = stoi(line.substr(0, 4)),
	      month = stoi(line.substr(5, 2)),
	      day = stoi(line.substr(8));

    if (year == 0)			// Year is 0001…9999
	return false;

    tm = {};				// Zero-initialize it all.
    tm.tm_hour = 12;			// Avoid DST problems.
    tm.tm_mon = month-1;		// Jan=0, Feb=1, …
    tm.tm_mday = day;
    tm.tm_year = year-1900;		// 1900-based year
    tm.tm_isdst = -1;			// You figure it out, std::mktime.
    
    // We succeed if nothing changed!
    return std::mktime(&tm) != -1
	   && tm.tm_mon==month-1
	   && tm.tm_mday==day
	   && tm.tm_year==year-1900;
}

static bool date(const std::string &line, struct tm &tm, const char *tag, int offset) {
    if (strcasecmp(line.c_str(), tag))
	return false;
    const auto now = time(nullptr);
    tm = *localtime(&now);
    tm.tm_mday += offset;
    std::mktime(&tm);
    return true;
}

// Translate a date.  Return true for success.
bool translate_date(const std::string &line, int &year, int &month, int &day) {
    struct tm tm;
    if (!(yyyy_ddd(line, tm) || yyyy_mm_dd(line, tm)
	|| date(line, tm, "yesterday", -1)
	|| date(line, tm, "today",      0)
	|| date(line, tm, "tomorrow",  +1)))
       return false;
    year = tm.tm_year + 1900;
    month = tm.tm_mon + 1;
    day = tm.tm_mday;
    return true;
}

