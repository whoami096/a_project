#include <map>
#include "global.h"
#include <algorithm>
#include "calc.h"
#include "output.h"
#include "myint.h"
#include <vector>
#include <iostream>
using namespace std;
map<string, string> lib;

string var_name(string &str)
{
    string tmp;
    reverse(str.begin(), str.end());
    while(str.back() != '=' && str.back() != ' ' && str.size())
    {
        tmp += str.back();
        str.pop_back();
    }
    while(str.back() == '=') str.pop_back();
    reverse(str.begin(), str.end());
    return tmp;
}
string f(string &str)
{
    //cout << '#' << str << endl;
    vector<string> ans;
    string op = "+-/*%^";
    while (str.front() != ')')
    {
        if(str.front() == '(')
        {
            str = str.substr(1, str.size());
            ans.push_back(f(str));
        }
        else
        {
            string tmp;
            for (int i = 0; i < str.size(); i++)
            {
                tmp += str[i];
                if(str[i] == ')')
                {
                    str = str.substr(i, str.size());
                    break;
                }
                
                if(op.find(str[i]) != string::npos)
                {
                    str = str.substr(i+1, str.size());
                    break;
                }
                
            }
            //cout << tmp << endl;
            ans.push_back(tmp);
            
        }
    }
    str = str.substr(1, str.size());
    //get the vector ans;
    vector<string> ans1;
    string op1 = "+-", op2 = "*/%^";
    
    for (int i = 0; i < ans.size(); i++)
    {
        if(op2.find(ans[i].back()) != string::npos)
        {
            ans1.push_back(calc(ans[i].substr(0, ans[i].size()-1), ans[i+1].substr(0, ans[i+1].size()-1), ans[i].back()));
            ans1.back().push_back(ans[i+1].back());
            //cout << '#' << ans1.back() << endl;
            for(int j=i+2;j<ans.size();j++)ans1.push_back(ans[j]);
            ans = ans1;
        }
        else ans1.push_back(ans[i]);
        //if(res) break; if there are only +- in vector ans then break;
    }
    //get the vector ans1 which has only +-
    ans.clear();
    while (ans1.size() != 1)
    {
        ans.push_back(calc(ans1[0].substr(0, ans1[0].size()-1), ans1[1].substr(0, ans1[1].size()-1), ans1[0].back()));
        ans.back().push_back(ans1[1].back());
        
        for(int j=2;j<ans1.size();j++)ans.push_back(ans1[j]);

        ans1 = ans;
        ans.clear();

    }
    return ans1.front();
    
}
string val_calc(string &str)
{
    string sp = "+-*/%^";

    bool res = true;
    for(auto i:sp) if(str.find(i) != string::npos) {res = false; break;}
    if(res && str.front() > '9') return lib[str];
    str.push_back(')');
    reverse(str.begin(), str.end());
    str.push_back('(');
    reverse(str.begin(), str.end());
    str = str.substr(1, str.size());
    string ans = f(str);
    ans.pop_back();
    return ans;
}

void input(string s)
{
    string left;
    string tmp = s;
    s.clear();
    for(auto i:tmp) if(i!=' ')s.push_back(i);
    s += "+0";
    if(s.front() >= 'a' && s.find('=') != string::npos)
    {
        left = var_name(s);
        if(left == "ibase")
        {
            Bigint ans = val_calc(s);
            ibase = 0;
            reverse(ans.val_.begin(), ans.val_.end());
            for(auto i:ans.val_) ibase *= 10, ibase += i;
            //cout << '#' << ibase << endl;
        }
        else if(left == "obase")
        {
            Bigint ans = val_calc(s);
            obase = 0;
            reverse(ans.val_.begin(), ans.val_.end());
            for(auto i:ans.val_) obase *= 10, obase += i;
        }
        else if(s.empty()) output(lib[left]);
        else lib[left] = val_calc(s);
    }
    else if(s.substr(0, 5) == "ibase") output(ibase);
    else if(s.substr(0, 5) == "obase") output(obase);
    else output(val_calc(s));
}