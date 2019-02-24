#pragma once

#ifndef STRUCTS_H
#define STRUCTS_H

#include "constant.h"

//单词结构
struct Word
{
	int detail; //详细属性字
	int line;   //源代码中所在行
	int col;    //源代码中所在列
	union
	{
		char name[MAX_WORD];    //标识符的名字
		int int_val;    //整型的数值
		bool is_sign;   //对于“+”、“-”，表示正负时为true，表示加减时为false
	} value;
};

//标识符结构
struct IdType
{
	char name[MAX_WORD];    //标识符的名字
	int index;  //标识符索引号（在汇编语言中使用）
};

//运算符优先级表
static const char OPTR_PRI[5][5] =
{
	//+    -    *    /    | 
	{ '>', '>', '<', '<', '>' },  // +
{ '>', '>', '<', '<', '>' },  // -
{ '>', '>', '>', '>', '>' },  // * 
{ '>', '>', '>', '>', '>' },  // /
{ '<', '<', '<', '<', '=' }   // |
};

#endif
