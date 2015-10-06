#include "FASTAFormat.h"

DataFASTA::DataFASTA() : _sequence(nullptr), _length(0) {

}

DataFASTA::~DataFASTA() {
	free(_sequence);
}

unsigned int DataFASTA::size() const {
	return _length;
}
char DataFASTA::operator[](unsigned int cursor) const {
	return _sequence[cursor];
}

void ParserFASTA::process(const std::string& filename, DataFASTA& data) {
	std::fstream file(filename, std::ios::in);

	if(!file.is_open())
		throw std::runtime_error("Unable to open \""+filename+"\"");

#	define ALLOC_BLOCK_SIZE 4096

	char buffer[ALLOC_BLOCK_SIZE];

	file.getline(buffer, ALLOC_BLOCK_SIZE);

	do {
		file.getline(buffer, ALLOC_BLOCK_SIZE);
		std::streamsize length = file.gcount()-1;

		if(length > 0) {
			unsigned int cursor = data._length;
			data._length += length;
			data._sequence = data._sequence != nullptr ? (char*)realloc(data._sequence, data._length) : (char*)malloc(data._length);
			memcpy(data._sequence+cursor, buffer, length);
		}

	} while(!file.eof());


}

DataFASTA DataFASTA::add_end_marker(char marker) const {
	DataFASTA data;
	data._sequence = (char*)malloc(_length);
	memcpy(data._sequence, _sequence, _length);
	data._length = _length+1;
	data._sequence[_length] = marker;
	return data;
}

DataFASTA DataFASTA::substr(unsigned int begin, unsigned int end) const {
	DataFASTA data;
	data._length = end > _length ? _length-begin : end-begin;
	data._sequence = (char*)malloc(data._length);
	memcpy(data._sequence, _sequence+begin, data._length);

	return data;
}

DataFASTA DataFASTA::reverse() const {
	DataFASTA data;
	data._sequence = (char*)malloc(_length);
	for(unsigned int i=0; i<_length; i++)
		data._sequence[_length-i-1] = _sequence[i];
	data._length = _length;
	return data;
}

std::ostream& operator<<(std::ostream& os, const DataFASTA& that)  {
	for(unsigned int i=0; i<that.size(); i++)
		os.put(that[i]);
	return os;
}
