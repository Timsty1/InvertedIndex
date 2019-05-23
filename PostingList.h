#pragma once
#include <list>
using namespace std;
//һ��term��Ӧһ��PostingList
//���ݰ���termId��docIds'list
//freq����list.size()
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
