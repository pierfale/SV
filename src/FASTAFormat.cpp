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

void DataFASTA::add_end_marker(const DataFASTA& model, char marker) {
	_sequence = (char*)malloc(model._length+1);
	memcpy(_sequence, model._sequence, model._length);
	_length = model._length+1;
	_sequence[model._length] = marker;
}

void DataFASTA::substr(const DataFASTA& model, unsigned int begin, unsigned int end) {
	_length = end > model._length ? model._length-begin : end-begin;
	_sequence = (char*)malloc(_length);
	memcpy(_sequence, model._sequence+begin, _length);
}

void DataFASTA::reverse() {
	std::reverse(&_sequence[0], &_sequence[_length]);
}

void DataFASTA::append(const DataFASTA& that) {
	_sequence = (char*)realloc(_sequence, _length+that._length);
	memcpy(_sequence+_length, that._sequence, that._length);
	_length += that._length;
}

std::ostream& operator<<(std::ostream& os, const DataFASTA& that)  {
	for(unsigned int i=0; i<that.size(); i++)
		os.put(that[i]);
	return os;
}
