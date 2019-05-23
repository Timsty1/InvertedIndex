#include "stdafx.h"
#include "Terms.h"
using namespace std;
/*strtok()用来将字符串分割成一个个片段。
参数s指向欲分割的字符串，参数delim则为分割字符串中包含的所有字符。
当strtok()在参数s的字符串中发现参数delim中包涵的分割字符时,则会将该字符改为\0 字符。
在第一次调用时，strtok()必需给予参数s字符串，往后的调用则将参数s设置成NULL。
每次调用成功则返回指向被分割出片段的指针。*/

Terms::Terms(File file)
{
	ifstream reader(file.getPathName().c_str());
	while (!reader.eof()) {
		string line;
		getline(reader, line);
		int len = line.length() + 1;
		char* str = new char[len+10]();
		
		memcpy(str, line.c_str() , len);
		//首先对文本进行规范，去掉不必要元素
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
	//首先对文本进行规范，去掉不必要元素
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
