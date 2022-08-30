#include"horn.h"

int find_predicate(string s)//��ν�ʱ��в���ν�ʣ����ҵ��������������򷵻�-1
{
	if (s.empty())
		return -1;
	vector<string>::iterator result = find(predicates.begin(), predicates.end(), s);
	if (result == predicates.end())
		return -1;
	else
		return int(result - predicates.begin());
}

int find_variable(string s)//�ڱ������в��ұ��������ҵ��������������򷵻�-1
{
	if (s.empty())
		return -1;
	vector<string>::iterator result = find(variables.begin(), variables.end(), s);
	if (result == variables.end())
		return -1;
	else
		return int(result - variables.begin());
}

int find_constant(string s)//�ڳ������в��ҳ��������ҵ��������������򷵻�-1
{
	if (s.empty())
		return -1;
	vector<string>::iterator result = find(constants.begin(), constants.end(), s);
	if (result == constants.end())
		return -1;
	else
		return int(result - constants.begin());
}

int atom_exist(Atom atom,vector<Atom> atom_set)//��ԭ����伯���в���ԭ����䣬���ҵ��������������򷵻�-1
{
	if (atom_set.empty())
		return -1;
	vector<Atom>::iterator result = find(atom_set.begin(), atom_set.end(), atom);
	if (result == atom_set.end())
		return -1;
	else
		return int(result - atom_set.begin());
}

