#include "StdAfx.h"
#include "Query.h"
#include <iostream>
#include <iterator>
#include "Terms.h"

Query::Query(BaseIndex * index, File& input) : pIndex(index), inputDir(input)
{
}

Query::~Query(void)
{
}
bool cmp(PostingList* const a, PostingList* const b)
{
	return a->getList()->size() < b->getList()->size();
}

/* 
 * Write a posting list with a given termID from the file 
 * You should seek to the file position of this specific
 * posting list and read it back.
 * */
PostingList * Query::readPosting(ifstream& in, int termId)//don't need.
{
	PostingList * posting = NULL;
	/*
	 * TODO: Your code here
	 */

	return posting;
}


/*
 * Intersect two listings to one listing
 */
list<int> * Query::intersect(list<int>& p1, list<int>& p2)
{
	list<int> * p = new list<int>();
	/*
	 * TODO: Your code here
	 */
	p1.sort(); p2.sort();
	list<int>::iterator ite1 = p1.begin();
	list<int>::iterator ite2 = p2.begin();
	while (ite1 != p1.end() && ite2 != p2.end()) {
		if (*ite1 == *ite2) {
			p->push_back(*ite1);
			ite1++; ite2++;
			continue;
		}
		else if (*ite1 < *ite2) {
			ite1++;continue;
		}
		else {
			ite2++;continue;
		}
	}
	return p;
}


void Query::execute() 
{
	/* Index file */
	//这里需要判断索引类型为basic还是vb
	ifstream indexFile = ifstream(inputDir.getPathName() + "/corpus.index", pIndex->type == 1 ? ios::in : ios::binary);

	/* Term dictionary */
	ifstream termReader((inputDir.getPathName() + "/term.dict").c_str());
	int termid;
	string term;
	while (termReader >> termid >> term) {
		termDict[term] = termid;
	}
	termReader.close();

	/* Doc dictionary */
	ifstream docReader((inputDir.getPathName() + "/doc.dict").c_str());
	int docid;
	string docname;
	while (docReader >> docid >> docname) {
		docDict[docid] = docname;
	}
	docReader.close();

	/* Posting dictionary */
	ifstream postReader((inputDir.getPathName() + "/posting.dict").c_str());
	int _termid;
	long pos;//streampos 也是long 类型
	while (postReader >> _termid >> pos) {
		posDict[_termid] = pos;
	}
	postReader.close();

	/* Processing queries */
	string line;

	cout << "输入为空时结束程序" << endl;
	cout << "请输入关键字" << endl;

	getline(cin, line);

	while (line != "") {
		/*
		 * TODO: Your code here
		 *       Perform query processing with the inverted index.
		 *       Make sure to print to stdout the list of documents
		 *       containing the query terms, one document file on each
		 *       line, sorted in lexicographical order.
		 */
		Terms terms(line);
		vector<PostingList*> postinglists;
		postinglists.clear();

		for (string term : terms.getTerms()) {
			int termid = termDict[term];
			if (termid != 0) {
				long pos = posDict[termid];
				postinglists.push_back(pIndex->readPosting(indexFile, pos));
			}
		}
		list<int> *result=nullptr;
		bool flag1 = false;
		if (postinglists.size() == 1) {
			result = postinglists[0]->getList();
			flag1 = true;
		}
		else if(postinglists.size()>=2){
			sort(postinglists.begin(), postinglists.end(), cmp);//排序
			vector<PostingList*>::iterator it;
			for (it = postinglists.begin(); it != postinglists.end(); it++) {
				if (result == nullptr) {
					result = intersect(*((*it)->getList()), *((*(it+1))->getList()));
					it++;
				}
				else {
					list<int> *p = result;
					result = intersect(*result, *((*it)->getList()));
					delete p;
				}
			}
		}
		if (result == nullptr) {
			cout << "no results found！" << endl;
		}
		else {
			cout << "在以下文件中出现过您要搜索的关键字" << endl;
			for (auto t : *result) {
				cout << docDict[t] << endl;
			}
		}
		//防止内存泄漏
		if (!flag1) {
			delete result;
		}
		for (auto postinglist : postinglists) {
			delete postinglist;
		}

		cout << "请输入关键字" << endl;
		getline(cin, line);
	}
		
	indexFile.close();
}



