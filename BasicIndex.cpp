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

//ָ��λ�ö�ȡ
PostingList* BasicIndex::readPosting(ifstream& in,streampos pos) {
	PostingList * posting = NULL;
	in.seekg(pos);//����ͨ��������λ
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
//������
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
//дһ��
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
//д����
void BasicIndex::writePostings(ofstream & out, map<int, PostingList*> &postinglists, map<int, streampos> &postingDict)
{
	out << postinglists.size() << endl;
	for (auto postinglist : postinglists) {

		//��¼λ�ã�����readPosting
		streampos streampos = out.tellp();
		postingDict[postinglist.second->getTermId()] = streampos;

		writePosting(out, postinglist.second);
	}
		/*postingDict[postinglist.second->getTermId()] = pos;
		writePosting(out, postinglist.second);
		pos++;*/
}
