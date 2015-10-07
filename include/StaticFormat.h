#ifndef _STATIC_FORMAT_H
#define _STATIC_FORMAT_H

#include <string>
#include <sstream>
#include <algorithm>

class StaticFormat {

public:
	StaticFormat();
	StaticFormat(const std::string& sequence);

	unsigned int size() const;
	char operator[](unsigned int cursor) const;

	void add_end_marker(const StaticFormat& model, char marker);
	void substr(const StaticFormat& model, unsigned int begin, unsigned int end);
	void reverse();
	void append(const StaticFormat& that);


	friend std::ostream& operator<<(std::ostream& os, const StaticFormat& that);

private:
	std::string _sequence;
};

#endif
