#include "SyntaxTree.h"


vector<production> vecTree;
vector<string> TokenValueVec;
vector<string> TokenTypeVec;

 AnalysisTreeNode* root;//语法分析树的根节点
 int global_id;//DOT Language中每个节点的id
 unsigned int global_token_index;//用于将ID转化为变量名后者函数名等
 unsigned int treeIndex;// 指示vTree的下标

void SNLCompiler()
{
	///***************************************************************************************
	//词法分析
	LexicalAnalysis();


	//语法分析预处理，从文件中读取产生式，构造产生式list
	string str("./CompilerData/productions_new.txt");
	productionList* ptrProducList = new productionList(str);


	//语法分析中的构造LL1分析表
		//采用手动构造的方式，构造LL1分析表(并没有用到产生式list)
		//CreatLL1Table();
		//采用自动构造的方式，根据产生式，生成First集，Follow集，最后求出Predict集，以此构造LL1分析表
		AutoMakeLL1Table(ptrProducList);


	//语法分析中的匹配
		//构造104个产生式对应的处理函数，手动匹配(因此并没有用到产生式list)，由于是用104个处理函数手动匹配，所以并没有匹配过程，直接输出语法树
		//ParseLL();
		//依据产生式，自动进行匹配(因此用到产生式list，参数为指向产生式list的指针)
		SyntaxAnalysis(ptrProducList);


	//语法分析自动匹配构造词法分析树
	//CreateSyntaxTree(ptrProducList);
	//***************************************************************************************/


}


void LexicalAnalysis()
{
	InputS = "program bubble\n var integer i1,j1,qwer123;\narray [1..20] of integer a;\nprocedure q(integer num);\nvar integer i,j,k;\ninteger t;\nbegin\ni:=1;\nwhile i<num do\nj:=num-i+1;\nk:=1;\nwhile k<j do\nif a[k+1]<a[k]\nthen\nt:=a[k];\na[k]:=a[k+1];\na[k+1]:=t\n";
	InputS += "else temp:=0\nfi;\nk:=k+1\nendwh;\ni:=i+1\nendwh\nend\nbegin\nread(num);\ni:=1;\nwhile i<(num+1) do\nread(j);\na[i]:=j;\ni:=i+1*5\nendwh;\nq(num);\ni:=1;\nwhile i<(num+1) do\nwrite(a[i]);\ni:=i+1\nendwh\nend.";
	TokenChain = new Token();
	CurChain = TokenChain;
	InputHandle();//至此，词法分析完成，TokenChain已被赋值，指向Token链表的头部
	//DisplayToken();//展示Token序列串
}


