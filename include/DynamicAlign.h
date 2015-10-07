#ifndef _DYNAMIC_ALIGN_H
#define _DYNAMIC_ALIGN_H

#include <cstring>
#include <iostream>
#include <limits>
#include <algorithm>
#include <stdexcept>

class DynamicAlign {

public:

	enum Command {
		INSERT,
		DELETE,
		MATCH,
		MISMATCH
	};

	template<typename Sequence1Type, typename Sequence2Type, template<typename, typename...> class OutputType>
	static void process(const Sequence1Type& sequence, const Sequence2Type& read, int match_score, int mismatch_score, int offset_score, int k, OutputType<Command>& output) {

		int* dynamic_array = new int[(sequence.size()+1)*(read.size()+1)];

	#	define	COORD_2D(X, Y) (Y)*(sequence.size()+1)+(X)
	#	define	GET_I(C) (C)%(sequence.size()+1)
	#	define	GET_J(C) (C)/(sequence.size()+1)
	#	define IN_K_BAND(X, Y) ((int)(Y) >= (int)(X)-(int)(sequence.size() > read.size() ? sequence.size()-read.size()+k : k) && (int)(Y) <= (int)(X)+(int)(read.size() > sequence.size() ? read.size()-sequence.size()+k : k))
	#	define START_K_BAND(Y) ((int)(Y) >= (read.size() > sequence.size() ? read.size()-sequence.size()+k : k)  ? (int)(Y)-(read.size() > sequence.size() ? read.size()-sequence.size()+k : k) : 0)

		/*
		 *	Fill dynamic array
		 */

		for(unsigned int i=0; i<sequence.size()+1; i++) {
			if(!IN_K_BAND(i, 0))
				break;

			dynamic_array[COORD_2D(i, 0)] = i*offset_score;
		}

		for(unsigned int j=1; j<read.size()+1; j++) {
			if(!IN_K_BAND(0, j))
				break;
			dynamic_array[COORD_2D(0, j)] = j*offset_score;
		}

		/*
		 *	Fill dynamic array
		 */
		for(unsigned int j=1; j<read.size()+1; j++) {
			for(unsigned int i=START_K_BAND(j) == 0 ? 1 : START_K_BAND(j); i<sequence.size()+1; i++) {

				if(!IN_K_BAND(i, j))
					break;
				int score_insert = IN_K_BAND(i-1, j) ? dynamic_array[COORD_2D(i-1, j)]+offset_score : std::numeric_limits<int>::min();
				int score_delete = IN_K_BAND(i, j-1)  ? dynamic_array[COORD_2D(i, j-1)]+offset_score : std::numeric_limits<int>::min();
				int score_match = dynamic_array[COORD_2D(i-1, j-1)]+(sequence[i-1] == read[j-1] ? match_score : mismatch_score);
				dynamic_array[COORD_2D(i, j)] = score_insert > score_delete ? (score_insert > score_match ? score_insert : score_match) : (score_delete > score_match ? score_delete : score_match);


			}
		}
/*
		std::cout << " \t";
		for(unsigned int i=0; i<sequence.size()+1; i++)
			std::cout << (i == 0 ? ' ' :sequence[i-1]) << "\t";
		std::cout << std::endl;

		for(unsigned int j=0; j<read.size()+1; j++) {
			std::cout << (j == 0 ? ' ' : read[j-1]) << "\t";
			for(unsigned int i=0; i<sequence.size()+1; i++) {
				std::cout << (IN_K_BAND(i, j) ? dynamic_array[COORD_2D(i, j)] : 0) << "\t";
			}
			std::cout << std::endl;
		}
*/
		/*
		 *	Search max
		 */

		int* max_ptr = std::max_element(&dynamic_array[COORD_2D(START_K_BAND(read.size()), read.size())], &dynamic_array[COORD_2D(sequence.size()+1, read.size())]);

		std::pair<int, int> current_coord(GET_I(max_ptr-dynamic_array), GET_J(max_ptr-dynamic_array));

		//std::cout << "max : (" << current_coord.first << ";" << current_coord.second << ") = " << *max_ptr << std::endl;

		/*
		 *	Backtrace
		 */

		while(current_coord.first != 0 || current_coord.second != 0) {
			int current_score = dynamic_array[COORD_2D(current_coord.first, current_coord.second)];

			if(IN_K_BAND(current_coord.first-1, current_coord.second) & current_coord.first > 0 && dynamic_array[COORD_2D(current_coord.first-1, current_coord.second)] == current_score - offset_score) {
				output.push_back(DELETE);
				current_coord = std::pair<unsigned int, unsigned int>(current_coord.first-1, current_coord.second);
			}
			else if(IN_K_BAND(current_coord.first, current_coord.second-1) & current_coord.second > 0 && dynamic_array[COORD_2D(current_coord.first, current_coord.second-1)] == current_score - offset_score) {
				output.push_back(INSERT);
				current_coord = std::pair<unsigned int, unsigned int>(current_coord.first, current_coord.second-1);
			}
			else if(current_coord.first > 0 && current_coord.second > 0 && dynamic_array[COORD_2D(current_coord.first-1, current_coord.second-1)] == current_score - match_score) {
				output.push_back(MATCH);
				current_coord = std::pair<unsigned int, unsigned int>(current_coord.first-1, current_coord.second-1);
			}
			else if(current_coord.first > 0 && current_coord.second > 0 && dynamic_array[COORD_2D(current_coord.first-1, current_coord.second-1)] == current_score - mismatch_score) {
				output.push_back(MISMATCH);
				current_coord = std::pair<unsigned int, unsigned int>(current_coord.first-1, current_coord.second-1);
			}
			else
				throw std::runtime_error("something wrong during backtrace...");
		}

		delete dynamic_array;
	}

