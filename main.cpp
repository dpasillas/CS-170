#include <iostream>
#include <stdlib.h>
#include "grid.h"
#include "agent.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cout << "Not enough arguments!" << endl;
		return -1;
	}
	if(argc > 3){
		cout << "Too many arguments!" << endl;
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	Grid grid(n,m);
	grid.print();
	Agent p(grid);
	p.run();
	grid.print();
}
