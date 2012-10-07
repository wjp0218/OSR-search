#include <iostream>
#include <stdio.h>
#include "index.h"
#include "query.h"

using namespace std;

int main()
{
        vector<doc> dv;
        load(dv);
        load(dv);
        load(dv);
        dtm tmp(dv);
        tmp.comp_tf_idf();
        for(set<string>::iterator j=tmp.all_term.begin(); j!=tmp.all_term.end(); j++) {
                cout<<*j<<endl;
        }
        for(int i=0; i<tmp.doc_num; i++) {
                for(int j=0; j<tmp.term_num; j++) {
                        cout<<tmp.tf_idf[i][j]<<" ";
                }
                cout<<endl;
        }
	cout<<"-------------------------"<<endl;
	osr tosr(tmp);

	cout<<"-------------------------"<<endl;
	vector<double> ret;
	ret=tosr.sin_query(0);
	for(int i=0;i<ret.size();i++){
		cout<<ret[i]<<endl;
	}
        return 0;
}
