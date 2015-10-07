#include "FASTQFormat.h"

ParserFASTQ::ParserFASTQ(const std::string& filename) : _file(filename, std::ios::in) {

}

ParserFASTQ::~ParserFASTQ() {
	_file.close();
}

bool ParserFASTQ::next(DataFASTA& data) {
	std::string line;

	std::getline(_file, line); // ID String
	std::getline(_file, line); // String
	data.set(line.c_str(), line.size());
	std::getline(_file, line); // ID String Quality
	std::getline(_file, line); // String Quality

	return !_file.eof();
}
