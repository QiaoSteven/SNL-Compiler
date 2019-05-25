#pragma once
#ifndef SYBTAXTREE_H
#define SYNTAXTREE_H

#include "globals.h"
#include "Scanner.h"
#include "PredictSet.h"
#include "Parse.h"


extern vector<production> vecTree;//���ڼ�¼ƥ�������ʹ�õ��Ĳ���ʽ��Ϊ��һ�������﷨��������׼��
extern vector<string> TokenValueVec;
extern vector<string> TokenTypeVec;

struct AnalysisTreeNode 
{
	int id;//�����ڻ�dotͼʱ���ֽڵ�
	string TokenValue;
	string TokenType;
	vector<AnalysisTreeNode *> children;
	AnalysisTreeNode(string s,int id) : TokenValue(s), id(id) {}
};

extern AnalysisTreeNode* root;//�﷨�������ĸ��ڵ�
extern int global_id;//DOT Language��ÿ���ڵ��id
extern unsigned int global_token_index;//���ڽ�IDת��Ϊ���������ߺ�������
extern unsigned int treeIndex;// ָʾvTree���±�


void SNLCompiler();//������
void AutoMakeLL1Table(productionList* ptrProducList);
void LexicalAnalysis();
void SyntaxAnalysis(productionList* ptrProducList);
void CreateSyntaxTree(productionList* ptrProducList);
void dfsBuildTree(AnalysisTreeNode*& node, productionList* ptrProducList);//������ȱ��������﷨��������

#endif