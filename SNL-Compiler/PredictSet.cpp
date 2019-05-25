#include"PredictSet.h"



productionList::productionList()
{

}

productionList::productionList(string dir)
{
	//������ʽlist�ͷ��ռ���set
	readProducList(dir);

	//����ռ���set
	fillTerminalSet();

	//Ԥ�����First��map��Follow��map��Predict��map��keyֵ��������һ���Ĺ���
	InitialFirstFollowPredictMap();

	//����ÿ�����ռ�����first��
	establishFirstMap();
	
	//����ÿ�����ռ�����follow��
	establishFollowMap();

	//����ÿ�����ռ�����predict��
	establishPredictMap();

	//ʵ�ִ�string��enum����LexType��ӳ�䣬���㹹��LL1������
	makeMapFromStringToLexType();

	//ʵ�ִ�string��enum����NontmlType��ӳ�䣬���㹹��LL1������
	makeMapFromStringToNontmlType();

	

}


void productionList::readProducList(string dir)
{
	//������ʽlist�ͷ��ռ���set
	/*****************************************************************************************/
	/*****************************************************************************************/
	ifstream infile;
	infile.open(dir);
	if (!infile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
	}
	string line;//���ļ��ж�ȡһ�в���ʽ
	string substr;//���طָ����ַ���
	production* ptrProduc;//ָ��һ������ʽ
	bool equalFlag = false;//��־�Ƿ��Ѷ���"::="�������
	int seqNum;//��ǰ�кţ�Ҳ������ʽ�ı��


	while (getline(infile, line))
	{
		istringstream stream(line);//���ڷָ��ַ�
		ptrProduc = new production();//һ�ж�Ӧһ������ʽ


		stream >> substr;//ÿ�еĵ�һ��Ԫ�رض����к�
		substr.pop_back();
		istringstream toInt(substr);
		toInt >> seqNum;
		ptrProduc->sequenceNum = seqNum;
		//cout << "Sequence:" << seqNum << endl;

		while (stream >> substr)
		{
			if (substr == "::=")
			{
				equalFlag = true;
				//cout << "::=" << endl;
				continue;//�ԵȺŲ����κδ���
			}

			if (!equalFlag)
			{
				ptrProduc->productionLeft = substr;
				//cout << "Left:" << substr << endl;
				nonTerminalSet.insert(substr);//�Ӳ���ʽ��������ռ�����������ռ�����
			}
			else
			{
				ptrProduc->productionRight.push_back(substr);
				//cout << "Right:" << substr << endl;
			}
		}


		producList.push_back(*ptrProduc);//���������ʽ������producList��
		equalFlag = false;//һ�����ݽ���������־��λ
		line.clear();
		substr.clear();
		//cout <<"This line is over!\n\n\n\n"<< endl;
	}
	infile.close();
	/*****************************************************************************************/
	/*****************************************************************************************/
}


void productionList::fillTerminalSet()
{
	list<production>::iterator iter;
	vector<string>::iterator it;
	for (iter = producList.begin(); iter != producList.end(); iter++)
	{
		for (it = (*iter).productionRight.begin(); it != (*iter).productionRight.end(); it++)
		{
			if (nonTerminalSet.find(*it) != nonTerminalSet.end())//���ķ������ڷ��ռ�������
			{

			}
			else//���ķ�����Ӧ�����ռ�������
			{
				TerminalSet.insert(*it);
			}
		}
	}
}


void productionList::showProducList()
{
	cout << "production List: " << endl<<endl;
	list<production>::iterator iter;
	vector<string>::iterator it;
	for (iter = producList.begin(); iter != producList.end(); iter++)
	{
		cout <<"("<< (*iter).sequenceNum <<")"<<"    "<<(*iter).productionLeft<<"   ::=";
		for (it = (*iter).productionRight.begin();it!= (*iter).productionRight.end();it++)
		{
			cout << "   " << *it;
		}
		cout << endl;
	}
	cout << endl << endl;

}


void productionList::showNonTerminal()
{
	cout << "NonTerminal Set: " << endl << endl;
	set<string>::iterator iter;
	//int count = 0;
	for (iter = nonTerminalSet.begin(); iter != nonTerminalSet.end(); iter++)
	{
		//count++;
		//cout << count;
		cout << *iter << endl;
	}
	cout << endl << endl;
}


