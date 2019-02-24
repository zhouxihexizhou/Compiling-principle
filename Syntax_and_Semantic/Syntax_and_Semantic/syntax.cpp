#include "syntax.h"

Syntax::Syntax(char * fn_in, char * fn_syn, char * fn_asm, char * fn_ers)
{
	char line[MAX_LINE];    //�������
	struct Word theword;    //����ĵ���
	int tmp;    //��ʱ����

	id_index = 0;
	lp_index = 0;

	memset(asm_all, 0, MAX_FILE);
	memset(asm_code, 0, MAX_FILE);
	memset(asm_data, 0, MAX_FILE);

	//���ʷ������������wordlist
	inFile.open(fn_in);
	if (!inFile.is_open())
	{
		return;
	}
	inFile.getline(line, MAX_LINE);
	while (!inFile.eof())
	{
		sscanf(line, "%d\t", &(theword.detail));
		theword.value.int_val = 0;
		if (theword.detail == P_INTLITERAL)
		{
			sscanf(line, "%d\t%d\t%d\t%d", &tmp, &(theword.line), &(theword.col), &(theword.value.int_val));
		}
		else
		{
			sscanf(line, "%d\t%d\t%d\t%s", &tmp, &(theword.line), &(theword.col), &(theword.value.name));
		}
		wordlist.push_back(theword);
		inFile.getline(line, MAX_LINE);
	}
	theword.detail = P_EOF;
	theword.value.int_val = 0;
	wordlist.push_back(theword);
	inFile.close();

	//��������ʹ����ļ�
	FILE * fp;
	fp = fopen(fn_syn, "w");
	fclose(fp);
	fp = fopen(fn_asm, "w");
	fclose(fp);
	fp = fopen(fn_ers, "w");
	fclose(fp);
	synFile.open(fn_syn, ios::out | ios::app);
	asmFile.open(fn_asm, ios::out | ios::app);
	ersFile.open(fn_ers, ios::out | ios::app);
}

Syntax::~Syntax()
{
	if (!synFile.is_open())
	{
		synFile.close();
	}
	if (!asmFile.is_open())
	{
		asmFile.close();
	}
	if (!ersFile.is_open())
	{
		ersFile.close();
	}
}

void Syntax::MakeTree()
{
	synFile << "�﷨������ʼ" << endl;
	ptr = wordlist.begin();
	Deal_Prg();
	synFile << "�﷨��������" << endl;
}

void Syntax::WriteAsm()
{
	strcpy(asm_all, ASMBEGIN);
	strcat(asm_all, STACKSG);
	strcat(asm_all, DATA_BEGIN);
	strcat(asm_all, asm_data);
	strcat(asm_all, DATA_END);
	strcat(asm_all, CODE_BEGIN);
	strcat(asm_all, asm_code);
	strcat(asm_all, CODE_END);
	asmFile << asm_all;
}

void Syntax::Deal_Prg()
{
	if (((Word)*ptr).detail == P_WHILE || ((Word)*ptr).detail == P_IDENTIFIER)
	{
		Deal_Stc();
		Deal_Prg();
	}
	else if (((Word)*ptr).detail == P_EOF)
	{
		return;
	}
	else
	{
		TellError("�Ƿ��������ʼ����");
	}
}

void Syntax::Deal_Stc()
{
	if (((Word)*ptr).detail == P_WHILE)
	{
		Deal_Swh();
	}
	else if (((Word)*ptr).detail == P_IDENTIFIER)
	{
		Deal_Sas();
	}
	else
	{
		TellError("�Ƿ��������ʼ����");
	}
}

