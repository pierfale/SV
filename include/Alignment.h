#ifndef _ALIGNMENT_H
#define _ALIGNMENT_H

#include <algorithm>
#include "BWT.h"
#include "DynamicAlign.h"
#include "FASTQFormat.h"

class Alignment {

public:

	template<typename Sequence1Type>
	static void process(const Sequence1Type& ref_sequence, ParserFASTQ& parser, unsigned int seed_length, float k_ratio) {
		std::cout << "Pre compute..." << std::endl;
		BWT index;
		index.pre_compute(ref_sequence);

		DataFASTA read;
		unsigned int cpt = 0;
		unsigned int cpt_found = 0;

		while(parser.next(read)) {
			std::cout << "### [" << cpt << "] Read " << read << std::endl;
			if(process(ref_sequence, read, index, seed_length, k_ratio))
				cpt_found++;
			cpt++;
		}

		std::cout << "Found " << cpt_found << "/" << cpt << " (" << ((float)cpt_found/(float)cpt)*100.f << "%)" << std::endl;

	}

	template<typename Sequence1Type, typename Sequence2Type, typename IndexType>
	static bool process(const Sequence1Type& ref_sequence, const Sequence2Type& read, IndexType& index, unsigned int seed_length, float k_ratio) {

		std::vector<std::pair<Index, Index>> list_seed;
		std::vector<Index> start_match;
		for(unsigned int begin=0; begin+seed_length < read.size(); begin += seed_length) {
			std::vector<Index> list_index;
			Sequence2Type current_read;
			current_read.substr(read, begin, begin+seed_length);
			index.search(current_read, list_index, 2);
			for(auto it = list_index.begin(); it != list_index.end(); ++it)
				if(std::find(start_match.begin(), start_match.end(), *it-begin) == start_match.end()) {
					list_seed.push_back(std::pair<Index, Index>(begin, *it));
					start_match.push_back(*it-begin);
				}
		}

		std::cout << "Seed result size : " << list_seed.size() << std::endl << std::endl;
		int k_length = read.size()*k_ratio;

		bool found = false;

		for(auto it = std::begin(list_seed); it != std::end(list_seed); ++it) {


			if(it->second-k_length < it->first || it->second+read.size()-it->first+k_length >= ref_sequence.size())
				continue;

			Sequence1Type seq_before;
			seq_before.substr(ref_sequence, std::max((int)it->second-(int)it->first-(int)k_length, 0), it->second);
			seq_before.reverse();
			Sequence1Type seq_after;
			seq_after.substr(ref_sequence, it->second, std::min(it->second+read.size()-it->first+k_length, ref_sequence.size()));

			Sequence2Type read_before;
			read_before.substr(read, 0, it->first);
			read_before.reverse();
			Sequence2Type read_after;
			read_after.substr(read, it->first, read.size());

			std::vector<DynamicAlign::Command> commands_before;
			std::vector<DynamicAlign::Command> commands_after;

			if(it->first > read.size()/2) {
				DynamicAlign::process(seq_before, read_before, 5, -4, -10, k_length, commands_before);

				if((float)DynamicAlign::error_count(commands_before)/(float)read.size() > k_ratio)
					continue;

				DynamicAlign::process(seq_after, read_after, 5, -4, -10, k_length, commands_after);
			}
			else {
				DynamicAlign::process(seq_after, read_after, 5, -4, -10, k_length, commands_after);

				if((float)DynamicAlign::error_count(commands_after)/(float)read.size() > k_ratio)
					continue;

				DynamicAlign::process(seq_before, read_before, 5, -4, -10, k_length, commands_before);
			}

			std::vector<DynamicAlign::Command> commands;

			commands.insert(commands.end(), commands_before.begin(), commands_before.end());
			commands.insert(commands.end(), commands_after.rbegin(), commands_after.rend());

			if((float)DynamicAlign::error_count(commands)/(float)commands.size() <= k_ratio) {
				unsigned int count_sequence_before = 0;
				for(auto it=commands_before.rbegin(); it != commands_before.rend(); ++it)
					if(*it != DynamicAlign::INSERT)
						count_sequence_before++;

				Sequence1Type seq_display;
				seq_display.substr(seq_before, 0, count_sequence_before);
				seq_display.reverse();
				seq_display.append(seq_after);

				std::cout << "Read (error_rate : " << ((float)DynamicAlign::error_count(commands)/(float)commands.size()*100.0f)
						  << "%) | Start at position " << (it->second-count_sequence_before)
						  << " | Seed position : " << it->first << std::endl;

				DynamicAlign::display(seq_display, read, commands, 80);
				std::cout << std::endl;
				found = true;
			}
		}

		return found;

	}

};

#endif
