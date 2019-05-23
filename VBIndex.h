#pragma once
#include "baseIndex.h"
typedef unsigned char byte;
class VBIndex : public BaseIndex
{
private:
	int readInt(ifstream &in);
	void writeInt(int n, ofstream &out);
public:
	VBIndex(void);
	~VBIndex(void);

	PostingList* readPosting(ifstream& in, streampos pos);
	void readPostings(ifstream& in, map<int, PostingList*> &postinglists);

	void writePosting(ofstream& out, PostingList *p);
	void writePostings(ofstream &out, map<int, PostingList*> &postinglists, map<int, streampos> &postingDict);
};