#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

int main(){

	/******************FILE INPUT********************/
	fstream newfile;
	newfile.open("Mealy.txt");

	// File Structure
	// 0 	-initial st
	// 0 A 1 A 	-DFA starts
	// -1 -1 2 B 	- -1 means no transition = phi
	// -1 -1 0 A

	string tp;
	getline(newfile,tp);

	// int ini_state=atoi(tp);
	int ini_state=stoi(tp);


	vector <vector <int> > DFA;
	vector <vector <string> > out;

	while(getline(newfile,tp)){
		vector <int> v;
		vector <string> s;
		int i=0;
		int n=tp.length();

		while(i<n){
			int t=0;
			while(i<n && tp[i]!=' '){
				if(tp[i]=='-'){
					i++;
					if(tp[i]=='1'){
						t=-1;
					}
				}else{
					t*=10;
					t+=(tp[i]-'0');
				}
				i++;
			}
			v.push_back(t);
			i++;

			string temp="";
			while(i<n && tp[i]!=' '){
				if(tp[i]=='-'){
					i++;
					if(tp[i]=='1'){
						temp="";
					}
				}else{
					temp+=tp[i];
					i++;
				}
			}
			s.push_back(temp);
			i++;
		}
		DFA.push_back(v);
		out.push_back(s);
	}


	/******************PRINT********************/

	cout<<"Initial State : "<<ini_state<<endl;

	cout<<"DFA: "<<endl;
	for(size_t i=0;i<DFA.size();i++){
		for(size_t j=0;j<DFA[i].size();j++){
			cout<<DFA[i][j]<<":"<<out[i][j]<<" ";
		}
		cout<<endl;
	}
	/******************ALGO********************/

	while(1){
		/******************STRING INPUT********************/
		string input;
		cout<<"Ënter Input String"<<endl;
		getline(cin,input);
		int state=ini_state;
		size_t i=0;
		string output="";

		cout<<"\nTransitions : "<<endl;
		while(state!=-1 && i<input.length()){
			cout<<DFA[state][input[i]-'0']<<":"<<out[state][input[i]-'0']<<" ";
			if(DFA[state][input[i]-'0']==-1)break;
			output+=out[state][input[i]-'0'];
			state=DFA[state][input[i]-'0'];
			i++;
		}
		cout<<"Output is : "<<output<<"\n"<<endl;

	}

	
	return 0;

}