void Syntax::Deal_Sas()
{
	char lvalue[MAX_WORD];
	synFile << "��ֵ���" << endl;
	Asm_Comment("��ֵ��ʼ");
	if (((Word)*ptr).detail == P_IDENTIFIER)
	{
		synFile << "  ��ֵ��" << ((Word)*ptr).value.name << endl;
		strcpy(lvalue, ((Word)*ptr).value.name);
		Asm_Define(lvalue);
		ptr++;
		if (((Word)*ptr).detail == P_EQ)
		{
			synFile << "  ��ֵ��";
			ptr++;
			Deal_Exp();
			if (((Word)*ptr).detail == P_SEMI)
			{
				Asm_Assign(lvalue);
				ptr++;
			}
			else
			{
				TellError("������ȱ�١�;��");
			}
		}
		else
		{
			TellError("��ʶ���������ʹ�á�=����ֵ");
		}
	}
	else
	{
		TellError("�Ƿ��������ʼ����");
	}
	Asm_Comment("����printf�����ʼ");
	char item[MAX_LINE];
	sprintf(item,
		"        PUSH    AX\n"
		"        LEA     BX, %s%d\n"
		"        PUSH    BX\n"
		"        LEA     BX, PRN_STR\n"
		"        PUSH    BX\n"
		"        CALL    printf\n"
		"        ADD     SP, 6\n"
		, DATA_NAME, Id_To_Index(lvalue));
	strcat(asm_code, item);
	Asm_Comment("����printf�������");
	Asm_Comment("��ֵ����");
}

void Syntax::Deal_Swh()
{
	synFile << "ѭ���忪ʼ" << endl;
	if (((Word)*ptr).detail == P_WHILE)
	{
		Asm_WhileBegin();
		ptr++;
		if (((Word)*ptr).detail == P_LPAREN)
		{
			ptr++;
			Deal_Exr();
			if (((Word)*ptr).detail == P_RPAREN)
			{
				ptr++;
				Deal_Sas();
				Asm_WhileEnd();
			}
			else
			{
				TellError("while���ıȽϱ��ʽȱ�١�)��");
			}
		}
		else
		{
			TellError("while���ıȽϱ��ʽȱ�١�(��");
		}
	}
	else
	{
		TellError("�Ƿ��������ʼ����");
	}
	synFile << "ѭ�������" << endl;
}

void Syntax::Deal_Exr()
{
	int cmp_detail; //�Ƚ����������ϸ������
	struct Word lvalue, rvalue; //�Ƚ϶���
	Asm_Comment("�Ƚϱ��ʽ��ʼ");
	synFile << "�Ƚϱ��ʽ" << endl;
	synFile << "  ��ֵ";
	Deal_Val(&lvalue);
	synFile << endl;
	synFile << "  ";
	Deal_Cmp(&cmp_detail);
	synFile << endl;
	synFile << "  ��ֵ";
	Deal_Val(&rvalue);
	synFile << endl;
	synFile << "  ��ʱ��Ϊ��" << endl;
	if (lvalue.detail == P_IDENTIFIER)
	{
		Asm_ToStack(lvalue.value.name);
	}
	else if (lvalue.detail == P_INTLITERAL)
	{
		Asm_ToStack(lvalue.value.int_val);
	}
	if (rvalue.detail == P_IDENTIFIER)
	{
		Asm_ToStack(rvalue.value.name);
	}
	else if (rvalue.detail == P_INTLITERAL)
	{
		Asm_ToStack(rvalue.value.int_val);
	}
	if (cmp_detail == P_LT)
	{
		Asm_WhileLoop("JNL");
	}
	else if (cmp_detail == P_GT)
	{
		Asm_WhileLoop("JNG");
	}
	Asm_Comment("�Ƚϱ��ʽ����");
}

void Syntax::Deal_Cmp(int * _detail)
{
	*_detail = ((Word)*ptr).detail;
	if (((Word)*ptr).detail == P_GT)
	{
		synFile << "����";
		ptr++;
	}
	else if (((Word)*ptr).detail == P_LT)
	{
		synFile << "С��";
		ptr++;
	}
	else
	{
		TellError("�Ƚϱ��ʽ�н�����ʹ�á�>���͡�<������");
	}
}