void productionList::showTerminal()
{
	cout << "Terminal Set: " << endl << endl;
	set<string>::iterator iter;
	//int count = 0;
	for (iter = TerminalSet.begin(); iter != TerminalSet.end(); iter++)
	{
		//count++;
		//cout << count;
		cout << *iter << endl;
	}
	cout << endl << endl;
}


void productionList::showFirstMap()
{
	cout << "First Map: " << endl << endl;
	map<string, set<string>>::iterator firstMapIter;
	set<string>::iterator setIter;
	for (firstMapIter = firstMap.begin(); firstMapIter != firstMap.end(); firstMapIter++)
	{
		cout << firstMapIter->first <<": { ";
		for (setIter = firstMapIter->second.begin(); setIter != firstMapIter->second.end(); setIter++)
		{
			cout << *setIter << " ";
		}
		cout <<" }"<<endl;
	}
	cout << endl << endl;
}


void productionList::showFollowMap()
{
	cout << "Follow Map: " << endl << endl;
	map<string, set<string>>::iterator followMapIter;
	set<string>::iterator setIter;
	for (followMapIter = followMap.begin(); followMapIter != followMap.end(); followMapIter++)
	{
		cout << followMapIter->first << ": { ";
		for (setIter = followMapIter->second.begin(); setIter != followMapIter->second.end(); setIter++)
		{
			cout << *setIter << " ";
		}
		cout << " }" << endl;
	}
	cout << endl << endl;
}


void productionList::showPredictMap()
{
	cout << "Predict Map: " << endl << endl;
	map<production, set<string>>::iterator predictMapIter;
	set<string>::iterator setIter;
	for (predictMapIter = predictMap.begin(); predictMapIter != predictMap.end(); predictMapIter++)
	{
		cout <<"("<<predictMapIter->first.sequenceNum <<")"<< " { ";
		for (setIter = predictMapIter->second.begin(); setIter != predictMapIter->second.end(); setIter++)
		{
			cout << *setIter << " ";
		}
		cout << " }" << endl;
	}
	cout << endl << endl;
}


void productionList::showLL1Table()
{
	int  count = 0;
	for (int i = 0; i < TABLESIZE; i++)
	{
		for (int j = 0; j < TABLESIZE; j++)
		{
			if (LL1Table[i][j] != 0)
			{
				cout << "(" << i << "," << j << ")-->" << LL1Table[i][j] << endl;
				count++;
			}

		}

	}
	cout <<"������" << count << endl;
}


void productionList::storeFirstMap()
{

	fstream outputFile;
	outputFile.open("./CompilerData/FirstSet.txt");
	if (!outputFile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "First Map: " << endl << endl;
	}

	for (map<string, set<string>>::iterator firstMapIter = firstMap.begin(); firstMapIter != firstMap.end(); firstMapIter++)
	{
		outputFile << firstMapIter->first << ": { ";
		for (set<string>::iterator setIter = firstMapIter->second.begin(); setIter != firstMapIter->second.end(); setIter++)
		{
			outputFile << *setIter << " ";
		}
		outputFile << " }" << endl;
	}
	outputFile << endl << endl;
	outputFile.close();

}


void productionList::storeFollowMap()
{
	fstream outputFile;
	outputFile.open("./CompilerData/FollowSet.txt");
	if (!outputFile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "Follow Map: " << endl << endl;
	}

	for (map<string, set<string>>::iterator followMapIter = followMap.begin(); followMapIter != followMap.end(); followMapIter++)
	{
		outputFile << followMapIter->first << ": { ";
		for (set<string>::iterator setIter = followMapIter->second.begin(); setIter != followMapIter->second.end(); setIter++)
		{
			outputFile << *setIter << " ";
		}
		outputFile << " }" << endl;
	}
	
	outputFile << endl << endl;
	outputFile.close();
}


void productionList::storePredictMap()
{
	fstream outputFile;
	outputFile.open("./CompilerData/PredictSet.txt");
	if (!outputFile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "Predict Map: " << endl << endl;
	}

	for (map<production, set<string>>::iterator predictMapIter = predictMap.begin(); predictMapIter != predictMap.end(); predictMapIter++)
	{
		outputFile << predictMapIter->first.sequenceNum << ")    ";
		outputFile << " { ";
		for (set<string>::iterator setIter = predictMapIter->second.begin(); setIter != predictMapIter->second.end(); setIter++)
		{
			outputFile << *setIter << " ";
		}
		outputFile << " }" << endl;
	}
	outputFile << endl << endl;

	outputFile.close();
}


