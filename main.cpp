#include<iostream>
#include "horn.h"
using namespace std;

vector<string> constants;//��¼���кϷ������ļ���
vector<string> variables;//��¼���кϷ������ļ���
vector<string> predicates;//��¼���кϷ�ν�ʵļ���
map<Clause,int,Comp_Clause> clause_set;//�������Ӿ�ļ���

int main()
{
	read_in();
	cout << endl; 
	cout << "������ĳ����У�" << endl;
	for (int i = 0; i < constants.size(); i++)
		cout << constants[i] << " ";
	cout << endl;
	cout << "������ı����У�" << endl;
	for (int i = 0; i < variables.size(); i++)
		cout << variables[i] << " ";
	cout << endl;
	cout << "�������ν���У�" << endl;
	for (int i = 0; i < predicates.size(); i++)
		cout << predicates[i] << " ";
	cout << endl;
	cout << "��������Ӿ伯���£�" << endl;
	for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
	{
		cout << "(" << it->second << ") " << it->first << endl;
	}
	cout << endl;
	cout << "���������£�" << endl;
	bool result = guijie();
	if (result == true)
		cout << "���ɹ���" << endl;
	else
		cout << "���ʧ�ܣ�" << endl;
	cout << endl;
	cout << "�ؼ�·�����£�" << endl;
	trace_back();
	return 0;
}