#ifndef _ALIGNMENT_H
#define _ALIGNMENT_H

#include "BWT.h"
#include "DynamicAlign.h"

class Alignment {

public:
	template<typename Sequence1Type, typename Sequence2Type>
	static void process(const Sequence1Type& ref_sequence, const Sequence2Type& read) {

		std::cout << "Pre compute..." << std::endl;
		BWT index;
		index.pre_compute(ref_sequence);




#define SEED_LENGTH 10
		std::vector<std::pair<Index, Index>> list_seed;
		for(unsigned int begin=0; begin+SEED_LENGTH < read.size(); begin += SEED_LENGTH) {
			std::vector<Index> list_index;
			index.search(read.substr(begin, SEED_LENGTH), list_index, 2);
			for(auto it = list_index.begin(); it != list_index.end(); ++it)
				list_seed.push_back(std::pair<Index, Index>(begin, *it));
		}

		std::cout << "Seed result size : " << list_seed.size() << std::endl << std::endl;

		int k_length = 20;

		for(auto it = std::begin(list_seed); it != std::end(list_seed); ++it) {

			std::cout << it->first << "; " << it->second << std::endl;


			Sequence1Type seq_before = ref_sequence.substr(it->second-k_length, it->second+it->first);
//			Sequence1Type seq_after = ref_sequence.substr(it->second+it->first, it->second+read.size()+k_length);

			Sequence2Type read_before = read.substr(0, it->first);
	//		Sequence2Type read_after = read.substr(it->first, read.size());

			std::vector<DynamicAlign::Command> commands_before;
			std::vector<DynamicAlign::Command> commands_after;

			DynamicAlign::process(seq_before/*.reverse()*/, read_before/*.reverse()*/, 5, -4, -10, k_length, commands_before);

			std::cout << seq_before.reverse() << std::endl << read_before.reverse() << std::endl;


			//DynamicAlign::process(seq_after, read_after, 5, -4, -10, k_length, commands_after);

			std::vector<DynamicAlign::Command> commands;

			commands.insert(commands.end(), commands_before.rbegin(), commands_before.rend());
			//commands.insert(commands.end(), commands_after.begin(), commands_after.end());

			if((float)DynamicAlign::error_count(commands)/(float)commands.size() <= 0.1) {
				std::cout << "Read (error_rate : " << ((float)DynamicAlign::error_count(commands)/(float)commands.size()*100.0f) << "%)" << std::endl;
				//DynamicAlign::display(ref_sequence.substr(it->second-k_length, it->second) , read, commands, 80);
				DynamicAlign::display(seq_before.substr(0, commands_before.size()) , read_before, commands_before, 80);
				DynamicAlign::display(seq_before.substr(0, commands_before.size()).reverse() , read_before.reverse(), commands, 80);
				std::cout << std::endl;
			}
		}

	}

};

#endif