void Syntax::Deal_Add(bool _sign, struct Word * word)
{
	if (((Word)*ptr).detail == P_PLUS)
	{
		if (word != NULL)
		{
			word->detail = P_PLUS;
			word->value.is_sign = false;
		}
		synFile << "+";
		ptr->value.is_sign = _sign;
		ptr++;
	}
	else if (((Word)*ptr).detail == P_SUB)
	{
		if (word != NULL)
		{
			word->detail = P_SUB;
			word->value.is_sign = false;
		}
		synFile << "-";
		ptr->value.is_sign = _sign;
		ptr++;
	}
	else
	{
		TellError("�˴��ڴ����֡�+����-�������");
	}
}

void Syntax::Deal_Mul(struct Word * word)
{
	if (((Word)*ptr).detail == P_STAR)
	{
		if (word != NULL)
		{
			word->detail = P_STAR;
			strcpy(word->value.name, "*");
		}
		synFile << "*";
		ptr++;
	}
	else if (((Word)*ptr).detail == P_SLASH)
	{
		if (word != NULL)
		{
			word->detail = P_SLASH;
			strcpy(word->value.name, "/");
		}
		synFile << "/";
		ptr++;
	}
	else
	{
		TellError("�˴��ڴ����֡�*����/�������");
	}
}

void Syntax::Deal_Val(struct Word * word)
{
	if (((Word)*ptr).detail == P_IDENTIFIER)
	{
		Asm_Define(((Word)*ptr).value.name);
		if (word != NULL)
		{
			word->detail = P_IDENTIFIER;
			strcpy(word->value.name, ((Word)*ptr).value.name);
		}
		synFile << ((Word)*ptr).value.name;
		ptr++;
	}
	else if (((Word)*ptr).detail == P_PLUS || ((Word)*ptr).detail == P_SUB || ((Word)*ptr).detail == P_INTLITERAL)
	{
		int _val;
		Deal_Int(&_val);
		if (word != NULL)
		{
			word->detail = P_INTLITERAL;
			word->value.int_val = _val;
		}
	}
	else
	{
		TellError("�Ƿ��Ĳ�����");
	}
}

void Syntax::Deal_Int(int * int_val)
{
	int sign = 1;   //1Ϊ����-1Ϊ��
	if (((Word)*ptr).detail == P_PLUS || ((Word)*ptr).detail == P_SUB)
	{
		sign = (((Word)*ptr).detail == P_PLUS) ? 1 : -1;
		Deal_Add(true);
		if (((Word)*ptr).detail == P_INTLITERAL)
		{
			if (int_val != NULL)
			{
				*int_val = sign * ((Word)*ptr).value.int_val;
			}
			synFile << ((Word)*ptr).value.int_val;
			ptr++;
		}
		else
		{
			TellError("�˴��ڴ�����һ��������");
		}
	}
	else if (((Word)*ptr).detail == P_INTLITERAL)
	{
		if (int_val != NULL)
		{
			*int_val = ((Word)*ptr).value.int_val;
		}
		synFile << ((Word)*ptr).value.int_val;
		ptr++;
	}
	else
	{
		TellError("�˴��ڴ�����һ��������");
	}
}

void Syntax::Deal_Exp()
{
	synFile << "������ʽ";
	Asm_Comment("������ʽ��ʼ");
	struct Word theoptr;
	theoptr.detail = P_BAR;
	optr.push(theoptr);
	Deal_Ext();
	Deal_Ex1();
	synFile << endl;
	Asm_Expression(&theoptr);
	opnd.pop();
	Asm_Comment("������ʽ����");
}

void Syntax::Deal_Ex1()
{
	if (((Word)*ptr).detail == P_PLUS || ((Word)*ptr).detail == P_SUB)
	{
		struct Word theoptr;
		Deal_Add(false, &theoptr);
		Asm_Expression(&theoptr);
		Deal_Ext();
		Deal_Ex1();
	}
	else
	{
		//��
	}
}

