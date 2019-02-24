#pragma once
#define HalfSpace 50    //缓冲区半区大小

class Lexical {
private:
	int counter;
	int position;
	char ch;
	FILE *fp;

	char sentence[HalfSpace * 2];   //缓冲区
	char *B;             //单词头指针
	char *F;             //单词尾指针
	int right_to_left;      //控制从右半区转移到左半区

	int character;     //属性类别号
	int router;     //构造状态图
	char key_word[30];
	bool is_comment;   //是注释
	bool is_string;    //是串
	bool is_char;     //是字符

	int line_number;     //行号
	bool error;     //是否出错

	int i;
public:
	Lexical();
	void read();            //读取
	void identify();        //预处理及单词识别
	void write();
};
