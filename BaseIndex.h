#pragma once
#include "postingList.h"

class BaseIndex
{
protected:
	//long pos;
public:
	int type;
	BaseIndex(void);
	virtual ~BaseIndex(void);

	virtual PostingList* readPosting(ifstream& in, streampos pos) = 0;
	virtual void readPostings(ifstream& in, map<int, PostingList*> &postinglists) = 0;
	virtual void writePosting(ofstream& out, PostingList *p) = 0;

	virtual void writePostings(ofstream &out, map<int,PostingList*> &postinglists, map<int, streampos> &postingDict) = 0;
};