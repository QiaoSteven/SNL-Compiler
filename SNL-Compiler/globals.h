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

/*LL1分析表的大小*/
#define  TABLESIZE  104

/********************** 单词的词法类型 ********************/
typedef enum
{
	/* 簿记单词符号 */
	ENDFILE, ERROR,
	/* 保留字 */
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO,
	ENDWH, BEGIN, END, READ, WRITE,
	ARRAY, OF, RECORD, RETURN,

	INTEGER, CHAR,
	/* 多字符单词符号 */
	ID, INTC, CHARC,
	/*特殊符号 */
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
} LexType;



/*所有非终极符，其各自含义可参考LL1文法*/
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


/*LL1分析表*/
extern int LL1Table[TABLESIZE][TABLESIZE];


/*************************************************************************************************/
/*************************************************************************************************/

//定义Token节点结构体
struct Token
{
	int Line;//单词所在的行数 
	string Type;//单词的类别 
	string Value;//单词的值 
	Token *next;//指向下一个节点 
	Token() {
		Line = 0;
		Type = "";
		Value = "";
		next = NULL;
	}
};


//保留字表
extern string ReservedList[21];

//全局变量 
extern int LineNumber, CurIndex;
extern string InputS;//输入流字符串 
extern Token *TokenChain;//Token结构体头指针 
extern Token *CurChain; //Tonken结构体当前指针

//手动构造LL1分析表
void CreatLL1Table();

#endif