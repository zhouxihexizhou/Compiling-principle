#pragma once

#ifndef STRUCTS_H
#define STRUCTS_H

#include "constant.h"

//���ʽṹ
struct Word
{
	int detail; //��ϸ������
	int line;   //Դ������������
	int col;    //Դ������������
	union
	{
		char name[MAX_WORD];    //��ʶ��������
		int int_val;    //���͵���ֵ
		bool is_sign;   //���ڡ�+������-������ʾ����ʱΪtrue����ʾ�Ӽ�ʱΪfalse
	} value;
};

//��ʶ���ṹ
struct IdType
{
	char name[MAX_WORD];    //��ʶ��������
	int index;  //��ʶ�������ţ��ڻ��������ʹ�ã�
};

//��������ȼ���
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
