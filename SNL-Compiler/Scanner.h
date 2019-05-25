#pragma once
#ifndef SCANNER_H
#define SCANNER_H
//�ʷ�����ͷ�ļ����� �洢Token���еĽṹ�� 


#include "globals.h" 

//�������� �����������������ģ� 
void DisplayToken();
void AddChain(int LineNumber, string type, string value);
bool FindReserved(string TarS);
//�����������������������ģ� 
void InputHandle();
void CharHandle(char &c);
void DigitHandle(char &c);
void ColonHandle(char &c);
void SingleDivideCHandle(char &c);
void AnnotationHandle(char &c);
void PeriodHandle(char &c);
void QuoteHandle(char &c);


#endif
