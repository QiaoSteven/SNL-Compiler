#include"Scanner.h"


void InputHandle()
{
	char c;
	bool EndFlag = false;
	c = InputS[CurIndex]; CurIndex++;
	while (!EndFlag&&CurIndex <= InputS.size())
	{
		if ((c <= 'z'&&c >= 'a') || (c <= 'Z'&&c >= 'A'))
		{
			CharHandle(c);
		}
		else if (c <= '9'&&c >= '0')
		{
			DigitHandle(c);
		}
		else if (c == ':')
		{
			ColonHandle(c);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ';' || c == '[' || c == ']' || c == '=' || c == '<' || c == ' ' || c == EOF || c == '>' || c == ',')
		{
			SingleDivideCHandle(c);
		}
		else if (c == '{')
		{
			AnnotationHandle(c);
		}
		else if (c == '.')
		{
			PeriodHandle(c);
		}
		else if (c == '\'')
		{
			QuoteHandle(c);
		}
		else if (c == '\n')
		{
			LineNumber++;
			c = InputS[CurIndex]; CurIndex++;
		}
		else
		{
			cout << "line " << LineNumber << " : illeagel character: " << c << endl;
			c = InputS[CurIndex]; CurIndex++;//TODO Error
		}
	}
}


void DisplayToken()
{
	Token *tempC = TokenChain->next;
	//	cout<<"Here"<<endl; 
	while (tempC != NULL)
	{
		cout << tempC->Line << " " << tempC->Type << " " << tempC->Value << endl;
		tempC = tempC->next;
	}
}


bool FindReserved(string TarS)
{
	for (int i = 0; i < 21; i++)
	{
		if (ReservedList[i] == TarS)
			return true;
	}
	return false;
}


void AddChain(int LineNumber, string type, string value)
{
	Token *tempT = new Token;
	tempT->Line = LineNumber;
	tempT->Type = type;
	tempT->Value = value;
	tempT->next = NULL;
	CurChain->next = tempT;
	CurChain = CurChain->next;
}


void CharHandle(char &c)
{
	string resultS = "";
	resultS += c;
	c = InputS[CurIndex]; CurIndex++;
	while ((c <= 'z'&&c >= 'a') || (c >= 'A'&&c <= 'Z') || (c >= '0'&&c <= '9'))
	{
		resultS += c;
		c = InputS[CurIndex]; CurIndex++;
	}
	if (FindReserved(resultS))//是保留字 
	{
		//	cout<<LineNumber<<" Reserved word:"<<resultS<<endl;
		AddChain(LineNumber, "Reserved word", resultS);
	}
	else
	{
		//cout<<LineNumber<<" ID:"<<resultS<<endl;
		AddChain(LineNumber, resultS, "ID");
	}

}


void DigitHandle(char &c)
{
	string resultS = "";
	resultS += c;
	c = InputS[CurIndex]; CurIndex++;
	while (c <= '9'&&c >= '0')
	{
		resultS += c;
		c = InputS[CurIndex]; CurIndex++;
	}
	//cout<<LineNumber<<" Digit:"<<resultS<<endl;
	AddChain(LineNumber, resultS, "INTC");
}


void ColonHandle(char &c)
{
	string resultS = "";
	resultS += c;
	c = InputS[CurIndex]; CurIndex++;
	if (c == '=')
	{
		resultS += c;
		c = InputS[CurIndex]; CurIndex++;
		//	cout<<LineNumber<<"Assign"<<endl;
		AddChain(LineNumber, "Assign", ":=");
	}
	else
	{
		cout << "line " << LineNumber << " : illeagel character: " << c << endl;
		c = InputS[CurIndex]; CurIndex++;
	}
}


