#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <iostream>
#include <string>

class Event {
    int yyyy, mm, dd;
  public:
    Event();
    Event(const std::string &);
    Event(const Event &) = default;
    Event(Event &&rhs) = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&rhs) = default;
    virtual ~Event() = default;
    void set(int, int, int);
    int year() const;
    int month() const;
    int day() const;
    virtual bool operator<(const Event &) const;
    virtual bool operator<(const std::string &) const;
};

bool translate_date(const std::string &line, int &year, int &month, int &day);
std::ostream &operator<<(std::ostream &, const Event &);

#endif /* EVENT_H_INCLUDED */