void  productionList::storeNonTerminalSet()
{

	fstream outputFile;
	outputFile.open("./CompilerData/NonTerminalSet.txt");
	if (!outputFile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "NonTerminal Set: " << endl << endl;
	}

	for (set<string>::iterator iter = nonTerminalSet.begin(); iter != nonTerminalSet.end(); iter++)
	{
		outputFile << *iter << endl;
	}

	outputFile << endl << endl;
	outputFile.close();

}


void  productionList::storeTerminalSet()
{
	fstream outputFile;
	outputFile.open("./CompilerData/TerminalSet.txt");
	if (!outputFile)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "Terminal Set: " << endl << endl;
	}

	for (set<string>::iterator iter = TerminalSet.begin(); iter != TerminalSet.end(); iter++)
	{
		outputFile << *iter << endl;
	}

	outputFile << endl << endl;
	outputFile.close();
}



void productionList::InitialFirstFollowPredictMap()
{
	//һ���յ�string��set��
	set<string> nullstrset;


	//���firstmap��followmap
	set<string>::iterator iter;
	for (iter = nonTerminalSet.begin(); iter != nonTerminalSet.end(); iter++)
	{
		firstMap.insert(pair<string, set<string>>(*iter, nullstrset));
		followMap.insert(pair<string, set<string>>(*iter, nullstrset));
	}

	//���ڿ�ʼ��S����"#"������follow����
	followMap.find(producList.begin()->productionLeft)->second.insert("#");

	//���predictmap
	list<production>::iterator produciter;
	for (produciter = producList.begin(); produciter != producList.end(); produciter++)
	{
		//map��keyֵ���Զ������ͣ����ܼӽ�ȥ��ԭ����û�����رȽ��������û���Ƚϣ���Ȼû����insert����--------------------�ѽ��
		predictMap.insert(pair<production, set<string >>((*produciter), nullstrset));
	}
	
}



void productionList::establishFirstMap()
{
	for (int i = 0; i < 10; i++)//Ϊ��֤�պϣ���������
	{
		set<string>::iterator NonTermimalIter;//�������ռ������ϵĵ�����
		for (NonTermimalIter = nonTerminalSet.begin(); NonTermimalIter != nonTerminalSet.end(); NonTermimalIter++)
		{
			findFirstSet(*NonTermimalIter);
		}
	}
	

}


