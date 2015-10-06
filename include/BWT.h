#ifndef _BWT_H
#define _BWT_H

#include "SuffixTable.h"
#include "DNA.h"

#include <stdexcept>
#include <cstring>

class BWT : public SuffixTable {

#define LETTER_NUMBER 4

public:
	const unsigned int RankSample = 32;

	BWT();
	~BWT();

	unsigned int letter_to_index(Letter l) {
		switch(l) {

		case 'A':
		case 'a':
			return 0;
		case 'c':
		case 'C':
			return 1;
		case 'g':
		case 'G':
			return 2;
		case 't':
		case 'T':
			return 3;
		case '$':
			return 4;
		default:
			std::runtime_error("Unrecognized letter '"+std::to_string(l)+"'");
		}
	}

	template<typename SequenceType>
	void pre_compute(const SequenceType& seq) {
		SequenceType sequence = seq.add_end_marker('$');
		_size = sequence.size();

		SuffixTable::pre_compute(sequence);

		_bwt = new Letter[_size];
		_rank = new RankCell[_size/RankSample+1]; // TODO seq > RankSample ?
		Letter current_suffix_letter = '\0';
		unsigned int bwt_letter_count[LETTER_NUMBER+1];
		memset(bwt_letter_count, 0, LETTER_NUMBER*sizeof(unsigned int));

		for(unsigned int i=0; i<_size; i++) {

			// BWT
			_bwt[i] = toupper(sequence[_suffix_table[i] == 0 ? _size-1 : _suffix_table[i]-1]);

			// C
			if(sequence[_suffix_table[i]] != current_suffix_letter) {
				current_suffix_letter = sequence[_suffix_table[i]];
				_c[letter_to_index(current_suffix_letter)] = i;
			}

			// Rank
			if(i%RankSample == 0) {
				memcpy(_rank[i/RankSample].letter, bwt_letter_count, LETTER_NUMBER*sizeof(unsigned int));
			}
			bwt_letter_count[letter_to_index(_bwt[i])]++;
		}

/*		for(unsigned int i=0; i<_size; i++) {
			std::cout << i << " :\t" << sequence[_suffix_table[i]] << " " << _bwt[i] << std::endl;
		}
		*/
	}

	Index rank(Letter l, Index i) {
		unsigned int counter = 0;
		if(i%RankSample != 0) {
			i--;
			while(i%RankSample != 0) {
				if(_bwt[i] == toupper(l))
					counter++;
				i--;
			}
		}
		return _rank[i/RankSample].letter[letter_to_index(l)]+counter;
	}

	template<typename SequenceType, typename ResultType>
	void search(const SequenceType& sequence, ResultType& result, unsigned int z) {
		_search(sequence, result, z, sequence.size()-1, 1, _size-1);
	}


private:
	unsigned int _size;
	Letter* _bwt;
	Index _c[5];

	struct RankCell {
		unsigned int letter[LETTER_NUMBER];
	};

	RankCell* _rank;

	template<typename SequenceType, typename ResultType>
	void _search(const SequenceType& sequence, ResultType& result, int z, int cursor, Index k, Index l) {
		if(z < 0)
			return;

		if(cursor < 0) {
			for(int i=k; i<=l; i++)
				if(std::find(std::begin(result), std::end(result), _suffix_table[i]) == std::end(result))
					result.push_back(_suffix_table[i]);
			return;
		}

		_search(sequence, result, z-1, cursor-1, k, l);

		static Letter list_letter[] = {'A', 'C', 'G', 'T'};

		for(unsigned current_letter = 0; current_letter < 4; current_letter++) {
			Index current_k = r_min(list_letter[current_letter], k);
			Index current_l = r_max(list_letter[current_letter], l);

			if(current_k <= current_l) {
				_search(sequence, result, z-1, cursor, current_k, current_l);
				if(list_letter[current_letter] == toupper(sequence[cursor]))
					_search(sequence, result, z, cursor-1, current_k, current_l);
				else
					_search(sequence, result, z-1, cursor-1, current_k, current_l);
			}
		}
	}

	Index r_min(Letter l, Index prev_r_min) {
		return _c[letter_to_index(l)]+rank(l, prev_r_min-1)+1;
	}

	Index r_max(Letter l, Index prev_r_max) {
		return _c[letter_to_index(l)]+rank(l, prev_r_max);
	}

};

#endif