void SingleDivideCHandle(char &c)
{
	if (c == ' ' || c == EOF)
	{
		c = InputS[CurIndex]; CurIndex++;
		return;
	}
	if (c == '+')
	{
		//	cout<<LineNumber<<"Plus"<<endl;
		AddChain(LineNumber, "Plus", "+");
	}
	else if (c == '-')
	{
		//cout<<LineNumber<<"Decrease"<<endl;
		AddChain(LineNumber, "Decrease", "-");
	}
	else if (c == '*')
	{
		//	cout<<LineNumber<<"Multiply"<<endl;
		AddChain(LineNumber, "Multiply", "*");
	}
	else if (c == '/')
	{
		//cout<<LineNumber<<"Divide"<<endl;
		AddChain(LineNumber, "Divide", "/");
	}
	if (c == '(')
	{
		//	cout<<LineNumber<<"LeftSmallParen"<<endl;
		AddChain(LineNumber, "LeftSmallParen", "(");
	}
	if (c == ')')
	{
		//	cout<<LineNumber<<"RightSmallParen"<<endl;
		AddChain(LineNumber, "RightSmallParen", ")");
	}
	if (c == '[')
	{
		//	cout<<LineNumber<<"LefttMidParen"<<endl;
		AddChain(LineNumber, "LeftMidParen", "[");
	}
	if (c == ']')
	{
		//	cout<<LineNumber<<"RightMidParen"<<endl;
		AddChain(LineNumber, "RightMidParen", "]");
	}
	if (c == ';')
	{
		//	cout<<LineNumber<<"Semicolon"<<endl;
		AddChain(LineNumber, "Semicolon", ";");
	}
	if (c == '=')
	{
		//	cout<<LineNumber<<"Equal"<<endl;
		AddChain(LineNumber, "Equal", "=");
	}
	if (c == '<')
	{
		//	cout<<LineNumber<<"LessThan"<<endl;
		AddChain(LineNumber, "LessThan", "<");
	}
	if (c == '>')
	{
		//	cout<<LineNumber<<"MoreThan"<<endl;
		AddChain(LineNumber, "MoreThan", ">");
	}
	if (c == ',')
	{
		//	cout<<LineNumber<<"MoreThan"<<endl;
		AddChain(LineNumber, "Comma", ",");
	}
	c = InputS[CurIndex]; CurIndex++;
}


void AnnotationHandle(char &c)
{
	while (c != '}')
	{
		c = InputS[CurIndex]; CurIndex++;
	}
	c = InputS[CurIndex]; CurIndex++;
}


void PeriodHandle(char &c)
{
	string resultS = "";
	resultS += c;
	c = InputS[CurIndex]; CurIndex++;
	if (c == '.')
	{
		resultS += c;
		c = InputS[CurIndex]; CurIndex++;
		//	cout<<LineNumber<<" Range"<<endl;
		AddChain(LineNumber, "Range", "..");
	}
	else
	{
		//	cout<<LineNumber<<" End Of Input";
		AddChain(LineNumber, "End Of Input", ".");
		resultS = ""; resultS += c;
		while (CurIndex < InputS.size())
		{
			resultS += InputS[CurIndex]; CurIndex++;
		}
		if (c != '\0')
			cout << "Extra Message:" << resultS << "" << endl;//resultS 以一个'\0'字符开始 
		//	exit(0);
	}
}


void QuoteHandle(char &c)
{
	string resultS = "";
	c = InputS[CurIndex]; CurIndex++;
	if ((c <= 'z'&&c >= 'a') || (c >= 'A'&&c <= 'Z') || (c >= '0'&&c <= '9'))
	{
		resultS += c;
		c = InputS[CurIndex]; CurIndex++;
		if (c == '\'')
		{
			//cout<<"Char: "<<tempc;
			AddChain(LineNumber, resultS, "CHARC");
		}
		else
		{
			cout << "line " << LineNumber << " : illeagel character: " << c << endl;
		}
	}
	else
	{
		cout << "line " << LineNumber << " : illeagel character: " << c << endl;
	}
	c = InputS[CurIndex]; CurIndex++;
}
