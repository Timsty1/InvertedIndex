#include "StdAfx.h"
#include "VBIndex.h"

//Variables byte codes 流程网址
//https://nlp.stanford.edu/IR-book/html/htmledition/variable-byte-codes-1.html
/*相较于basicindex，VBindex多了encode、decode过程 和 加过程*/

int VBIndex::readInt(ifstream &in)//decode位级编码
{
	byte r = 0;

	int ret = 0;
	bool flag = true;
	int zero = 0;
	int p = 1;
	do {
		in.read((char *)&r, sizeof(r));
		if (r >= 128) {//一个字节的第一位是编码结束标识符。1：结束。
			flag = false;
			r -= 128;
		}
		ret += r * p;
		p *= 128;
	} while (flag);

	while (zero--) ret *= 128;
	return ret;
}

void VBIndex::writeInt(int n, ofstream & out)//encode
{
	byte r;
	while (n >= 128) {
		r = n % 128;
		cout << "write r:" << (int)r << endl;
		out.write((const char*)&r, sizeof(r));
		n /= 128;
	}

	r = (n % 128) + 128;

	cout << "write r:" << (int)r << endl; 
	out.write((const char*)&r, sizeof(r));

}

VBIndex::VBIndex(void)
{
	type = 2;
}

VBIndex::~VBIndex(void)
{

}


PostingList* VBIndex::readPosting(ifstream& in, streampos pos) {
	PostingList* posting = NULL;
	in.seekg(pos);
	list<int> docids;
	int termid = readInt(in);
	int freq = readInt(in);
	int docid = 0;
	for (int i = 0; i < freq; i++) {
		int tool = readInt(in);
		docid += tool;
		docids.push_back(docid);
	}
	posting = new PostingList(termid, docids);
	return posting;
}

void VBIndex::readPostings(ifstream & in, map<int, PostingList*>& postinglists)
{
	int size;
	size = readInt(in);
	int termid, n;
	for (int i = 0; i < size; i++) {
		termid = readInt(in);
		n = readInt(in);

		cout << termid << ' ' << n << endl;

		if (postinglists[termid] == NULL) {
			list<int> docids;
			int docid = 0;
			int tool;
			for (int i = 0; i < n; i++) {
				tool = readInt(in);
				docid += tool;
				docids.push_back(docid);
			}
			PostingList *postinglist = new PostingList(termid, docids);
			postinglists[termid] = postinglist;
		}
		else {
			PostingList * postinglist = postinglists[termid];
			list<int>* docids = postinglist->getList();
			int docid = 0;
			int tool;
			for (int i = 0; i < n; i++) {
				tool = readInt(in);
				docid += tool;
				docids->push_back(docid);
			}
		}
	}

}

void VBIndex::writePosting(ofstream& out, PostingList *p) {
	writeInt(p->getTermId(), out);
	writeInt(p->getList()->size(), out);
	bool flag1 = true;
	int pre_docid;
	for (int docid : *p->getList()) {
		if (flag1) {//第一个输出docid
			pre_docid = docid;
			writeInt(docid, out);
			flag1 = false;
		}
		else {//非第一则输出该docid与前一docid的差值
			writeInt(docid - pre_docid, out);
			pre_docid = docid;
		}
	}
}
//没变化
void VBIndex::writePostings(ofstream & out, map<int, PostingList*>& postinglists, map<int, streampos> &postingDict)
{
	writeInt(postinglists.size(), out);
	for (auto postinglist : postinglists) {
		streampos _streampos = out.tellp();
		postingDict[postinglist.second->getTermId()] = _streampos;
		writePosting(out, postinglist.second);
	}
}
