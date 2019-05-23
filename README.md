# Inverter Index

---

## 概述
在Web搜索技术中，为了能够快速的查找搜索字符串所在的网页，需要对网络爬虫抓取的网页建立索引。在建立索引的过程中，需要对所有出现的单词建立索引，用来记录每个单词出现在那些文件之中。利用该索引，就可以快速找到包含搜索关键词的所有文件。在本项目中，我会使用倒排索引（inverted index）和BSBI技术建立索引。并利用变长字节编码VB技术对索引文件进行压缩。
## 倒排索引
倒排索引（Inverted index），也常被称为反向索引，是一种索引方法，被用来存储在全文搜索下某个单词在一个文档或者一组文档中的存储位置的映射。它是文档检索系统中最常用的数据结构。通过倒排索引，可以根据单词快速检索到包含该单词的文档列表。
### 样例格式
Term:Frequency:DocId1:DocId2:…:DocIdn

比如：hello:6:1:2:3:5:6:8.  Term“hello”这个词出现在了6个文件里。分别是1,2,3,5,6,8号文件。

Frequency表示Term出现的文件数。实际存储时，我们不是存储的Term，而是存储的TermID（int型的整数），即为： 2:6:1:2:3:5:6:8（假设hello的Id为2），当然也不需要存储分隔符“：”。

上述结构存储在成为PostingList的结构中。每一个Term对应一个PostingList。

## BSBI（blocked sort-based indexing）算法

说明：文件的存储结构应当遵循toy_example/data/目录下的文档结构

把每个子目录当做一个Block，建立索引，每个Block的索引分别存储为一个文件。所有Block的索引文件都建立好后，然后两两归并排序，生成最终索引文件。中间结果也保存在文件里。

## 倒排索引中的数据结构
Document Dictionary

所有的文件名存储在DocDict里面，Key：FileName，value: DocID

Term Dictionary

TermDict保存所有文件中出现的词（空格分开），Key：Term，value: TermID。

不需要考虑大小写问题， 所有的term都是小写的。

Posting Dictionary

用来存储每个Term在Index文件中的位置和Doc Frequency。

Key：TermID, value: Pair(position, frequency).

PostingList List

用来存储所有的PostingList

## 索引文件
索引建立完成后，生成4个文件

corpus.index：用来保存所有的PostingList

doc.dict：保存Doc Dictionary

term.dict：保存Term Dictionary

posting.dict：保存每个PostingList在索引文件corpus.index中的位置

## VB(Variable byte codes)压缩
对于每个PostingList，如果不采取压缩技术，我们会采用一个int类型（4字节）来存储所有的Doc ID。
在VB编码中，我们只存储第一个Doc ID，剩下的Doc ID只存储和前面Doc ID的差值。
样例:

| docIDs | 824 | 829 | 215406
| :------| :------: | :------: |:-----:
| gaps |  | 5 | 214577
| VBCODE | 00000110 10111000 | 10000101 |00001101 00001100 10110001

编码规则：每个字节的第一位表示编码是否结束，1：结束；0：未结束。
## 查询
查找包含所有单词的文件，比如hello our world。

首先找到每个词的PostingList(如果在Term Dictionary里没有找到该单词，则直接返回“no results found”)
然后两两Term的DocId求交集，最后的交集结果就是我们的查询结果。

求交集时，要先对每个Term的frequency排序，先求frequency小的Term的交集。

## 本项目的运行参数格式
运行参数可以配置为两种格式：
**格式1**:  Index Basic|VB data_dir output_dir
**格式2**:  Query Basic|VB index_dir

格式1用于创建索引，Basic|VB表示选择的压缩方式，Basic表示索引文件无压缩，VB表示索引文件采用VB压缩。data_dir表示原始数据（用于构建索引的文件）所在的目录。output_dir表示生成的索引文件存储目录。

格式2用于查询，Basic|VB表示选择的压缩方式。index_dir表示索引文件所在的目录。

假设我们用toy_example作为建立索引的原始数据，采用无压缩方式创建索引，生成的索引文件存储在output目录下，则配置为：

Index toy_Basic example/data output

如果想基于上述索引进行查询，则配置格式为：

Query Basic output
## 项目感受
这是我着手写的第一个工程量较大的C++项目，综合运用了C++的多项特性。这也是我第一次尝试从零开始复现论文算法，其间花费了大量的时间和精力，不过好在最终还是得偿所愿完成了整个项目。

非常感谢孙延涛老师的教导和杨宇同学的帮助！