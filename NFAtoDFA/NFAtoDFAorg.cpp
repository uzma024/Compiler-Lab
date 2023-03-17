#include <iostream>
#include <fstream>
// #include <sstream>
#include <string> 
#include <vector>
#include <map>
#include <set>

using namespace std;

int main(){

	/******************FILE INPUT********************/
	fstream newfile;
	newfile.open("NFAtoDFA.txt");

	// File Structure
	// 0
	// 1
	// 0,1,2 1 -1
	// -1 2 1
	// -1 0,2 -1

	string tp;

	getline(newfile,tp);
	int ini_state=stoi(tp);
	getline(newfile,tp);
	int final_state=stoi(tp);
	

	vector <vector <vector <int> > > NFA;

	while(getline(newfile,tp)){
		vector <vector <int> > temp;
		int i=0,n=tp.length();

		while(i<n){
			vector <int> v;
			while(i<n && tp[i]!=' '){
				int t=0;
				while(i<n && (tp[i]!=',' && tp[i]!=' ')){
					if(tp[i]=='-'){
						t=-1;
						i+=2;
					}else{
						t=t*10+tp[i]-'0';
						i++;
					}
				}
				// cout<<t<<"/";
				v.push_back(t);
				t=0;
				if(i<n && tp[i]==',')i++;
			}
			i++;
			temp.push_back(v);
			// cout<<endl;
		}
		NFA.push_back(temp);
	}


	/******************PRINT********************/

	cout<<"Initial State : "<<ini_state<<endl;
	cout<<"Final State : "<<final_state<<endl;

	cout<<"NFA: "<<endl;
	for(size_t i=0;i<NFA.size();i++){
		for(size_t j=0;j<NFA[i].size();j++){
			for(size_t k=0;k<NFA[i][j].size();k++){
				cout<<NFA[i][j][k];
				if(k!=NFA[i][j].size()-1)cout<<",";
			}
			cout<<" ";
		}
		cout<<endl;
	}
	/******************ALGO********************/

	int k = NFA.size();
	int no_of_inp = NFA[0].size();

	map <vector <int> , int > m;

	for(size_t i=0;i<NFA.size();i++){
		for(size_t j=0;j<NFA[i].size();j++){

			if(NFA[i][j].size()>1){
				// cout<<"i:"<<i<<" j:"<<j<<endl;
				
				if(m.find(NFA[i][j])!=m.end()){
					// Debugging : print maps
					// cout<<"here: ";
					// for(int jj=0;jj<NFA[i][j].size();jj++){
					// 	cout<<NFA[i][j][jj]<<" ";
					// }
					// cout<<"-> "<<m[NFA[i][j]]<<endl;
					vector <int> curr= NFA[i][j];
					NFA[i][j].clear();
					NFA[i][j].push_back(m[curr]);
				}else{
					vector <vector <int> > temp;
					for(int l=0;l<no_of_inp;l++){
						set <int> t;
						for(size_t m=0;m<NFA[i][j].size();m++){
							int state = NFA[i][j][m];
							if(state!=-1){
								for(size_t n=0;n<NFA[state][l].size();n++){
									if(NFA[state][l][n]!=-1){
										t.insert(NFA[state][l][n]);
									}
								}
							}
						}
						if(t.size()==0)t.insert(-1);
						vector<int> vc(t.begin(), t.end());
						temp.push_back(vc);
					}
					m[NFA[i][j]]=k;

					// Debugging : print maps
					// cout<<"here: ";
					// for(int jj=0;jj<NFA[i][j].size();jj++){
					// 	cout<<NFA[i][j][jj]<<" ";
					// }
					// cout<<"-> "<<k<<endl;

					NFA[i][j].clear();
					NFA[i][j].push_back(k);
					NFA.push_back(temp);

					// Debugging : print new state
					cout<<k<<" : ";
					for(size_t yy=0;yy<temp.size();yy++){
						for(size_t zz=0;zz<temp[yy].size();zz++){
							cout<<temp[yy][zz]<<" ";
						}
						cout<<" ";
					}
					cout<<endl;
					k++;
				}
			}
			if(k>10)break;
		}
	}

	cout<<"DFA: "<<endl;
	for(size_t i=0;i<NFA.size();i++){
		cout<<"q"<<i<<" : ";
		for(size_t j=0;j<NFA[i].size();j++){
			for(size_t k=0;k<NFA[i][j].size();k++){
				cout<<NFA[i][j][k];
				if(k!=NFA[i][j].size()-1)cout<<",";
			}
			cout<<" ";
		}
		cout<<endl;
	}

	
	return 0;

}

// Requiremnet engineering
// tommorrow's lecture 14 feb
