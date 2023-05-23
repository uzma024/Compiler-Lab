// Clean code file to be submitted
// considering cfg is unambiguous & no left recursion is present

#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <map>
#include <unordered_map>
#include <set>


using namespace std;

set <string> CalFirst(string V, map <string, set <string> > &first, unordered_map <string, vector<vector <string>> > &prod){
    if(first.find(V)!=first.end())return first[V];    
    for(size_t i=0;i!=prod[V].size();i++){
        if(prod[V][i][0]==V)continue;
        for(size_t j = 0;j<prod[V][i].size();j++){
            if(prod[V][i][j]==V)continue;
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

set <string> CalFollow(string original, map <string, set <string> > &follow, map <string, set <string> > &first,unordered_map <string, vector<vector <string>> > &prod, vector <string> terminals, string start_symbol){
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
                    CalFollow(it->first,follow,first,prod,terminals,start_symbol);
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
                                CalFollow(it->first,follow,first,prod,terminals,start_symbol);
                                follow_curr.insert(follow[it->first].begin(),follow[it->first].end());
                            }
                        }
                    }
                }
            }
        }
    }
    follow_curr.erase("#");
    if(original== start_symbol){
        follow_curr.insert("$");
    }
    follow[original]=follow_curr;
    return follow[original];

}

void makeLLTable(map <string, set <string> > &follow, map <string, set <string> > &first,vector <pair<string,vector<string> >> production_no, vector <string> terminals,vector <string> variables,map <pair<string,string>,set <int> > &LLParseTable){

	/****************** Algorithms ********************/
    
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
    /******************OUTPUT FILE********************/
    ofstream outfile;
    outfile.open ("LLTable.txt");

    if (outfile.is_open()){
        for(size_t i=0;i<terminals.size();i++){
            if(terminals[i]=="#")continue;outfile<<terminals[i]<<" ";
        }
        outfile<<"$"<<" "<<endl;

        for(size_t i=0;i<variables.size();i++){
            outfile<<variables[i]<<" ";
            for(size_t j=0;j<terminals.size();j++){
                if(terminals[j]=="#")continue;
                set <int> col = LLParseTable[{variables[i],terminals[j]}];
                if(col.size()){
                    auto end = col.end();
                    end--;
                    string s = "";
                    for(auto itr = col.begin();itr != col.end(); itr++){
                        s+=('0'+(*itr));
                        if(itr!=end)s+=",";
                    }
                    outfile<<s<<" ";
                }else{
                    outfile<<"-1"<<" ";
                }
            }
            if(LLParseTable[{variables[i],"$"}].size()){
                set <int> col = LLParseTable[{variables[i],"$"}];
                auto end = col.end();
                end--;
                string s = "";
                for(auto itr = col.begin();itr != col.end(); itr++){
                    s+=('0'+(*itr));
                    if(itr!=end)s+=",";
                }
                outfile<<s<<" ";
            }else{
                outfile<<"-1"<<" ";
            }
            outfile<<endl;
        }
    }else{
        cout<<"Unable to open file"<<endl;
        exit(0);
    }
}

int main(){
/******************FILE INPUT********************/
	fstream newfile;
    newfile.open("test1.txt");

	string tp;
	unordered_map <string, vector<vector <string>> > productions;
    vector <pair<string,vector<string> >> production_no;
    vector <string> prod_order;
    vector <string> terminals;
    vector <string> variables;

    if (newfile.is_open()){

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
    }else{
        cout << "Unable to open file"; 
        exit(0);
    }

	/******************First n Follow Function calls********************/
    map <string, set <string> > first;
    for(size_t i=0;i<terminals.size();i++){
        first[terminals[i]].insert(terminals[i]);
    }
    for(auto it=productions.begin();it!=productions.end();it++){
        CalFirst(it->first, first, productions);
    }

    map <string, set <string> > follow;
    follow[prod_order[0]].insert("$");
    cout<<"\nStart symbol : "<<prod_order[0]<<endl;

    while(follow.size()!=variables.size()){
        for(size_t i=0;i<variables.size();i++){
            CalFollow(variables[i], follow, first,productions,terminals,prod_order[0]);
        }
    }

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

    /******************LL Table formation********************/
    map <pair<string,string>,set <int> > LLParseTable;
    makeLLTable(follow, first,production_no,terminals,variables,LLParseTable);

    /******************String checking********************/

    vector <string> check_this;
    cout<<"\nEnter string to check: ";
    string teemmpp,t="";
    getline(cin,teemmpp);
    for(size_t i=0;i<=teemmpp.length();i++){
        if(i==teemmpp.length() || teemmpp[i]==' '){
            check_this.push_back(t);
            t="";
        }else{
            t+=teemmpp[i];
        }
    }
    check_this.push_back("$");
    int size_input = check_this.size();

    stack <string> st;
    st.push("$");
    st.push(prod_order[0]);
    int input_index = 0;

    vector <stack<string> > stack_history;
    vector <int> input_index_history;
    vector<int> prod_history;

    
    while(!st.empty() || input_index<size_input){
        stack_history.push_back(st);
        input_index_history.push_back(input_index);

        if(st.empty() || input_index>=size_input){
            // cout<<endl;
            cout<<"String not accepted"<<endl;
            prod_history.push_back(-1);
            break;
        }
        string top = st.top();
        st.pop();
        // cout<<setw(10)<<top<<setw(10)<<check_this[input_index]<<setw(10);
        
        if(top==check_this[input_index]){
            prod_history.push_back(-1);
            // cout<<endl;
            input_index++;
        }else if(LLParseTable.find({top,check_this[input_index]})==LLParseTable.end() || LLParseTable[{top,check_this[input_index]}].size()==0){
            prod_history.push_back(-1);
            // cout<<endl;
            cout<<"String not accepted"<<endl;
            break;
        }else{
            int prod_no = *(LLParseTable[{top,check_this[input_index]}].begin());
            auto rhs = production_no[prod_no].second;
            // cout<<setw(10)<<prod_no<<endl;
            for(int i=rhs.size()-1;i>=0;i--){
                if(rhs[i]!="#")st.push(rhs[i]);
            }
            prod_history.push_back(prod_no);
        }
    }


    if(st.empty() && input_index==size_input){
        cout<<"\nString is accepted\n"<<endl;
    }

   cout<<setw(10)<<"Stack"<<setw(20)<<"Input"<<setw(20)<<"Prodxn"<<endl;

    for(size_t i=0;i<stack_history.size();i++){
        string s="";
        while(!stack_history[i].empty()){
            s+=stack_history[i].top();
            stack_history[i].pop();
        }
        reverse(s.begin(),s.end());
        cout<<setw(10)<<s;
        string s1="";
        for(int j=input_index_history[i];j<size_input;j++){
            s1+=check_this[j];
        }
        cout<<setw(20)<<s1;

        if(prod_history[i]==-1){
            cout<<setw(20)<<"-";
        }else{
            string s2=production_no[prod_history[i]].first + " -> ";
            auto rhs = production_no[prod_history[i]].second;
            for(size_t j=0;j<rhs.size();j++){
                s2+=rhs[j];
            }
            cout<<setw(20)<<s2;
        }
        cout<<endl;
    }
}
// g++ --std=c++17 -Wall -Wextra LLStringCheck.cpp && ./a.out