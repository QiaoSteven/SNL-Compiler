#include "Parse.h"


stack<string> CharacterS;
stack<TreeNode *> TreeNodeS;
stack<TreeNode *> OperatorS;
stack<TreeNode *> DataS;
stack<TreeNode *> saveS;
int LineNo;
int level;
vector<string> SelfType;
TreeNode *t, *saveP;
bool getExpResult = false, getExpResult2 = false;
int expflag = 0;

string TerminalCharacter[43] = {
	/* 簿记单词符号 */
	"ENDFILE",	"ERROR",
	/* 保留字 */
	"PROGRAM",	"PROCEDURE",	"TYPE",	"VAR",		"IF",
	"THEN",		"ELSE",		"FI",		"WHILE",		"DO",
	"ENDWH",		"BEGIN",		"END",	"READ",		"WRITE",
	"ARRAY",		"OF",			"RECORD",	"RETURN",

	"INTEGER",	"CHAR",
	/* 多字符单词符号 */
	"ID",			"INTC",		"CHARC",
	/*特殊符号 */
	":=",		"=",			"<",		"+",		"-",
	"*",		"/",		"(",	")",		"DOT",
	":",		";",		",",	"[",	"]",
	"UNDERANGE",    ".."
};



bool IsFinalCharacter(string tars)
{
	for (int i = 0; i < 43; i++)
	{
		if (TerminalCharacter[i] == tars) return true;
	}
	return false;
}
void ParseLL()
{
	CreatLL1Table();//调用创建LL（1）分析表的函数
	string TopStr = "Program";
	CharacterS.push(TopStr);
	TreeNode *Troot = new TreeNode; Troot->child[2] = new TreeNode; Troot->child[1] = new TreeNode; Troot->child[0] = new TreeNode;
	Troot->nodekind = "ProK";
	TreeNodeS.push(Troot->child[2]);
	TreeNodeS.push(Troot->child[1]);
	TreeNodeS.push(Troot->child[0]);
	saveS.push(Troot);
	saveP = saveS.top();
	CurChain = TokenChain->next;
	LineNo = CurChain->Line;
	while (!CharacterS.empty())
	{
		TopStr = CharacterS.top();
		//	cout<<"  "<<TopStr<<" "<<CurChain->Value<<endl;
		if (IsFinalCharacter(TopStr))//Todo 判断是否是终极符函数 
		{
			if (TopStr == ChangeToTerminal(CurChain->Value))
			{
				CharacterS.pop();
				//	cout<<"匹配成功"<<endl;cout<<saveS.top()<<endl;
				if (TopStr == "END")
				{
					saveS.pop();
					if (!saveS.empty()) saveP = saveS.top();
				}
				CurChain = CurChain->next;
				LineNo = CurChain->Line;
			}
			else
			{
				cout << "出现语法错误" << " 在第 " << LineNo << " 行：" << CurChain->Value << endl; //Todo
				exit(0);
			}
		}
		else
		{
			NontmlType first = ChangeToEnum(TopStr);
			LexType second = ChangeToEnumTer(CurChain->Value);
			int pnum = LL1Table[first][second];
			if (pnum == 0) { cout << "出现语法错误" << " 在第 " << LineNo << " 行：" << CurChain->Value << endl; exit(0); }
			//		cout<<first<<" "<<second<<" pnum: "<<pnum<<" 在 "<<LineNo<<" 行"<<endl;
			CharacterS.pop();
			ChooseExpress(pnum);
		}
	}
	if (CurChain->Value != ".")
	{
		cout << "Error: End early" << endl;
	}
	else
	{
		cout << "Parse Success" << endl;
	}
	TestDisplay(Troot, 0);
	//	DisplayParseTree(Troot);
	//	cout<<"over"<<endl;
}
string ChangeToTerminal(string str)
{
	if (str == "program") return "PROGRAM";
	if (str == "type") return "TYPE";
	if (str == "integer") return "INTEGER";
	if (str == "char") return "CHAR";
	if (str == "var") return "VAR";
	if (str == "array") return "ARRAY";
	if (str == "of") return "OF";
	if (str == "procedure") return "PROCEDURE";
	if (str == "begin") return "BEGIN";
	if (str == "end") return "END";

	if (str == "return") return "RETURN";
	if (str == "record") return "RECORD";
	if (str == "if") return "IF";
	if (str == "then") return "THEN";
	if (str == "else") return "ELSE";
	if (str == "fi") return "FI";
	if (str == "while") return "WHILE";
	if (str == "do") return "DO";
	if (str == "endwh") return "ENDWH";
	if (str == "read") return "READ";
	if (str == "write") return "WRITE";
	return str;
}
void TestDisplay(TreeNode *Troot, int blank)
{

	if (Troot->nodekind != "")
	{
		BlankPrint(blank);
		cout << Troot->nodekind << " ";
		if (Troot->attr.ArrayType != "")cout << Troot->attr.ArrayType << " ";
		if (Troot->attr.Arraylow != 0)cout << Troot->attr.Arraylow << " ";
		if (Troot->attr.Arraytop != 0)cout << Troot->attr.Arraytop << " ";
		if (Troot->nodekind == "DecK") cout << Troot->kind.dec << " ";
		if (Troot->nodekind == "StmtK") cout << Troot->kind.stmt << " ";
		if (Troot->nodekind == "ExpK") cout << Troot->kind.exp << " ";
		for (int i = 0; i < Troot->idname.size(); i++) cout << Troot->idname[i] << " ";
		if (Troot->attr.Paramt != "")cout << Troot->attr.Paramt << " ";
		if (Troot->type_name != "")cout << Troot->type_name << " ";
		cout << endl;
	}
	level++;
	if (Troot->child[0] != NULL)TestDisplay(Troot->child[0], blank + 1);
	if (Troot->child[1] != NULL)TestDisplay(Troot->child[1], blank + 1);
	if (Troot->child[2] != NULL)TestDisplay(Troot->child[2], blank + 1);
	if (Troot->sibling != NULL)TestDisplay(Troot->sibling, blank);
}
void DisplayParseTree(TreeNode *Troot)
{
	int BlankNum = 0; TreeNode *temp;
	cout << Troot->nodekind << endl; BlankNum++;
	//第一个孩子节点 
	BlankPrint(BlankNum); cout << Troot->child[0]->nodekind << " ";
	for (int i = 0; i < Troot->child[0]->idname.size(); i++)cout << Troot->child[0]->idname[i] << " ";
	//第二个孩子节点 
	temp = Troot->child[1];
	//	cout<<temp<<temp->nodekind<<endl;
	if (temp->nodekind == "TypeK")
	{
		cout << temp->nodekind << endl;
		temp = temp->child[0];
		while (temp != NULL)
		{
			BlankPrint(BlankNum + 1); cout << temp->nodekind << " " << temp->type_name << " ";
			for (int i = 0; i < temp->idname.size(); i++)cout << temp->idname[i] << " ";
			temp = temp->sibling;

		}
	}//类型声明输出完毕//todo 考虑声明类型是数组和结构体时的输出 
	cout << endl;
	temp = temp->sibling;
	if (temp->nodekind == "VarK")
	{
		BlankPrint(BlankNum + 1); cout << temp->nodekind << endl;
		temp = temp->child[0];
		while (temp != NULL)
		{
			BlankPrint(BlankNum + 2); cout << temp->nodekind << " " << temp->type_name << " ";
			for (int i = 0; i < temp->idname.size(); i++)cout << temp->idname[i] << " ";
			temp = temp->sibling;
			cout << endl;
		}
	}//变量声明输出完毕

	temp = temp->sibling;
	if (temp->nodekind == "ProcK")
	{
		cout << temp->nodekind << endl;
		temp = temp->child[0];//Todo 过程声明输出 
	}//过程声明输出完毕
	temp = Troot->child[2];
	if (temp->nodekind == "StmLK")
	{

	}
}
int Priosity(string s)
{
	if (s == "END") return 0;
	if (s == "<" || s == "=") return 1;
	if (s == "+" || s == "-") return 2;
	if (s == "*" || s == "/") return 3;
	else return -1;
}
void BlankPrint(int i)
{
	for (int j = 0; j < i; j++)cout << "   ";
}
LexType ChangeToEnumTer(string str)
{
	if (str == "ID") return ID;
	if (str == "INTC") return INTC;
	if (str == "CHARC") return CHARC;
	if (str == "program") return PROGRAM;
	if (str == "procedure") return PROCEDURE;
	if (str == "type") return TYPE;
	if (str == "var") return VAR;
	if (str == "if") return IF;
	if (str == "then") return THEN;
	if (str == "else") return ELSE;
	if (str == "fi") return FI;
	if (str == "while") return WHILE;
	if (str == "do") return DO;
	if (str == "endwh") return ENDWH;
	if (str == "begin") return BEGIN;
	if (str == "end") return END;
	if (str == "read") return READ;
	if (str == "write") return WRITE;
	if (str == "array") return ARRAY;
	if (str == "of") return OF;
	if (str == "record") return RECORD;
	if (str == "return") return RETURN;
	if (str == "integer") return INTEGER;
	if (str == "char") return CHAR;
	if (str == ":=") return ASSIGN;
	if (str == "=") return EQ;
	if (str == "<") return LT;
	if (str == "+") return PLUS;
	if (str == "-") return MINUS;
	if (str == "*") return TIMES;
	if (str == "/") return OVER;
	if (str == "(") return LPAREN;
	if (str == ")") return RPAREN;
	if (str == ".") return DOT;
	if (str == ":") return COLON;
	if (str == ";") return SEMI;
	if (str == ",") return COMMA;
	if (str == "[") return LMIDPAREN;
	if (str == "]") return RMIDPAREN;
	//	if(str=="") return UNDERANGE;
	//	if(str=="") return ERROR;
	//	if(str=="") return ENDFILE;
}
void ChooseExpress(int pnum)//Todo 选择合适的函数进行处理 
{
	switch (pnum)
	{
		//	case 0:cout<<"出现0号产生式";exit(0);
	case 1:Process1(); break;
	case 2:Process2(); break;
	case 3:Process3(); break;
	case 4:Process4(); break;
	case 5:Process5(); break;
	case 6:Process6(); break;
	case 7:Process7(); break;
	case 8:Process8(); break;
	case 9:Process9(); break;
	case 10:Process10(); break;
	case 11:Process11(); break;
	case 12:Process12(); break;
	case 13:Process13(); break;
	case 14:Process14(); break;
	case 15:Process15(); break;
	case 16:Process16(); break;
	case 17:Process17(); break;
	case 18:Process18(); break;
	case 19:Process19(); break;
	case 20:Process20(); break;
	case 21:Process21(); break;
	case 22:Process22(); break;
	case 23:Process23(); break;
	case 24:Process24(); break;
	case 25:Process25(); break;
	case 26:Process26(); break;
	case 27:Process27(); break;
	case 28:Process28(); break;
	case 29:Process29(); break;
	case 30:Process30(); break;
	case 31:Process31(); break;
	case 32:Process32(); break;
	case 33:Process33(); break;
	case 34:Process34(); break;
	case 35:Process35(); break;
	case 36:Process36(); break;
	case 37:Process37(); break;
	case 38:Process38(); break;
	case 39:Process39(); break;
	case 40:Process40(); break;
	case 41:Process41(); break;
	case 42:Process42(); break;
	case 43:Process43(); break;
	case 44:Process44(); break;
	case 45:Process45(); break;
	case 46:Process46(); break;
	case 47:Process47(); break;
	case 48:Process48(); break;
	case 49:Process49(); break;
	case 50:Process50(); break;
	case 51:Process51(); break;
	case 52:Process52(); break;
	case 53:Process53(); break;
	case 54:Process54(); break;
	case 55:Process55(); break;
	case 56:Process56(); break;
	case 57:Process57(); break;
	case 58:Process58(); break;
	case 59:Process59(); break;
	case 60:Process60(); break;
	case 61:Process61(); break;
	case 62:Process62(); break;
	case 63:Process63(); break;
	case 64:Process64(); break;
	case 65:Process65(); break;
	case 66:Process66(); break;
	case 67:Process67(); break;
	case 68:Process68(); break;
	case 69:Process69(); break;
	case 70:Process70(); break;
	case 71:Process71(); break;
	case 72:Process72(); break;
	case 73:Process73(); break;
	case 74:Process74(); break;
	case 75:Process75(); break;
	case 76:Process76(); break;
	case 77:Process77(); break;
	case 78:Process78(); break;
	case 79:Process79(); break;
	case 80:Process80(); break;
	case 81:Process81(); break;
	case 82:Process82(); break;
	case 83:Process83(); break;
	case 84:Process84(); break;
	case 85:Process85(); break;
	case 86:Process86(); break;
	case 87:Process87(); break;
	case 88:Process88(); break;
	case 89:Process89(); break;
	case 90:Process90(); break;
	case 91:Process91(); break;
	case 92:Process92(); break;
	case 93:Process93(); break;
	case 94:Process94(); break;
	case 95:Process95(); break;
	case 96:Process96(); break;
	case 97:Process97(); break;
	case 98:Process98(); break;
	case 99:Process99(); break;
	case 100:Process100(); break;
	case 101:Process101(); break;
	case 102:Process102(); break;
	case 103:Process103(); break;
	case 104:Process104(); break;
	default:break;
	}
}
NontmlType ChangeToEnum(string str) //Todo 把字符串类型数据改为枚举类型  
{
	if (str == "Program")return Program;
	if (str == "ProgramHead") return ProgramHead;
	if (str == "ProgramName")return ProgramName;
	if (str == "DeclarePart")return DeclarePart;
	if (str == "TypeDec")return TypeDec;
	if (str == "TypeDeclaration")return TypeDeclaration;
	if (str == "TypeDecList")return TypeDecList;
	if (str == "TypeDecMore")return TypeDecMore;
	if (str == "TypeId")return TypeId;
	if (str == "TypeName")return TypeName;
	if (str == "BaseType")return BaseType;
	if (str == "StructureType")return StructureType;
	if (str == "ArrayType")return ArrayType;
	if (str == "Low")return Low;
	if (str == "Top")return Top;
	if (str == "RecType")return RecType;
	if (str == "FieldDecList")return FieldDecList;
	if (str == "FieldDecMore")return FieldDecMore;
	if (str == "IdList")return IdList;
	if (str == "IdMore")return IdMore;
	if (str == "VarDec")return VarDec;
	if (str == "VarDeclaration")return VarDeclaration;
	if (str == "VarDecList")return VarDecList;
	if (str == "VarDecMore")return VarDecMore;
	if (str == "VarIdList")return VarIdList;
	if (str == "VarIdMore")return VarIdMore;
	if (str == "ProcDec")return ProcDec;
	if (str == "ProcDeclaration")return ProcDeclaration;
	if (str == "ProcDecMore")return ProcDecMore;
	if (str == "ProcName")return ProcName;
	if (str == "ParamList")return ParamList;
	if (str == "ParamDecList")return ParamDecList;
	if (str == "ParamMore")return ParamMore;
	if (str == "Param")return Param;
	if (str == "FormList")return FormList;
	if (str == "FidMore")return FidMore;
	if (str == "ProcDecPart")return ProcDecPart;
	if (str == "ProcBody")return ProcBody;
	if (str == "ProgramBody")return ProgramBody;
	if (str == "StmList")return StmList;
	if (str == "StmMore")return StmMore;
	if (str == "Stm")return Stm;
	if (str == "AssCall")return AssCall;
	if (str == "AssignmentRest")return AssignmentRest;
	if (str == "ConditionalStm")return ConditionalStm;
	if (str == "StmL")return StmL;
	if (str == "LoopStm")return LoopStm;
	if (str == "InputStm")return InputStm;
	if (str == "InVar")return InVar;
	if (str == "OutputStm")return OutputStm;
	if (str == "ReturnStm")return ReturnStm;
	if (str == "CallStmRest")return CallStmRest;
	if (str == "ActParamList")return ActParamList;
	if (str == "ActParamMore")return ActParamMore;
	if (str == "RelExp")return RelExp;
	if (str == "OtherRelE")return OtherRelE;
	if (str == "Exp")return Exp;
	if (str == "OtherTerm")return OtherTerm;
	if (str == "Term")return Term;
	if (str == "OtherFactor")return OtherFactor;
	if (str == "Factor")return Factor;
	if (str == "Variable")return Variable;
	if (str == "VariMore")return VariMore;
	if (str == "FieldVar")return FieldVar;
	if (str == "FieldVarMore")return FieldVarMore;
	if (str == "CmpOp")return CmpOp;
	if (str == "AddOp")return AddOp;
	if (str == "MultOp")return MultOp;
}



