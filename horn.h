#define CONSTANT 0
#define VARIABLE 1
#include <iostream>
#include <vector>
#include<iterator>
#include <string>
#include <map>
#include <fstream>
#include<algorithm>
using namespace std;

extern vector<string> constants;//��¼���кϷ������ļ���
extern vector<string> variables;//��¼���кϷ������ļ���
extern vector<string> predicates;//��¼���кϷ�ν�ʵļ���

struct identifier//�������ͱ�����Ϊ��ʶ��ͳһ����
{
	bool category;//CONSTANT or VVARIABLE
	int id;//�����������Ӧ���ϵ��±�
};

struct Atom//ԭ��ʽ
{
	bool positive ;//���񶨷�����Ϊ0�������񶨷�����Ϊ1
	int predicate_id;//ν�ʶ�Ӧ����
	vector<identifier> element;//��˳���г�ԭ��ʽ�еı�ʶ��
	bool erase_leagal(const Atom& a, vector<int>&change_list);//�ж�����ԭ������ܷ�������,������change_list�����¼�ź�һ�û���Ӧ��ʶ���±�
};

struct Clause//�Ӿ�
{
	int source_1 = -1;//��Դ1�Ӿ��Ӧid��ԭʼ�Ӿ�Ϊ-1��
	int source_2 = -1;//��Դ2�Ӿ��Ӧid��ԭʼ�Ӿ�Ϊ-1��
	vector<Atom> atom_set;//��˳���г��Ӿ��е�ԭ��ʽ
};

int find_predicate(string s);//��ν�ʱ��в���ν�ʣ����ҵ��������������򷵻�-1
int find_variable(string s);//�ڱ������в��ұ��������ҵ��������������򷵻�-1
int find_constant(string s);//�ڳ������в��ҳ��������ҵ��������������򷵻�-1
int atom_exist(Atom atom, vector<Atom> atom_set);//��ԭ����伯���в���ԭ����䣬���ҵ��������������򷵻�-1
int clause_exist(Clause clause);//���Ӿ伯�в����Ӿ䣬���ҵ��������������򷵻�-1
bool atom_legal(string s, Atom& ret);//�ж�ĳ�ַ����Ƿ��ǺϷ���ԭ�����
bool clause_legal(string s, Clause& ret);//�ж�ĳ�ַ����Ƿ��ǺϷ����Ӿ�
void read_in();//���������������ν�ʡ��Ӿ伯
bool operator==(const identifier& i1,const identifier& i2);//���������
bool operator<(const identifier& i1, const identifier& i2);//���������
bool operator>(const identifier& i1, const identifier& i2);//���������
bool operator==(const Atom& a1, const Atom& a2);//���������
bool operator<(const Atom& a1, const Atom& a2);//���������
bool operator>(const Atom& a1, const Atom& a2);//���������
bool comp_atom(const Atom& a1,const Atom& a2);//��������ԭ�����ıȽ�����
bool operator==(const Clause& c1, const Clause& c2);//���������
ostream& operator<<(ostream& out, const Atom& a);//���������
ostream& operator<<(ostream& out, const Clause& c);//���������
bool judge_target(Clause clause);//�ж�ĳ�Ӿ��Ƿ�Ϊ��Ҫ�Ľ��
bool guijie_clause(int father_id, int mother_id);//�������Ӿ���й��
bool guijie();//���Ӿ伯�е��Ӿ���й�ᣨ���ɹ�����1����ʧ�ܷ���0��
void trace_back();//��clause_set�е��Ӿ���л���

struct Comp_Clause//�Զ���map��key����
{
	bool operator()(const Clause& c1, const Clause& c2) const
	{
		if (c1.atom_set.size() < c2.atom_set.size())
			return true;
		else if (c1.atom_set.size() > c2.atom_set.size())
			return false;
		else
		{
			for (int i = 0; i < c1.atom_set.size(); i++)
			{
				if (c1.atom_set[i] < c2.atom_set[i])
					return true;
				if (c1.atom_set[i] > c2.atom_set[i])
					return false;
			}
			return false;
		}
	}
};

extern map<Clause, int, Comp_Clause> clause_set;//�������Ӿ�ļ���