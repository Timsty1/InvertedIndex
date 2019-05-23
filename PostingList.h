#pragma once
#include <list>
using namespace std;
//一个term对应一个PostingList
//内容包括termId和docIds'list
//freq等于list.size()
class PostingList
{
private:
	 int termId;
	/* A list of docIDs (i.e. postings) */
	list<int> postings;

public:
	PostingList(int termId, list<int> &list);
	PostingList(int termId) ;
	~PostingList(void);
	
	int getTermId() ;
	list<int> * getList() ;
};
