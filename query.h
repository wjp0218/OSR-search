#ifndef QUERY_H
#define QUERY_H

#include <queue>
#include <vector>

#include "index.h"

#define DOC	0
#define TERM	1
#define UNVISED	0
#define VISING	1
#define VISED	2

using namespace std;

class node
{
public:
	int type;
	int number;
	double energy;
	int status;
	int deg;
	vector<node> adj;
	void set_node(int type,int number,double energy,int status){
		this->type=type;
		this->number=number;
		this->energy=energy;
		this->status=status;
	}
};

class osr
{
public:
	node *doc_node;
	node *term_node;
	double **tf_idf;
	int doc_num;
	int term_num;
	osr(dtm &dtmp){
		int tmpd=0;
		vector<node> vn;
		this->doc_num=dtmp.doc_num;
		this->term_num=dtmp.term_num;
		doc_node=new node[doc_num];
		term_node=new node[term_num];
		for(int i=0; i<doc_num; i++){
			doc_node[i].set_node(DOC,i,0,UNVISED);
			for(int j=0;j<term_num;j++){
				if(dtmp.dtc[i][j]!=0){
					tmpd++;
				}
			}
			doc_node[i].deg=tmpd;
			tmpd=0;
		}
		for(int i=0; i<term_num; i++){
			term_node[i].set_node(TERM,i,0,UNVISED);
			for(int j=0;j<doc_num;j++){
				if(dtmp.dtc[j][i]!=0){
					tmpd++;
				}
			}
			term_node[i].deg=tmpd;
			tmpd=0;
		}
		for(int i=0; i<doc_num; i++){
			for(int j=0;j<term_num;j++){
				if(dtmp.dtc[i][j]!=0){
					vn.push_back(term_node[j]);
				}
			}
			doc_node[i].adj=vn;
			vn.clear();
		}
		for(int i=0; i<term_num; i++){
			for(int j=0;j<doc_num;j++){
				if(dtmp.dtc[j][i]!=0){
					vn.push_back(doc_node[j]);
				}
			}
			term_node[i].adj=vn;
			vn.clear();
		}
		tf_idf=new double*[doc_num];
                for(int i=0; i<doc_num; i++) {
                        tf_idf[i]=new double[term_num];
                }
		for(int i=0;i<doc_num;i++){
			for(int j=0;j<term_num;j++){
				this->tf_idf[i][j]=dtmp.tf_idf[i][j];
			}
		}
	}
	vector<double> sin_query(int b){
		term_node[b].status=VISING;
		term_node[b].energy=1;
		queue<node> Q;
		Q.push(term_node[b]);		
		double threshold=0;
		double branch_energy;
		int deg;
		node tmp;
		double weight;
		vector<double> ret;
		int t;
		while(!Q.empty()){
			tmp=Q.front();
			Q.pop();
			if(tmp.type==DOC){
				deg=doc_node[tmp.number].deg;
				branch_energy=doc_node[tmp.number].energy/(double)deg;
				if(deg==0){
					doc_node[tmp.number].status=VISED;
					continue;
				}
			}else{
				deg=term_node[tmp.number].deg;
				branch_energy=term_node[tmp.number].energy/(double)deg;
				if(deg==0){
					term_node[tmp.number].status=VISED;
					continue;
				}
			}			
			if(branch_energy>threshold){
				if(tmp.type==DOC){
					for(int i=0; i<deg; i++){
						t=doc_node[tmp.number].adj[i].number;
						if(term_node[t].status==UNVISED){	
							weight=tf_idf[tmp.number][t];
							term_node[t].energy+=branch_energy*weight;
							Q.push(term_node[t]);
							term_node[t].status=VISING;
						}
					}
				}else{
					for(int i=0; i<deg; i++){
						t=term_node[tmp.number].adj[i].number;
						if(doc_node[t].status==UNVISED){							
							weight=tf_idf[t][tmp.number];
							doc_node[t].energy+=branch_energy*weight;
							Q.push(doc_node[t]);
							doc_node[t].status=VISING;
						}
					}
				}
			}
			if(tmp.type==DOC){
				doc_node[tmp.number].status=VISED;
			}else{
				term_node[tmp.number].status=VISED;
			}
		}
		for(int i=0;i<doc_num;i++){
			ret.push_back(doc_node[i].energy);
		}
		return ret;
	}
};


#endif
