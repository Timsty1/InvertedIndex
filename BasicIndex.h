#pragma once
#include "baseIndex.h"
#include <iostream>

class BasicIndex : public BaseIndex
{
public:
	BasicIndex(void);
	virtual ~BasicIndex(void);

	PostingList* readPosting(ifstream& in,streampos pos);
	void readPostings(ifstream& in, map<int, PostingList*> &postinglists);

	void writePosting(ofstream& out, PostingList *p);
	void writePostings(ofstream &out, map<int, PostingList*> &postinglists, map<int, streampos> &postingDict);
};
