#pragma once
#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include <list>
#include <vector>
#include <map>
#include <set>
#include<stack>
#include<queue>
#include <algorithm>
using namespace std;

/*LL1������Ĵ�С*/
#define  TABLESIZE  104

/********************** ���ʵĴʷ����� ********************/
typedef enum
{
	/* ���ǵ��ʷ��� */
	ENDFILE, ERROR,
	/* ������ */
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO,
	ENDWH, BEGIN, END, READ, WRITE,
	ARRAY, OF, RECORD, RETURN,

	INTEGER, CHAR,
	/* ���ַ����ʷ��� */
	ID, INTC, CHARC,
	/*������� */
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
} LexType;



/*���з��ռ���������Ժ���ɲο�LL1�ķ�*/
typedef enum
{
	Program, ProgramHead, ProgramName, DeclarePart,
	TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
	TypeId, TypeName, BaseType, StructureType,
	ArrayType, Low, Top, RecType,
	FieldDecList, FieldDecMore, IdList, IdMore,
	VarDec, VarDeclaration, VarDecList, VarDecMore,
	VarIdList, VarIdMore, ProcDec, ProcDeclaration,
	ProcDecMore, ProcName, ParamList, ParamDecList,
	ParamMore, Param, FormList, FidMore,
	ProcDecPart, ProcBody, ProgramBody, StmList,
	StmMore, Stm, AssCall, AssignmentRest,
	ConditionalStm, StmL, LoopStm, InputStm,
	InVar, OutputStm, ReturnStm, CallStmRest,
	ActParamList, ActParamMore, RelExp, OtherRelE,
	Exp, OtherTerm, Term, OtherFactor,
	Factor, Variable, VariMore, FieldVar,
	FieldVarMore, CmpOp, AddOp, MultOp

}  NontmlType;


/*LL1������*/
extern int LL1Table[TABLESIZE][TABLESIZE];


/*************************************************************************************************/
/*************************************************************************************************/

//����Token�ڵ�ṹ��
struct Token
{
	int Line;//�������ڵ����� 
	string Type;//���ʵ���� 
	string Value;//���ʵ�ֵ 
	Token *next;//ָ����һ���ڵ� 
	Token() {
		Line = 0;
		Type = "";
		Value = "";
		next = NULL;
	}
};


//�����ֱ�
extern string ReservedList[21];

//ȫ�ֱ��� 
extern int LineNumber, CurIndex;
extern string InputS;//�������ַ��� 
extern Token *TokenChain;//Token�ṹ��ͷָ�� 
extern Token *CurChain; //Tonken�ṹ�嵱ǰָ��

//�ֶ�����LL1������
void CreatLL1Table();

#endif