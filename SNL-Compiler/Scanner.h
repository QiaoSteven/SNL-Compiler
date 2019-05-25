#pragma once
#ifndef SCANNER_H
#define SCANNER_H
//词法分析头文件内容 存储Token序列的结构体 


#include "globals.h" 

//函数声明 （不调用其他函数的） 
void DisplayToken();
void AddChain(int LineNumber, string type, string value);
bool FindReserved(string TarS);
//函数声明（调用其他函数的） 
void InputHandle();
void CharHandle(char &c);
void DigitHandle(char &c);
void ColonHandle(char &c);
void SingleDivideCHandle(char &c);
void AnnotationHandle(char &c);
void PeriodHandle(char &c);
void QuoteHandle(char &c);


#endif