void SyntaxAnalysis(productionList* ptrProducList)
{
	

	fstream outputFile;
	outputFile.open("./CompilerData/AnalysisLog.txt");
	if (!outputFile)
	{
		cout << "文件打开失败！" << endl;
	}
	else
	{
		outputFile << "log Num)   [Stack Top Element]   <Input Element>     {action}"<<endl;
	}
	/***********************************************************************************************/
	/***************************************语法分析************************************************/

	//在Token序列的末尾添加一个Token--"#"
	while (CurChain->next != NULL)//找到Token序列的最后一个Token
	{
		CurChain = CurChain->next;
	}
	Token *tempT = new Token;
	tempT->Line = 0;
	tempT->Type = "Sharp";
	tempT->Value = "#";
	tempT->next = NULL;
	CurChain->next = tempT;
	CurChain = CurChain->next;

	//构造分析栈，并设置好最开始的栈底元素“#”+“文法开始符号”
	list<string> AnalysisStack;//用一个链表来充当分析栈，方便调试时确定栈内元素。实际上用Stack更为标准
	AnalysisStack.push_back("#");
	AnalysisStack.push_back("Program");//注意，最开始压栈的是开始符Program(非终极符),而不是终极符PROGRAM。二者是不同的。

	Token *ReadToken = TokenChain->next;


	int count = 0;//匹配日志编号

	//开始进行匹配
	while (!AnalysisStack.empty())
	{

		int seqNum = 0;
		string InputStr = ReadToken->Value;//输入流(文法符号)
		if (ReadToken->Type == "Reserved word")
		{
			//为了方便处理，把保留字转化为大写(与产生式一致，方便下一步匹配)
			transform(InputStr.begin(), InputStr.end(), InputStr.begin(), ::toupper);
		}
		string StackTopStr = AnalysisStack.back();//栈顶符号

		count++;//每一次循环，日志序号递增
		outputFile << count << ") [" << StackTopStr << "]  <" << InputStr << ">                             ";//向日志文件中写入信息

		//判断栈顶文法符号是终极符还是非终极符
		bool NonTerminalFlag = false;
		if (ptrProducList->nonTerminalSet.find(StackTopStr) != ptrProducList->nonTerminalSet.end())
		{
			NonTerminalFlag = true;
		}
		else
		{
			NonTerminalFlag = false;
		}



		if (!NonTerminalFlag)//如果栈顶符号是终极符
		{
			if (StackTopStr == "#"&&InputStr == "#")
			{
				outputFile << "Success! Match!" << endl;
				break;//匹配成功，跳出循环
			}
			else if (StackTopStr == InputStr)
			{
				//match成功
				AnalysisStack.pop_back();//弹出栈顶元素
				TokenValueVec.push_back(InputStr);//存储下每一个Token的Value，为建立语法分析树做准备
				TokenTypeVec.push_back(ReadToken->Type);//存储下每一个Token的Type，为建立语法分析树做准备
				ReadToken = ReadToken->next;//指向下一个输入流文法符号
				outputFile << "match!" << endl;
				continue;
			}
			else if (StackTopStr == "?")
			{
				AnalysisStack.pop_back();
				outputFile <<"pop Epsilon char! "<< endl;
			}
			else
			{
				outputFile << "Fail!!" << endl;
				break;//匹配失败，跳出循环
			}
		}
		else//栈顶符号是非终极符
		{
			//根据LL1分析表找到了对应产生式的序号
			if (ptrProducList->NontmlTypeMap.find(StackTopStr) != ptrProducList->NontmlTypeMap.end() && ptrProducList->LexTypeMap.find(InputStr) != ptrProducList->LexTypeMap.end())//能找到才赋值
			{
				seqNum = LL1Table[ptrProducList->NontmlTypeMap.find(StackTopStr)->second][ptrProducList->LexTypeMap.find(InputStr)->second];
				outputFile << "/" << seqNum << "/ ";
			}
			else//找不到对应枚举类型就报错
			{
				outputFile << "Error! Can't find suitable enum type" << endl;
				break;//匹配失败，跳出循环
			}

			if (seqNum == 0)//产生式序号为0表示没有找到对应的产生式，说明LL1分析表构造不正确
			{
				outputFile << "Error! Can't find No.0 production " << endl;
				break;
			}

			AnalysisStack.pop_back();//弹出栈顶符号
			//倒序遍历产生式右部，并将这些文法符号压入栈中
			vector<string>::reverse_iterator vecIt;
			for (vecIt = ptrProducList->FindAProduction(seqNum).productionRight.rbegin(); vecIt != ptrProducList->FindAProduction(seqNum).productionRight.rend(); ++vecIt)
			{
				outputFile << "{" << *vecIt << "}  ";
				AnalysisStack.push_back(*vecIt);
			}
			vecTree.push_back(ptrProducList->FindAProduction(seqNum));//将用到的产生式记录下来，为下一步构造语法分析树做准备
			outputFile << endl;//输出匹配过程时换行
			continue;
		}

	}

	outputFile.close();

/***************************************************存储信息，方便图形界面显示***********************************************************************/
	ptrProducList->storeNonTerminalSet();
	ptrProducList->storeTerminalSet();
	ptrProducList->storeFirstMap();
	ptrProducList->storeFollowMap();
	ptrProducList->storePredictMap();
/***************************************************存储信息，方便图形界面显示***********************************************************************/

}


void AutoMakeLL1Table(productionList* ptrProducList)
{
	/***********************************************************************************************/
	/***********************************这些注释没有什么用处，调试时使用，暂时保存一下*****************/
	//测试文法，不是SNL语言的巴斯克范式
	//string str("./CompilerData/productionTest.txt");
	//string str("./CompilerData/productionTest2.txt");
	//string str("./CompilerData/productionTest3.txt");
	//string str("./CompilerData/productions_ppt.txt");

	//SNL语言的巴斯克范式(三个版本)
	//string str("./CompilerData/production.txt");
	//string str("./CompilerData/production(backup).txt"); 
	//string str("./CompilerData/productions_ppt.txt");

	//构造函数
	//productionList* ptrProducList = new productionList();
	//productionList* ptrProducList = new productionList(str);

	//展示函数
	//ptrProducList->showProducList();
	//ptrProducList->showNonTerminal();
	//ptrProducList->showTerminal();
	//ptrProducList->showFirstMap();
	//ptrProducList->showFollowMap();
	//ptrProducList->showPredictMap();




	/*string str("./CompilerData/productions_ppt.txt");
	productionList* ptrProducList = new productionList(str);
	ptrProducList->showProducList();
	ptrProducList->showNonTerminal();
	ptrProducList->showTerminal();
	ptrProducList->showFirstMap();
	ptrProducList->showFollowMap();
	ptrProducList->showPredictMap();*/

	/*************************************这些注释没有什么用处，调试时使用，暂时保存一下***************************/
	/***********************************************************************************************************/

	ptrProducList->fillLL1Table();//构造LL1分析表
	//ptrProducList->showLL1Table();
}


