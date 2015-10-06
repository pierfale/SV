#include "SuffixTable.h"

SuffixTable::SuffixTable() : _suffix_table(nullptr) {

}

SuffixTable::~SuffixTable() {
	delete _suffix_table;
}
