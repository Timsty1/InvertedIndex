#include "StdAfx.h"
#include "Index.h"
#include <iostream>
#include "postingList.h"
#include "indexCompression.h"
#include "Terms.h"

Index::Index(BaseIndex * index, File& root, File& out) : pIndex(index), rootdir(root),outdir(out)
{
	totalFileCount = 0;
	docIdCounter = 0;
	wordIdCounter = 0;
}

Index::~Index(void)
{
}

void Index::writePosting(ofstream &out, PostingList* posting) {//没用。
	/*
	* TODO: Your code here
	*	 
	*/
	
}

/* BSBI indexing algorithm */
void Index::BSBI() {

	File block = rootdir.firstFileInDir();
	while(block.exists()) {
		/*
		  *     postinglists is used to store postinglist for each term in a block.
		  *    
		  */
		//list<PostingList*> postinglists;   //一个block一个总表
		map<int, PostingList*> blockpostinglists;//termid->postinglist
		File blockFile (outdir, block.getName());
		blockQueue.push_back(blockFile);

		File blockDir(rootdir, block.getName());
		File file = blockDir.firstFileInDir();
		while(file.exists())
		{
			++totalFileCount;
			docDict[file.getPathName()] = ++docIdCounter;

			Terms terms(file);
			for (string term : terms.getTerms()) {
				if (termDict[term] == 0) {//new term
					//termid
					wordIdCounter++;
					termDict[term] = wordIdCounter;

				}
				if (blockpostinglists[termDict[term]] == NULL) {
					list<int> posting;
					posting.push_back(docIdCounter);
					PostingList* post = new PostingList(termDict[term], posting);//生成一个postinglist
					blockpostinglists[termDict[term]] = post;//加入总表中
				}
				else {
					PostingList *post = blockpostinglists[termDict[term]];//已存在则添加docid
					list<int> *_list = post->getList();
					if (_list->back()!=docIdCounter) {
						_list->push_back(docIdCounter);
					}
				}
			}
				/*else {//old term
					bool postinglist_tag = false;
					for (PostingList*t : postinglists) {
						if (t->getTermId == termDict[term]) {//寻找对应的term
							postinglist_tag = true;//该词已存在postinglist则不再添加
							list<int>* _list = t->getList();
							bool docid_tag = false;
							for (int docid : *_list) {
								if (docid == docIdCounter) {
									docid_tag = true;//同一文件内该term多次出现则不添加posting
									break;
								}
							}
							if (!docid_tag) {
								_list->push_back(docIdCounter);
							}
						}
					}
					if (!postinglist_tag) {//非同一个block
						list<int> posting;
						posting.push_back(docIdCounter);
						PostingList * post = new PostingList(termDict[term], posting);
						postinglists.push_back(post);
					}
				}
			}*/

			//get next file
			file = blockDir.nextFileInDir();
		} //end while(file.exists())

		/* Sort and output */
		if (!blockFile.createNewFile()) {
			cerr << "Create new block failure."  << endl;
			return;
		}
		ofstream writer(blockFile.getPathName().c_str());
		pIndex->writePostings(writer, blockpostinglists, postingDict);//一个block一个文件
		writer.close();
		//释放内存
		for (auto p : blockpostinglists) {
			delete p.second;
		}

		//get next block
		block = rootdir.nextFileInDir();
	} //end while(block.exists()) 


	/* Required: output total number of files. */
	cout << "totalFileCount:" << endl;
	cout << totalFileCount << endl;

	/* Merge blocks */
	while (true) {
		if (blockQueue.size() <= 1)
			break;

		File b1 = blockQueue.front();
		blockQueue.pop_front();
		File b2 = blockQueue.front();
		blockQueue.pop_front();
		cout << "111" << endl;
		File combfile(outdir.getPathName(), (string)(b1.getName() + "+" + b2.getName()));

		if (!combfile.createNewFile()) {
			cerr << "Create new block failure."  << endl;
			return;
		}

		ifstream bf1(b1.getPathName().c_str());
		ifstream bf2(b2.getPathName().c_str());
		ofstream mf(combfile.getPathName().c_str());

		
		map<int,PostingList*> postinglists;

		pIndex->readPostings(bf1,postinglists);//从bf1读入数据至postinglists
		pIndex->readPostings(bf2, postinglists);

		pIndex->writePostings(mf, postinglists, postingDict);//输出到文件

		for (auto p : postinglists) {
			delete p.second;
		}

		bf1.close();
		bf2.close();
		mf.close();
		b1.deleteFile();
		b2.deleteFile();
		blockQueue.push_front(combfile);
	}


	/* Dump constructed index back into file system */
	File indexFile = blockQueue.front();
	blockQueue.pop_front();

	File indexDeleteFile(outdir.getPathName() + "/corpus.index");
	if (indexDeleteFile.exists()) {
		indexDeleteFile.deleteFile();
	}
	bool _rename = indexFile.renameTo(outdir.getPathName() + "/corpus.index");
	cout << _rename << endl;

	ofstream termWriter((outdir.getPathName()+ "/term.dict").c_str());
	//TODO: write  term dictionary to disc.
	map<string, int>::iterator it = termDict.begin();
	for (; it != termDict.end(); it++) {
		termWriter << it->second << " " << it->first << endl;
	}
	termWriter.close();


	ofstream docWriter((outdir.getPathName() +  "/doc.dict").c_str());
	//TODO: write  document dictionary to disc.
	it = docDict.begin();
	for (; it != docDict.end(); it++) {
		docWriter << it->second << " " << it->first << endl;
	}
	docWriter.close();


	ofstream postWriter((outdir.getPathName() + "/posting.dict").c_str());
	//TODO: write  postingLists to disc.
	map<int, streampos>::iterator ite = postingDict.begin();
	for (; ite != postingDict.end(); ite++) {
		postWriter << ite->first << " " << ite->second << endl;
	}
	postWriter.close();
	cout << "index has been completed!" << endl;
}