void Syntax::Deal_Ext()
{
	struct Word theopnd;
	Deal_Val(&theopnd);
	opnd.push(theopnd);
	if (theopnd.detail == P_IDENTIFIER)
	{
		Asm_ToStack(theopnd.value.name);
	}
	else if (theopnd.detail == P_INTLITERAL)
	{
		Asm_ToStack(theopnd.value.int_val);
	}
	Deal_Et1();
}

void Syntax::Deal_Et1()
{
	if (((Word)*ptr).detail == P_STAR || ((Word)*ptr).detail == P_SLASH)
	{
		struct Word theoptr, theopnd;
		Deal_Mul(&theoptr);
		Asm_Expression(&theoptr);
		Deal_Val(&theopnd);
		opnd.push(theopnd);
		if (theopnd.detail == P_IDENTIFIER)
		{
			Asm_ToStack(theopnd.value.name);
		}
		else if (theopnd.detail == P_INTLITERAL)
		{
			Asm_ToStack(theopnd.value.int_val);
		}
		Deal_Et1();
	}
	else
	{
		//��
	}
}

void Syntax::TellError(const char * reason)
{
	ersFile << "�﷨����" << reason << endl;
	ersFile << "  λ�ã���" << ptr->line << "�У���" << ptr->col << "��" << endl;
}

int Syntax::Id_To_Index(const char * id)
{
	int ret = -1;
	for (idptr = idlist.begin(); idptr != idlist.end(); idptr++)
	{
		if (!strcmp(id, ((IdType)*idptr).name))
		{
			ret = ((IdType)*idptr).index;
			break;
		}
	}
	return ret;
}

void Syntax::Asm_Define(const char * id)
{
	//�״ζ�ĳ������ֵʱ�������������
	if (Id_To_Index(id) < 0)
	{
		struct IdType newid;
		newid.index = id_index++;
		strcpy(newid.name, id);
		idlist.push_back(newid);
		char item[MAX_LINE];
		sprintf(item, "%s%d%s \'%s\', 0\n", DATA_NAME, newid.index, DATA_IDDB, id);
		strcat(asm_data, item);
		sprintf(item, "%s%d%s", DATA_HEAD, newid.index, DATA_ITEM);
		strcat(asm_data, item);
	}
}

void Syntax::Asm_Assign(const char * id)
{
	char item[MAX_LINE];
	Asm_PopToReg("AX");
	sprintf(item, "        MOV     %s%d, AX\n", DATA_HEAD, Id_To_Index(id));

	strcat(asm_code, item);
}

void Syntax::Asm_ToReg(const char * id, const char * regname)
{
	char item[MAX_LINE];
	sprintf(item, "        MOV     %s, [%s%d]\n", regname, DATA_HEAD, Id_To_Index(id));
	strcat(asm_code, item);
}

void Syntax::Asm_ToReg(const int imnum, const char * regname)
{
	char item[MAX_LINE];
	sprintf(item, "        MOV     %s, %d\n", regname, imnum);
	strcat(asm_code, item);
}

void Syntax::Asm_ToStack(const char * id)
{
	Asm_ToReg(id, "SI");
	char item[MAX_LINE];
	sprintf(item, "        PUSH    SI\n");
	strcat(asm_code, item);
}

void Syntax::Asm_ToStack(const int imnum)
{
	Asm_ToReg(imnum, "SI");
	char item[MAX_LINE];
	sprintf(item, "        PUSH    SI\n");
	strcat(asm_code, item);
}

void Syntax::Asm_PopToReg(const char * regname)
{
	char item[MAX_LINE];
	sprintf(item, "        POP     %s\n", regname);
	strcat(asm_code, item);
}

void Syntax::Asm_WhileBegin()
{
	char item[MAX_LINE];
	sprintf(item, "%s%d:\n", WHILE_BEGIN, lp_index);
	strcat(asm_code, item);
}

