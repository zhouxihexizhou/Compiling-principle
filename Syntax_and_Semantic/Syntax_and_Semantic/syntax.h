#pragma once

#ifndef SYNTAX_H
#define SYNTAX_H

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include "constant.h"
#include "structs.h"
#include "asmlang.h"

using namespace std;

//语法、语义分析器类
class Syntax
{
private:
	ifstream inFile;    //输入文件
	ofstream synFile, asmFile, ersFile;  //输出文件与错误文件

	vector<Word> wordlist;  //单词序列（词法分析结果）
	vector<Word>::iterator ptr; //单词序列指针
	vector<IdType> idlist;  //标识符序列
	vector<IdType>::iterator idptr; //标识符序列指针

	stack<Word> opnd;   //操作数栈
	stack<Word> optr;   //运算符栈

	char asm_all[MAX_FILE]; //生成的汇编语言全文
	char asm_code[MAX_FILE];    //生成的汇编语言代码段
	char asm_data[MAX_FILE];    //生成的汇编语言数据段

	int id_index;   //标识符索引
	int lp_index;   //循环体索引

	void TellError(const char * reason);   //报告错误

										   //递归下降分析器
	void Deal_Prg();
	void Deal_Stc();
	void Deal_Sas();
	void Deal_Swh();
	void Deal_Exr();
	void Deal_Cmp(int * _detail);    //参数为返回的比较运算符的详细属性字
	void Deal_Add(bool _sign, struct Word * word = NULL);  //_sign：对于“+”、“-”，表示正负时为true，表示加减时为false
														   //word：参数为返回的运算符
	void Deal_Mul(struct Word * word = NULL);   //参数为返回的运算符
	void Deal_Val(struct Word * word = NULL);   //参数为返回的单词
	void Deal_Int(int * int_val = NULL);  //参数为返回的整数值
	void Deal_Exp();
	void Deal_Ex1();
	void Deal_Ext();
	void Deal_Et1();

	int Id_To_Index(const char * id);  //由标识符查找索引，不存在返回-1
	char GetOptrPri(struct Word * word1, struct Word * word2);  //查找运算符优先级

																//生成汇编语句
	void Asm_Define(const char * id);   //定义变量
	void Asm_Assign(const char * id);   //赋值
	void Asm_ToReg(const char * id, const char * regname);  //标识符对应变量读入寄存器
	void Asm_ToReg(const int imnum, const char * regname);  //立即数读入寄存器
	void Asm_ToStack(const char * id);  //标识符对应变量压栈
	void Asm_ToStack(const int imnum);  //立即数压栈
	void Asm_PopToReg(const char * regname);    //出栈
	void Asm_WhileBegin();  //循环开始
	void Asm_WhileLoop(const char * jump);  //循环判断（jump=跳出循环的跳转语句）
	void Asm_WhileEnd();    //循环结束
	void Asm_DoAdd();   //做加法
	void Asm_DoSub();   //做减法
	void Asm_DoMul();   //做乘法
	void Asm_DoDiv();   //做除法
	void Asm_Expression(struct Word * theoptr_ptr); //处理表达式
	void Asm_Comment(const char * comment); //加入注释
public:
	Syntax(char * fn_in, char * fn_syn, char * fn_asm, char * fn_ers);  //构造函数
	~Syntax(); //析构函数
	void MakeTree(); //构造语法树
	void WriteAsm(); //生成汇编语言
	bool Ready(); //是否就绪
};

#endif

