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

		unsigned int compressed_sequence_size = sequence_size % 4 != 0 ? sequence_size/4+1 : sequence_size/4;
		Base4* compressed_sequence = new Base4[compressed_sequence_size];
		for(unsigned int i=0; i<compressed_sequence_size; i++) {
			compressed_sequence[i].base_1 = Base4::base_2_base4(sequence[i*4]);
			compressed_sequence[i].base_2 = Base4::base_2_base4(i*4+1 < sequence_size ? sequence[i*4+1] : Base4::Base_A);
			compressed_sequence[i].base_3 = Base4::base_2_base4(i*4+2 < sequence_size ? sequence[i*4+2] : Base4::Base_A);
			compressed_sequence[i].base_4 = Base4::base_2_base4(i*4+3 < sequence_size ? sequence[i*4+3] : Base4::Base_A);
		}

		std::sort(_suffix_table, _suffix_table+sequence_size, [sequence_size, &sequence, compressed_sequence, compressed_sequence_size, this](Index i1, Index i2) {
			/*if(i1%4 == i2%4) {
				uint8_t mask = 0x0;

				union mask_field {
					mask_field() : mask1(0) {

					}

					struct {
						uint8_t mask1 : 2;
						uint8_t mask2 : 2;
						uint8_t mask3 : 2;
						uint8_t mask4 : 2;
					};
					uint8_t mask;
				};

				static mask_field mask_field_1;
				mask_field_1.mask1 = 0x3;
				static mask_field mask_field_2;
				mask_field_2.mask2 = 0x3;
				static mask_field mask_field_3;
				mask_field_3.mask3 = 0x3;
				static mask_field mask_field_4;
				mask_field_4.mask4 = 0x3;


				switch(i1%4) {
				case 0: mask |= mask_field_1.mask;
				case 1: mask |= mask_field_2.mask;
				case 2: mask |= mask_field_3.mask;
				case 3: mask |= mask_field_4.mask;
				}

				if((compressed_sequence[i1/4].base & mask) != (compressed_sequence[i2/4].base & mask)) {
					switch(i1%4) {
						case 0:
							if((compressed_sequence[i1/4].base & mask_field_1.mask) != (compressed_sequence[i2/4].base & mask_field_1.mask))
								return (compressed_sequence[i1/4].base & mask_field_1.mask) < (compressed_sequence[i2/4].base & mask_field_1.mask);
						case 1:
							if((compressed_sequence[i1/4].base & mask_field_2.mask) != (compressed_sequence[i2/4].base & mask_field_2.mask))
								return (compressed_sequence[i1/4].base & mask_field_2.mask) < (compressed_sequence[i2/4].base & mask_field_2.mask);
						case 2:
							if((compressed_sequence[i1/4].base & mask_field_3.mask) != (compressed_sequence[i2/4].base & mask_field_3.mask))
								return (compressed_sequence[i1/4].base & mask_field_3.mask) < (compressed_sequence[i2/4].base & mask_field_3.mask);
						case 3:
							if((compressed_sequence[i1/4].base & mask_field_4.mask) != (compressed_sequence[i2/4].base & mask_field_4.mask))
								return (compressed_sequence[i1/4].base & mask_field_4.mask) < (compressed_sequence[i2/4].base & mask_field_4.mask);
					}
				}

				for(Index i = 1; (i1/4)+i < compressed_sequence_size && (i2/4)+i < compressed_sequence_size; i++) {
					if(compressed_sequence[i1/4+i] != compressed_sequence[i2/4+i]) {
						if((compressed_sequence[i1/4+i].base & mask_field_1.mask) != (compressed_sequence[i2/4+i].base & mask_field_1.mask))
							return (compressed_sequence[i1/4+i].base & mask_field_1.mask) < (compressed_sequence[i2/4+i].base & mask_field_1.mask);
						if((compressed_sequence[i1/4+i].base & mask_field_2.mask) != (compressed_sequence[i2/4+i].base & mask_field_2.mask))
							return (compressed_sequence[i1/4+i].base & mask_field_2.mask) < (compressed_sequence[i2/4+i].base & mask_field_2.mask);
						if((compressed_sequence[i1/4+i].base & mask_field_3.mask) != (compressed_sequence[i2/4+i].base & mask_field_3.mask))
							return (compressed_sequence[i1/4+i].base & mask_field_3.mask) < (compressed_sequence[i2/4+i].base & mask_field_3.mask);
						if((compressed_sequence[i1/4+i].base & mask_field_4.mask) != (compressed_sequence[i2/4+i].base & mask_field_4.mask))
							return (compressed_sequence[i1/4+i].base & mask_field_4.mask) < (compressed_sequence[i2/4+i].base & mask_field_4.mask);
					}
				}
			}
			else {*/

				for(Index i = 0; i1+i < sequence_size && i2+i < sequence_size; i++) {
					if(sequence[i1+i] != sequence[i2+i]) {
						return sequence[i1+i] < sequence[i2+i];
					}
				}

			/*}*/

			return i1 > i2;
		});

		delete compressed_sequence;

	}

	Index operator[](unsigned int i) const {
		return _suffix_table[i];
	}


protected:
	 Index* _suffix_table;


};

#endif