void Syntax::Asm_WhileLoop(const char * jump)
{
	char item[MAX_LINE];
	Asm_PopToReg("BX");
	Asm_PopToReg("AX");
	sprintf(item, "        CMP     AX, BX\n");
	strcat(asm_code, item);
	sprintf(item, "        %s     %s%d\n", jump, WHILE_END, lp_index);
	strcat(asm_code, item);
}

void Syntax::Asm_WhileEnd()
{
	char item[MAX_LINE];
	sprintf(item, "        JMP     %s%d\n", WHILE_BEGIN, lp_index);
	strcat(asm_code, item);
	sprintf(item, "%s%d:\n", WHILE_END, lp_index);
	strcat(asm_code, item);
	lp_index++;
}

void Syntax::Asm_DoAdd()
{
	char item[MAX_LINE];
	sprintf(item, "        ADD     AX, BX\n");
	strcat(asm_code, item);
	sprintf(item, "        PUSH    AX\n");
	strcat(asm_code, item);
}

void Syntax::Asm_DoSub()
{
	char item[MAX_LINE];
	sprintf(item, "        SUB     AX, BX\n");
	strcat(asm_code, item);
	sprintf(item, "        PUSH    AX\n");
	strcat(asm_code, item);
}

void Syntax::Asm_DoMul()
{
	char item[MAX_LINE];
	sprintf(item, "        IMUL    BX\n");
	strcat(asm_code, item);
	sprintf(item, "        XOR     DX, DX\n");
	strcat(asm_code, item);
	sprintf(item, "        PUSH    AX\n");
	strcat(asm_code, item);
}

void Syntax::Asm_DoDiv()
{
	char item[MAX_LINE];
	sprintf(item, "        XOR     DX, DX\n");
	strcat(asm_code, item);
	sprintf(item, "        IDIV    BX\n");
	strcat(asm_code, item);
	sprintf(item, "        PUSH    AX\n");
	strcat(asm_code, item);
}

void Syntax::Asm_Expression(struct Word * theoptr_ptr)
{
	struct Word tmp;
cmp_pri:
	switch (GetOptrPri(&(optr.top()), theoptr_ptr))
	{
	case '<':
		optr.push(*theoptr_ptr);
		break;
	case '>':
		//������2
		Asm_PopToReg("BX");
		opnd.pop();
		//������1
		Asm_PopToReg("AX");
		opnd.pop();
		//�����
		switch (optr.top().detail)
		{
		case P_PLUS:
			Asm_DoAdd();
			break;
		case P_SUB:
			Asm_DoSub();
			break;
		case P_STAR:
			Asm_DoMul();
			break;
		case P_SLASH:
			Asm_DoDiv();
			break;
		default:
			break;
		}
		optr.pop();
		tmp.detail = P_INTLITERAL;
		tmp.value.int_val = 0;
		opnd.push(tmp);
		goto cmp_pri;
		break;
	case '=':
		optr.pop();
		break;
	default:
		break;
	}
}

void Syntax::Asm_Comment(const char * comment)
{
	char item[MAX_LINE];
	sprintf(item, "        ;%s\n", comment);
	strcat(asm_code, item);
}

char Syntax::GetOptrPri(struct Word * word1, struct Word * word2)
{
	int i, j;
	switch (word1->detail)
	{
	case P_PLUS:
		i = 0;
		break;
	case P_SUB:
		i = 1;
		break;
	case P_STAR:
		i = 2;
		break;
	case P_SLASH:
		i = 3;
		break;
	case P_BAR:
		i = 4;
		break;
	default:
		i = 4;
		break;
	}
	switch (word2->detail)
	{
	case P_PLUS:
		j = 0;
		break;
	case P_SUB:
		j = 1;
		break;
	case P_STAR:
		j = 2;
		break;
	case P_SLASH:
		j = 3;
		break;
	case P_BAR:
		j = 4;
		break;
	default:
		j = 4;
		break;
	}
	return OPTR_PRI[i][j];
}

bool Syntax::Ready()
{
	return !(wordlist.empty());
}
