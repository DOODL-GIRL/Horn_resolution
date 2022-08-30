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

extern vector<string> constants;//记录所有合法常量的集合
extern vector<string> variables;//记录所有合法变量的集合
extern vector<string> predicates;//记录所有合法谓词的集合

struct identifier//将常量和变量作为标识符统一处理
{
	bool category;//CONSTANT or VVARIABLE
	int id;//常量或变量对应集合的下标
};

struct Atom//原子式
{
	bool positive ;//带否定符号则为0；不带否定符号则为1
	int predicate_id;//谓词对应索引
	vector<identifier> element;//按顺序列出原子式中的标识符
	bool erase_leagal(const Atom& a, vector<int>&change_list);//判断两个原子语句能否互相消解,若能则change_list表里记录着合一置换对应标识符下标
};

struct Clause//子句
{
	int source_1 = -1;//来源1子句对应id（原始子句为-1）
	int source_2 = -1;//来源2子句对应id（原始子句为-1）
	vector<Atom> atom_set;//按顺序列出子句中的原子式
};

int find_predicate(string s);//在谓词表中查找谓词，若找到返回索引，否则返回-1
int find_variable(string s);//在变量表中查找变量，若找到返回索引，否则返回-1
int find_constant(string s);//在常量表中查找常量，若找到返回索引，否则返回-1
int atom_exist(Atom atom, vector<Atom> atom_set);//在原子语句集合中查找原子语句，若找到返回索引，否则返回-1
int clause_exist(Clause clause);//在子句集中查找子句，若找到返回索引，否则返回-1
bool atom_legal(string s, Atom& ret);//判断某字符串是否是合法的原子语句
bool clause_legal(string s, Clause& ret);//判断某字符串是否是合法的子句
void read_in();//读入变量、常量、谓词、子句集
bool operator==(const identifier& i1,const identifier& i2);//运算符重载
bool operator<(const identifier& i1, const identifier& i2);//运算符重载
bool operator>(const identifier& i1, const identifier& i2);//运算符重载
bool operator==(const Atom& a1, const Atom& a2);//运算符重载
bool operator<(const Atom& a1, const Atom& a2);//运算符重载
bool operator>(const Atom& a1, const Atom& a2);//运算符重载
bool comp_atom(const Atom& a1,const Atom& a2);//定义两个原子语句的比较运算
bool operator==(const Clause& c1, const Clause& c2);//运算符重载
ostream& operator<<(ostream& out, const Atom& a);//运算符重载
ostream& operator<<(ostream& out, const Clause& c);//运算符重载
bool judge_target(Clause clause);//判断某子句是否为想要的结果
bool guijie_clause(int father_id, int mother_id);//对两个子句进行归结
bool guijie();//对子句集中的子句进行归结（若成功返回1；若失败返回0）
void trace_back();//对clause_set中的子句进行回溯

struct Comp_Clause//自定义map的key排序
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

extern map<Clause, int, Comp_Clause> clause_set;//待归结的子句的集合