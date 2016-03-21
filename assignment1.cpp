#include <iostream>
#include <string>

using namespace std;

void getstring(string* s)	// get input string
{
	char temp;
	while(!cin.eof())
	{
		cin.get(temp);
		(*s) += temp;
	}
	(*s).erase((*s).length()-1);
}

bool is_letter(char ch)		//ch is a letter, return true;
{
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

bool is_digit(char ch)		//ch is a digit, return true;
{
	return (ch >= '0' && ch <= '9');
}

bool in_alphabet(char ch)
{
	return (ch == 9 || ch == 0x0a || (ch >= 0x20 && ch <= 0x7e));
}

int string_analyze(string s, int begin)		// analyze a string
{
	int index = begin+1;
	for(; index < s.length(); index++)
	{
		if(s[index] == '"')	// end
			break;
		if(s[index] == '\n' || !in_alphabet(s[index]))	// ERR2
			break;
	}
	return index;
}

int writespace_analyze(string s, int begin)	// analyze writespace
{
	int index = begin+1;
	for(; index < s.length() && (s[index] == '\t' || s[index] == ' '); index++);
	return index;
}

int int_float_analyze(string s, int begin)	
{
	int index = begin+1;
	for(; index < s.length() && is_digit(s[index]); index++);
	return index;
}

int comment_analyze(string s, int begin)
{
	int index = begin + 1;
	for(; index < s.length() && s[index] != '\n'; index++);
	return index;
}

bool is_keyword(string s, int begin, string key)
{
	int index = 1;
	for(;index+begin < s.length() && index < key.length() && s[begin+index] == key[index]; index++);
	if(index == key.length()) return true;
	else return false;
}

bool is_identifier_char(char ch)
{
	return (is_digit(ch) || is_letter(ch) || ch == '_');
}

int identifier_analyze(string s, int begin)
{
	int index = begin + 1;
	for(; index < s.length() && is_identifier_char(s[index]); index++);
	return index;
}


int err3_analyze(string s, int begin)
{
	int index = begin + 1;
	for(; index < s.length() && !in_alphabet(s[index]); index++);
	return index;
}

void morphology_analyze(string s, int pattern)
{
	int index = 0;
	int tid = 1, pattern1 = 0, pattern2 = 0;
	while(index < s.length())
	{
		//string
		if(s[index] == '"') 	
		{
			int r = string_analyze(s, index);
			if(s.length() == r || s[r] == '\n' || !in_alphabet(s[r])) 	//err2
			{
				if(pattern != 1) cout << "TID: " << tid << ", TYPE: 98(ERR2), POS: " << index << ", LEN: " << (r - index) << endl;
				pattern2++;
				index = r;
			}
			else
			{
				string s1(s, index+1, r-index-1);
				if(pattern != 2) cout << "TID: " << tid << ", TYPE: 2(STR), POS: " << index << ", LEN: " << (r - index+1) << ", VALUE: " << s1 << endl;
				pattern1++;				
				index = r + 1;
			}
			tid ++;
			continue;
		}
		// newline
		if(s[index] == '\n')	
		{
			if(pattern != 1) cout << "TID: " << tid << ", TYPE: 6(NEWLINE), POS: " << index << ", LEN: 1" << endl;
			pattern2++;
			index++;
			tid++;
			continue;
		}
		// whitespace
		if(s[index] == '\t' || s[index] == ' ')	
		{
			int r = writespace_analyze(s, index);
			if(pattern != 1) cout << "TID: " << tid << ", TYPE: 5(WS), POS: " << index << ", LEN: " << (r-index) << endl;
			pattern2++;
			index = r;
			tid++;
			continue;
		}
		//integer or float(start with a digit)
		if(is_digit(s[index]))
		{
			int r = int_float_analyze(s, index);
			if(r >= s.length() || s[r] != '.')	//integer
			{
				string s1(s, index, r-index);
				if(pattern != 2) cout << "TID: " << tid << ", TYPE: 3(INT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
			}
			else
			{
				r = int_float_analyze(s, r);
				string s1(s, index, r-index);
				if(pattern != 2) cout << "TID: " << tid << ", TYPE: 4(FLOAT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
			}
			pattern1++;
			index = r;
			tid++;
			continue;
		}
		//float(start with '.')
		if(s[index] == '.')
		{
			int r = int_float_analyze(s, index);
			if(r == index+1) //ERR4, the back of . is not a digit
			{
				if(pattern != 1) cout << "TID: " << tid << ", TYPE: 100(ERR4), POS: " << index << ", LEN: 1" << endl;
				pattern2++;
			}
			else
			{
				string s1(s, index, r-index);
				if(pattern != 2) cout << "TID: " << tid << ", TYPE: 4(FLOAT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
				pattern1++;
			}
			index = r;
			tid++;
			continue;
		}
		//comment
		if(s[index] == '#')
		{
			int r = comment_analyze(s, index);
			string s1(s, index, r-index);
			if(pattern != 1) cout << "TID: " << tid << ", TYPE: 7(COMMENT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
			pattern2++;
			index = r;
			tid++;
			continue;
		}
		//for
		if(s[index] == 'f' && is_keyword(s, index, "for"))
		{
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: 8(FOR), POS: " << index << ", LEN: 3" << ", VALUE: for" << endl;
			pattern1++;
			index += 3;
			tid++;
			continue;
		}
		//while
		if(s[index] == 'w' && is_keyword(s, index, "while"))
		{
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: 9(WHILE), POS: " << index << ", LEN: 5" << ", VALUE: while" << endl;
			pattern1++;
			index += 5;
			tid++;
			continue;
		}
		//if
		if(s[index] == 'i' && is_keyword(s, index, "if"))
		{
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: 10(IF), POS: " << index << ", LEN: 2" << ", VALUE: if" << endl;
			pattern1++;
			index += 2;
			tid++;
			continue;
		}
		//else
		if(s[index] == 'e' && is_keyword(s, index, "else"))
		{
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: 11(ELSE), POS: " << index << ", LEN: 4" << ", VALUE: else" << endl;
			pattern1++;
			index += 4;
			tid++;
			continue;
		}
		// identifier
		if(is_letter(s[index]) || s[index] == '_')
		{
			int r = identifier_analyze(s, index);
			string s1(s, index, r-index);
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: 1(ID), POS: " << index << ", LEN: " << (r-index) << ", VALUE: " << s1 << endl;
			pattern1++;
			index = r;
			tid++;
			continue;
		}
		// <= >= != :=
		string operator1 = "<>!:";
		int oper_index1 = operator1.find(s[index]);
		if(oper_index1 != -1 && index+1 < s.length() && s[index+1] == '=')
		{
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: " << (29+oper_index1) << "(" << s[index] << "=), POS: " << index << ", LEN: 2" << endl;
			pattern1++;
			index += 2;
			tid++;
			continue;
		}// !%&|+-*/{}[];,<>=
		string operator2 = "!%&|+-*/{}[];,<>=";	//type starts 13 
		int oper_index2 = operator2.find(s[index]);
		if(-1 != oper_index2)
		{
			if(pattern != 2) cout << "TID: " << tid << ", TYPE: " << (13+oper_index2) << "(" << s[index] << "), POS: " << index << ", LEN: 1" << endl;
			pattern1++;
			index++;
			tid++;
			continue;
		}
		// err3
		if(!in_alphabet(s[index]))
		{
			int r = err3_analyze(s, index);
			if(pattern != 1) cout << "TID: " << tid << ", TYPE: 99(ERR3), POS: " << index << ", LEN: " << (r-index) << endl;
			pattern2++;
			index = r;
			tid++;
			continue;
		}
		// invalid, err4
		if(pattern != 1) cout << "TID: " << tid << ", TYPE: 100(ERR4), POS: " << index << ", LEN: 1" << endl;
		pattern2++;
		index++;
		tid++;
	}
	int tokens = tid-1;
	int printed = tokens;
	if(pattern == 1) printed = pattern1;
	if(pattern == 2) printed = pattern2;
	cout << "\nTotals: len = " << s.length() << ", tokens = " << tokens << ", printed = " << printed << "\n" << endl;
}

#include <string.h>
int main(int argc, char* argv[])
{
//	cout << argc << " " << strcmp(argv[1], "1") << endl;
	int pattern = 0;
	string s = "";
	getstring(&s);
	if(argc > 2 || (argc == 2 && strcmp(argv[1], "0") && strcmp(argv[1], "1") && strcmp(argv[1], "2")))
	{
		cout << "TID: 1, TYPE: 97(ERR1), POS: 0, LEN 0\n\n";
		cout << "Totals: len = 0, tokens = 1, printed = 1\n" << endl;
		return 1;
	}
//	cout << s << endl;
/*	cout << s.length() << endl;
	for(int i = 0; i < s.length(); i++)
		cout << s[i] << endl; */
	if(argc == 2 && !strcmp(argv[1], "1")) pattern = 1;
	if(argc == 2 && !strcmp(argv[1], "2")) pattern = 2;
	morphology_analyze(s, pattern);
	return 0;
} 
