/****************************************************/
/* �ļ� PredictSet.h									*/
/* ˵�� ��SNL���Զ�Ӧ�ķ���Predict��					*/
/****************************************************/

#ifndef _PREDICTSET_H_
#define _PREDICTSET_H_


#include "globals.h"	/* ��ͷ�ļ�������ȫ��������� */


using namespace std;

class production//�����洢һ������ʽ
{
public:
	int sequenceNum;
	string productionLeft;
	vector<string> productionRight;
public:
	bool operator<(const production &p) const //�����������ʹ֮����Ϊmap��keyֵ
	{
		return (this->sequenceNum<p.sequenceNum);
	}
};

class productionList//����ʽ����
{
public:
	list<production> producList;//����ʽ����
	set<string> nonTerminalSet;//���ռ�����
	set<string> TerminalSet;//���ռ�����
	map<string, set<string>> firstMap;//���з��ռ�����First��
	map<string, set<string>> followMap;//���з��ռ�����Follow��
	map<production, set<string>> predictMap;//ÿ������ʽ��Ӧ��Predict��
	map<string, LexType> LexTypeMap;//���ķ�����ӳ��Ϊö������LexType�����ڹ���LL1������
	map<string, NontmlType> NontmlTypeMap;//���ķ�����ӳ��Ϊö������NontmlType�����ڹ���LL1������
public:
	productionList();//Ĭ�Ϲ��캯��
	productionList(string dir);//����Ϊ����ʽ�ļ����ڵ�Ŀ¼·��+�ļ���
	void showProducList();
	void showNonTerminal();
	void showTerminal();
	void showFirstMap();
	void showFollowMap();
	void showPredictMap();
	void showLL1Table();

	void storeFirstMap();//��First���洢��txt�ļ���
	void storeFollowMap();//��Follow���洢��txt�ļ���
	void storePredictMap();//��predict���洢��txt�ļ���
	void storeNonTerminalSet();//�����ռ������洢��txt�ļ���
	void storeTerminalSet();//���ռ����洢��txt�ļ���

	void InitialFirstFollowPredictMap();//���First��map��Follow��map��Predict��map��keyֵ��������һ���Ĺ���
	void readProducList(string dir);//��ȡ����ʽ�ļ������productionList�ͷ��ռ���set
	void fillTerminalSet();//����ռ�����������֮����˹�����String��LexType��ӳ��
	void establishFirstMap();//�������з��ռ�����First�����������map��
	void findFirstSet(string nonTerStr);//�õ�һ�����ռ�����First��
	set<string> getStringFirstSet(list<string> strList);//���������γɵķ��Ŵ���First����
	void establishFollowMap();//�������з��ռ�����Follow�����������map��
	void findFollowSet(string nonTerStr);//�õ�һ�����ռ�����Follow��
	int countStrInAVec(string str,vector<string> &strVec);//��һ����string���͵�vector�У���¼���β�str�����˼���
	vector<string>::iterator findPosStrInAVec(int seq, string str, vector<string>& strVec);//���ص�seq��strԪ�صĵ�����+1(�����ظ�Ԫ�غ����Ԫ�صĵ�����)
	void addItemToFollow(string productionLeft, string nonTerStr,list<string>& strList);//��һ�����ռ�����Ӧ��follow�������Ԫ��
	void establishPredictMap();//�������в���ʽ��Follow�����������map��
	
	void makeMapFromStringToLexType();//ʵ�ִ�string��enum����LexType��ӳ�䣬���㹹��LL1������
	void makeMapFromStringToNontmlType();//ʵ�ִ�string��enum����NontmlType��ӳ�䣬���㹹��LL1������
	void fillLL1Table();//���������predict������乫��ȫ������LL1������

	production& FindAProduction(int num);//���ݲ���ʽ��ţ��ҵ�ĳ������ʽ
	
};

#endif