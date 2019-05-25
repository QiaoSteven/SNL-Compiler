#pragma once
#ifndef PARSE_H
#define PARSE_H

#include"globals.h"

//结构体声明 
struct TreeKind
{
	string dec;
	string stmt;
	string exp;
};

struct TreeAttribute
{
	int Arraylow=0;
	int Arraytop=0;
	string ArrayType;
	string Paramt;
	string ExpOp;
	int Expval;
	string Expvarkind;
	string Exptype;
};

struct TreeNode//语法树节点 
{
	TreeNode *child[3] = { NULL,NULL,NULL };
	TreeNode *sibling = NULL;
	int Lineno;
	string nodekind;
	TreeKind kind;
	vector<string> idname;
	string type_name;
	TreeAttribute attr;
};

extern stack<string> CharacterS;
extern stack<TreeNode *> TreeNodeS;
extern stack<TreeNode *> OperatorS;
extern stack<TreeNode *> DataS;
extern stack<TreeNode *> saveS;
extern int LineNo;
extern int level;
extern vector<string> SelfType;
extern TreeNode *t, *saveP;
extern bool getExpResult;
extern bool getExpResult2;
extern int expflag;
extern string TerminalCharacter[43];


//函数声明 
void TestDisplay(TreeNode *Troot, int blank);
int Priosity(string s);
void BlankPrint(int i);
void DisplayParseTree(TreeNode *Troot);
bool IsFinalCharacter(string tars);
void ParseLL();
void ChooseExpress(int pnum);
string ChangeToTerminal(string str);
NontmlType ChangeToEnum(string str);
LexType ChangeToEnumTer(string str);


//104个产生式对应的函数
int StringToInt(string s);
void Process1();
void Process2();
void Process3();
void Process4();
void Process5();
void Process6();
void Process7();
void Process8();
void Process9();
void Process10();
void Process11();
void Process12();
void Process13();
void Process14();
void Process15();
void Process16();
void Process17();
void Process18();
void Process19();
void Process20();
void Process21();
void Process22();
void Process23();
void Process24();
void Process25();
void Process26();
void Process27();
void Process28();
void Process29();
void Process30();
void Process31();
void Process32();
void Process33();
void Process34();
void Process35();
void Process36();
void Process37();
void Process38();
void Process39();
void Process40();
void Process41();
void Process42();
void Process43();
void Process44();
void Process45();
void Process46();
void Process47();
void Process48();
void Process49();
void Process50();
void Process51();
void Process52();
void Process53();
void Process54();
void Process55();
void Process56();
void Process57();
void Process58();
void Process59();
void Process60();
void Process61();
void Process62();
void Process63();
void Process64();
void Process65();
void Process66();
void Process67();
void Process68();
void Process69();
void Process70();
void Process71();
void Process72();
void Process73();
void Process74();
void Process75();
void Process76();
void Process77();
void Process78();
void Process79();
void Process80();
void Process81();
void Process82();
void Process83();
void Process84();
void Process85();
void Process86();
void Process87();
void Process88();
void Process89();
void Process90();
void Process91();
void Process92();
void Process93();
void Process94();
void Process95();
void Process96();
void Process97();
void Process98();
void Process99();
void Process100();
void Process101();
void Process102();
void Process103();
void Process104();

#endif
