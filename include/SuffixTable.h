#ifndef _SUFFIX_TABLE_H
#define _SUFFIX_TABLE_H

#include <algorithm>
#include <iterator>
#include <iostream>
#include <bitset>
#include "DNA.h"

#include "StaticFormat.h"

typedef unsigned int Index;

class SuffixTable {

public:
	SuffixTable();
	~SuffixTable();

	template<typename SequenceType>
	void pre_compute(const SequenceType& sequence) {
		unsigned int sequence_size = sequence.size();
		_suffix_table = new Index[sequence_size];

		for(unsigned int i=0; i<sequence_size; i++)
			_suffix_table[i] = i;

		std::sort(_suffix_table, _suffix_table+sequence_size, [sequence_size, &sequence, this](Index i1, Index i2) {
				for(Index i = 0; i1+i < sequence_size && i2+i < sequence_size; i++) {
					if(sequence[i1+i] != sequence[i2+i]) {
						return sequence[i1+i] < sequence[i2+i];
					}
				}
			return i1 > i2;
		});

	}

	Index operator[](unsigned int i) const {
		return _suffix_table[i];
	}


protected:
	 Index* _suffix_table;


};

#endif
