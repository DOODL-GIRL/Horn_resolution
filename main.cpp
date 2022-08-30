#include<iostream>
#include "horn.h"
using namespace std;

vector<string> constants;//记录所有合法常量的集合
vector<string> variables;//记录所有合法变量的集合
vector<string> predicates;//记录所有合法谓词的集合
map<Clause,int,Comp_Clause> clause_set;//待归结的子句的集合

int main()
{
	read_in();
	cout << endl; 
	cout << "您输入的常量有：" << endl;
	for (int i = 0; i < constants.size(); i++)
		cout << constants[i] << " ";
	cout << endl;
	cout << "您输入的变量有：" << endl;
	for (int i = 0; i < variables.size(); i++)
		cout << variables[i] << " ";
	cout << endl;
	cout << "您输入的谓词有：" << endl;
	for (int i = 0; i < predicates.size(); i++)
		cout << predicates[i] << " ";
	cout << endl;
	cout << "您输入的子句集如下：" << endl;
	for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
	{
		cout << "(" << it->second << ") " << it->first << endl;
	}
	cout << endl;
	cout << "归结过程如下：" << endl;
	bool result = guijie();
	if (result == true)
		cout << "归结成功！" << endl;
	else
		cout << "归结失败！" << endl;
	cout << endl;
	cout << "关键路径如下：" << endl;
	trace_back();
	return 0;
}