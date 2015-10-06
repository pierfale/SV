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
	static void process(const Sequence1Type& sequence1, const Sequence2Type& sequence2, int match_score, int mismatch_score, int offset_score, int k_length, OutputType<Command>& output) {

		int* dynamic_array = new int[(sequence1.size()+1)*(sequence2.size()+1)];

	#	define	COORD_2D(X, Y) (X)*(sequence1.size()+1)+(Y)
	#	define	GET_I(X) (X)/(sequence1.size()+1)
	#	define	GET_J(X) (X)%(sequence1.size()+1)
	#	define IN_K_BAND(X, Y, K) ((int)(Y) >= (int)(X)-(int)(K) && (Y) <= (X)+(K))


		/*
		 *	Fill dynamic array
		 */

		for(unsigned int i=0; i<sequence2.size()+1; i++) {
			if(!IN_K_BAND(i, 0, k_length))
				break;

			dynamic_array[COORD_2D(i, 0)] = 0;/*i*offset_score;*/
		}

		for(unsigned int j=1; j<sequence1.size()+1; j++) {
			if(!IN_K_BAND(0, j, k_length))
				break;
			dynamic_array[COORD_2D(0, j)] = j*offset_score;
		}

		std::pair<int, int> current_coord;
		int max_value = std::numeric_limits<int>::min();
		/*
		 *	Fill dynamic array
		 */
		for(unsigned int i=1; i<sequence2.size()+1; i++) {
			for(unsigned int j=(i > k_length ? i-k_length : 1); j<sequence1.size()+1; j++) {

				if(!IN_K_BAND(i, j, k_length))
					break;
				int score_insert = IN_K_BAND(i-1, j, k_length) ? dynamic_array[COORD_2D(i-1, j)]+offset_score : std::numeric_limits<int>::min();
				int score_delete = IN_K_BAND(i, j-1, k_length)  ? dynamic_array[COORD_2D(i, j-1)]+offset_score : std::numeric_limits<int>::min();
				int score_match = dynamic_array[COORD_2D(i-1, j-1)]+(sequence1[j-1] == sequence2[i-1] ? match_score : mismatch_score);
				dynamic_array[COORD_2D(i, j)] = score_insert > score_delete ? (score_insert > score_match ? score_insert : score_match) : (score_delete > score_match ? score_delete : score_match);

				if(dynamic_array[COORD_2D(i, j)] > max_value) {
					max_value = dynamic_array[COORD_2D(i, j)];
					current_coord.first = i;
					current_coord.second = j;
				}
			}
		}

		//std::cout << sequence2.size() << ";" << sequence1.size() << std::endl;

		/*
		 *	Search max
		 */
/*
		int* max_ptr = std::max_element(&dynamic_array[COORD_2D(sequence2.size(), sequence1.size() > k_length ? sequence1.size()-k_length : 0)], &dynamic_array[COORD_2D(sequence2.size(), sequence1.size()+1)]);

		std::pair<int, int> current_coord(GET_I(max_ptr-dynamic_array), GET_J(max_ptr-dynamic_array));
*/
		//std::cout << "max : (" << current_coord.first << ";" << current_coord.second << ") = " << *max_ptr << std::endl;

		/*
		 *	Backtrace
		 */

		while(current_coord.first != 0 || current_coord.second != 0) {
			int current_score = dynamic_array[COORD_2D(current_coord.first, current_coord.second)];

			if(IN_K_BAND(current_coord.first-1, current_coord.second, k_length) & current_coord.first > 0 && dynamic_array[COORD_2D(current_coord.first-1, current_coord.second)] == current_score - offset_score) {
				output.push_back(INSERT);
				current_coord = std::pair<unsigned int, unsigned int>(current_coord.first-1, current_coord.second);
			}
			else if(IN_K_BAND(current_coord.first, current_coord.second-1, k_length) & current_coord.second > 0 && dynamic_array[COORD_2D(current_coord.first, current_coord.second-1)] == current_score - offset_score) {
				output.push_back(DELETE);
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
	}

	template<template<typename, typename...> class OutputType>
	static unsigned int error_count(OutputType<Command>& backtrace) {
		unsigned int counter = 0;
		for(auto it=backtrace.begin(); it < backtrace.end(); ++it) {
			if(*it == INSERT || *it == DELETE)
				counter++;
		}
		return counter;
	}

	template<typename Sequence1Type, typename Sequence2Type, template<typename, typename...> class OutputType>
	static void display(const Sequence1Type& sequence1, const Sequence2Type& sequence2, OutputType<Command>& backtrace, unsigned int line_length) {
		unsigned int cursor_sequence1 = 0;
		unsigned int cursor_sequence2 = 0;

		for(auto it=backtrace.rbegin(); it < backtrace.rend(); it+=line_length) {
			unsigned int i = 0;
			for(auto it2(it); it2 != backtrace.rend() && i < line_length; ++it2, i++) {
				if(*it2 == INSERT)
					std::cout << "-";
				else
					std::cout << (cursor_sequence1 < sequence1.size() ? sequence1[cursor_sequence1++] : ' ');
			}
			while(i < line_length && cursor_sequence1 < sequence1.size()) {
				std::cout << sequence1[cursor_sequence1++];
				i++;
			}
			std::cout << std::endl;
			i = 0;
			for(auto it2(it); it2 != backtrace.rend() && i < line_length; ++it2, i++) {
				if(*it2 == MATCH)
					std::cout << "|";
				else
					std::cout << " ";
			}
			std::cout << std::endl;
			i = 0;
			for(auto it2(it); it2 != backtrace.rend() && i < line_length; ++it2, i++) {
				if(*it2 == DELETE)
					std::cout << "-";
				else
					std::cout << (cursor_sequence2 < sequence2.size() ? sequence2[cursor_sequence2++] : ' ');
			}
			while(i < line_length && cursor_sequence2 < sequence2.size()) {
				std::cout << sequence2[cursor_sequence2++];
				i++;
			}
			std::cout << std::endl;
		}

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
		}

	}
};

#endif
