#include"root.h"

ofstream out_file;

Lexical::Lexical() {
	right_to_left = 0;
	line_number = 1;
	router = 0;
	character = -1;
	error = false;
	is_string = false;
	is_comment = false;
	is_char = false;
}

void Lexical::read() {
	counter = 0;
	position = 0;
	B = sentence;
	F = sentence;
	errno_t err;
	err = fopen_s(&fp, "Test-Lexical.java", "r");

	out_file.open("scan_out.txt");

	while ((ch = fgetc(fp)) != EOF) {
		if (position >= HalfSpace * 2) {      //前后半缓冲区间轮替
			position = 0;
			right_to_left = 1;
		}
		sentence[position] = ch;
		position++;
		counter++;
		if (counter >= HalfSpace) {
			counter = 0;

			identify();

			continue;
		}
	}
	identify();
	out_file.close();

	fclose(fp);
}

void Lexical::identify() {


	while (F < &sentence[position] + (right_to_left*HalfSpace * 2)) {

		if (F >= &sentence[HalfSpace * 2]) {
			F = sentence;
		}

		if (router == 0) {
			if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z')) {
				router = 1;
			}
			else if (*F == '-' || (*F >= '0'&&*F <= '9')) {
				router = 4;
			}
			else {
				switch (*F) {
				case '_':router = 3; break;
				case '$':router = 3; break;
				case '"':router = 7; is_string = true; break;
				case '\'':router = 86; is_char = true; break;
				case '{':
					character = 0x121;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case '}':
					character = 0x121;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case '[':
					character = 0x11d;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case ']':
					character = 0x11d;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case ',':
					character = 0x120;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case '(':
					character = 0x11d;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case ')':
					character = 0x11d;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case '.':router = 16; break;
				case ';':
					character = 0x122;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case '+':router = 18; break;
				case '-':router = 22; break;
				case '*':router = 26; break;
				case '/':router = 29; break;
				case '&':router = 37; break;
				case '~':
					character = 0x11c;       //分隔符
					F++;
					write();

					router = 0;
					B = F;
					continue;
					break;
				case '|':router = 42; break;
				case '^':router = 46; break;
				case '?':router = 49; break;
				case '=':router = 51; break;
				case '>':router = 54; break;
				case '%':router = 63; break;
				case '<':router = 66; break;
				case '!':router = 72; break;
				case ' ':router = 75; break;
				case '\\':router = 76; break;
				}
			}
		}
		else {
			switch (router) {
			case 1:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z')) {
						router = 2;
					}
					else if ((*F >= '0'&&*F <= '9') || *F == '$' || *F == '_') {
						router = 3;
					}
					else {
						character = 0x104;    //标识符
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 2:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z')) {
						router = 2;
					}
					else if ((*F >= '0'&&*F <= '9') || *F == '$' || *F == '_') {
						router = 3;
					}
					else {
						for (i = 0; i < F - B; i++) {
							key_word[i] = *(B + i);
						}
						key_word[i] = '\0';
						if (strcmp(key_word, "abstract") == 0 || strcmp(key_word, "boolean") == 0 || strcmp(key_word, "break") == 0 || strcmp(key_word, "byte") == 0 ||
							strcmp(key_word, "case") == 0 || strcmp(key_word, "catch") == 0 || strcmp(key_word, "char") == 0 || strcmp(key_word, "class") == 0 ||
							strcmp(key_word, "const") == 0 || strcmp(key_word, "continue") == 0 || strcmp(key_word, "default") == 0 || strcmp(key_word, "do") == 0 ||
							strcmp(key_word, "double") == 0 || strcmp(key_word, "else") == 0 || strcmp(key_word, "extends") == 0 || strcmp(key_word, "while") == 0 ||
							strcmp(key_word, "final") == 0 || strcmp(key_word, "finally") == 0 || strcmp(key_word, "float") == 0 || strcmp(key_word, "for") == 0 ||
							strcmp(key_word, "goto") == 0 || strcmp(key_word, "if") == 0 || strcmp(key_word, "implements") == 0 || strcmp(key_word, "import") == 0 ||
							strcmp(key_word, "instanceof") == 0 || strcmp(key_word, "int") == 0 || strcmp(key_word, "interface") == 0 || strcmp(key_word, "long") == 0 ||
							strcmp(key_word, "native") == 0 || strcmp(key_word, "new") == 0 || strcmp(key_word, "null") == 0 || strcmp(key_word, "package") == 0 ||
							strcmp(key_word, "private") == 0 || strcmp(key_word, "protected") == 0 || strcmp(key_word, "public") == 0 || strcmp(key_word, "return") == 0 ||
							strcmp(key_word, "short") == 0 || strcmp(key_word, "static") == 0 || strcmp(key_word, "super") == 0 || strcmp(key_word, "switch") == 0 ||
							strcmp(key_word, "synchronzed") == 0 || strcmp(key_word, "this") == 0 || strcmp(key_word, "throw") == 0 || strcmp(key_word, "throws") == 0 ||
							strcmp(key_word, "transient") == 0 || strcmp(key_word, "volatile") == 0 || strcmp(key_word, "try") == 0 || strcmp(key_word, "void") == 0) {
							character = 0x103;    //关键字
						}
						else if (strcmp(key_word, "true") == 0 || strcmp(key_word, "false") == 0) {    //布尔型
							character = 0x105;
						}
						else {
							character = 0x104;         //标识符
						}
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 3:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z') || (*F >= '0'&&*F <= '9') || *F == '$' || *F == '_') {
						router = 3;
					}
					else {
						character = 0x104;         //标识符
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 4:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z') || (*F >= '0'&&*F <= '9')) {
						router = 4;
					}
					else if (*F == '.') {
						router = 6;
					}
					else {
						character = 0x107;       //整型
						write();

						if (*F == '$' || *F == '_') {
							out_file << "error!" << endl;
						}

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 6:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z') || (*F >= '0'&&*F <= '9')) {
						router = 6;
					}
					else {
						character = 0x108;      //浮点型
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 7:
				if (*F == '"') {
					F++;
					is_string = false;
					character = 0x109;        //字符串
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 86:
				if (*F == '\'') {
					F++;
					is_char = false;
					character = 0x106;         //字符
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 16:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if ((*F >= 'a'&&*F <= 'z') || (*F >= 'A'&&*F <= 'Z') || *F == '$' || *F == '_') {
						character = 0x11d;           //分隔符.
						write();

						router = 0;
						B = F;
						continue;
					}
					else if (*F >= '0'&&*F <= '9') {
						router = 6;
					}
					else {
						character = 0x108;      //浮点型
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 18:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '+') {
						router = 20;
					}
					else if (*F == '=') {
						router = 21;
					}
					else {
						character = 0x11a;           //运算符+
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 22:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '-') {
						router = 24;
					}
					else if (*F == '=') {
						router = 25;
					}
					else {
						character = 0x11a;           //运算符-
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 26:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 24;
					}
					else {
						character = 0x11b;           //运算符*
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 29:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 31;
					}
					else if (*F == '/') {
						router = 32;
						is_comment = true;
					}
					else {
						character = 0x11b;           //运算符/
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 37:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '&') {
						router = 39;
					}
					else if (*F == '=') {
						router = 40;
					}
					else {
						character = 0x116;           //运算符&
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 42:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '|') {
						router = 44;
					}
					else if (*F == '=') {
						router = 45;
					}
					else {
						character = 0x114;           //运算符|
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 46:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 48;
					}
					else {
						character = 0x115;           //运算符^
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 49:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == ':') {
						router = 50;
					}
					else {
						write();
						out_file << "error!" << endl;
						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 51:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 53;
					}
					else {
						character = 0x110;    //运算符=
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 54:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 56;
					}
					else if (*F == '>') {
						router = 57;
					}
					else {
						character = 0x118;     //运算符>
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 63:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 65;
					}
					else {
						character = 0x11b;     //运算符%
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 66:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 68;
					}
					else if (*F == '<') {
						router = 69;
					}
					else {
						character = 0x118;     //运算符<
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 72:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 74;
					}
					else {
						character = 0x11c;     //运算符!
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 75:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == ' ') {
						router = 75;
					}
					else {
						character = 0x102;     //空格
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 20:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x11c;     //运算符++
					write();

					if (*F == '+' || *F == '-' || *F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 21:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符+=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 24:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x11c;     //运算符--
					write();

					if (*F == '+' || *F == '-' || *F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 25:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符-=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 28:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符*=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 31:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符/=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 32:
				if (*F == '\n') {
					is_comment = false;
					character = 0x101;     //注释结束
					write();

					F++;
					router = 0;
					B = F;
					continue;
				}
				break;
			case 39:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x113;     //运算符&&
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 40:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符&=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 44:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x112;     //运算符||
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 45:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符|=
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 48:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符^=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 50:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x111;     //运算符?:
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 53:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x117;     //运算符==
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 56:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x118;     //运算符>=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 57:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 59;
					}
					else if (*F == '>') {
						router = 60;
					}
					else {
						character = 0x119;     //运算符>>
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 59:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符>>=
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 60:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 62;
					}
					else {
						character = 0x119;     //运算符>>>
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 62:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符>>>=
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 65:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符%=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 68:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x118;     //运算符<=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			case 69:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					if (*F == '=') {
						router = 71;
					}
					else {
						character = 0x119;     //运算符<<
						write();

						router = 0;
						B = F;
						continue;
					}
				}
				break;
			case 71:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x110;     //运算符<<=
					write();

					router = 0;
					B = F;
					continue;
				}
				break;
			case 74:
				if (is_string == true || is_char == true || is_comment == true) {

				}
				else {
					character = 0x117;     //运算符!=
					write();

					if (*F == '*' || *F == '/' || *F == '=') {
						out_file << "error!" << endl;
					}

					router = 0;
					B = F;
					continue;
				}
				break;
			}
		}

		if (*F == '\n') {
			line_number++;
		}

		F++;
	}
}



void Lexical::write() {
	while (B < F + (right_to_left*HalfSpace * 2)) {
		out_file << *B;
		B++;
		if (B >= &sentence[HalfSpace * 2]) {
			B = sentence;
			right_to_left = 0;
		}
	}
	out_file << hex << "   " << "0x" << character << "   ";    //写入文件
	out_file << dec << "line:" << line_number << endl;
}