//104个产生式对应的函数
int StringToInt(string s)
{
	int result = 0;
	for (int i = s.size() - 1; i >= 0; i--)
	{
		result += (s[i] - '0')*pow(10, s.size() - i - 1);
	}
	return result;
}
void Process1()
{
	CharacterS.push("ProgramBody");
	CharacterS.push("DeclarePart");
	CharacterS.push("ProgramHead");
}
void Process2()
{
	CharacterS.push("ProgramName");
	CharacterS.push("PROGRAM");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "PheadK";
}
void Process3()
{
	CharacterS.push("ID");
	t->idname.push_back(CurChain->Type);
}
void Process4()
{
	CharacterS.push("ProcDec");
	CharacterS.push("VarDec");
	CharacterS.push("TypeDec");
}
void Process5()
{

}
void Process6()
{
	CharacterS.push("TypeDeclaration");
}
void Process7()
{
	CharacterS.push("TypeDecList");
	CharacterS.push("TYPE");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->child[0] = new TreeNode; t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
	TreeNodeS.push(t->child[0]);
	t->nodekind = "TypeK";
}
void Process8()
{
	CharacterS.push("TypeDecMore");
	CharacterS.push(";");
	CharacterS.push("TypeName");//
	CharacterS.push("=");
	CharacterS.push("TypeId");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->sibling = new TreeNode; t->nodekind = "DecK";
	TreeNodeS.push(t->sibling);
}
void Process9()
{
	TreeNodeS.pop();
}
void Process10()
{
	CharacterS.push("TypeDecList");
}
void Process11()
{
	CharacterS.push("ID");
	if (CurChain->Value == "ID") { SelfType.push_back(CurChain->Type); }
	t->idname.push_back(CurChain->Type);
}
void Process12()
{
	CharacterS.push("BaseType");
}
void Process13()
{
	CharacterS.push("StructureType");
}
void Process14()//在此函数内要进行判断确实为已经声明的类型标识符 
{
	CharacterS.push("ID");
	bool localflag = false;
	for (int i = 0; i < SelfType.size(); i++) if (SelfType[i] == CurChain->Type) localflag = true;
	if (!localflag) { cout << "未定义类型： " << CurChain->Type; exit(0); }
	else
	{
		t->kind.dec = "IdK";
		t->type_name = CurChain->Type;
	}
}
void Process15()
{
	CharacterS.push("INTEGER");
	t->kind.dec = "IntegerK";
}
void Process16()
{
	CharacterS.push("CHAR");
	t->kind.dec = "CharK";
}
void Process17()
{
	CharacterS.push("ArrayType");
}
void Process18()
{
	CharacterS.push("RecType");
}
void Process19()
{
	CharacterS.push("BaseType");
	CharacterS.push("OF");
	CharacterS.push("]");
	CharacterS.push("Top");
	CharacterS.push("..");
	CharacterS.push("Low");
	CharacterS.push("[");
	CharacterS.push("ARRAY");
	t->attr.ArrayType = "ArrayK";
}
void Process20()
{
	CharacterS.push("INTC");
	t->attr.Arraylow = StringToInt(CurChain->Type);
	//	cout<<t->kind.dec<<" ";
	//	cout<<t->attr.Arraylow<<endl;
}
void Process21()
{
	CharacterS.push("INTC");
	t->attr.Arraytop = StringToInt(CurChain->Type);
	//	cout<<CurChain->Type<<" ";
	//	cout<<t->attr.Arraytop<<endl;
}
void Process22()
{
	CharacterS.push("END");
	CharacterS.push("FieldDecList");
	CharacterS.push("RECORD");
	t->kind.dec = "RecordK";
	saveS.push(t);
	saveP = t;
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
}
void Process23()
{
	CharacterS.push("FieldDecMore");
	CharacterS.push(";");
	CharacterS.push("IdList");
	CharacterS.push("BaseType");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->sibling = new TreeNode; TreeNodeS.push(t->sibling);
}
void Process24()
{
	CharacterS.push("FieldDecMore");
	CharacterS.push(";");
	CharacterS.push("IdList");
	CharacterS.push("ArrayType");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->kind.dec = "ArrayK";
	t->sibling = new TreeNode; TreeNodeS.push(t->sibling);
}
void Process25()
{
	TreeNodeS.pop(); t = saveP;
}
void Process26()
{
	CharacterS.push("FieldDecList");
}
void Process27()
{
	CharacterS.push("IdMore");
	CharacterS.push("ID");
	t->idname.push_back(CurChain->Type);
}
void Process28()
{

}
void Process29()
{
	CharacterS.push("IdList");
	CharacterS.push(",");
}
//变量声明部分 
void Process30()
{

}
void Process31()
{
	CharacterS.push("VarDeclaration");
}
void Process32()
{
	CharacterS.push("VarDecList");
	CharacterS.push("VAR");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->child[0] = new TreeNode; t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
	TreeNodeS.push(t->child[0]);
	t->nodekind = "VarK";
}
void Process33()
{
	CharacterS.push("VarDecMore");
	CharacterS.push(";");
	CharacterS.push("VarIdList");
	CharacterS.push("TypeName");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "DecK";
	t->sibling = new TreeNode; TreeNodeS.push(t->sibling);
}
void Process34()
{
	TreeNodeS.pop();
}
void Process35()
{
	CharacterS.push("VarDecList");
}
void Process36()
{
	CharacterS.push("VarIdMore");
	CharacterS.push("ID");
	t->idname.push_back(CurChain->Type);
}
void Process37()
{

}
void Process38()
{
	CharacterS.push("VarIdList");
	CharacterS.push(",");
}
void Process39()
{

}
void Process40()
{
	CharacterS.push("ProcDeclaration");
}
void Process41()
{
	CharacterS.push("ProcDecMore");
	CharacterS.push("ProcBody");
	CharacterS.push("ProcDecPart");
	CharacterS.push(";");
	CharacterS.push(")");
	CharacterS.push("ParamList");
	CharacterS.push("(");
	CharacterS.push("ProcName");
	CharacterS.push("PROCEDURE");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "ProcK";
	saveS.push(t);
	saveP = t;
	t->sibling = new TreeNode; t->child[0] = new TreeNode; t->child[1] = new TreeNode; t->child[2] = new TreeNode;
	TreeNodeS.push(t->sibling); TreeNodeS.push(t->child[2]); TreeNodeS.push(t->child[1]); TreeNodeS.push(t->child[0]);
}
void Process42()
{

}
void Process43()
{
	CharacterS.push("ProcDeclaration");
}
void Process44()
{
	CharacterS.push("ID");
	t->idname.push_back(CurChain->Type);
}
void Process45()
{
	TreeNodeS.pop();
}
void Process46()
{
	CharacterS.push("ParamDecList");
}
void Process47()
{
	CharacterS.push("ParamMore");
	CharacterS.push("Param");
}
void Process48()
{
	TreeNodeS.pop();
	t = saveP;
}
void Process49()
{
	CharacterS.push("ParamDecList");
	CharacterS.push(";");
}
void Process50()
{
	CharacterS.push("FormList");
	CharacterS.push("TypeName");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "DecK";
	t->attr.Paramt = "value param";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process51()
{
	CharacterS.push("FormList");
	CharacterS.push("TypeName");
	CharacterS.push("Var");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "DecK";
	t->attr.Paramt = "var param";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process52()
{
	CharacterS.push("FidMore");
	CharacterS.push("ID");
	t->idname.push_back(CurChain->Type);
}
void Process53()
{

}
void Process54()
{
	CharacterS.push("FormList");
	CharacterS.push(",");
}
void Process55()
{
	CharacterS.push("DeclarePart");

}
void Process56()
{
	CharacterS.push("ProgramBody");
}
void Process57()
{
	CharacterS.push("END");
	CharacterS.push("StmList");
	CharacterS.push("BEGIN");
	while (t != saveP->child[2])
	{
		t = TreeNodeS.top(); TreeNodeS.pop();
	}
	t->nodekind = "StmLk";
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
}
void Process58()
{
	CharacterS.push("StmMore");
	CharacterS.push("Stm");
}
void Process59()
{
	TreeNodeS.pop();
}
void Process60()
{
	CharacterS.push("StmList");
	CharacterS.push(";");
}
void Process61()
{
	CharacterS.push("ConditionalStm");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "StmtK";
	t->kind.stmt = "If";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process62()
{
	CharacterS.push("LoopStm");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "StmtK";
	t->kind.stmt = "While";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process63()
{
	CharacterS.push("InputStm");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "StmtK";
	t->kind.stmt = "Read";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process64()
{
	CharacterS.push("OutputStm");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "StmtK";
	t->kind.stmt = "Write";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process65()
{
	CharacterS.push("ReturnStm");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "StmtK";
	t->kind.stmt = "Return";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process66()
{
	CharacterS.push("AssCall");
	CharacterS.push("ID");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->nodekind = "StmtK";
	t->child[0] = new TreeNode; t->child[0]->nodekind = "ExpK"; t->child[0]->idname.push_back(CurChain->Type); t->child[0]->kind.exp = "IdV";
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process67()
{
	CharacterS.push("AssignmentRest");
	t->kind.stmt = "Assign";
}
void Process68()
{
	CharacterS.push("CallStmRest");
	t->kind.stmt = "Call";
}
void Process69()
{
	CharacterS.push("Exp");
	CharacterS.push(":=");
	CharacterS.push("VariMore");
	t->child[1] = new TreeNode; TreeNodeS.push(t->child[1]);
	t = t->child[0];
	//todo 压入一个特殊栈底标识 
	TreeNode *temp = new TreeNode;
	temp->type_name = "END";
	OperatorS.push(temp);
	getExpResult = true;
}
void Process70()
{
	CharacterS.push("FI");
	CharacterS.push("StmList");
	CharacterS.push("ELSE");
	CharacterS.push("StmList");
	CharacterS.push("THEN");
	CharacterS.push("RelExp");
	CharacterS.push("IF");
	t->child[2] = new TreeNode;
	TreeNodeS.push(t->child[2]);
	t->child[1] = new TreeNode;
	TreeNodeS.push(t->child[1]);
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
}
void Process71()
{
	CharacterS.push("ENDWH");
	CharacterS.push("StmList");
	CharacterS.push("DO");
	CharacterS.push("RelExp");
	CharacterS.push("WHILE");
	t->child[1] = new TreeNode;
	TreeNodeS.push(t->child[1]);
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
}
void Process72()
{
	CharacterS.push(")");
	CharacterS.push("InVar");
	CharacterS.push("(");
	CharacterS.push("READ");
}
void Process73()
{
	CharacterS.push("ID");
	t->idname.push_back(CurChain->Type);
}
void Process74()
{
	CharacterS.push(")");
	CharacterS.push("Exp");
	CharacterS.push("(");
	CharacterS.push("WRITE");
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
	TreeNode *temp = new TreeNode; temp->type_name = "END";
	OperatorS.push(temp);
}
void Process75()
{
	CharacterS.push("RETURN");
}
void Process76()
{
	CharacterS.push(")");
	CharacterS.push("ActParamList");
	CharacterS.push("(");
	t->child[1] = new TreeNode;
	TreeNodeS.push(t->child[1]);
}
void Process77()
{
	TreeNodeS.pop();
}
void Process78()
{
	CharacterS.push("ActParamMore");
	CharacterS.push("Exp");
	TreeNode *temp = new TreeNode; temp->type_name = "END";
	OperatorS.push(temp);
}
void Process79()
{

}
void Process80()
{
	CharacterS.push("ActParamList");
	CharacterS.push(",");
	t->sibling = new TreeNode;
	TreeNodeS.push(t->sibling);
}
void Process81()
{
	CharacterS.push("OtherRelE");
	CharacterS.push("Exp");
	TreeNode *temp = new TreeNode; temp->type_name = "END";
	OperatorS.push(temp);
	getExpResult = false;
}
void Process82()
{
	CharacterS.push("Exp");
	CharacterS.push("CmpOp");
	TreeNode *temp = new TreeNode; TreeNode *temp1 = new TreeNode;
	temp->type_name = CurChain->Value;
	temp1 = OperatorS.top();
	while (Priosity(temp1->type_name) >= Priosity(temp->type_name))
	{
		t = temp1; OperatorS.pop();
		//	t->child[0]=new TreeNode;t->child[1]=new TreeNode;
		t->child[1] = DataS.top(); DataS.pop();
		t->child[0] = DataS.top(); DataS.pop();
		DataS.push(t);
		temp1 = OperatorS.top();
	}
	OperatorS.push(temp);
	getExpResult = true;
}
void Process83()
{
	CharacterS.push("OtherTerm");
	CharacterS.push("Term");
}
void Process84()//todo
{
	TreeNode *temp;
	if (CurChain->Value == ")"&&expflag != 0)
	{
		temp = OperatorS.top();
		while (temp->type_name != "(")
		{
			t = temp; OperatorS.pop();
			t->child[1] = DataS.top(); DataS.pop();
			t->child[0] = DataS.top(); DataS.pop();
			DataS.push(t);
			temp = OperatorS.top();
		}
		OperatorS.pop(); expflag--;
	}
	else if (getExpResult || getExpResult2)
	{
		temp = OperatorS.top();
		while (temp->type_name != "END")
		{
			t = temp; OperatorS.pop();
			t->child[1] = DataS.top(); DataS.pop();
			t->child[0] = DataS.top(); DataS.pop();
			DataS.push(t);
			temp = OperatorS.top();
		}
		OperatorS.pop();
		temp = DataS.top(); DataS.pop();
		t = TreeNodeS.top(); TreeNodeS.pop();
		t->child[0] = temp->child[0]; t->child[1] = temp->child[1]; t->child[1] = temp->child[1]; t->sibling = temp->sibling; t->nodekind = temp->nodekind;
		t->kind.exp = temp->kind.exp; t->type_name = temp->type_name;
		getExpResult2 = false;
	}
}
void Process85()
{
	CharacterS.push("Exp");
	CharacterS.push("AddOp");
	TreeNode *temp = new TreeNode; TreeNode *temp1 = new TreeNode;
	temp->type_name = CurChain->Value;
	temp1 = OperatorS.top();
	while (Priosity(temp1->type_name) >= Priosity(temp->type_name))
	{
		t = temp1; OperatorS.pop();
		//	t->child[0]=new TreeNode;t->child[1]=new TreeNode;
		t->child[1] = DataS.top(); DataS.pop();
		t->child[0] = DataS.top(); DataS.pop();
		DataS.push(t);
		temp1 = OperatorS.top();
	}
	OperatorS.push(temp);
}
void Process86()
{
	CharacterS.push("OtherFactor");
	CharacterS.push("Factor");
}
void Process87()
{

}
void Process88()
{
	CharacterS.push("Term");
	CharacterS.push("MultOp");
	TreeNode *temp = new TreeNode; TreeNode *temp1 = new TreeNode;
	temp->type_name = CurChain->Value;
	temp1 = OperatorS.top();
	while (Priosity(temp1->type_name) >= Priosity(temp->type_name))
	{
		t = temp1; OperatorS.pop();
		//	t->child[0]=new TreeNode;t->child[1]=new TreeNode;
		t->child[1] = DataS.top(); DataS.pop();
		t->child[0] = DataS.top(); DataS.pop();
		DataS.push(t);
		temp1 = OperatorS.top();
	}
	OperatorS.push(temp);

}
void Process89()
{
	CharacterS.push(")");
	CharacterS.push("Exp");
	CharacterS.push("(");
	TreeNode *temp = new TreeNode;
	temp->nodekind = "ExpK";
	temp->type_name = "(";
	OperatorS.push(temp);
	expflag++;
}
void Process90()
{
	CharacterS.push("INTC");
	TreeNode *temp = new TreeNode;
	temp->nodekind = "ExpK";
	temp->kind.exp = "Const";
	temp->type_name = CurChain->Type;
	DataS.push(temp);
}
void Process91()
{
	CharacterS.push("Variable");
}
void Process92()
{
	CharacterS.push("VariMore");
	CharacterS.push("ID");
	TreeNode *temp = new TreeNode;
	temp->nodekind = "ExpK";
	temp->type_name = CurChain->Type;
	DataS.push(temp);
	t = temp;
}
void Process93()
{
	t->kind.exp = "IdV";
}
void Process94()
{
	CharacterS.push("]");
	CharacterS.push("Exp");
	CharacterS.push("[");
	t->kind.exp = "Array";
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
	TreeNode *temp = new TreeNode;
	temp->type_name = "END";
	OperatorS.push(temp);
	getExpResult2 = true;
}
void Process95()
{
	CharacterS.push("FieldVar");
	CharacterS.push(".");
	t->kind.exp = "Field";
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
}
void Process96()
{
	CharacterS.push("FieldVarMore");
	CharacterS.push("ID");
	t = TreeNodeS.top(); TreeNodeS.pop();
	t->type_name = CurChain->Type;
	t->nodekind = "ExpK";
}
void Process97()
{
	t->kind.exp = "IdV";
}
void Process98()
{
	CharacterS.push("]");
	CharacterS.push("Exp");
	CharacterS.push("[");
	t->kind.exp = "Array";
	t->child[0] = new TreeNode;
	TreeNodeS.push(t->child[0]);
	TreeNode *temp = new TreeNode;
	temp->type_name = "END";
	OperatorS.push(temp);
	getExpResult2 = true;
}
void Process99()
{
	CharacterS.push("<");
}
void Process100()
{
	CharacterS.push("=");
}
void Process101()
{
	CharacterS.push("+");
}
void Process102()
{
	CharacterS.push("-");
}
void Process103()
{
	CharacterS.push("*");
}
void Process104()
{
	CharacterS.push("/");
}