void CreateSyntaxTree(productionList* ptrProducList)
{
	global_id = 1;
	//根节点及其直接子节点构造完毕
	root = new AnalysisTreeNode(vecTree[0].productionLeft, global_id++);
	for (vector<string>::iterator strVecIter = vecTree[0].productionRight.begin(); strVecIter != vecTree[0].productionRight.end(); strVecIter++)
	{
		root->children.emplace_back(new AnalysisTreeNode(*strVecIter,global_id++));
	}

	//下面用递归的方法继续向下构造语法分析树
	 treeIndex = 1;
	 global_token_index = 0;
	for (unsigned int i = 0; i < root->children.size(); i++) 
	{
		if (ptrProducList->TerminalSet.find(root->children[i]->TokenValue) != ptrProducList->TerminalSet.end())
		{
			//是终极符，不需要继续向下构造节点,进入下一个循环
			continue;
		}
		else
		{
			//是非终极符，需要继续向下建树
			dfsBuildTree(root->children[i], ptrProducList);
		}
	}


	/**********************************************************************************************************/
	/**********************************建树完成，开始生成Dot语句************************************************/

	stringstream ss;//把DOT Language表示的语法树的字符串放到这里边
	ss.clear();
	ss << "digraph GrammarTree {" << endl;
	queue<AnalysisTreeNode *> q;
	q.push(root);
	while (!q.empty()) 
	{
		AnalysisTreeNode *c = q.front();
		q.pop();

		// 该节点的样式和内容
		if (ptrProducList->TerminalSet.find(c->TokenValue) != ptrProducList->TerminalSet.end())
		{
			if (c->TokenValue == "?") 
			{
				ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=cornsilk; label=\""
					<< "ε" << "\"];" << endl;
			}
			else if (c->TokenValue == "ID"|| c->TokenValue == "INTC")
			{
				ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=lightpink; label=\""
					<< c->TokenType << "\"];" << endl;
			}
			else 
			{
				ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=chartreuse1; label=\""
					<< c->TokenValue << "\"];" << endl;
			}
		}
		else 
		{
			ss << "\"" << c->id << "\" [style=filled; fillcolor=cyan; label=\""
				<< c->TokenValue << "\"];" << endl;
		}


		if (c->children.size() == 0) 
		{
			// 叶子结点
			continue;
		}

		// 跟其他节点的关系
		string children = "";
		for (unsigned int i = 0; i < c->children.size(); i++) 
		{
			children += "\"" + to_string(c->children[i]->id) + "\"; "; // "id; "
		}
		ss << "\"" << c->id << "\" -> {" << children << "}" << endl;
		ss << "{rank=same; " << children << "}" << endl;

		// 入栈
		for (AnalysisTreeNode *nd : c->children) 
		{
			q.push(nd);
		}

	}
	ss << "}" << endl;


	fstream outFile;
	outFile.open("./CompilerData/Graphviz.txt");
	if (!outFile)
	{
		cout << "文件打开失败！" << endl;
	}
	outFile<<ss.str();
	outFile.close();



}


void dfsBuildTree(AnalysisTreeNode*& node, productionList* ptrProducList)
{
	// 构造子节点
	for (vector<string>::iterator strVecIter = vecTree[treeIndex].productionRight.begin(); strVecIter != vecTree[treeIndex].productionRight.end(); strVecIter++)
	{
		node->children.emplace_back(new AnalysisTreeNode(*strVecIter, global_id++));
	}
	
	// 构造完推导式后,下标就加加
	treeIndex++;
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		if (ptrProducList->TerminalSet.find(node->children[i]->TokenValue) != ptrProducList->TerminalSet.end())
		{
			//是终极符
			if (node->children[i]->TokenValue == "?") {}
			else
			{
				// 对于ID、INTC、CHARC的Token,将其Type赋值为其实际值
				node->children[i]->TokenType = TokenTypeVec[global_token_index];
				global_token_index++;
			}
			continue;
			
		}
		dfsBuildTree(node->children[i], ptrProducList);
	}

}




