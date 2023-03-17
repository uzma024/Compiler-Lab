#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

using namespace std;

set <string> CalFirst(string V, map <string, set <string> > &first, unordered_map <string, vector<vector <string>> > &prod){
    // cout<<V<<" ";
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
        

        // for(auto j = 0;j< prod[next].size();j++){
        //     if(prod[next][j][0]=="#"){
        //         set <string> temp2 = CalFirst(next, first, prod);
        //     }
        // }
    }
    return first[V];
}

set <string> CalFollow(string original, map <string, set <string> > &follow, map <string, set <string> > &first,unordered_map <string, vector<vector <string>> > &prod, vector <string> terminals){
    // cout<<"String : "<<original<<endl;
    
    if(follow.find(original)!=follow.end())return follow[original];
    set <string> follow_curr;
    
    for(auto it=prod.begin();it!=prod.end();it++){
        vector <vector<string>> temp= it->second;
    
        for(int i=0;i<temp.size();i++){
            /********************* debugging *********************/
            // cout<<it->first<<" -> ";
            // for(int p=0;p<temp[i].size();p++){
            //     cout<<temp[i][p];
            // }
            // cout<<endl;
            /********************* debugging *********************/
            auto last_ele = temp[i].end();
            last_ele--;
            auto curr_ele = find(temp[i].begin(),temp[i].end(),original);
            if(*last_ele=="#"){
                // cout<<"*last_ele=='#'"<<endl;
                break;
            }
            if(curr_ele!=temp[i].end()){
                // cout<<"curr_ele!=temp[i].end()"<<endl;
                // check if element if present in production
                // check if element is last in production
                if(curr_ele == last_ele){
                    // cout<<"curr_ele == last_ele"<<endl;
                    // calculate n add follow of parent
                    if(it->first==*curr_ele){
                        // cout<<"it->first==*curr_ele";
                        break;
                    }
                    CalFollow(it->first,follow,first,prod,terminals);
                    follow_curr.insert(follow[it->first].begin(),follow[it->first].end());
                }else{
                    // cout<<"curr_ele != last_ele"<<endl;
                    // else check next element -> for loop
                    auto next_ele = curr_ele;
                    next_ele++;

                    for(;next_ele!=temp[i].end();next_ele++){
                        // if terminal or no # productions add first to follow of original
                        if(find(terminals.begin(),terminals.end(),*next_ele)!=terminals.end()){
                            follow_curr.insert(*next_ele);
                            break;
                        }else{
                            follow_curr.insert(first[*next_ele].begin(),first[*next_ele].end());

                            bool hasEpsilon = first[*next_ele].find("#")!=first[*next_ele].end();
                            if(!hasEpsilon){
                                break;
                            }else if(next_ele==last_ele){
                                // produces # and last element
                                // add prev follow too
                                CalFollow(it->first,follow,first,prod,terminals);
                                follow_curr.insert(follow[it->first].begin(),follow[it->first].end());
                            }
                        }
                    }
                }


                    // else if # productions then 
                        // check if th element becomes last -> calculate n add follow of parent break
                        // else check next element and break if no # productions
            }else{
                // cout<<" element not in production "<<endl;
            }
        }
    }

    follow_curr.erase("#");
    follow[original]=follow_curr;
    return follow[original];

}

int main(){

	/******************FILE INPUT********************/
	fstream newfile;
	newfile.open("test2.txt");

	// File Structure
	// S A b B          - Productions S->AbB
    // S c S
    // A B A
    // A a
    // B b B
    // b #          - # represents epsilon

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

    /******************PRINT********************/
    cout<<"\nProductions: "<<endl;
    for(auto it=productions.begin();it!=productions.end();it++){
        cout<<it->first<<"->";
        for(size_t i=0;i<it->second.size();i++){
            for(size_t j=0;j<it->second[i].size();j++){
                cout<<it->second[i][j];
            }
            if(i!=it->second.size()-1)cout<<"|";
        }
        cout<<endl;
    }
	/******************ALGO********************/
    map <string, set <string> > first;

    // cout<<"\nTerminals: ";
    for(size_t i=0;i<terminals.size();i++){
        // cout<<terminals[i]<<",";
        first[terminals[i]].insert(terminals[i]);
    }
    // cout<<endl;


    for(auto it=productions.begin();it!=productions.end();it++){
        CalFirst(it->first, first, productions);
        // cout<<endl;
    }

    map <string, set <string> > follow;
    follow[prod_order[0]].insert("$");
    cout<<"\nStart symbol : "<<prod_order[0]<<endl;

    // cout<<"variables : ";
    while(follow.size()!=variables.size()){
        for(size_t i=0;i<variables.size();i++){
            // cout<<variables[i]<<" ";
            CalFollow(variables[i], follow, first,productions,terminals);
        }
        // cout<<"follow.size():"<<follow.size()<<" variables.size():"<<variables.size()<<endl;
    }
    // cout<<endl;

    /******************PRINT********************/
    cout<<"\nFirst: "<<endl;
    for(auto it=first.begin();it!=first.end();it++){
        if(it->first[0]!='#'){
            cout<<it->first<<"->";
            for(auto it2=it->second.begin();it2!=it->second.end();it2++){
                cout<<*it2<<" ";
            }
            cout<<endl;
        }
    }

    cout<<"\nFollow: "<<endl;
    for(auto it=follow.begin();it!=follow.end();it++){
        if(it->first.size() && it->first[0]>='A' && it->first[0]<='Z'){
            cout<<it->first<<"->";
            if(it->second.size()==0){
                cout<<"NA"<<endl;
            }else{
                for(auto it2=it->second.begin();it2!=it->second.end();it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;
            }
            
        }
    }

}
// g++ --std=c++17 -Wall -Wextra FirstnFollow.cpp && ./a.out