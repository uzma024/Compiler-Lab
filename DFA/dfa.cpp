#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int main(){

    /***********************FILE INPUT*****************************/
    fstream newfile;
    newfile.open("dfa.txt");
    
    string tp;
    getline(newfile,tp);

    int ini_st=stoi(tp);

    getline(newfile,tp);
    vector <int> final_st;
    int temp=0;
    for(size_t i=0;i<tp.length();i++){
        if(tp[i]==','){
            final_st.push_back(temp);
            temp=0;
        }else{
            temp*=10;
            temp+=tp[i]-'0';
        }
    }
    final_st.push_back(temp);

    vector <vector <int>> dfa;
    while(getline(newfile,tp)){
        vector <int> v;
        int temp=0;
        for(size_t i=0;i<tp.length();i++){
            if(tp[i]=='-'){
                i++;
                if(tp[i]=='1'){
                    temp=-1;
                }
            }else if(tp[i]==' '){
                v.push_back(temp);
                temp=0;
            }else{
                temp*=10;
                temp+=tp[i]-'0';
            }
        }
        v.push_back(temp);
        dfa.push_back(v);
        // cout<<state[0]<<" "<<state[1]<<" "<<state[2]<<endl;
    }
    

    /***********************PRINT*****************************/
    cout<<"Initial State: "<<ini_st<<endl;
    cout<<"Final States: ";
    for(size_t i=0;i<final_st.size();i++){
        cout<<final_st[i]<<" ";
    }
    cout<<endl;

    cout<<"DFA: "<<endl;
    for(size_t i=0;i<dfa.size();i++){
        for(size_t j=0;j<dfa[i].size();j++){
            cout<<dfa[i][j]<<" ";
        }
        cout<<endl;
    }


    while(1){
        /***********************STRING INPUT*****************************/
        string input;
        cout<<"Enter Input String:"<<endl;
        getline(cin,input);

        /***********************LOGIC*****************************/
        int state=ini_st;
        size_t i=0;

        int n=dfa.size(),m= dfa[0].size();

        while(i<input.length() && (state>=0 && state<n) && (input[i]-'0')>=0 && (input[i]-'0')<m){
            state=dfa[state][input[i]-'0'];
            i++;
        }
        bool finalState=false;
        for(size_t i=0;i<final_st.size();i++){
            if(state==final_st[i]){
                finalState=true;
                break;
            }
        }
        if(finalState && i==input.length()){
            cout<<"String "<<input<<" is "<<"accepted\n"<<endl;
        }else{
            cout<<"String "<<input<<" is "<<"rejected\n"<<endl;
        }
    }
    

    return 0;
}
// g++ --std=c++17 -Wall -Wextra dfa.cpp && ./a.out