#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "constant.h"
#include "syntax.h"

using namespace std;

int main(int argc, char * argv[])
{
	char fn_in[MAX_LINE];   //输入文件名（由词法分析器生成的“*_lex.txt”文件）
	char fn_syn[MAX_LINE]; //输出语法树文件名
	char fn_asm[MAX_LINE];  //输出汇编程序文件名
	char fn_ers[MAX_LINE];  //输出错误信息文件名
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
