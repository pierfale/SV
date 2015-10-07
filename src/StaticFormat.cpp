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

void StaticFormat::add_end_marker(const StaticFormat& model, char marker) {
	_sequence = model._sequence+marker;
}

void StaticFormat::substr(const StaticFormat& model, unsigned int begin, unsigned int end) {
	_sequence = model._sequence.substr(begin, end-begin);
}

void StaticFormat::reverse(){
	std::reverse(_sequence.begin(), _sequence.end());
}

void StaticFormat::append(const StaticFormat& that) {
	_sequence += that._sequence;
}

std::ostream& operator<<(std::ostream& os, const StaticFormat& that) {
	for(unsigned int i=0; i<that.size(); i++)
		os.put(that[i]);
	return os;
}
