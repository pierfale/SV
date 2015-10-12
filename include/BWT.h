#ifndef _BWT_H
#define _BWT_H

#include "SuffixTable.h"
#include "DNA.h"

#include <stdexcept>
#include <cstring>

class BWT {

#define LETTER_NUMBER 4

public:
	const unsigned int RankSample = 32;

	BWT();
	~BWT();

	unsigned int letter_to_index(Letter l) {
		switch(l) {

		case 'A':
			return 0;
		case 'C':
			return 1;
		case 'G':
			return 2;
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
		SequenceType sequence;
		sequence.add_end_marker(seq, '$');
		_size = sequence.size();

		std::cout << "Compute suffix table..." << std::endl;
		_suffix_table.pre_compute(sequence);

		_bwt = new Letter[_size];
		_rank = new RankCell[_size/RankSample+1];
		Letter current_suffix_letter = '\0';
		unsigned int bwt_letter_count[LETTER_NUMBER+1];
		memset(bwt_letter_count, 0, LETTER_NUMBER*sizeof(unsigned int));

		std::cout << "Compute BWT..." << std::endl;

		for(unsigned int i=0; i<_size; i++) {

			// BWT
			_bwt[i] = toupper(sequence[_suffix_table[i] == 0 ? _size-1 : _suffix_table[i]-1]);

			// C
			if(sequence[_suffix_table[i]] != current_suffix_letter) {
				current_suffix_letter = sequence[_suffix_table[i]];
				_c[letter_to_index(current_suffix_letter)] = i-1; //for $
				std::cout << "C " << current_suffix_letter << " : " << i << std::endl;
			}

			// Rank
			bwt_letter_count[letter_to_index(_bwt[i])]++;
			if(i%RankSample == 0) {
				memcpy(_rank[i/RankSample].letter, bwt_letter_count, LETTER_NUMBER*sizeof(unsigned int));
			}

		}

		memcpy(_rank[_size/RankSample].letter, bwt_letter_count, LETTER_NUMBER*sizeof(unsigned int));


		// Compute Inverse BWT

		SequenceType sequence_reverse(seq);
		sequence_reverse.reverse();
		sequence_reverse.add_end_marker(seq, '$');

		std::cout << "Compute reverse suffix table..." << std::endl;
		SuffixTable suffix_table_reverse;
		suffix_table_reverse.pre_compute(sequence_reverse);

		_bwt_reverse = new Letter[_size];
		_rank_reverse = new RankCell[_size/RankSample+1];
		memset(bwt_letter_count, 0, LETTER_NUMBER*sizeof(unsigned int));

		std::cout << "Compute reverse BWT..." << std::endl;

		for(unsigned int i=0; i<_size; i++) {

			// BWT
			_bwt_reverse[i] = toupper(sequence[suffix_table_reverse[i] == 0 ? _size-1 : suffix_table_reverse[i]-1]);

			// Rank
			bwt_letter_count[letter_to_index(_bwt_reverse[i])]++;
			if(i%RankSample == 0) {
				memcpy(_rank_reverse[i/RankSample].letter, bwt_letter_count, LETTER_NUMBER*sizeof(unsigned int));
			}

		}

		memcpy(_rank_reverse[_size/RankSample].letter, bwt_letter_count, LETTER_NUMBER*sizeof(unsigned int));


/*
		for(unsigned int i=0; i<_size; i++) {
			std::cout << i << " :\t";// << sequence[_suffix_table[i]] << " " << _bwt[i] << " -> " << _suffix_table[i] << std::endl;
			for(unsigned int j=_suffix_table[i]; j<sequence.size(); j++)
				std::cout << sequence[j];
			std::cout << std::endl;
		}*/

		//std::exit(0);
	}

	template<typename SequenceType, typename ResultType>
	void search(const SequenceType& sequence, ResultType& result, unsigned int z) {
		compute_d(sequence);
		_search(sequence, result, z, sequence.size()-1, 1, _size);
	}


private:
	unsigned int _size;

	SuffixTable _suffix_table;

	Letter* _bwt;
	Letter* _bwt_reverse;

	Index _c[LETTER_NUMBER+1];
	int* _d;

	struct RankCell {
		unsigned int letter[LETTER_NUMBER];
	};

	RankCell* _rank;
	RankCell* _rank_reverse;

	template<typename SequenceType, typename ResultType>
	void _search(const SequenceType& sequence, ResultType& result, int z, int cursor, Index k, Index l) {
		if(/*cursor >= 0 && z < _d[cursor]*/z < 0)
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

	/*
	 * Rank
	 */

	Index rank(Letter l, Index i) {
		unsigned int counter = 0;
		while(i%RankSample != 0) {
			if(_bwt[i] == toupper(l))
				counter++;
			i--;

		}
		return _rank[i/RankSample].letter[letter_to_index(l)]+counter;
	}

	Index r_min(Letter l, Index prev_r_min) {
		return _c[letter_to_index(l)]+rank(l, prev_r_min-1)+1;
	}

	Index r_max(Letter l, Index prev_r_max) {
		return _c[letter_to_index(l)]+rank(l, prev_r_max);
	}

	/*
	 *	Reverse Rank
	 */
	Index rank_reverse(Letter l, Index i) {
		unsigned int counter = 0;
		while(i%RankSample != 0) {
			if(_bwt_reverse[i] == toupper(l))
				counter++;
			i--;

		}
		return _rank_reverse[i/RankSample].letter[letter_to_index(l)]+counter;
	}

	Index r_min_reverse(Letter l, Index prev_r_min) {
		return _c[letter_to_index(l)]+rank_reverse(l, prev_r_min-1)+1;
	}

	Index r_max_reverse(Letter l, Index prev_r_max) {
		return _c[letter_to_index(l)]+rank_reverse(l, prev_r_max);
	}

	template<typename SequenceType>
	void compute_d(const SequenceType& sequence) {
		delete _d;

		_d = new int[sequence.size()];

		unsigned int k = 1;
		unsigned int l = _size;
		int z = 0;

		for(unsigned int i=0; i<sequence.size(); i++) {
			k = r_min_reverse(sequence[i], k);
			l = r_max_reverse(sequence[i], l);

			if(k > l) {
				k = 1;
				l = _size;
				z++;
			}

			_d[i] = z;
		}


	}
};

#endif
