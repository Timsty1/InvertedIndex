#include "stdafx.h"
#include "Terms.h"
using namespace std;
/*strtok()�������ַ����ָ��һ����Ƭ�Ρ�
����sָ�����ָ���ַ���������delim��Ϊ�ָ��ַ����а����������ַ���
��strtok()�ڲ���s���ַ����з��ֲ���delim�а����ķָ��ַ�ʱ,��Ὣ���ַ���Ϊ\0 �ַ���
�ڵ�һ�ε���ʱ��strtok()����������s�ַ���������ĵ����򽫲���s���ó�NULL��
ÿ�ε��óɹ��򷵻�ָ�򱻷ָ��Ƭ�ε�ָ�롣*/

Terms::Terms(File file)
{
	ifstream reader(file.getPathName().c_str());
	while (!reader.eof()) {
		string line;
		getline(reader, line);
		int len = line.length() + 1;
		char* str = new char[len+10]();
		
		memcpy(str, line.c_str() , len);
		//���ȶ��ı����й淶��ȥ������ҪԪ��
		for (int i = 0; i < len; i++) {
			if (str[i] >= 'a'&&str[i] <= 'z')continue;
			else if (str[i] >= 'A'&&str[i] <= 'Z')continue;
			else if (str[i] == '-' || str[i] == '\'')continue;
			else str[i] = ' ';
		}
		char* p = strtok(str, " ");
		while (p!=NULL) {
			string term = p;
			terms.push_back(term);
			p = strtok(NULL, " ");
		}
		delete[] str;
	}
}
Terms::Terms(string line) {
	int len = line.length() + 1;
	char* str = new char[len+10]();
	memcpy(str, line.c_str(), len);
	//���ȶ��ı����й淶��ȥ������ҪԪ��
	for (int i = 0; i < len; i++) {
		if (str[i] >= 'a'&&str[i] <= 'z')continue;
		else if (str[i] >= 'A'&&str[i] <= 'Z')continue;
		else if (str[i] == '-' || str[i] == '\'')continue;
		else str[i] = ' ';
	}
	char* p = strtok(str, " ");
	while (p != NULL) {
		string term = p;
		terms.push_back(term);
		p = strtok(NULL, " ");
	}
	delete[] str;
}
Terms::~Terms(){}

vector<string> Terms::getTerms()
{
	return terms;
}
