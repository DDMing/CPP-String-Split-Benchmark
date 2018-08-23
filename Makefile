
string_split_comp: string_split_comp.cc
	g++ -std=c++17 -O3 -o string_split_comp string_split_comp.cc -lbenchmark -lboost_system

clean:
	rm string_split_comp