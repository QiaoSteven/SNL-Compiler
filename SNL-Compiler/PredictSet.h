/****************************************************/
/* 文件 PredictSet.h									*/
/* 说明 求SNL语言对应文法的Predict集					*/
/****************************************************/

#ifndef _PREDICTSET_H_
#define _PREDICTSET_H_


#include "globals.h"	/* 该头文件定义了全局类与变量 */


using namespace std;

class production//用来存储一条产生式
{
public:
	int sequenceNum;
	string productionLeft;
	vector<string> productionRight;
public:
	bool operator<(const production &p) const //重载运算符，使之能作为map的key值
	{
		return (this->sequenceNum<p.sequenceNum);
	}
};

class productionList//产生式链表
{
public:
	list<production> producList;//产生式链表
	set<string> nonTerminalSet;//非终极符集
	set<string> TerminalSet;//非终极符集
	map<string, set<string>> firstMap;//所有非终极符的First集
	map<string, set<string>> followMap;//所有非终极符的Follow集
	map<production, set<string>> predictMap;//每条产生式对应的Predict集
	map<string, LexType> LexTypeMap;//将文法符号映射为枚举类型LexType，便于构造LL1分析表
	map<string, NontmlType> NontmlTypeMap;//将文法符号映射为枚举类型NontmlType，便于构造LL1分析表
public:
	productionList();//默认构造函数
	productionList(string dir);//参数为产生式文件所在的目录路径+文件名
	void showProducList();
	void showNonTerminal();
	void showTerminal();
	void showFirstMap();
	void showFollowMap();
	void showPredictMap();
	void showLL1Table();

	void storeFirstMap();//将First集存储到txt文件中
	void storeFollowMap();//将Follow集存储到txt文件中
	void storePredictMap();//将predict集存储到txt文件中
	void storeNonTerminalSet();//将非终极符集存储到txt文件中
	void storeTerminalSet();//将终极符存储到txt文件中

	void InitialFirstFollowPredictMap();//填充First集map，Follow集map，Predict集map的key值，便于下一步的构造
	void readProducList(string dir);//读取产生式文件，填充productionList和非终极符set
	void fillTerminalSet();//填充终极符集，便于之后的人工构造String到LexType的映射
	void establishFirstMap();//构造所有非终极符的First集，将其存入map中
	void findFirstSet(string nonTerStr);//得到一个非终极符的First集
	set<string> getStringFirstSet(list<string> strList);//求多个符号形成的符号串的First集合
	void establishFollowMap();//构造所有非终极符的Follow集，将其存入map中
	void findFollowSet(string nonTerStr);//得到一个非终极符的Follow集
	int countStrInAVec(string str,vector<string> &strVec);//在一个个string类型的vector中，记录下形参str出现了几次
	vector<string>::iterator findPosStrInAVec(int seq, string str, vector<string>& strVec);//返回第seq个str元素的迭代器+1(即返回该元素后面的元素的迭代器)
	void addItemToFollow(string productionLeft, string nonTerStr,list<string>& strList);//向一个非终极符对应的follow集中添加元素
	void establishPredictMap();//构造所有产生式的Follow集，将其存入map中
	
	void makeMapFromStringToLexType();//实现从string到enum类型LexType的映射，方便构造LL1分析表
	void makeMapFromStringToNontmlType();//实现从string到enum类型NontmlType的映射，方便构造LL1分析表
	void fillLL1Table();//根据求出的predict集，填充公有全局数组LL1分析表

	production& FindAProduction(int num);//根据产生式编号，找到某个产生式
	
};

#endif