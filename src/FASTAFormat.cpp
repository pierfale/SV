#include "FASTAFormat.h"

DataFASTA::DataFASTA() : _sequence(nullptr), _length(0) {

}

DataFASTA::DataFASTA(const DataFASTA& that) : _sequence((char*)malloc(that._length)), _length(that._length)  {
	memcpy(_sequence, that._sequence, _length);
}

DataFASTA::~DataFASTA() {
	free(_sequence);
}

void DataFASTA::set(const char* str, unsigned int size) {
	free(_sequence);

	_length = size;
	_sequence = (char*)malloc(_length);
	memcpy(_sequence, str, _length);

	for(unsigned int i=0; i< _length; i++) {
		switch(_sequence[i]) {

		case 'a': _sequence[i] = 'A'; break;
		case 'A': break;
		case 'c': _sequence[i] = 'C'; break;
		case 'C': break;
		case 'g': _sequence[i] = 'G'; break;
		case 'G': break;
		case 't': _sequence[i] = 'T'; break;
		case 'T': break;
		default: switch(::rand()%4) {
			case 0:
				_sequence[i] = 'A';
				break;
			case 1:
				_sequence[i] = 'C';
				break;
			case 2:
				_sequence[i] = 'G';
				break;
			case 3:
				_sequence[i] = 'T';
				break;
			}
		}
	}
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
			for(unsigned int i=0; i< length; i++) {
				switch(buffer[i]) {

				case 'a': buffer[i] = 'A'; break;
				case 'A': break;
				case 'c': buffer[i] = 'C'; break;
				case 'C': break;
				case 'g': buffer[i] = 'G'; break;
				case 'G': break;
				case 't': buffer[i] = 'T'; break;
				case 'T': break;
				default: switch(::rand()%4) {
					case 0:
						buffer[i] = 'A';
						break;
					case 1:
						buffer[i] = 'C';
						break;
					case 2:
						buffer[i] = 'G';
						break;
					case 3:
						buffer[i] = 'T';
						break;
					}
				}
			}

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
