#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <set>
#include <string.h>
#include <cmath>
#include <iostream>
#include <cstring>

using namespace std;

#define MAX_LEN	1000

class term
{
public:
        string tem;
        int cnt;
        void set_term(string tm,int ct) {
                tem=tm;
                cnt=ct;
        }
};

class doc
{
public:
        string content;
        vector<term> term_list;
	doc(string ctt,vector<term> tl) {
		content=ctt;
		term_list=tl;
	}
	void set_doc(string &ctt,vector<term> &tl) {
		content=ctt;
		term_list=tl;
	}
	int search(string tkey) {
		for(int i=0; i<term_list.size(); i++) {
			if(!strcmp(term_list[i].tem.c_str(),tkey.c_str())) {
				return term_list[i].cnt;
			}
		}
		return -1;
	}
};

class dtm
{
public:
        int doc_num;
        int term_num;
        int **dtc;
        double **tf_idf;
        double **bm25;
        set<string> all_term;
        dtm(vector<doc> &vd) {
                doc_num=vd.size();
                dtc=new int*[doc_num];
                for(int i=0; i<doc_num; i++) {
                        for(int j=0; j<vd[i].term_list.size(); j++) {
                                all_term.insert(vd[i].term_list[j].tem);
                        }
                }
                term_num=all_term.size();
                for(int i=0; i<vd.size(); i++) {
                        dtc[i]=new int[term_num];
                }
                set<string>::iterator it;
                int k=0;
                for(int i=0; i<vd.size(); i++) {
                        for(it=all_term.begin(); it!=all_term.end(); it++) {
                                if(vd[i].search(*it)==-1) {
                                        dtc[i][k++]=0;
                                } else {
                                        dtc[i][k++]=vd[i].search(*it);
                                }
                        }
                        k=0;
                }
        }
        void comp_tf_idf() {
                tf_idf=new double*[doc_num];
                for(int i=0; i<doc_num; i++) {
                        tf_idf[i]=new double[term_num];
                }
                double max_tf_idf=-1;
                int *doc_term=new int[doc_num];
                int *term_doc=new int[term_num];
                for(int i=0; i<doc_num; i++) {
                        doc_term[i]=0;
                        for(int j=0; j<term_num; j++) {
                                doc_term[i]+=dtc[i][j];
                        }
                }
                for(int i=0; i<term_num; i++) {
                        term_doc[i]=0;
                        for(int j=0; j<doc_num; j++) {
                                term_doc[i]+=dtc[j][i];
                        }
                }
                double tf,idf;
                for(int i=0; i<doc_num; i++) {
                        for(int j=0; j<term_num; j++) {
                                tf=(double)dtc[i][j]/(double)((doc_term[i]));
                                idf=log((double)doc_num/(term_doc[j]));
                                tf_idf[i][j]=tf*idf;
                                if(max_tf_idf<tf_idf[i][j]) {
                                        max_tf_idf=tf_idf[i][j];
                                }
                        }
                }
                for(int i=0; i<doc_num; i++) {
                        for(int j=0; j<term_num; j++) {
                                tf_idf[i][j]/=max_tf_idf;
                        }
                }
        }
};

void load(vector<doc> &vd)
{
        char whole[MAX_LEN];
        cin.getline(whole,MAX_LEN);
        char ctt[MAX_LEN];
        vector<string> vs;
        int k=0;
        int i;
        for(i=0; i<strlen(whole); i++) {
                if(whole[i]=='"') {
                        while(whole[++i]!='"') {
                                ctt[k++]=whole[i];
                        }
                        break;
                }
        }
        ctt[k]='\0';
        char tmp[MAX_LEN];
        int tmpc=0;
        int j=0;
        int flag=0;
        for(i++; i<strlen(whole); i++) {
                while(whole[i]!=' ') {
                        tmp[j++]=whole[i++];
                        flag=1;
                        if(i==strlen(whole)) {
                                break;
                        }
                }
                if(flag==1) {
                        tmp[j]='\0';
                        vs.push_back(tmp);
                        j=0;
                        flag=0;
                }
        }
        vector<term> vt;
        term tmpt;
        tmpc=0;
        for(i=0; i<vs.size(); i+=2) {
                for(int k=0; k<vs[i+1].length(); k++) {
                        tmpc=10*tmpc+vs[i+1][k]-'0';
                }
                tmpt.set_term(vs[i],tmpc);
                vt.push_back(tmpt);
                tmpc=0;
        }
        doc tmpd(ctt,vt);
        vd.push_back(tmpd);
}

#endif
