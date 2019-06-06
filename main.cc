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
int Euler_circuit_check(NetworkManager *nm,int v_count,int e_count,int *degree);
void vertex_push(Vertex** v_array,Vertex *v,int& sp);
Vertex* vertex_pop(Vertex** v_array,int& sp);
Vertex* findnextnode(NetworkManager *nm,Vertex* curnode,int v_count);
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
	
	Gplot *Inputgp=new Gplot();
	Inputgp->gp_add(nm->elist);
	Inputgp->gp_dump(true);
	Inputgp->gp_export("inputgraph");
	int start_v;
	int vertex_count;
	int edge_count;
	Vertex* tmpnode;
	vertex_count=get_node_count(nm);
	edge_count=get_edge_count(nm);
	int vertex_degree[vertex_count];
	//判斷connect
	if(Euler_circuit_check(nm,vertex_count,edge_count,vertex_degree)==0){
		//歐拉化
		int odd_degree_count=0;
		Vertex *odd_degree_list[vertex_count];
		for(int i=0;i<vertex_count;i++){
			cout<<vertex_degree[i]<<endl;
			if(vertex_degree[i]%2!=0){
				odd_degree_list[odd_degree_count]=nm->vlist[i];
				odd_degree_count++;
			}
		}
		int odd_degree_pair=odd_degree_count/2;
		for(int i=0;i<odd_degree_pair;i++){
			cout<<"Connect "<<odd_degree_list[i*2]->name<<" and "<<odd_degree_list[i*2+1]->name<<endl;
			nm->connect(odd_degree_list[i*2]->name,odd_degree_list[i*2+1]->name);
		}
	}
	nm->print_all_v();
	nm->print_all_e();

	//Find Euler_circuit;
	start_v=0;
	//for(int i=0;i<v_count;i++)
	Vertex *nextnode,*curnode;
	Vertex* tmppath[edge_count+1];
	Vertex* Resultpath[edge_count+1];
	int sp1=0;
	int sp2=0;
	vertex_push(tmppath,nm->vlist[start_v],sp1);
	curnode=nm->vlist[start_v];
	cout<<"cur node is "<<curnode->name<<endl;
	nextnode=findnextnode(nm,curnode,vertex_count);

	cout<<"next node is "<<nextnode->name<<endl;
	vertex_push(tmppath,nextnode,sp1);
	nm->print_all_e();
	nm->disconnect(nm->vlist[start_v]->name,nextnode->name);
	nm->print_all_e();
	curnode=nextnode;
	nextnode=findnextnode(nm,curnode,vertex_count);
	while(nextnode!=NULL){
		vertex_push(tmppath,nextnode,sp1);
		if(nm->connected_d(nextnode->name,curnode->name)==0){
			nm->print_all_e();
			nm->disconnect(nextnode->name,curnode->name);
		}
		else{
			nm->print_all_e();
			nm->disconnect(curnode->name,nextnode->name);
			nm->print_all_e();
		}
		curnode=nextnode;
		nextnode=findnextnode(nm,curnode,vertex_count);
		
	}

	
	nm->print_all_v();
	nm->print_all_e();
	if(nm->elist!=NULL){
		curnode=vertex_pop(tmppath,sp1);

		while(nm->elist!=NULL){
			if(findnextnode(nm,curnode,vertex_count)==NULL){
				vertex_push(Resultpath,curnode,sp2);
				curnode=vertex_pop(tmppath,sp1);
			}
			else {
				nextnode=findnextnode(nm,curnode,vertex_count);
				vertex_push(tmppath,nextnode,sp1);
				cout<<"curnode is "<<curnode->name<<endl;
				cout<<"nextnode is "<<nextnode->name<<endl;
				if(nm->connected_d(nextnode->name,curnode->name)==0){
					nm->print_all_e();
					nm->disconnect(nextnode->name,curnode->name);
					nm->print_all_e();
					cout<<"br1"<<endl;
				}
				else if(nm->connected_d(curnode->name,nextnode->name)==0){
					nm->print_all_e();
					nm->disconnect(curnode->name,nextnode->name);
					nm->print_all_e();
					cout<<"br2"<<endl;
				}
				
				curnode=nextnode;
				cout<<"curnode is "<<curnode->name<<endl;
			}
		}
	}
	cout<<"ALL EDGE"<<endl;
	curnode=vertex_pop(tmppath,sp1);
	while(curnode!=NULL){
		vertex_push(Resultpath,curnode,sp2);
		curnode=vertex_pop(tmppath,sp1);
	}
	NetworkManager *result= new NetworkManager();
	nextnode=vertex_pop(Resultpath,sp2);
	while(nextnode!=NULL){
		curnode=nextnode;
		result->add_switch(curnode->name);
		nextnode=vertex_pop(Resultpath,sp2);
		if(nextnode!=NULL){
			result->add_switch(nextnode->name);
			result->connect(curnode->name,nextnode->name);
		}
	}
	result->print_all_v();
	result->print_all_e();
	Gplot *Resultgp=new Gplot();
	Resultgp->gp_add(result->elist);
	Resultgp->gp_dump(true);
	Resultgp->gp_export("Resultpath");
	
    return 0;
}
int get_node_count(NetworkManager *nm){
	Vertex *node;
	node=nm->vlist[0];
	int count=0;
	while(node!=NULL){
	cout<<node->name<<endl;
	count++;
	node=nm->vlist[count];
	}
	cout<<count<<endl;
	return count;
}
int get_edge_count(NetworkManager *nm){
	int count=0;
	Edge *e;
	e=nm->elist;
	while(e!=NULL){
		e=e->next;
		count++;
	}
	cout<<count<<endl;
	return count;
}

int Euler_circuit_check(NetworkManager *nm,int v_count,int e_count,int *degree){
	//Vertex* vlisttmp;
	Edge* e;
	e=(nm->elist);

	for(int j=0;j<v_count;j++)
		degree[j]=0;
	
	cout<<e_count<<endl;
	for(int i=0;i<e_count;i++){
		cout<<"Edge head is "<<e->head->name<<", tail is "<<e->tail->name<<endl;
		for(int j=0;j<v_count;j++){
			if(e->head==nm->vlist[j] || e->tail==nm->vlist[j]){
				degree[j]++;
			}
		}
		e=e->next;
	}
	for(int j=0;j<v_count;j++){
		if(degree[j]%2!=0){
			cout<<"The graph is not eulerian\n";
			return 0;
		}
	}
	cout<<"The graph is eulerian\n";
	return 1;

}
void vertex_push(Vertex** v_array,Vertex *v,int& sp){
	cout<<"push "<<v->name<<endl;
	v_array[sp]=v;
	sp++;
}
Vertex* vertex_pop(Vertex** v_array,int& sp){
	if(sp!=0){
		sp--;
		cout<<"pop "<<v_array[sp]->name<<endl;
		return v_array[sp];
	}
	else
		return NULL;
}
Vertex* findnextnode(NetworkManager *nm,Vertex* curnode,int v_count){
	for(int i=0;i<v_count;i++)
		if((nm->connected(curnode->name,nm->vlist[i]->name)==0) && (curnode !=nm->vlist[i])){
			cout<<"Find node "<<nm->vlist[i]->name<<endl;

			return nm->vlist[i];
		}
	cout<<"Not Found any node"<<endl;

	return NULL;
}