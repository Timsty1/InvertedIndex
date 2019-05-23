#include "StdAfx.h"
#include "BasicIndex.h"
using namespace std;

BasicIndex::BasicIndex(void)
{
	type = 1;
}

BasicIndex::~BasicIndex(void)
{
}

//指定位置读取
PostingList* BasicIndex::readPosting(ifstream& in,streampos pos) {
	PostingList * posting = NULL;
	in.seekg(pos);//尝试通过流来定位
	int termid;
	list<int> docids;
	in >> termid;
	int freq;
	in >> freq;
	for (int i = 0; i < freq; i++) {
		int docid;
		in >> docid;
		docids.push_back(docid);
	}
	posting = new PostingList(termid, docids);
	return posting;
}
//读所有
void  BasicIndex::readPostings(ifstream & in, map<int, PostingList*> &postinglists)
{
	int size;
	in >> size;
	int termid, n;
	for (int i = 0; i < size;i++){
		in >> termid >> n;
		if (postinglists[termid] == NULL) {
			list<int> docids;
			int docid = 0;
			for (int i = 0; i < n; i++) {
				in >> docid;
				docids.push_back(docid);
			}
			PostingList* postinglist = new PostingList(termid, docids);
			postinglists[termid] = postinglist;
		}
		else {
			PostingList* postinglist = postinglists[termid];
			list<int> * docids = postinglist->getList();
			int docid = 0;
			for (int i = 0; i < n; i++) {
				in >> docid;
				docids->push_back(docid);
			}
		}
	}
}
//写一个
void BasicIndex::writePosting(ofstream& out, PostingList * p) {
	out << p->getTermId() << ' ';
	out << p->getList()->size() << endl;
	bool tool = true;
	for (int docid : *p->getList()) {
		if (tool) {
			out << docid;
			tool = false;
		}
		else {
			out << ' ' << docid;
		}
	}
	out << endl;
}
//写所有
void BasicIndex::writePostings(ofstream & out, map<int, PostingList*> &postinglists, map<int, streampos> &postingDict)
{
	out << postinglists.size() << endl;
	for (auto postinglist : postinglists) {

		//记录位置，便于readPosting
		streampos streampos = out.tellp();
		postingDict[postinglist.second->getTermId()] = streampos;

		writePosting(out, postinglist.second);
	}
		/*postingDict[postinglist.second->getTermId()] = pos;
		writePosting(out, postinglist.second);
		pos++;*/
}
