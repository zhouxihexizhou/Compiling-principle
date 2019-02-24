#pragma once
#define HalfSpace 50    //������������С

class Lexical {
private:
	int counter;
	int position;
	char ch;
	FILE *fp;

	char sentence[HalfSpace * 2];   //������
	char *B;             //����ͷָ��
	char *F;             //����βָ��
	int right_to_left;      //���ƴ��Ұ���ת�Ƶ������

	int character;     //��������
	int router;     //����״̬ͼ
	char key_word[30];
	bool is_comment;   //��ע��
	bool is_string;    //�Ǵ�
	bool is_char;     //���ַ�

	int line_number;     //�к�
	bool error;     //�Ƿ����

	int i;
public:
	Lexical();
	void read();            //��ȡ
	void identify();        //Ԥ��������ʶ��
	void write();
};
