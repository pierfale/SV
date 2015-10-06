#ifndef _FORMAT_FASTA_H
#define _FORMAT_FASTA_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <cstring>

class DataFASTA {

	friend class ParserFASTA;

public:
	DataFASTA();
	~DataFASTA();

	unsigned int size() const;
	char operator[](unsigned int cursor) const;


	DataFASTA add_end_marker(char marker) const;
	DataFASTA substr(unsigned int begin, unsigned int end) const;
	DataFASTA reverse() const;

	friend std::ostream& operator<<(std::ostream& os, const DataFASTA& that);

private:
	char* _sequence;
	unsigned int _length;
};

class ParserFASTA {

public:
	static void process(const std::string& filename, DataFASTA& data);

private:
	ParserFASTA();
	ParserFASTA(const ParserFASTA& that);
	ParserFASTA& operator=(const ParserFASTA& that);
};


#endif
