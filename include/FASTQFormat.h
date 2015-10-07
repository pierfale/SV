#ifndef _FORMAT_FASTQ_H
#define _FORMAT_FASTQ_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <algorithm>

#include "FASTAFormat.h"

class ParserFASTQ {

public:
	ParserFASTQ(const std::string& filename);
	~ParserFASTQ();

	bool next(DataFASTA& data);

private:
	std::ifstream _file;

};


#endif
