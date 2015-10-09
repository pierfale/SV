#include "BWT.h"

BWT::BWT() : _d(nullptr), _bwt(nullptr), _bwt_reverse(nullptr), _rank(nullptr), _rank_reverse(nullptr) {

}

BWT::~BWT() {
	delete _d;
	delete _bwt;
	delete _bwt_reverse;
	delete _rank;
	delete _rank_reverse;
}