	template<template<typename, typename...> class OutputType>
	static unsigned int error_count(OutputType<Command>& backtrace) {
		unsigned int counter = 0;
		for(auto it=backtrace.begin(); it < backtrace.end(); ++it) {
			if(*it == INSERT || *it == DELETE || *it == MISMATCH)
				counter++;
		}
		return counter;
	}

	template<typename Sequence1Type, typename Sequence2Type, template<typename, typename...> class OutputType>
	static void display(const Sequence1Type& sequence1, const Sequence2Type& sequence2, OutputType<Command>& backtrace, unsigned int line_length) {
		unsigned int cursor_sequence1 = 0;
		unsigned int cursor_sequence2 = 0;

		for(auto it=backtrace.begin(); it < backtrace.end(); it+=line_length) {
			unsigned int i = 0;
			for(auto it2(it); it2 != backtrace.end() && i < line_length; ++it2, i++) {
				if(*it2 == INSERT)
					std::cout << "-";
				else
					std::cout << (cursor_sequence1 < sequence1.size() ? sequence1[cursor_sequence1++] : ' ');
			}
			/*while(i < line_length && cursor_sequence1 < sequence1.size()) {
				std::cout << sequence1[cursor_sequence1++];
				i++;
			}*/
			std::cout << std::endl;
			i = 0;
			for(auto it2(it); it2 != backtrace.end() && i < line_length; ++it2, i++) {
				if(*it2 == MATCH)
					std::cout << "|";
				else
					std::cout << " ";
			}
			std::cout << std::endl;
			i = 0;
			for(auto it2(it); it2 != backtrace.end() && i < line_length; ++it2, i++) {
				if(*it2 == DELETE)
					std::cout << "-";
				else
					std::cout << (cursor_sequence2 < sequence2.size() ? sequence2[cursor_sequence2++] : ' ');
			}
		/*	while(i < line_length && cursor_sequence2 < sequence2.size()) {
				std::cout << sequence2[cursor_sequence2++];
				i++;
			}*/
			std::cout << std::endl;
		}
/*
		while(cursor_sequence1 < sequence1.size() || cursor_sequence2 < sequence2.size()) {
			if(cursor_sequence1 >= sequence1.size())
				std::cout << std::endl << std::endl;
			for(unsigned int i=0; cursor_sequence1 < sequence1.size() && i < line_length; i++) {
				std::cout << sequence1[cursor_sequence1++];
			}
			std::cout << std::endl << std::endl;
			if(cursor_sequence2 >= sequence2.size())
				std::cout << std::endl << std::endl;
			for(unsigned int i=0; cursor_sequence2 < sequence2.size() && i < line_length; i++) {
				std::cout << sequence2[cursor_sequence2++];
			}
			std::cout << std::endl;
		}*/

	}
};

#endif
