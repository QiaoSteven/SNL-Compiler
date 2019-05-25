#include "SyntaxTree.h"


vector<production> vecTree;
vector<string> TokenValueVec;
vector<string> TokenTypeVec;

 AnalysisTreeNode* root;//�﷨�������ĸ��ڵ�
 int global_id;//DOT Language��ÿ���ڵ��id
 unsigned int global_token_index;//���ڽ�IDת��Ϊ���������ߺ�������
 unsigned int treeIndex;// ָʾvTree���±�

void SNLCompiler()
{
	///***************************************************************************************
	//�ʷ�����
	LexicalAnalysis();


	//�﷨����Ԥ�������ļ��ж�ȡ����ʽ���������ʽlist
	string str("./CompilerData/productions_new.txt");
	productionList* ptrProducList = new productionList(str);


	//�﷨�����еĹ���LL1������
		//�����ֶ�����ķ�ʽ������LL1������(��û���õ�����ʽlist)
		//CreatLL1Table();
		//�����Զ�����ķ�ʽ�����ݲ���ʽ������First����Follow����������Predict�����Դ˹���LL1������
		AutoMakeLL1Table(ptrProducList);


	//�﷨�����е�ƥ��
		//����104������ʽ��Ӧ�Ĵ��������ֶ�ƥ��(��˲�û���õ�����ʽlist)����������104���������ֶ�ƥ�䣬���Բ�û��ƥ����̣�ֱ������﷨��
		//ParseLL();
		//���ݲ���ʽ���Զ�����ƥ��(����õ�����ʽlist������Ϊָ�����ʽlist��ָ��)
		SyntaxAnalysis(ptrProducList);


	//�﷨�����Զ�ƥ�乹��ʷ�������
	//CreateSyntaxTree(ptrProducList);
	//***************************************************************************************/


}


void LexicalAnalysis()
{
	InputS = "program bubble\n var integer i1,j1,qwer123;\narray [1..20] of integer a;\nprocedure q(integer num);\nvar integer i,j,k;\ninteger t;\nbegin\ni:=1;\nwhile i<num do\nj:=num-i+1;\nk:=1;\nwhile k<j do\nif a[k+1]<a[k]\nthen\nt:=a[k];\na[k]:=a[k+1];\na[k+1]:=t\n";
	InputS += "else temp:=0\nfi;\nk:=k+1\nendwh;\ni:=i+1\nendwh\nend\nbegin\nread(num);\ni:=1;\nwhile i<(num+1) do\nread(j);\na[i]:=j;\ni:=i+1*5\nendwh;\nq(num);\ni:=1;\nwhile i<(num+1) do\nwrite(a[i]);\ni:=i+1\nendwh\nend.";
	TokenChain = new Token();
	CurChain = TokenChain;
	InputHandle();//���ˣ��ʷ�������ɣ�TokenChain�ѱ���ֵ��ָ��Token�����ͷ��
	//DisplayToken();//չʾToken���д�
}


