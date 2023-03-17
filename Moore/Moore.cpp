#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

int main(){

	/******************FILE INPUT********************/
	fstream newfile;
	newfile.open("Moore.txt");

	// File Structure
	// 0 	-initial st
	// 0 1 A 	-DFA starts
	// -1 2 B 	- -1 means no transition = phi
	// -1 0 A

	string tp;
	getline(newfile,tp);

	// int ini_state=atoi(tp);
	int ini_state=stoi(tp);


	vector <vector <int> > DFA;
	vector <string> out;

	while(getline(newfile,tp)){

		int n=tp.length();

		vector <int> v;
		string s="";

		int i=n-1;
		while(i>=0 && tp[i]!=' '){
			s=tp[i]+s;
			i--;
		}
		out.push_back(s);

		int j=0;
		while(j<i){
			if(tp[j]==' '){
				j++;
				continue;
			}
			int t=0;
			while(j<i && tp[j]!=' '){
				if(tp[j]=='-'){
					j++;
					if(tp[j]=='1'){
						t=-1;
					}
				}else{
					t*=10;
					t+=(tp[j]-'0');
				}
				j++;
			}
			v.push_back(t);
			j++;
		}

		DFA.push_back(v);

	}


	/******************PRINT********************/

	cout<<"Initial State : "<<ini_state<<endl;

	cout<<"DFA: "<<endl;
	for(size_t i=0;i<DFA.size();i++){
		for(size_t j=0;j<DFA[i].size();j++){
			cout<<DFA[i][j]<<":"<<out[i]<<" ";
		}
		cout<<out[i]<<endl;
	}
	/******************ALGO********************/

	while(1){
		/******************STRING INPUT********************/
		string input;
		cout<<"Ënter Input String"<<endl;
		getline(cin,input);
		int state=ini_state;
		size_t i=0;
		string output=out[state];

		cout<<"\nTransitions : "<<state;
		while(state!=-1 && i<input.length()){
			if(DFA[state][input[i]-'0']==-1)break;
			state=DFA[state][input[i]-'0'];
			cout<<"->"<<state;
			output+=out[state];
			i++;
		}
		cout<<"\nOutput is : "<<output<<"\n"<<endl;

	}

	
	return 0;

}