void productionList::findFirstSet(string nonTerStr)
{
	list<production>::iterator producIter;//��������ʽ����ĵ�����
	for (producIter = producList.begin(); producIter!= producList.end(); producIter++)
	{
		if ((*producIter).productionLeft == nonTerStr)//�ҵ����������Ĳ���ʽ
		{
			vector<string>::iterator producRightIter;//�����ò���ʽ�Ҳ���ķ�����vector�ĵ�����
			for (producRightIter = (*producIter).productionRight.begin(); producRightIter != (*producIter).productionRight.end(); producRightIter++)//�����ò���ʽ���Ҳ�
			{
				if (nonTerminalSet.find(*producRightIter) != nonTerminalSet.end())//���ķ������Ƿ��ռ���
				{
					/**<-------------(firstMap.find(*producRightIter)->second)�Ǹò���ʽ�Ҳ��һ�����ռ�����Ӧ��firstSet------------------------>**/
					if ((firstMap.find(*producRightIter)->second).find("?")!= (firstMap.find(*producRightIter)->second).end())//������ڸ÷��ռ�����first������,����Ҫ���������ң����Բ���break
					{
						if (*producRightIter == (*producIter).productionRight.back())//�����ķ������ǲ���ʽ���Ҳ�ķ��ռ������Ҹ��ռ��������Ƴ����գ���ô�������ʽ�����ķ��ռ�����first���бض��п�
						{
							(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
						}
						else//�����ķ����Ų��ǲ���ʽ���Ҳ�ķ��ռ������Ѹ��ķ����ŵ�first�����˿�֮���Ԫ�ؼ��뵽��ߵķ��ռ�����first����
						{
							(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
							(firstMap.find(nonTerStr)->second).erase("?");
						}
					}
					else//����ղ��ڸ÷��ռ�����first�����У������������ˣ�ֱ��break
					{
						//firstMap.find(nonTerStr)->second)�Ǹò���ʽ���ķ��ռ�����Ӧ��first��
						(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
						break;//����ʽ�Ҳ��������ķ����ŵ�first���в����գ����Բ��������Ҳ��������
					}
					
				}
				else//�ò���ʽ�Ҳ������ķ��������ռ���
				{
					(firstMap.find(nonTerStr)->second).insert(*producRightIter);
					break;//����ʽ�Ҳ��������ķ��������ռ��������Բ��������Ҳ��������
				}
			}
		}
	}
}


void productionList::establishFollowMap()
{
	for (int i = 0; i < 10; i++)//Ϊ��֤�պϣ���������
	{
		set<string>::iterator NonTermimalIter;//�������ռ������ϵĵ�����
		for (NonTermimalIter = nonTerminalSet.begin(); NonTermimalIter != nonTerminalSet.end(); NonTermimalIter++)
		{
			findFollowSet(*NonTermimalIter);
		}
	}
}


void productionList::findFollowSet(string nonTerStr)
{
	for (list<production>::iterator producIter = producList.begin(); producIter != producList.end(); producIter++)//����ÿһ������ʽ
	{
		int count = countStrInAVec(nonTerStr,producIter->productionRight);
		if (count == 0)
		{
			continue;
		}
		else//����ʽ�Ҳ��ж�����������ķ��ռ���
		{
			/*****************************************************************************************************************************/
			for (int i = 0; i < count; i++)
			{
				list<string> strList;//��¼���ķ������Ҳ���ķ�����
				strList.insert(end(strList), findPosStrInAVec(i+1, nonTerStr, (*producIter).productionRight), (*producIter).productionRight.end());
				if (strList.empty())
				{
					(followMap.find(nonTerStr)->second).insert((followMap.find((*producIter).productionLeft)->second).begin(), (followMap.find((*producIter).productionLeft)->second).end());
				}
				//ɨ����ɺ󣬽�����λ�ڸ÷��ռ����Ҳ�������ķ����ŵ�strList��first��������first�����뵽�÷��ռ�����follow���У�ͬʱҲ������жϲ�ʵʩ�Ƿ���Ҫ������ʽ���ķ��ռ�����follow�����뵽�÷��ռ�����follow��������
				addItemToFollow(producIter->productionLeft, nonTerStr, strList);
			}
			/*****************************************************************************************************************************/
		}

	}


}


vector<string>::iterator productionList::findPosStrInAVec(int seq, string str, vector<string>& strVec)
{
	int count = 0;
	for (vector<string>::iterator vecIter = strVec.begin(); vecIter != strVec.end(); vecIter++)
	{
		if (*vecIter == str)
		{
			count++;
		}
		if (count == seq)
		{
			return vecIter + 1;
		}
	}
	return strVec.end();
}


void productionList::addItemToFollow(string productionLeft, string nonTerStr,list<string>& strList)
{
	if (strList.empty())//�������ʽ�Ҳ�û�������ķ�����(���账��)�������ķ����������Ҳ�(��ǰ����ֱ�Ӵ���)
	{
		return;//���Ż���Ĵ����У��������Ӧ�ò�����֡���Ϊ֮ǰ�Ѿ�ͨ��count�ж��˸ò���ʽ�Ҳൽ����û��������ռ���
	}
	else//�÷��ռ������������ʽ���Ҳ��Ҳ������ұߣ���Ҫ����
	{
		set<string> strSet;
		strSet = getStringFirstSet(strList);//ͨ�����Һ������ҵ�����ַ�����first��
		if (strSet.find("?") != strSet.end())//����ַ����ĵ�first�����п�
		{
			//�Ѳ���ʽ���ķ��ռ�����follow���ӽ�����ʽ�Ҳ��������ռ�����follow����
			(followMap.find(nonTerStr)->second).insert((followMap.find(productionLeft)->second).begin(), (followMap.find(productionLeft)->second).end());
			strSet.erase("?");
			//������ַ�����first�����뵽ǰ���Ǹ����ռ�����follow����
			(followMap.find(nonTerStr)->second).insert(strSet.begin(), strSet.end());
		}
		else//����ַ����ĵ�first����û�п�
		{
			//������ַ�����first�����뵽ǰ���Ǹ����ռ�����follow����
			(followMap.find(nonTerStr)->second).insert(strSet.begin(), strSet.end());
		}
	}
}


int productionList::countStrInAVec(string str, vector<string> &strVec)
{
	int count=0;
	for (vector<string>::iterator strVecIter = strVec.begin(); strVecIter != strVec.end(); strVecIter++)
	{
		if (*strVecIter == str)
		{
			count++;
		}
	}
	return count;

}


set<string> productionList::getStringFirstSet(list<string> strList)
{
	set<string> resultSet;
	list<string> ::iterator listIter;
	for (listIter = strList.begin(); listIter != strList.end(); listIter++)//����stringList
	{
		if (nonTerminalSet.find(*listIter) != nonTerminalSet.end())//��һ���ķ������Ƿ��ռ���
		{
			if (firstMap.find(*listIter)->second.find("?") != firstMap.find(*listIter)->second.end())//�˷��ռ�����first�����п�
			{
				resultSet.insert(firstMap.find(*listIter)->second.begin(), firstMap.find(*listIter)->second.end());
				if (*listIter == strList.back())//�����һ�����ţ��Ͳ���ɾȥ��
				{

				}
				else//�������һ�����ţ���Ҫɾȥ��
				{
					resultSet.erase("?");
				}
			}
			else//�˷��ռ�����first�����޿�,������������
			{
				resultSet.insert(firstMap.find(*listIter)->second.begin(), firstMap.find(*listIter)->second.end());
				break;//����forѭ��
			}
		}
		else//��һ���ķ�������һ���ռ�������������������
		{
			resultSet.insert(*listIter);
			break;//����forѭ��
		}
	}

	return resultSet;
}


void productionList::establishPredictMap()
{
	list<production>::iterator procListIter;
	vector<string>::iterator strIt;
	for (procListIter = producList.begin(); procListIter != producList.end(); procListIter++)//����ÿһ������ʽ
	{
		list<string> strList;
		set<string> strSet;
		for (strIt = (*procListIter).productionRight.begin(); strIt != (*procListIter).productionRight.end(); strIt++)
		{
			strList.push_back(*strIt);
		}
		strSet = getStringFirstSet(strList);
		if (strSet.find("?") != strSet.end())//���ķ����Ŵ���first�����п�,predict(�ò���ʽ)=first(���ķ����Ŵ�)-{��}+follow(�ò���ʽ���ķ��ռ���)
		{
			strSet.erase("?");
			predictMap.find(*procListIter)->second.insert(strSet.begin(), strSet.end());
			predictMap.find(*procListIter)->second.insert(followMap.find(procListIter->productionLeft)->second.begin(), followMap.find(procListIter->productionLeft)->second.end());
		}
		else//���ķ����Ŵ���first����û�пգ�predict(�ò���ʽ)=first(���ķ����Ŵ�)
		{
			predictMap.find(*procListIter)->second.insert(strSet.begin(), strSet.end());
		}
	}
}


void productionList::fillLL1Table()
{
	//Ĭ����ȫ������
	for (int i = 0; i < TABLESIZE; i++)
	{
		for (int j = 0; j < TABLESIZE; j++)
		{
			LL1Table[i][j] = 0;
		}
	}

	set<string>::iterator setIter;
	map<production, set<string>>::iterator predictMapIter;
	for (predictMapIter = predictMap.begin(); predictMapIter != predictMap.end(); predictMapIter++)
	{
		for (setIter = predictMapIter->second.begin(); setIter != predictMapIter->second.end(); setIter++)
		{
			if (NontmlTypeMap.find(predictMapIter->first.productionLeft) != NontmlTypeMap.end() && LexTypeMap.find(*setIter) != LexTypeMap.end())//���ҵ����Ÿ�ֵ
			{
				LL1Table[NontmlTypeMap.find(predictMapIter->first.productionLeft)->second][LexTypeMap.find(*setIter)->second] = predictMapIter->first.sequenceNum;
			}
		}
		
	}

	
}


void productionList::makeMapFromStringToLexType()
{
	/********************************��map�����Ԫ�صĶ��ַ���*********************************************/
	//LexTypeMap["Program"] = PROGRAM;
	//LexTypeMap.insert({ "Program", PROGRAM });
	//LexTypeMap.insert(pair<string, LexType>("Program", PROGRAM));
	//LexTypeMap.insert(map<string, LexType>::value_type("Program", PROGRAM));
	/*****************************************************************************************************/


	//LexTypeMap.insert({ "",ENDFILE});//ʵ���϶�Ӧ����  EOF  �������(�ļ�������)�����ڴʷ������еĶ�ȡ�ַ�
	//LexTypeMap.insert({ "",ERROR});//û�ж�Ӧ�ľ����ַ����Ǵ����ǣ��ڴʷ������У�������δ֪�ķ��ţ���token��lex����ֵ��ΪERROR
	LexTypeMap.insert({ "PROGRAM",PROGRAM});
	LexTypeMap.insert({ "PROCEDURE",PROCEDURE});
	LexTypeMap.insert({ "TYPE", TYPE});
	LexTypeMap.insert({ "VAR", VAR});
	LexTypeMap.insert({ "IF", IF});
	LexTypeMap.insert({ "THEN", THEN});
	LexTypeMap.insert({ "ELSE", ELSE});
	LexTypeMap.insert({ "FI", FI});
	LexTypeMap.insert({ "WHILE", WHILE});
	LexTypeMap.insert({ "DO", DO});
	LexTypeMap.insert({ "ENDWH", ENDWH});
	LexTypeMap.insert({ "BEGIN", BEGIN});
	LexTypeMap.insert({ "END", END});
	LexTypeMap.insert({ "READ", READ});
	LexTypeMap.insert({ "WRITE", WRITE});
	LexTypeMap.insert({ "ARRAY", ARRAY});
	LexTypeMap.insert({ "OF", OF});
	LexTypeMap.insert({ "RECORD", RECORD});
	LexTypeMap.insert({ "RETURN", RETURN});
	LexTypeMap.insert({ "INTEGER", INTEGER});
	LexTypeMap.insert({ "CHAR", CHAR});
	LexTypeMap.insert({ "ID",ID });
	LexTypeMap.insert({ "INTC", INTC});
	LexTypeMap.insert({ "'", CHARC});//ʵ���϶�Ӧ����  '  ������ţ����ڴʷ������еĶ�ȡ�ַ�
	LexTypeMap.insert({ ":=", ASSIGN});
	LexTypeMap.insert({ "=", EQ});
	LexTypeMap.insert({ "<", LT});
	LexTypeMap.insert({ "+",PLUS});
	LexTypeMap.insert({ "-", MINUS});
	LexTypeMap.insert({ "*", TIMES});
	LexTypeMap.insert({ "/", OVER});
	LexTypeMap.insert({ "(", LPAREN});
	LexTypeMap.insert({ ")", RPAREN});
	LexTypeMap.insert({ ".", DOT});
	LexTypeMap.insert({ ":", COLON});//ʵ���϶�Ӧ����  :  ������ţ����ڴʷ������еĶ�ȡ�ַ�,��SNL�����У�û�е���ʹ�ã��������ͨ������::=��:=��
	LexTypeMap.insert({ ";", SEMI});
	LexTypeMap.insert({ ",", COMMA});
	LexTypeMap.insert({ "[", LMIDPAREN});
	LexTypeMap.insert({ "]", RMIDPAREN});
	LexTypeMap.insert({ "..", UNDERANGE});


}


void productionList::makeMapFromStringToNontmlType()
{
	NontmlTypeMap.insert({"Program",Program });
	NontmlTypeMap.insert({ "ProgramHead", ProgramHead });
	NontmlTypeMap.insert({ "ProgramName", ProgramName });
	NontmlTypeMap.insert({ "DeclarePart", DeclarePart});
	NontmlTypeMap.insert({ "TypeDec", TypeDec});
	NontmlTypeMap.insert({ "TypeDeclaration", TypeDeclaration });/***************************************************/
	NontmlTypeMap.insert({ "TypeDecList", TypeDecList});
	NontmlTypeMap.insert({ "TypeDecMore", TypeDecMore});
	NontmlTypeMap.insert({ "TypeId", TypeId});
	NontmlTypeMap.insert({ "TypeName", TypeName});/***************************************************************/
	NontmlTypeMap.insert({ "BaseType", BaseType});
	NontmlTypeMap.insert({ "StructureType", StructureType});
	NontmlTypeMap.insert({ "ArrayType", ArrayType});
	NontmlTypeMap.insert({ "Low", Low});
	NontmlTypeMap.insert({ "Top", Top});
	NontmlTypeMap.insert({ "RecType", RecType});
	NontmlTypeMap.insert({ "FieldDecList", FieldDecList});
	NontmlTypeMap.insert({ "FieldDecMore", FieldDecMore });
	NontmlTypeMap.insert({ "IdList",  IdList});
	NontmlTypeMap.insert({ "IdMore", IdMore});
	NontmlTypeMap.insert({ "VarDec", VarDec});
	NontmlTypeMap.insert({ "VarDeclaration", VarDeclaration});/********************************************************/
	NontmlTypeMap.insert({ "VarDecList", VarDecList});
	NontmlTypeMap.insert({ "VarDecMore", VarDecMore});
	NontmlTypeMap.insert({ "VarIdList", VarIdList});
	NontmlTypeMap.insert({ "VarIdMore", VarIdMore});
	NontmlTypeMap.insert({ "ProcDec", ProcDec});
	NontmlTypeMap.insert({ "ProcDeclaration", ProcDeclaration});/**********************************************************/
	NontmlTypeMap.insert({ "ProcDecMore", ProcDecMore});
	NontmlTypeMap.insert({ "ProcName", ProcName});
	NontmlTypeMap.insert({ "ParamList", ParamList});
	NontmlTypeMap.insert({ "ParamDecList", ParamDecList});
	NontmlTypeMap.insert({ "ParamMore", ParamMore});
	NontmlTypeMap.insert({ "Param", Param });
	NontmlTypeMap.insert({ "FormList", FormList});
	NontmlTypeMap.insert({ "FidMore", FidMore});
	NontmlTypeMap.insert({ "ProcDecPart", ProcDecPart});
	NontmlTypeMap.insert({ "ProcBody", ProcBody});
	NontmlTypeMap.insert({ "ProgramBody", ProgramBody});
	NontmlTypeMap.insert({ "StmList", StmList});
	NontmlTypeMap.insert({ "StmMore", StmMore});
	NontmlTypeMap.insert({ "Stm", Stm});
	NontmlTypeMap.insert({ "AssCall", AssCall});
	NontmlTypeMap.insert({ "AssignmentRest", AssignmentRest});
	NontmlTypeMap.insert({ "ConditionalStm", ConditionalStm});
	//NontmlTypeMap.insert({ "", StmL});
	NontmlTypeMap.insert({ "LoopStm", LoopStm});
	NontmlTypeMap.insert({ "InputStm", InputStm});
	NontmlTypeMap.insert({ "Invar", InVar});
	NontmlTypeMap.insert({ "OutputStm", OutputStm});
	NontmlTypeMap.insert({ "ReturnStm", ReturnStm});
	NontmlTypeMap.insert({ "CallStmRest", CallStmRest});
	NontmlTypeMap.insert({ "ActParamList", ActParamList});
	NontmlTypeMap.insert({ "ActParamMore", ActParamMore});
	NontmlTypeMap.insert({ "RelExp", RelExp});
	NontmlTypeMap.insert({ "OtherRelE", OtherRelE});
	NontmlTypeMap.insert({ "Exp", Exp});
	NontmlTypeMap.insert({ "OtherTerm", OtherTerm});
	NontmlTypeMap.insert({ "Term", Term});
	NontmlTypeMap.insert({ "OtherFactor", OtherFactor});
	NontmlTypeMap.insert({ "Factor", Factor});
	NontmlTypeMap.insert({ "Variable", Variable});
	NontmlTypeMap.insert({ "VariMore", VariMore});
	NontmlTypeMap.insert({ "FieldVar", FieldVar});
	NontmlTypeMap.insert({ "FieldVarMore", FieldVarMore});
	NontmlTypeMap.insert({ "CmpOp", CmpOp});
	NontmlTypeMap.insert({ "AddOp", AddOp});
	NontmlTypeMap.insert({ "MultOp", MultOp});
}


production& productionList::FindAProduction(int num)
{
	list<production>::iterator iter;
	for (iter = producList.begin(); iter != producList.end(); iter++)
	{
		if (iter->sequenceNum == num)
		{
			return *iter;
		}
	}
}