void SyntaxAnalysis(productionList* ptrProducList)
{
	

	fstream outputFile;
	outputFile.open("./CompilerData/AnalysisLog.txt");
	if (!outputFile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
	}
	else
	{
		outputFile << "log Num)   [Stack Top Element]   <Input Element>     {action}"<<endl;
	}
	/***********************************************************************************************/
	/***************************************�﷨����************************************************/

	//��Token���е�ĩβ���һ��Token--"#"
	while (CurChain->next != NULL)//�ҵ�Token���е����һ��Token
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

	//�������ջ�������ú��ʼ��ջ��Ԫ�ء�#��+���ķ���ʼ���š�
	list<string> AnalysisStack;//��һ���������䵱����ջ���������ʱȷ��ջ��Ԫ�ء�ʵ������Stack��Ϊ��׼
	AnalysisStack.push_back("#");
	AnalysisStack.push_back("Program");//ע�⣬�ʼѹջ���ǿ�ʼ��Program(���ռ���),�������ռ���PROGRAM�������ǲ�ͬ�ġ�

	Token *ReadToken = TokenChain->next;


	int count = 0;//ƥ����־���

	//��ʼ����ƥ��
	while (!AnalysisStack.empty())
	{

		int seqNum = 0;
		string InputStr = ReadToken->Value;//������(�ķ�����)
		if (ReadToken->Type == "Reserved word")
		{
			//Ϊ�˷��㴦���ѱ�����ת��Ϊ��д(�����ʽһ�£�������һ��ƥ��)
			transform(InputStr.begin(), InputStr.end(), InputStr.begin(), ::toupper);
		}
		string StackTopStr = AnalysisStack.back();//ջ������

		count++;//ÿһ��ѭ������־��ŵ���
		outputFile << count << ") [" << StackTopStr << "]  <" << InputStr << ">                             ";//����־�ļ���д����Ϣ

		//�ж�ջ���ķ��������ռ������Ƿ��ռ���
		bool NonTerminalFlag = false;
		if (ptrProducList->nonTerminalSet.find(StackTopStr) != ptrProducList->nonTerminalSet.end())
		{
			NonTerminalFlag = true;
		}
		else
		{
			NonTerminalFlag = false;
		}



		if (!NonTerminalFlag)//���ջ���������ռ���
		{
			if (StackTopStr == "#"&&InputStr == "#")
			{
				outputFile << "Success! Match!" << endl;
				break;//ƥ��ɹ�������ѭ��
			}
			else if (StackTopStr == InputStr)
			{
				//match�ɹ�
				AnalysisStack.pop_back();//����ջ��Ԫ��
				TokenValueVec.push_back(InputStr);//�洢��ÿһ��Token��Value��Ϊ�����﷨��������׼��
				TokenTypeVec.push_back(ReadToken->Type);//�洢��ÿһ��Token��Type��Ϊ�����﷨��������׼��
				ReadToken = ReadToken->next;//ָ����һ���������ķ�����
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
				break;//ƥ��ʧ�ܣ�����ѭ��
			}
		}
		else//ջ�������Ƿ��ռ���
		{
			//����LL1�������ҵ��˶�Ӧ����ʽ�����
			if (ptrProducList->NontmlTypeMap.find(StackTopStr) != ptrProducList->NontmlTypeMap.end() && ptrProducList->LexTypeMap.find(InputStr) != ptrProducList->LexTypeMap.end())//���ҵ��Ÿ�ֵ
			{
				seqNum = LL1Table[ptrProducList->NontmlTypeMap.find(StackTopStr)->second][ptrProducList->LexTypeMap.find(InputStr)->second];
				outputFile << "/" << seqNum << "/ ";
			}
			else//�Ҳ�����Ӧö�����;ͱ���
			{
				outputFile << "Error! Can't find suitable enum type" << endl;
				break;//ƥ��ʧ�ܣ�����ѭ��
			}

			if (seqNum == 0)//����ʽ���Ϊ0��ʾû���ҵ���Ӧ�Ĳ���ʽ��˵��LL1�������첻��ȷ
			{
				outputFile << "Error! Can't find No.0 production " << endl;
				break;
			}

			AnalysisStack.pop_back();//����ջ������
			//�����������ʽ�Ҳ���������Щ�ķ�����ѹ��ջ��
			vector<string>::reverse_iterator vecIt;
			for (vecIt = ptrProducList->FindAProduction(seqNum).productionRight.rbegin(); vecIt != ptrProducList->FindAProduction(seqNum).productionRight.rend(); ++vecIt)
			{
				outputFile << "{" << *vecIt << "}  ";
				AnalysisStack.push_back(*vecIt);
			}
			vecTree.push_back(ptrProducList->FindAProduction(seqNum));//���õ��Ĳ���ʽ��¼������Ϊ��һ�������﷨��������׼��
			outputFile << endl;//���ƥ�����ʱ����
			continue;
		}

	}

	outputFile.close();

/***************************************************�洢��Ϣ������ͼ�ν�����ʾ***********************************************************************/
	ptrProducList->storeNonTerminalSet();
	ptrProducList->storeTerminalSet();
	ptrProducList->storeFirstMap();
	ptrProducList->storeFollowMap();
	ptrProducList->storePredictMap();
/***************************************************�洢��Ϣ������ͼ�ν�����ʾ***********************************************************************/

}


