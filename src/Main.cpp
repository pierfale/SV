#include "FASTAFormat.h"
#include "StaticFormat.h"
#include "DynamicAlign.h"
#include "BWT.h"
#include "Alignment.h"

const char* usage() {
	return "<fasta_filename> <fastq_filename>";
}


int main(int argc, char** argv) {

	if(argc != 3) {
		std::cout << usage() << std::endl;
		return EXIT_FAILURE;
	}
/*
	DataFASTA sequence;
	ParserFASTA::process(std::string(argv[1]), sequence);

	ParserFASTQ read_parser(argv[2]);

	Alignment::process(sequence, read_parser, 25, 0.1);*/

	StaticFormat sequence1("tgggatggatcaaccctaacagtggtggcacaaactatgcacagaagtttcagggcagggtcaccatgaccagggacacgtccatcagcacagcctacatggagctgagcaggctgagatctgacgacacggccgtgtattactgtgcgagaga");

	BWT sf;

	sf.pre_compute(sequence1);

	std::vector<Index> list_index;

	sf.search(StaticFormat("gggg"), list_index, 0);

	std::cout << "Result (" << list_index.size() << ") : " << std::endl;

	for(auto it = std::begin(list_index); it != std::end(list_index); ++it) {
		std::cout << *it << " ";
		for(int i=0; i<4; i++)
			std::cout << sequence1[*it+i];
		std::cout << std::endl;

	}

	//DynamicAlign::process(sequence1, sequence2, 5, -4, -10, (sequence1.size() > sequence2.size() ? sequence1.size()-sequence2.size()  : sequence2.size()-sequence1.size())+30);
	//Alignment::process(StaticFormat("tcagggcagggtcaccatgaccagggacacgtccatcagcacag--ccta"), StaticFormat("gaagaccaggctcaccatctccaaggacacctccaaaaac-caggtggtc"), 5, -4, -10, 10);

	return EXIT_SUCCESS;
}
