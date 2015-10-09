#ifndef _FORMAT_FASTA_H
#define _FORMAT_FASTA_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <algorithm>

class DataFASTA {

	friend class ParserFASTA;

public:
	DataFASTA();
	DataFASTA(const DataFASTA& that);

	~DataFASTA();

	void set(const char* str, unsigned int size);

	unsigned int size() const;
	char operator[](unsigned int cursor) const;


	void add_end_marker(const DataFASTA& model, char marker);
	void substr(const DataFASTA& model, unsigned int begin, unsigned int end);
	void reverse();
	void append(const DataFASTA& that);

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
