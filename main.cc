/* 
    Your main program goes here
*/
#include <iostream>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();

int main(int argc, char** argv){
	
	if(argc<2)
		printf("No input graph!!\n");
	else
		nm->intrepret(argv[1]);
    /* start your program */
	nm->print_all_v();
	nm->print_all_e();
    return 0;
}