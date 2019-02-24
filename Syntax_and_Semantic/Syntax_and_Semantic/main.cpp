#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "constant.h"
#include "syntax.h"

using namespace std;

int main(int argc, char * argv[])
{
	char fn_in[MAX_LINE];   //�����ļ������ɴʷ����������ɵġ�*_lex.txt���ļ���
	char fn_syn[MAX_LINE]; //����﷨���ļ���
	char fn_asm[MAX_LINE];  //����������ļ���
	char fn_ers[MAX_LINE];  //���������Ϣ�ļ���
	Syntax * syn;

	if (argc == 1)
	{
		cout << "Input file name: ";
		cin >> fn_in;
	}
	else
	{
		strcpy(fn_in, argv[1]);
		cout << "File name: " << fn_in << endl;
	}
	strcpy(fn_syn, fn_in);
	strcat(fn_syn, "_syn.txt");
	strcpy(fn_asm, fn_in);
	strcat(fn_asm, "_asm.txt");
	strcpy(fn_ers, fn_in);
	strcat(fn_ers, "_ers.txt");

	syn = new Syntax(fn_in, fn_syn, fn_asm, fn_ers);
	if (syn->Ready())
	{
		syn->MakeTree();
		syn->WriteAsm();
	}
	else
	{
		cout << "File not found or that is an empty file." << endl;
	}
	delete syn;

	return 0;
}
