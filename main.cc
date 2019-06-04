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
int get_node_count(NetworkManager *nm);
int get_edge_count(NetworkManager *nm);
int Euler_circuit_check(NetworkManager *nm,int v_count,int e_count);


int main(int argc, char** argv){
	
	if(argc<2){
		printf("No input graph!!\n");
		return 0;                                       
	}
	else
		nm->interpret(argv[1]);
    /* start your program */
	nm->print_all_v();
	nm->print_all_e();
	
	int vertex_count;
	int edge_count;
	vertex_count=get_node_count(nm);
	edge_count=get_edge_count(nm);
	Euler_circuit_check(nm,vertex_count,edge_count);
	/*if(Euler_circuit_check(nm)==1)
		//Find Euler_circuit;
	else
		//*/
	
	
    return 0;
}
int get_node_count(NetworkManager *nm){
	Vertex *node;
	node=nm->get_all_nodes();
	int count=0;
	while(node!=NULL){
	//cout<<node->name<<endl;
	node=node->next;
	count++;
	}
	cout<<count<<endl;
	return count;
}
int get_edge_count(NetworkManager *nm){
	int count=0;
	Edge *e;
	e=nm->elist;
	while(e!=NULL){
		//cout<<"Head is "<<e->head->name<<", tail is "<<e->tail->name<<endl;
		e=e->next;
		count++;
	}
	cout<<count<<endl;
	return count;
}

int Euler_circuit_check(NetworkManager *nm,int v_count,int e_count){
	//Vertex* vlisttmp;
	Edge* elisttmp;
	elisttmp=nm->elist;
	//vlisttmp=nm->get_all_nodes();
	
	int vertex_degree[v_count];
	for(int j=0;j<v_count;j++)
		vertex_degree[j]=0;
	
	
	for(int i=0;i<e_count;i++){
		for(int j=0;j<v_count;j++)
			if(elisttmp->head==nm->vlist[j] || elisttmp->tail==nm->vlist[j])
				vertex_degree[j]++;
	}
	for(int j=0;j<v_count;j++){
		if(vertex_degree[j]%2!=0){
			cout<<"The graph is not eulerian\n";
			return 0;
		}
	}
	cout<<"The graph is eulerian\n";
	return 1;

}