#include "FASTAFormat.h"
#include "StaticFormat.h"
#include "DynamicAlign.h"
#include "BWT.h"
#include "Alignment.h"

const char* usage() {
	return "<sequence1_filename> <sequence2_filename>";
}


int main(int argc, char** argv) {

	if(argc != 3) {
		std::cout << usage() << std::endl;
		return EXIT_FAILURE;
	}

	DataFASTA sequence1;
	ParserFASTA::process(std::string(argv[1]), sequence1);

	//StaticFormat sequence1("tgggatggatcaaccctaacagtggtggcacaaactatgcacagaagtttcagggcagggtcaccatgaccagggacacgtccatcagcacagcctacatggagctgagcaggctgagatctgacgacacggccgtgtattactgtgcgagaga");

	DataFASTA sequence2;
	ParserFASTA::process(std::string(argv[2]), sequence2);



	BWT sf;

	StaticFormat seq("TATCGTGGTTTGTTTCAGAGCCATATCACCAAGATAGAGAACAACCTAGGTCTCCGGAGGGGGCAAGGGCATCAGTGTGCTCAGTTGAAAGTCCCTTGTCAACATCTAGGCCTTATCACATCACAAGTTCCGCCTTAAACTCTGCAGGGTGATCCAACAACCTTAATAGCAACATTATTGTTAAAGGACAGCCTTAGTTCCCAGTCAACCACGCAAGATTGAGAATTAACTTTGATTTTGACCCTGAACACCCAGAGGACTGGAGACTCAACACCCCTAAAGCCTCGGTTAAACCA");


	Alignment::process(sequence1, seq);
	/*sf.pre_compute(sequence1);

	std::vector<Index> list_index;

	sf.search(StaticFormat("TAT"), list_index, 1);

	std::cout << "Result (" << list_index.size() << ") : " << std::endl;

	for(auto it = std::begin(list_index); it != std::end(list_index); ++it) {
		std::cout << *it << " ";
		for(int i=0; i<4; i++)
			std::cout << seq[*it+i];
		std::cout << std::endl;

	}*/

	//DynamicAlign::process(sequence1, sequence2, 5, -4, -10, (sequence1.size() > sequence2.size() ? sequence1.size()-sequence2.size()  : sequence2.size()-sequence1.size())+30);
	//Alignment::process(StaticFormat("tcagggcagggtcaccatgaccagggacacgtccatcagcacag--ccta"), StaticFormat("gaagaccaggctcaccatctccaaggacacctccaaaaac-caggtggtc"), 5, -4, -10, 10);

	return EXIT_SUCCESS;
}
