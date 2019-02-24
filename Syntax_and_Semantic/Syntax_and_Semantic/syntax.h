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

//�﷨�������������
class Syntax
{
private:
	ifstream inFile;    //�����ļ�
	ofstream synFile, asmFile, ersFile;  //����ļ�������ļ�

	vector<Word> wordlist;  //�������У��ʷ����������
	vector<Word>::iterator ptr; //��������ָ��
	vector<IdType> idlist;  //��ʶ������
	vector<IdType>::iterator idptr; //��ʶ������ָ��

	stack<Word> opnd;   //������ջ
	stack<Word> optr;   //�����ջ

	char asm_all[MAX_FILE]; //���ɵĻ������ȫ��
	char asm_code[MAX_FILE];    //���ɵĻ�����Դ����
	char asm_data[MAX_FILE];    //���ɵĻ���������ݶ�

	int id_index;   //��ʶ������
	int lp_index;   //ѭ��������

	void TellError(const char * reason);   //�������

										   //�ݹ��½�������
	void Deal_Prg();
	void Deal_Stc();
	void Deal_Sas();
	void Deal_Swh();
	void Deal_Exr();
	void Deal_Cmp(int * _detail);    //����Ϊ���صıȽ����������ϸ������
	void Deal_Add(bool _sign, struct Word * word = NULL);  //_sign�����ڡ�+������-������ʾ����ʱΪtrue����ʾ�Ӽ�ʱΪfalse
														   //word������Ϊ���ص������
	void Deal_Mul(struct Word * word = NULL);   //����Ϊ���ص������
	void Deal_Val(struct Word * word = NULL);   //����Ϊ���صĵ���
	void Deal_Int(int * int_val = NULL);  //����Ϊ���ص�����ֵ
	void Deal_Exp();
	void Deal_Ex1();
	void Deal_Ext();
	void Deal_Et1();

	int Id_To_Index(const char * id);  //�ɱ�ʶ�����������������ڷ���-1
	char GetOptrPri(struct Word * word1, struct Word * word2);  //������������ȼ�

																//���ɻ�����
	void Asm_Define(const char * id);   //�������
	void Asm_Assign(const char * id);   //��ֵ
	void Asm_ToReg(const char * id, const char * regname);  //��ʶ����Ӧ��������Ĵ���
	void Asm_ToReg(const int imnum, const char * regname);  //����������Ĵ���
	void Asm_ToStack(const char * id);  //��ʶ����Ӧ����ѹջ
	void Asm_ToStack(const int imnum);  //������ѹջ
	void Asm_PopToReg(const char * regname);    //��ջ
	void Asm_WhileBegin();  //ѭ����ʼ
	void Asm_WhileLoop(const char * jump);  //ѭ���жϣ�jump=����ѭ������ת��䣩
	void Asm_WhileEnd();    //ѭ������
	void Asm_DoAdd();   //���ӷ�
	void Asm_DoSub();   //������
	void Asm_DoMul();   //���˷�
	void Asm_DoDiv();   //������
	void Asm_Expression(struct Word * theoptr_ptr); //������ʽ
	void Asm_Comment(const char * comment); //����ע��
public:
	Syntax(char * fn_in, char * fn_syn, char * fn_asm, char * fn_ers);  //���캯��
	~Syntax(); //��������
	void MakeTree(); //�����﷨��
	void WriteAsm(); //���ɻ������
	bool Ready(); //�Ƿ����
};

#endif