int clause_exist(Clause clause)//���Ӿ伯�в����Ӿ䣬���ҵ��������������򷵻�-1
{
	if (clause_set.empty())
		return -1;
	for (std::map<Clause,int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
	{
		if (it->first == clause)
		    return it->second;
	}
	return -1;
}

bool atom_legal(string s, Atom& ret)//�ж�һ���ַ����Ƿ�����Ϊ�Ϸ�ԭ����䣨���Ϸ���ת��Ϊԭ�����洢��ret�У�
{
	Atom temp;
	int index = 0;

	//����񶨴�
	if (s[index] == '!')
	{
		temp.positive = 0;
		index++;
	}
	else
		temp.positive = 1;

	//����ν��
	string temp_predicate;
	while (index < s.size())
	{
		char cur = s[index];
		if (cur == '(')
		{
			index++;
			break;
		}
		else
		{
			temp_predicate += cur;
			index++;
		}
	}
	int result = find_predicate(temp_predicate);
	if (result == -1)
		return false;
	else
		temp.predicate_id = result;

	//���볣���ͱ�����ʶ��
	string temp_identifier;
	while (index < s.size())
	{
		char cur = s[index];

		if (cur == ',' || cur == ')')
		{
			int result = find_variable(temp_identifier);
			if (result != -1)
			{
				identifier ret;
				ret.category = VARIABLE;
				ret.id = result;
				temp.element.push_back(ret);
			}
			else
			{
				result = find_constant(temp_identifier);
				if (result != -1)
				{
					identifier ret;
					ret.category = CONSTANT;
					ret.id = result;
					temp.element.push_back(ret);
				}
				else
					return false;
			}
			if (cur == ')')
			{
				if (temp.element.empty())//ν�ʲ��������κγ��������
					return false;
				else if (index != s.size() - 1)
					return false;
				else
				{
					ret = temp;
					return true;
				}
			}
			if (cur == ',')
			{
				index++;
				temp_identifier.clear();
			}
		}
		else
		{
			temp_identifier += cur;
			index++;
		}
	}
	return false;
}

bool clause_legal(string s, Clause& ret)//�ж�һ���ַ����Ƿ�����Ϊ�Ϸ��Ӿ䣨���Ϸ���ת��Ϊ�Ӿ�洢��ret�У�
{
	Clause temp;
	string temp_atom;
	int index = 0;
	while (index < s.size())
	{
		char cur = s[index];
		if (cur == '|')
		{
			Atom atom;
			if (!atom_legal(temp_atom, atom))
				return false;
			else
			{
				temp.atom_set.push_back(atom);
				index++;
				temp_atom.clear();
			}
		}
		else
		{
			temp_atom += cur;
			index++;
		}
	}
	Atom atom;
	if (!atom_legal(temp_atom, atom))
		return false;
	else
	{
		temp.atom_set.push_back(atom);
		sort(temp.atom_set.begin(), temp.atom_set.end(), comp_atom);
		ret = temp;
		return true;
	}
}

ostream& operator<<(ostream& out, const Atom& a)
{
	if (!a.positive)
		out << "!";
	out << predicates[a.predicate_id];
	out << "(";
	for (int i = 0; i < a.element.size(); i++)
	{
		if (a.element[i].category == VARIABLE)
			out << variables[a.element[i].id];
		else
			out << constants[a.element[i].id];
		if (i != a.element.size() - 1)
			out << ",";
	}
	out << ")";
	return out;
}

ostream& operator<<(ostream& out, const Clause& c)
{
	for (int i = 0; i < c.atom_set.size(); i++)
	{
		out << c.atom_set[i];
		if (i != c.atom_set.size() - 1)
			out << "|";
	}
	return out;
}

bool operator==(const identifier& i1, const identifier& i2)
{
	if (i1.category == i2.category && i1.id == i2.id)
		return true;
	else
		return false;
}

bool operator<(const identifier& i1, const identifier& i2)
{
	if (i1.category < i2.category)
		return true;
	if (i1.category > i2.category)
		return false;
	else
	{
		if (i1.id < i2.id)
			return true;
		else
			return false;
	}
}

bool operator>(const identifier& i1, const identifier& i2)
{
	if (i1.category > i2.category)
		return true;
	if (i1.category < i2.category)
		return false;
	else
	{
		if (i1.id > i2.id)
			return true;
		else
			return false;
	}
}

bool operator==(const Atom& a1, const Atom& a2)
{
	if (a1.positive != a2.positive)
		return false;
	if (a1.predicate_id != a2.predicate_id)
		return false;
	if (a1.element.size() != a2.element.size())
		return false;
	for (int i = 0; i < a1.element.size(); i++)
	{
		if (!(a1.element[i] == a2.element[i]))
			return false;
	}
	return true;
}

bool operator<(const Atom& a1, const Atom& a2)
{
	if (a1.predicate_id < a2.predicate_id)
		return true;
	if (a1.predicate_id > a2.predicate_id)
		return false;
	else
	{
		if (a1.positive < a2.positive)
			return true;
		if (a1.positive > a2.positive)
			return false;
		else
		{
			if (a1.element.size() < a2.element.size())
				return true;
			if (a1.element.size() > a2.element.size())
				return false;
			for (int i = 0; i < a1.element.size(); i++)
			{
				if (a1.element[i] < a2.element[i])
					return true;
				if (a1.element[i] > a2.element[i])
					return false;
			}
			return false;
		}
	}
}

bool operator>(const Atom& a1, const Atom& a2)
{
	if (a1 == a2)
		return false;
	else if (a1 < a2)
		return false;
	else
		return true;
}

bool comp_atom(const Atom& a1,const Atom& a2)
{
	if (a1.predicate_id < a2.predicate_id)
		return true;
	if (a1.predicate_id > a2.predicate_id)
		return false;
	else
	{
		if (a1.positive < a2.positive)
			return true;
		if (a1.positive > a2.positive)
			return false;
		else
		{
			if (a1.element.size() < a2.element.size())
				return true;
			if (a1.element.size() > a2.element.size())
				return false;
			for (int i = 0; i < a1.element.size(); i++)
			{
				if (a1.element[i] < a2.element[i])
					return true;
				if (a1.element[i] > a2.element[i])
					return false;
			}
			return false;
		}
	}
}

bool operator==(const Clause& c1, const Clause& c2)
{
	if (c1.atom_set.size() != c2.atom_set.size())
		return false;
	for (int i = 0; i < c1.atom_set.size(); i++)
	{
		if (!(c1.atom_set[i] == c2.atom_set[i]))
			return false;
	}
	return true;
}

void read_in()//���������������ν�ʡ��Ӿ伯
{
	cout << "���������б�����ʶ���ļ��ϣ���#������uΪ�����֣��������룩��" << endl;
	variables.push_back("u");
	while (1)
	{
		string temp;
		cin >> temp;
		if (temp == "#")
			break;
		else
			variables.push_back(temp);
	}
	cout << "���������г�����ʶ���ļ��ϣ���#������" << endl;
	while (1)
	{
		string temp;
		cin >> temp;
		if (temp == "#")
			break;
		else
			constants.push_back(temp);
	}
	cout << "����������ν�ʵļ��ϣ���#��������ANSWERΪ�����֣��������룩" << endl;
	predicates.push_back("ANSWER");
	while (1)
	{
		string temp;
		cin >> temp;
		if (temp == "#")
			break;
		else
			predicates.push_back(temp);
	}
	int n;
	cout << "�������Ӿ伯���Ӿ�������";
	cin >> n;
	cout << "����������" << n << "���Ӿ䣺" << endl;
	cout << "�����á�!����ʾ�����á�|����ʾ��" << endl;
	for (int i = 0; i < n; i++)
	{
		string s;
		Clause clause;
		cin >> s;
		if (!clause_legal(s, clause))
		{
			cout << "���ǺϷ��Ӿ䣬���������룡" << endl;
			i--;
			continue;
		}
		else
			clause_set.emplace(make_pair(clause,clause_set.size()));
	}
	cout << "������˴ι���Ŀ���Ӿ䣨���������u��ʾ��" << endl;
	while(1)
	{
		string s;
		Clause clause;
		cin >> s;
		if (!clause_legal(s, clause))
		{
			cout << "���ǺϷ��Ӿ䣬���������룡" << endl;
			continue;
		}
		else
		{
			//��Ŀ���Ӿ�ȡ������Answer(u)��ȡ�������Ӿ伯
			Clause new_clause;
			new_clause.source_1 = -1;
			new_clause.source_2 = -1;
			for (int i = 0; i < clause.atom_set.size(); i++)
			{
				Atom temp = clause.atom_set[i];
				temp.positive = clause.atom_set[i].positive ? 0 : 1;
				new_clause.atom_set.push_back(temp);
			}
			Atom answer;
			answer.positive = 1;
			answer.predicate_id = 0;
			identifier u;
			u.category = VARIABLE;
			u.id = 0;
			answer.element.push_back(u);
			new_clause.atom_set.push_back(answer);
			clause_set.emplace(make_pair(new_clause, clause_set.size()));
			break;
		}
	}
}

bool Atom::erase_leagal(const Atom& a, vector<int>& change_list)//�ж�����ԭ������ܷ�������
{
	if (a.positive == false && positive == false || a.positive == true && positive == true)
		return false;
	if (a.predicate_id != predicate_id)
		return false;
	if (a.element.size() != element.size())
		return false;
	for (int i = 0; i < a.element.size(); i++)
	{
		if (element[i] == a.element[i])
			continue;
		else
		{
			if (a.element[i].category != element[i].category)
			{
				change_list.push_back(i);
				continue;
			}
			else
				return false;
		}
	}
	return true;
}

bool judge_target(Clause clause)//�ж�ĳ�Ӿ��Ƿ�Ϊ��Ҫ�Ľ��
{
	if (clause.atom_set.size() != 1)
		return false;
	Atom atom = clause.atom_set[0];
	if (atom.positive == 0)
		return false;
	if (atom.predicate_id != 0)
		return false;
	if (atom.element.size() != 1)
		return false;
	if (atom.element[0].category != CONSTANT)
		return false;
	return true;
}

bool guijie_clause(int father_id, int mother_id)//��������Ӿ䲢�����Ӿ�����Ӿ伯��
{
	Clause father;
	Clause mother;
	for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
	{
		if (it->second == father_id)
			father = it->first;
		if (it->second == mother_id)
			mother = it->first;
	}
	for (int i = 0; i < father.atom_set.size(); i++)
	{
		Atom atom1 = father.atom_set[i];
		for (int j = 0; j < mother.atom_set.size(); j++)
		{
			Atom atom2 = mother.atom_set[j];
			vector<int> change_list;
			if (!atom1.erase_leagal(atom2, change_list))
				continue;
			else
			{
				Clause new_clause;//�������Ӿ���������Ӿ�
				new_clause.source_1 = father_id;
				new_clause.source_2 = mother_id;
				for (int m = 0; m < father.atom_set.size(); m++)
				{
					if (m != i && atom_exist(father.atom_set[m], new_clause.atom_set) == -1)
						new_clause.atom_set.push_back(father.atom_set[m]);
				}
				for (int m = 0; m < mother.atom_set.size(); m++)
				{
					if (m != j && atom_exist(mother.atom_set[m], new_clause.atom_set) == -1)
						new_clause.atom_set.push_back(mother.atom_set[m]);
				}
				//if (new_clause.atom_set.empty())//�������Ӿ䣬ֱ�ӷ���true
				if(judge_target(new_clause))
				{
					clause_set.insert(make_pair(new_clause, clause_set.size()));//�����Ӿ���뼯����
					if (father_id == -1)
						cout << "father:   " << -1 << endl;
					else
						cout << "father:   " << father << endl;
					if (mother_id == -1)
						cout << "mother:   " << -1 << endl;
					else
						cout << "mother:   " << mother << endl;
					//cout << "(" << clause_set.size()-1 << ") " << "NULL" << "......(" << father_id << ")(" << mother_id << ") " << endl;
					cout << "(" << clause_set.size() - 1 << ") " << new_clause << "......(" << father_id << ")(" << mother_id << ") " << endl;
					return true;
				}
				//�����û���Թ��������Ӿ���к�һ�û�
				for (int k = 0; k < change_list.size(); k++)
				{
					int change_variable;
					int change_constant;
					if (atom1.element[change_list[k]].category == VARIABLE)
					{
						change_variable = atom1.element[change_list[k]].id;
						change_constant = atom2.element[change_list[k]].id;
					}
					else
					{
						change_variable = atom2.element[change_list[k]].id;
						change_constant = atom1.element[change_list[k]].id;
					}
					for (int p = 0; p < new_clause.atom_set.size(); p++)
					{
						Atom temp = new_clause.atom_set[p];
						for (int q = 0; q < temp.element.size(); q++)
						{
							if (temp.element[q].category == VARIABLE && temp.element[q].id == change_variable)
							{
								new_clause.atom_set[p].element[q].category = CONSTANT;
								new_clause.atom_set[p].element[q].id = change_constant;
							}
						}
					}
				}
				sort(new_clause.atom_set.begin(), new_clause.atom_set.end(), comp_atom);
				if (clause_exist(new_clause) == -1)
				{
					clause_set.insert(make_pair(new_clause, clause_set.size()));//���¹������Ӿ���뼯����
					if (father_id == -1)
						cout << "father:   " << -1 << endl;
					else
						cout << "father:   " << father << endl;
					if (mother_id == -1)
						cout << "mother:   " << -1 << endl;
					else
						cout << "mother:   " << mother << endl;
					cout << "(" << clause_set.size() - 1 << ") " << new_clause << "......(" << father_id << ")(" << mother_id << ") " << endl;
					cout << endl;
				}
			}
		}
	}
	return false;//δ����Ŀ���Ӿ�
}

bool guijie()//���Ӿ伯�е��Ӿ���й�ᣨ���ɹ�����1����ʧ�ܷ���0��
{
	int pre_size = 0;
	while (1)
	{
		vector<pair<Clause, int>> clause_set_copy(clause_set.begin(), clause_set.end());
		for (int i = 0; i < clause_set_copy.size(); i++)
		{
			for (int j = i + 1; j < clause_set_copy.size(); j++)
			{
				bool result = guijie_clause(clause_set_copy[i].second, clause_set_copy[j].second);
				if (result == 1)
					return true;
			}
		}
		if (clause_set_copy.size() == clause_set.size())
			break;
	}
	return false;
}

void trace_back()
{
	vector<int>path_num;

	path_num.push_back(clause_set.size() - 1);

	for (int i = 0; i < path_num.size(); i++)
	{
		Clause clause;// = clause_set[path_num[i]];
		for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
		{
			if (it->second == path_num[i])
				clause = it->first;
		}

		if (clause.source_1 != -1)
		{
			vector<int>::iterator result = find(path_num.begin(), path_num.end(), clause.source_1);
			if (result == path_num.end())
				path_num.push_back(clause.source_1);
		}
		if (clause.source_2 != -1)
		{
			vector<int>::iterator result = find(path_num.begin(), path_num.end(), clause.source_2);
			if (result == path_num.end())
				path_num.push_back(clause.source_2);
		}

	}
	for (int i = path_num.size() - 1; i >= 0; i--)
	{
		Clause clause;
		for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
		{
			if (it->second == path_num[i])
				clause = it->first;
		}
		//if (path_num[i] == clause_set.size() - 1)
			//cout << "NULL" << " ";
		//else
		cout << "father_id��(" << clause.source_1 << ")  mother_id��(" << clause.source_2 << ")" << endl;
		cout << "(" << path_num[i] << ") ";
		cout << clause << endl;
		//cout << clause.source_1 << " " << clause.source_2 << endl;
	}


	ofstream out("tree.dot", ios::out | ios::binary);
	if (out.is_open() == NULL)
	{
		cout << "�����������ļ�ʧ�ܣ�" << endl;
		exit(EXIT_FAILURE);
	}
	out << "digraph {" << endl;
	out << "node[shape = box]" << endl;
	for (int i = path_num.size() - 1; i >= 0; i--)
	{
		out << path_num[i];
		out << "[label=\"";
		Clause clause;
		for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
		{
			if (it->second == path_num[i])
				clause = it->first;
		}
		//if (path_num[i] == clause_set.size() - 1)
			//out << "NULL";
		//else
			out << clause;
		out << "\"];" << endl;
	}
	for (int i = 0; i < path_num.size(); i++)
	{
		Clause clause;
		for (std::map<Clause, int>::iterator it = clause_set.begin(); it != clause_set.end(); it++)
		{
			if (it->second == path_num[i])
				clause = it->first;
		}
		if (clause.source_1 != -1)
			out << clause.source_1 << "->" << path_num[i] << ";" << endl;
		if (clause.source_2 != -1)
			out << clause.source_2 << "->" << path_num[i] << ";" << endl;
	}
	out << "}" << endl;
	out.close();
	return;
}