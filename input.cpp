#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

int main() {

  /******************FILE INPUT********************/
  fstream newfile;
  newfile.open("test.txt");

  string tp;
  map<string, vector<vector<string>>> productions;

  while (getline(newfile, tp)) {
    string lhs = "";
    int n = tp.length(), i = 0;

    for (; i < n; i++) {
      if (tp[i] == ' ') {
        i++;
        break;
      } else {
        lhs += tp[i];
      }
    }

    vector<string> rhs;
    for (; i < n; i++) {
      string temp = "";
      while (i < n && tp[i] != ' ') {
        temp += tp[i];
        i++;
      }
      rhs.push_back(temp);
    }
    productions[lhs].push_back(rhs);
  }
  cout << "here" << endl;
  map<string, vector<vector<string>>>::iterator itr = productions.begin();

  for (; itr != productions.end(); itr++) {
    string lhs = itr->first;
    vector<vector<string>> rhs = itr->second;

    for (int i = 0; i < rhs.size(); i++) {
      cout << lhs << " -> ";
      for (int j = 0; j < rhs[i].size(); j++) {
        cout << rhs[i][j] << " ";
      }
      cout << endl;
    }
  }
}