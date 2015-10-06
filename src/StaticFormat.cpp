#include "StaticFormat.h"

StaticFormat::StaticFormat() : _sequence() {

}

StaticFormat::StaticFormat(const std::string& sequence) : _sequence(sequence) {

}

unsigned int StaticFormat::size() const {
	return _sequence.size();
}

char StaticFormat::operator[](unsigned int cursor) const {
	return _sequence[cursor];
}

StaticFormat StaticFormat::add_end_marker(char marker) const {
	return StaticFormat(_sequence+marker);
}

StaticFormat StaticFormat::substr(unsigned int begin, unsigned int end) const {
	return StaticFormat(_sequence.substr(begin, end-begin));
}

StaticFormat StaticFormat::reverse() const {
	return StaticFormat(std::string(_sequence.rbegin(), _sequence.rend()));
}

std::ostream& operator<<(std::ostream& os, const StaticFormat& that) {
	for(unsigned int i=0; i<that.size(); i++)
		os.put(that[i]);
	return os;
}
