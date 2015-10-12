#ifndef _DNA_H
#define _DNA_H

#include <cinttypes>
#include <stdexcept>

typedef char Letter;

struct Base4 {

	static const uint8_t Base_A = 0x00;
	static const uint8_t Base_C = 0x01;
	static const uint8_t Base_G = 0x02;
	static const uint8_t Base_T = 0x03;

	static uint8_t base_2_base4(Letter l) {
		switch(l) {
		case 'a':
		case 'A' :
			return Base_A;
		case 'c':
		case 'C' :
			return Base_C;
		case 'g':
		case 'G' :
			return Base_G;
		case 't':
		case 'T' :
			return Base_T;
		default:
			return ::rand()%4;
		}
	}

	bool operator==(const Base4& that) {
		return base == that.base;
	}

	bool operator!=(const Base4& that) {
		return base != that.base;
	}

	bool operator<(const Base4& that) {
		return base < that.base;
	}

	union {
		struct {
			uint8_t base_1 : 2;
			uint8_t base_2 : 2;
			uint8_t base_3 : 2;
			uint8_t base_4 : 2;
		};
		uint8_t base;
	};
};

template<typename SequenceType>
void rewrite_sequence(SequenceType& sequence) {
	for(unsigned int i=0; i< sequence.size(); i++) {
		switch(sequence[i]) {

		case 'a': sequence[i] = 'A'; break;
		case 'A': break;
		case 'c': sequence[i] = 'C'; break;
		case 'C': break;
		case 'g': sequence[i] = 'G'; break;
		case 'G': break;
		case 't': sequence[i] = 'T'; break;
		case 'T': break;
		default: switch(::rand()%4) {
			case 0:
				sequence[i] = 'A';
				break;
			case 1:
				sequence[i] = 'C';
				break;
			case 2:
				sequence[i] = 'G';
				break;
			case 3:
				sequence[i] = 'T';
				break;
			}
		}
	}
}

#endif
