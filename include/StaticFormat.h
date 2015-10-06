#ifndef _STATIC_FORMAT_H
#define _STATIC_FORMAT_H

#include <string>
#include <iostream>

class StaticFormat {

public:
	StaticFormat();
	StaticFormat(const std::string& sequence);

	unsigned int size() const;
	char operator[](unsigned int cursor) const;
	StaticFormat add_end_marker(char marker) const;
	StaticFormat substr(unsigned int begin, unsigned int end) const;
	StaticFormat reverse() const;

	friend std::ostream& operator<<(std::ostream& os, const StaticFormat& that);

private:
	std::string _sequence;
};

#endif
