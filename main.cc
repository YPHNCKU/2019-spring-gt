/* 
    Your main program goes here
*/
#include <iostream>
#include <fstream>

#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;


// create NetworkManager first
NetworkManager *nm = new NetworkManager();

//Graph check
int get_node_count(NetworkManager *nm);
int get_edge_count(NetworkManager *nm);
int Euler_circuit_check(NetworkManager *nm,int v_count,int e_count,int *degree);

//Stack operation
void vertex_push(Vertex** v_array,Vertex *v,int& sp);
Vertex* vertex_pop(Vertex** v_array,int& sp);
Vertex* vertex_looktop(Vertex** v_array,int& sp);

//Find node
Vertex* findnextnode(NetworkManager *nm,Vertex* curnode,int v_count);

//Permutation
void print(int N,Vertex** a,vector<vector<Vertex*> > &pset);
void backtrack(int n, int N,Vertex** a,Vertex** element,bool* used,vector<vector<Vertex*> > &pset);
void enumerate_permutations(int N,Vertex** a,Vertex** element,bool* used,vector<vector<Vertex*> > &pset);

int main(int argc, char** argv){
	
	if(argc<2){
		printf("No input graph!!\n");
		return 0;                                       
	}
	else
		nm->interpret(argv[1]);
    /* start your program */
	cout<<"======================================================\n";
	cout<<"Load graph from input file"<<endl;
	cout<<"======================================================\n";
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

	cout<<"======================================================\n";
	cout<<"Check Eularian"<<endl;
	cout<<"======================================================\n";

	Euler_circuit_check(nm,vertex_count,edge_count,vertex_degree);
	for(int i=0;i<vertex_count;i++)
		if(vertex_degree[i]==0){
			cout<<"The graph is not connect. The problem cannot be solved."<<endl;
			return 0;
		}
	if(Euler_circuit_check(nm,vertex_count,edge_count,vertex_degree)==0){
		//歐拉化
		int odd_degree_count=0;
		Vertex *odd_degree_list[vertex_count];
		for(int i=0;i<vertex_count;i++){
			//cout<<vertex_degree[i]<<endl;
			if(vertex_degree[i]%2!=0){
				odd_degree_list[odd_degree_count]=nm->vlist[i];
				odd_degree_count++;
			}
			
		}
		int odd_degree_pair=odd_degree_count/2;
		if(odd_degree_pair==1 && nm->connected(odd_degree_list[0]->name,odd_degree_list[1]->name)==0)
			nm->connect(odd_degree_list[0]->name,odd_degree_list[1]->name);
		else if(odd_degree_pair>=1){
			NetworkManager *subgraph = new NetworkManager();
			for(int i=0;i<vertex_count;i++){
				subgraph->add_switch(nm->vlist[i]->name);
			}
			for(int i=0;i<vertex_count;i++)
				for(int j=i;j<vertex_count;j++)
					if(nm->connected(nm->vlist[i]->name,nm->vlist[j]->name)==0){
						subgraph->connect(nm->vlist[j]->name,nm->vlist[i]->name);
						subgraph->connect(nm->vlist[i]->name,nm->vlist[j]->name);
					}
			
			int combination_count;
			
			
			//找出所有排列
			
			Vertex* a[odd_degree_count];
			bool used[odd_degree_count];
			for(int i=0;i<odd_degree_count;i++)
				used[odd_degree_count]=0;
			vector<vector<Vertex*> > pset;
			enumerate_permutations(odd_degree_count,a,odd_degree_list,used,pset);
			cout<<"Num of point_permutation: "<<pset.size()<<endl;
			cout<<"Num of point: "<<pset[0].size()<<endl;
			int total_add_path[pset.size()];
			combination_count=pset.size();

			//Calculate all path
			int path_sum=0;
			Path *path;
			path=new Path();
			path->append(subgraph->elist);
			vector<vector<Edge *> > path1;
			for(int i=0;i<combination_count;i++){
				for(int j=0;j<odd_degree_pair;j++){
					
					path1=path->find_paths(pset[i][j*2]->name, pset[i][j*2+1]->name);
					path_sum+=path1[path1.size()-1].size();
					path1.clear();
				}
				total_add_path[i]=path_sum;
				path_sum=0;
			}
			int min_path_pair;
			int tmp=1000;
			
			//Find the shortest solution
			for(int i=0;i<combination_count;i++){
				if(total_add_path[i]<tmp)
					min_path_pair=i;
			}
			//Add edge to graph
			for(int j=0;j<odd_degree_pair;j++){
					path1=path->find_paths(pset[min_path_pair][j*2]->name, pset[min_path_pair][j*2+1]->name);
					cout<<"Add path: "<<j<<endl;
					for(int i=0;i<path1[path1.size()-1].size();i++)
						nm->connect(path1[path1.size()-1][i]->head->name,path1[path1.size()-1][i]->tail->name);
			}
			

		}
	}
	edge_count=get_edge_count(nm);
	
	
	nm->print_all_v();
	nm->print_all_e();
	cout<<"======================================================\n";
	cout<<"Find Euler Circuit"<<endl;
	cout<<"======================================================\n";
	//Find Euler_circuit;
	start_v=0;
	Vertex *nextnode,*curnode;
	Vertex* tmppath[edge_count+1];
	Vertex* Resultpath[edge_count+1];
	int sp1=0;
	int sp2=0;
	vertex_push(tmppath,nm->vlist[start_v],sp1);
	curnode=nm->vlist[start_v];
	//cout<<"cur node is "<<curnode->name<<endl;
	nextnode=findnextnode(nm,curnode,vertex_count);

	//cout<<"next node is "<<nextnode->name<<endl;
	vertex_push(tmppath,nextnode,sp1);
	nm->disconnect(nm->vlist[start_v]->name,nextnode->name);
	curnode=nextnode;
	nextnode=findnextnode(nm,curnode,vertex_count);
	while(nextnode!=NULL){
		vertex_push(tmppath,nextnode,sp1);
		if(nm->connected_d(nextnode->name,curnode->name)==0){
			nm->disconnect(nextnode->name,curnode->name);
		}
		else{
			nm->disconnect(curnode->name,nextnode->name);
		}
		curnode=nextnode;
		nextnode=findnextnode(nm,curnode,vertex_count);
		
	}

	
	//nm->print_all_v();
	//nm->print_all_e();
	if(nm->elist!=NULL){
		curnode=vertex_pop(tmppath,sp1);
		vertex_push(Resultpath,curnode,sp2);
		curnode=vertex_looktop(tmppath,sp1);
		while(nm->elist!=NULL){
			if(findnextnode(nm,curnode,vertex_count)==NULL){
				curnode=vertex_pop(tmppath,sp1);
				vertex_push(Resultpath,curnode,sp2);
				curnode=vertex_looktop(tmppath,sp1);
			}
			else {
				nextnode=findnextnode(nm,curnode,vertex_count);
				vertex_push(tmppath,nextnode,sp1);
				//cout<<"curnode is "<<curnode->name<<endl;
				//cout<<"nextnode is "<<nextnode->name<<endl;
				if(nm->connected_d(nextnode->name,curnode->name)==0){
					nm->disconnect(nextnode->name,curnode->name);
				}
				else if(nm->connected_d(curnode->name,nextnode->name)==0){
					nm->disconnect(curnode->name,nextnode->name);
				}
				
				curnode=nextnode;
				//cout<<"curnode is "<<curnode->name<<endl;
			}
		}
	}
	cout<<"All edges are disconneted"<<endl;
	
	cout<<"\n======================================================\n";
	cout<<"Build Result path"<<endl;
	cout<<"======================================================\n";
	curnode=vertex_pop(tmppath,sp1);
	while(curnode!=NULL){
		vertex_push(Resultpath,curnode,sp2);
		curnode=vertex_pop(tmppath,sp1);
	}
	fstream out;
	out.open("Result_path.txt",ios::out);
	out<<"The result path is:"<<endl;
	//cout<<"The result path is:"<<endl;
	NetworkManager *result= new NetworkManager();
	nextnode=vertex_pop(Resultpath,sp2);
	while(nextnode!=NULL){
		curnode=nextnode;
		result->add_switch(curnode->name);
		//cout<<curnode->name;
		out<<curnode->name;
		nextnode=vertex_pop(Resultpath,sp2);
		if(nextnode!=NULL){
			//cout<<"->";
			out<<"->";
			result->add_switch(nextnode->name);
			result->connect(curnode->name,nextnode->name);
		}
	}
	out<<endl;
	result->print_all_v();
	result->print_all_e();
	Gplot *Resultgp=new Gplot();
	Resultgp->gp_add(result->elist);
	Resultgp->gp_dump(true);
	Resultgp->gp_export("Resultpath");
	out.close();
	cout<<"\n\n";
	cout<<"Save Result path in Result_path.txt"<<endl;
	
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

	Edge* e;
	e=(nm->elist);

	for(int j=0;j<v_count;j++)
		degree[j]=0;
	
	//cout<<e_count<<endl;
	for(int i=0;i<e_count;i++){
		//cout<<"Edge head is "<<e->head->name<<", tail is "<<e->tail->name<<endl;
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
	//cout<<"push "<<v->name<<endl;
	v_array[sp]=v;
	sp++;
}
Vertex* vertex_pop(Vertex** v_array,int& sp){
	if(sp!=0){
		sp--;
		//cout<<"pop "<<v_array[sp]->name<<endl;
		return v_array[sp];
	}
	else
		return NULL;
}
Vertex* vertex_looktop(Vertex** v_array,int& sp){
	if(sp!=0){
		//cout<<"top is "<<v_array[sp-1]->name<<endl;
		return v_array[sp-1];
	}
	else
		return NULL;
}
Vertex* findnextnode(NetworkManager *nm,Vertex* curnode,int v_count){
	for(int i=0;i<v_count;i++)
		if((nm->connected(curnode->name,nm->vlist[i]->name)==0) && (curnode !=nm->vlist[i])){
			//cout<<"Find node "<<nm->vlist[i]->name<<endl;

			return nm->vlist[i];
		}
	//cout<<"Not Found any node"<<endl;

	return NULL;
}
void print(int N,Vertex** a,vector<vector<Vertex*> > &pset)
{
    vector<Vertex*> tmp;
    for (int i=0; i<N; ++i) tmp.push_back(a[i]);
    pset.push_back(tmp);
}

void backtrack(int n, int N,Vertex** a,Vertex** element,bool* used,vector<vector<Vertex*> > &pset)
{
    if (n == N) {print(N,a,pset); return;}

    for (int i=0; i<N; i++)
        if (!used[i])
        {
            used[i] = true;
            a[n] = element[i];
            backtrack(n+1,N,a,element,used,pset);
            used[i] = false;
        }
}

void enumerate_permutations(int N,Vertex** a,Vertex** element,bool* used,vector<vector<Vertex*> > &pset)
{
    for (int i=0; i<N; i++) used[i] = false;
    backtrack(0, N,a,element,used,pset);
}