// Considering cfg has no left recusion and no left factoring
// First and Follow sets are computed using fnf.cpp

#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <iomanip>
#include "fnf.h"

using namespace std;

int main(){
    // Calling function from previous file to compute first and follow sets
    MakeFirstFollow();

    /******************FILE INPUT********************/
	fstream newfile;
    newfile.open("test1.txt");
    
	string tp;
	unordered_map <string, vector<vector <string>> > productions;
    vector <pair<string,vector<string> >> production_no;
    vector <string> prod_order;
    vector <string> terminals;
    vector <string> variables;

	while(getline(newfile,tp)){
        size_t i=0,n = tp.length();
        string lhs="";
        for(;i<n;i++){
            if(tp[i]==' '){
                i++;
                break;
            }else{
                lhs+=tp[i];
            }
        }
        if(find(variables.begin(),variables.end(),lhs)==variables.end())variables.push_back(lhs); 
        string temp = "";
        vector <string> prod;
        for(;i<tp.size();i++){
            if(tp[i]==' '){
                prod.push_back(temp);
                if(temp.size() && !(temp[0]>='A' && temp[0]<='Z')){
                    if(find(terminals.begin(),terminals.end(),temp)==terminals.end())terminals.push_back(temp);
                }else{
                    if(find(variables.begin(),variables.end(),temp)==variables.end())variables.push_back(temp);
                }
                temp = "";
            }else{
                temp+=tp[i];
            }
        }
        if(temp.size() && !(temp[0]>='A' && temp[0]<='Z')){
            if(find(terminals.begin(),terminals.end(),temp)==terminals.end())terminals.push_back(temp);
        }else{
            if(find(variables.begin(),variables.end(),temp)==variables.end())variables.push_back(temp);
        }
        prod.push_back(temp);
        production_no.push_back(make_pair(lhs,prod));
        productions[lhs].push_back(prod);
        prod_order.push_back(lhs);
    } 
    newfile.close();

    /******************FILE INPUT FIRST n FOLLOW********************/
    newfile.open("first.txt");
    map <string, set <string> > first;
    while(getline(newfile,tp)){
        size_t i=0,n = tp.length();
        string lhs="";
        for(;i<n;i++){
            if(tp[i]==' '){
                i++;
                break;
            }else{
                lhs+=tp[i];
            }
        }
        string temp = "";
        set <string> f;
        for(;i<tp.size();i++){
            if(tp[i]==' '){
                f.insert(temp);
                temp = "";
            }else{
                temp+=tp[i];
            }
        }
        f.insert(temp);
        first[lhs] = f;
    }
    newfile.close();
    newfile.open("follow.txt");
    map <string, set <string> > follow;
    while(getline(newfile,tp)){
        size_t i=0,n = tp.length();
        string lhs="";
        for(;i<n;i++){
            if(tp[i]==' '){
                i++;
                break;
            }else{
                lhs+=tp[i];
            }
        }
        string temp = "";
        set <string> f;
        for(;i<tp.size();i++){
            if(tp[i]==' '){
                f.insert(temp);
                temp = "";
            }else{
                temp+=tp[i];
            }
        }
        f.insert(temp);
        follow[lhs] = f;
    }
    newfile.close();

    /******************PRINT********************/
    cout<<"\nProductions: "<<endl;

    for(size_t i=0;i<production_no.size();i++){
        string s=production_no[i].first + " -> ";
        auto rhs = production_no[i].second;
        for(size_t j=0;j<rhs.size();j++){
            s+=rhs[j];
        }
        cout<<setw(10)<<s<<setw(6)<<":"<<i<<endl;
    }

	/****************** Algorithms ********************/
    map <pair<string,string>,set <int> > LLParseTable;
    int maxbox = 0;
    for(size_t i=0;i<production_no.size();i++){
        int p_no = i;
        string lhs = production_no[i].first;
        set <string> col;

        for(size_t j=0;j<production_no[i].second.size();j++){
            bool has_epsilon = false;
            string rhs = production_no[i].second[j];
            if(rhs == "#"){
                for (auto itr = follow[lhs].begin();itr != follow[lhs].end(); itr++){
                    if(*itr =="#")has_epsilon=true;
                    col.insert(*itr);
                }
                break;
            }
            for (auto itr = first[rhs].begin();itr != first[rhs].end(); itr++){
                if(*itr =="#")has_epsilon=true;
                else col.insert(*itr);
            }
            if(!has_epsilon)break;
            else if(j==production_no[i].second.size()-1){
                for (auto itr = follow[lhs].begin();itr != follow[lhs].end(); itr++){
                    col.insert(*itr);
                }
            }
        }
        for(auto itr = col.begin();itr != col.end(); itr++){
            LLParseTable[{lhs,*itr}].insert(p_no);
            maxbox = max(maxbox,(int)LLParseTable[{lhs,*itr}].size());
        }
    }
    
    /****************** PRINT ********************/
    cout<<"\nLL Parsing Table"<<endl;
    cout<<"  ";
    for(size_t i=0;i<terminals.size();i++)cout<<"=======";
    cout<<endl;
    cout<<"  ";
    for(size_t i=0;i<terminals.size();i++){
        if(terminals[i]=="#")continue;
        cout<<"|"<<setw(6)<<terminals[i];
    }
    cout<<"|"<<setw(6)<<"$"<<"|"<<endl;
    cout<<"===";
    for(size_t i=0;i<terminals.size();i++)cout<<"=======";
    cout<<endl;
    for(size_t i=0;i<variables.size();i++){
        cout<<"|"<<variables[i];
        for(size_t j=0;j<terminals.size();j++){
            if(terminals[j]=="#")continue;
            set <int> col = LLParseTable[{variables[i],terminals[j]}];
            cout<<"|";
            if(col.size()){
                auto end = col.end();
                end--;
                string s = "";
                for(auto itr = col.begin();itr != col.end(); itr++){
                    s+=('0'+(*itr));
                    if(itr!=end)s+=",";
                }
                cout<<setw(6)<<s;
            }else{
                cout<<setw(6)<<"-1";
            }
        }
        if(LLParseTable[{variables[i],"$"}].size()){
            set <int> col = LLParseTable[{variables[i],"$"}];
            auto end = col.end();
            end--;
            string s = "";
            cout<<"|";
            for(auto itr = col.begin();itr != col.end(); itr++){
                s+=('0'+(*itr));
                if(itr!=end)s+=",";
            }
            cout<<setw(6)<<s;
        }else{
            cout<<"|"<<setw(6)<<"-1";
        }
        cout<<"|"<<endl;
        cout<<"===";
        for(size_t i=0;i<terminals.size();i++)cout<<"=======";
        cout<<endl;
    }
}
// g++ --std=c++17 -Wall -Wextra parser.cpp fnf.cpp && ./a.out