void AutoMakeLL1Table(productionList* ptrProducList)
{
	/***********************************************************************************************/
	/***********************************��Щע��û��ʲô�ô�������ʱʹ�ã���ʱ����һ��*****************/
	//�����ķ�������SNL���Եİ�˹�˷�ʽ
	//string str("./CompilerData/productionTest.txt");
	//string str("./CompilerData/productionTest2.txt");
	//string str("./CompilerData/productionTest3.txt");
	//string str("./CompilerData/productions_ppt.txt");

	//SNL���Եİ�˹�˷�ʽ(�����汾)
	//string str("./CompilerData/production.txt");
	//string str("./CompilerData/production(backup).txt"); 
	//string str("./CompilerData/productions_ppt.txt");

	//���캯��
	//productionList* ptrProducList = new productionList();
	//productionList* ptrProducList = new productionList(str);

	//չʾ����
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

	/*************************************��Щע��û��ʲô�ô�������ʱʹ�ã���ʱ����һ��***************************/
	/***********************************************************************************************************/

	ptrProducList->fillLL1Table();//����LL1������
	//ptrProducList->showLL1Table();
}


void CreateSyntaxTree(productionList* ptrProducList)
{
	global_id = 1;
	//���ڵ㼰��ֱ���ӽڵ㹹�����
	root = new AnalysisTreeNode(vecTree[0].productionLeft, global_id++);
	for (vector<string>::iterator strVecIter = vecTree[0].productionRight.begin(); strVecIter != vecTree[0].productionRight.end(); strVecIter++)
	{
		root->children.emplace_back(new AnalysisTreeNode(*strVecIter,global_id++));
	}

	//�����õݹ�ķ����������¹����﷨������
	 treeIndex = 1;
	 global_token_index = 0;
	for (unsigned int i = 0; i < root->children.size(); i++) 
	{
		if (ptrProducList->TerminalSet.find(root->children[i]->TokenValue) != ptrProducList->TerminalSet.end())
		{
			//���ռ���������Ҫ�������¹���ڵ�,������һ��ѭ��
			continue;
		}
		else
		{
			//�Ƿ��ռ�������Ҫ�������½���
			dfsBuildTree(root->children[i], ptrProducList);
		}
	}


	/**********************************************************************************************************/
	/**********************************������ɣ���ʼ����Dot���************************************************/

	stringstream ss;//��DOT Language��ʾ���﷨�����ַ����ŵ������
	ss.clear();
	ss << "digraph GrammarTree {" << endl;
	queue<AnalysisTreeNode *> q;
	q.push(root);
	while (!q.empty()) 
	{
		AnalysisTreeNode *c = q.front();
		q.pop();

		// �ýڵ����ʽ������
		if (ptrProducList->TerminalSet.find(c->TokenValue) != ptrProducList->TerminalSet.end())
		{
			if (c->TokenValue == "?") 
			{
				ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=cornsilk; label=\""
					<< "��" << "\"];" << endl;
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
			// Ҷ�ӽ��
			continue;
		}

		// �������ڵ�Ĺ�ϵ
		string children = "";
		for (unsigned int i = 0; i < c->children.size(); i++) 
		{
			children += "\"" + to_string(c->children[i]->id) + "\"; "; // "id; "
		}
		ss << "\"" << c->id << "\" -> {" << children << "}" << endl;
		ss << "{rank=same; " << children << "}" << endl;

		// ��ջ
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
		cout << "�ļ���ʧ�ܣ�" << endl;
	}
	outFile<<ss.str();
	outFile.close();



}


void dfsBuildTree(AnalysisTreeNode*& node, productionList* ptrProducList)
{
	// �����ӽڵ�
	for (vector<string>::iterator strVecIter = vecTree[treeIndex].productionRight.begin(); strVecIter != vecTree[treeIndex].productionRight.end(); strVecIter++)
	{
		node->children.emplace_back(new AnalysisTreeNode(*strVecIter, global_id++));
	}
	
	// �������Ƶ�ʽ��,�±�ͼӼ�
	treeIndex++;
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		if (ptrProducList->TerminalSet.find(node->children[i]->TokenValue) != ptrProducList->TerminalSet.end())
		{
			//���ռ���
			if (node->children[i]->TokenValue == "?") {}
			else
			{
				// ����ID��INTC��CHARC��Token,����Type��ֵΪ��ʵ��ֵ
				node->children[i]->TokenType = TokenTypeVec[global_token_index];
				global_token_index++;
			}
			continue;
			
		}
		dfsBuildTree(node->children[i], ptrProducList);
	}

}




