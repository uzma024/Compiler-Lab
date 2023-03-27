// Clean code file to be submitted
// considering cfg is unambiguous & no left recusion is present

#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

#include "fnf.h"

using namespace std;

set <string> CalFirst(string V, map <string, set <string> > &first, unordered_map <string, vector<vector <string>> > &prod){
    if(first.find(V)!=first.end()){
        return first[V];
    }
    for(size_t i=0;i!=prod[V].size();i++){
        if(prod[V][i][0]==V){
            continue;
        }
        for(size_t j = 0;j<prod[V][i].size();j++){
            if(prod[V][i][j]==V){
                continue;
            }
            string next = prod[V][i][j];
            set <string> temp = CalFirst(next, first, prod);

            // Check if cuurent first generates epsilon
            bool isEpsilon = false;

            for(auto it = temp.begin();it!=temp.end();it++){
                if(*it!="#"){
                    first[V].insert(*it);
                }else{
                    isEpsilon = true;
                    if(j==prod[V][i].size()-1){
                        first[V].insert(*it);
                    }
                }
            }
            
            // break loop if it does not generate epsilon
            if(!isEpsilon){
                break;
            }
        }
    }
    return first[V];
}

set <string> CalFollow(string original, map <string, set <string> > &follow, map <string, set <string> > &first,unordered_map <string, vector<vector <string>> > &prod, vector <string> terminals){
    if(follow.find(original)!=follow.end())return follow[original];
    set <string> follow_curr;
    
    for(auto it=prod.begin();it!=prod.end();it++){
        vector <vector<string>> temp= it->second;
    
        for(size_t i=0;i<temp.size();i++){
            auto last_ele = temp[i].end();
            last_ele--;
            auto curr_ele = find(temp[i].begin(),temp[i].end(),original);
            if(*last_ele=="#"){
                break;
            }
            if(curr_ele!=temp[i].end()){
                // check if element if present in production
                if(curr_ele == last_ele){
                // check if element is last in production
                    // calculate n add follow of parent
                    if(it->first==*curr_ele)break;
                    CalFollow(it->first,follow,first,prod,terminals);
                    follow_curr.insert(follow[it->first].begin(),follow[it->first].end());
                }else{
                    // check next element -> for loop untill no epsilon
                    auto next_ele = curr_ele;
                    next_ele++;

                    for(;next_ele!=temp[i].end();next_ele++){
                        // if terminal or no # productions add first of next to follow of original
                        if(find(terminals.begin(),terminals.end(),*next_ele)!=terminals.end()){
                            follow_curr.insert(*next_ele);
                            break;
                        }else{
                            follow_curr.insert(first[*next_ele].begin(),first[*next_ele].end());

                            bool hasEpsilon = first[*next_ele].find("#")!=first[*next_ele].end();
                            if(!hasEpsilon){
                                break;
                            }else if(next_ele==last_ele){
                                // produces # and last element : add follow of lhs too
                                CalFollow(it->first,follow,first,prod,terminals);
                                follow_curr.insert(follow[it->first].begin(),follow[it->first].end());
                            }
                        }
                    }
                }
            }
        }
    }
    follow_curr.erase("#");
    follow[original]=follow_curr;
    return follow[original];

}


void MakeFirstFollow(){

	/******************FILE INPUT********************/
	fstream newfile;
	// newfile.open("20BCS052_P5_FirstFollow.txt");
    // Test7.txt  -  test case given by sir to test
    newfile.open("test1.txt");

	// File Structure
	// S A b B          - Productions S->AbB
    // S c S
    // A B A
    // A a
    // B b B
    // B #          - # represents epsilon

	string tp;
	unordered_map <string, vector<vector <string>> > productions;
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
        productions[lhs].push_back(prod);
        prod_order.push_back(lhs);
    }

    newfile.close();

	/****************** Function calls********************/
    // First
    map <string, set <string> > first;
    for(size_t i=0;i<terminals.size();i++){
        first[terminals[i]].insert(terminals[i]);
    }
    for(auto it=productions.begin();it!=productions.end();it++){
        CalFirst(it->first, first, productions);
    }

    // Follow
    map <string, set <string> > follow;
    follow[prod_order[0]].insert("$");
    // cout<<"\nStart symbol : "<<prod_order[0]<<endl;

    while(follow.size()!=variables.size()){
        for(size_t i=0;i<variables.size();i++){
            CalFollow(variables[i], follow, first,productions,terminals);
        }
    }

    /******************OUTPUT FILE********************/
    ofstream outfile;
    outfile.open ("first.txt");
    // myfile << "Writing this to a file.\n";

    // outfile<<"First"<<endl;
    for(auto it=first.begin();it!=first.end();it++){
        if(it->first[0]!='#'){
            outfile<<it->first<<" ";
            for(auto it2=it->second.begin();it2!=it->second.end();it2++){
                outfile<<*it2<<" ";
            }
            outfile<<endl;
        }
    }

    outfile.close();

    // ofstream outfile;
    outfile.open ("follow.txt");
    // outfile<<"Follow"<<endl;
    for(auto it=follow.begin();it!=follow.end();it++){
        if(it->first.size() && it->first[0]>='A' && it->first[0]<='Z'){
            outfile<<it->first<<" ";
            if(it->second.size()==0){
                outfile<<"NA"<<endl;
            }else{
                for(auto it2=it->second.begin();it2!=it->second.end();it2++){
                    outfile<<*it2<<" ";
                }
                outfile<<endl;
            }
        }
    }

    outfile.close();
}

// g++ --std=c++17 -Wall -Wextra FirstnFollow.cpp && ./a.out