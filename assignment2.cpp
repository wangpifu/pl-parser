#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <sstream>
using namespace std;

string tree = "";

class Node
{   int tid;
	int typenum;
    string _typename;
    int pos;
    int len;
    string value;

public:
    Node(int TID,int Typenum,string _Typename,int POS, int LEN, string VALUE){
        tid=TID;
        typenum=Typenum;
        _typename=_Typename;
		pos = POS;
        len=LEN;
        value=VALUE;
    };
    Node(int TID,int Typenum,string _Typename,int POS, int LEN){
        tid=TID;
        typenum=Typenum;
        _typename=_Typename;
		pos = POS;
        len=LEN;
        value="";
    };
	int gettid() const { return tid; };
	int gettypenum() const { return typenum; };
	string get_typename() const { return _typename; };
	int getpos() const { return pos; };
	int getlen() const { return len; };
	string getvalue() const { return value;};
    
};














void getstring(string* s)   // get input string
{
    char temp;
    while(!cin.eof())
    {
        cin.get(temp);
        (*s) += temp;
    }
    (*s).erase((*s).length()-1);
}

bool is_letter(char ch)     //ch is a letter, return true;
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

bool is_digit(char ch)      //ch is a digit, return true;
{
   return (ch >= '0' && ch <= '9');
}

bool in_alphabet(char ch)
{
    return (ch == 9 || ch == 0x0a || (ch >= 0x20 && ch <= 0x7e));
}

int string_analyze(string s, int begin)     // analyze a string
{
    int index = begin+1;
    for(; index < s.length(); index++)
    {
        if(s[index] == '"') // end
            break;
        if(s[index] == '\n' || !in_alphabet(s[index]))  // ERR2
            break;
    }
    return index;
}

int writespace_analyze(string s, int begin) // analyze writespace
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

vector<Node> morphology_analyze(string s, int pattern)
{
	
	
	
    int index = 0;
    int tid = 1, pattern1 = 0, pattern2 = 0;
    vector<Node> dataraw;
    while(index < s.length())
    {
        //string
        if(s[index] == '"')     
        {
            int r = string_analyze(s, index);
            if(s.length() == r || s[r] == '\n' || !in_alphabet(s[r]))   //err2
            {
                //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 98(ERR2), POS: " << index << ", LEN: " << (r - index) << endl;
				//Node a(tid, 98, "ERR2", index, r - index);
				//dataraw.push_back(a);
				cout << "Lex error" << endl;
				exit(0);
                pattern2++;
                index = r;
            }
            else
            {
                string s1(s, index+1, r-index-1);
                //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 2(STR), POS: " << index << ", LEN: " << (r - index+1) << ", VALUE: " << s1 << endl;
				Node a(tid, 2, "STR", index, r - index+1,s1);
				dataraw.push_back(a);
				pattern1++;             
                index = r + 1;
            }
            tid ++;
            continue;
        }
        // newline
        if(s[index] == '\n')    
        {
            //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 6(NEWLINE), POS: " << index << ", LEN: 1" << endl;
			pattern2++;
            index++;
            tid++;
            continue;
        }
        // whitespace
        if(s[index] == '\t' || s[index] == ' ') 
        {
            int r = writespace_analyze(s, index);
            //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 5(WS), POS: " << index << ", LEN: " << (r-index) << endl;
			pattern2++;
            index = r;
            tid++;
            continue;
        }
        //integer or float(start with a digit)
        if(is_digit(s[index]))
        {
            int r = int_float_analyze(s, index);
            if(r >= s.length() || s[r] != '.')  //integer
            {
                string s1(s, index, r-index);
                //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 3(INT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
				Node a(tid, 3, "INT", index, r - index,s1);
				dataraw.push_back(a);
			}
            else
            {
                r = int_float_analyze(s, r);
                string s1(s, index, r-index);
                //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 4(FLOAT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
				Node a(tid, 4, "FLOAT", index, r - index,s1);
				dataraw.push_back(a);
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
                //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 100(ERR4), POS: " << index << ", LEN: 1" << endl;
				//Node a(tid, 100, "ERR4", index, 1);
				//dataraw.push_back(a);
				cout << "Lex error" << endl;
				exit(0);
				pattern2++;
            }
            else
            {
                string s1(s, index, r-index);
                //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 4(FLOAT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
				Node a(tid, 4, "FLOAT", index, r - index,s1);
				dataraw.push_back(a);
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
            //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 7(COMMENT), POS: " << index << ", LEN: " << (r - index) << ", VALUE: " << s1 << endl;
			pattern2++;
            index = r;
            tid++;
            continue;
        }
        //for
        if(s[index] == 'f' && is_keyword(s, index, "for"))
        {
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 8(FOR), POS: " << index << ", LEN: 3" << ", VALUE: for" << endl;
			Node a(tid, 8, "FOR", index, 3,"for");
			dataraw.push_back(a);
			pattern1++;
            index += 3;
            tid++;
            continue;
        }
        //while
        if(s[index] == 'w' && is_keyword(s, index, "while"))
        {
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 9(WHILE), POS: " << index << ", LEN: 5" << ", VALUE: while" << endl;
			Node a(tid, 9, "WHILE", index, 5, "while");
			dataraw.push_back(a);
			pattern1++;
            index += 5;
            tid++;
            continue;
        }
        //if
        if(s[index] == 'i' && is_keyword(s, index, "if"))
        {
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 10(IF), POS: " << index << ", LEN: 2" << ", VALUE: if" << endl;
			Node a(tid, 10, "IF", index, 2, "if");
			dataraw.push_back(a);
			pattern1++;
            index += 2;
            tid++;
            continue;
        }
        //else
        if(s[index] == 'e' && is_keyword(s, index, "else"))
        {
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 11(ELSE), POS: " << index << ", LEN: 4" << ", VALUE: else" << endl;
			Node a(tid, 11, "ELSE", index, 4, "else");
			dataraw.push_back(a);
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
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: 1(ID), POS: " << index << ", LEN: " << (r-index) << ", VALUE: " << s1 << endl;
			Node a(tid, 1, "ID", index, r-index, s1);
			dataraw.push_back(a);
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
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: " << (29+oper_index1) << "(" << s[index] << "=), POS: " << index << ", LEN: 2" << endl;
			//http://stackoverflow.com/questions/17201590/c-convert-from-1-char-to-string
			Node a(tid, 29+oper_index1, string(1, s[index])+"=", index, 2);
			dataraw.push_back(a);
			pattern1++;
            index += 2;
            tid++;
            continue;
        }// !%&|+-*/{}[];,<>=
        string operator2 = "!%&|+-*/{}[];,<>="; //type starts 12
        int oper_index2 = operator2.find(s[index]);
        if(-1 != oper_index2)
        {
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: " << (12+oper_index2) << "(" << s[index] << "), POS: " << index << ", LEN: 1" << endl;
			
			if (12 + oper_index2 == 13 || 12 + oper_index2 == 19 || 12 + oper_index2 == 25){
				//cout << 12 + oper_index2 << endl;
				cout << "Unimplemented error" << endl;
				exit(0);
			};

			Node a(tid, 12+oper_index2, string(1,s[index]), index,1);
			dataraw.push_back(a);
			pattern1++;
            index++;
            tid++;
            continue;
        }
        // 13+oper_index2 ?
        string operator3="()";
        int oper_index3=operator3.find(s[index]);
        if(-1 != oper_index3)
        {
            //if(pattern != 2) cout << "TID: " << tid << ", TYPE: " << (33+oper_index3) << "(" << s[index] << "), POS: " << index << ", LEN: 1" << endl;
			Node a(tid, 33 + oper_index3, string(1, s[index]), index, 1);
			dataraw.push_back(a);
			pattern1++;
            index++;
            tid++;
            continue;
        }
        // err3
        if(!in_alphabet(s[index]))
        {
            int r = err3_analyze(s, index);
            //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 99(ERR3), POS: " << index << ", LEN: " << (r-index) << endl;
			//Node a(tid, 99, "ERR3", index, r-index);
			//dataraw.push_back(a);
			cout << "Lex error" << endl;
			exit(0);
			pattern2++;
            index = r;
            tid++;
            continue;
        }
        // invalid, err4
        //if(pattern != 1) cout << "TID: " << tid << ", TYPE: 100(ERR4), POS: " << index << ", LEN: 1" << endl;
		//Node a(tid, 100, "ERR4", index, 1);
		//dataraw.push_back(a);
		cout << "Lex error" << endl;
		exit(0);
		pattern2++;
        index++;
        tid++;
    }
    int tokens = tid-1;
    int printed = tokens;
    if(pattern == 1) printed = pattern1;
    if(pattern == 2) printed = pattern2;
    //cout << "\nTotals: len = " << s.length() << ", tokens = " << tokens << ", printed = " << printed << endl;
	return dataraw;
}




// assingment2 start
bool flag = false;
bool expr(int index, vector<Node> dataraw, int indent);
bool stmt(int index, vector<Node> dataraw, int indent);

string basic_expr(int index, vector<Node> dataraw, int indent,string tree){
	flag = false;
	if (dataraw[index].get_typename() == "ID" || "STR" || "INT" || "FLOAT"){ 
		index++;
		string temp = "<id>" + std::to_string(dataraw[index].gettid())+"</id>"+"<typenum>"+std::to_string(dataraw[index].gettypenum())+"</typenum>"+"<typename>"+dataraw[index].get_typename+"</typename>"+"<position>"+std::to_string(dataraw[index].getpos())+"</position>"+"<length>"+std::to_string(dataraw[index].getlen())+"</length>"+dataraw[index].getvalue();
		tree += temp;
		flag = true;
		return tree;
	}
	else {
		cout << "Parse error" << endl;
		exit(0); 
		return false;
	}
}
bool paren_expr(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "("){
		index++;
		if (expr(index, dataraw, indent)) {
			if (dataraw[index].get_typename() == ")") { index++; return true; }
		}
	}
	if (basic_expr(index, dataraw, indent)) { return true; }
	cout << "Parse error" << endl;
	exit(0);
	return false;
}
bool unary_expr(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "!"){ 
		index++;
		if (expr(index, dataraw, indent)) { return true; }
	}
	if (dataraw[index].get_typename() == "+"){
		index++;
		if (expr(index, dataraw, indent)) { return true; }
	}
	if (dataraw[index].get_typename() == "-"){
		index++;
		if (expr(index, dataraw, indent)) { return true; }
	}
	if (paren_expr(index, dataraw, indent)) { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool mult_expr_R(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "*"){
		index++;
		if (unary_expr(index, dataraw, indent)){
			if (mult_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() != "*") { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool mult_expr(int index, vector<Node> dataraw, int indent){
	if (unary_expr(index, dataraw, indent)){
		if (mult_expr_R(index, dataraw, indent)) { return true; }
	};
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool expr_R(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "+"){
		index++;
		if (mult_expr(index, dataraw, indent)){
			if (expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() != "+") { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool expr(int index, vector<Node> dataraw, int indent){
	if (mult_expr(index, dataraw, indent)){
		if (expr_R(index, dataraw, indent)) { return true; }
	};
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool cmp_expr_R(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "<"){
		index++;
		if (expr(index, dataraw, indent)){
			if (cmp_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() == "<="){
		index++;
		if (expr(index, dataraw, indent)){
			if (cmp_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() == ">"){
		index++;
		if (expr(index, dataraw, indent)){
			if (cmp_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() == ">="){
		index++;
		if (expr(index, dataraw, indent)){
			if (cmp_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() != "<" || dataraw[index].get_typename() != "<=" || dataraw[index].get_typename() != ">" || dataraw[index].get_typename() != ">=") { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool cmp_expr(int index, vector<Node> dataraw, int indent){
	if (expr(index, dataraw, indent)){
		if (cmp_expr_R(index, dataraw, indent)){ return true; }
	};
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool eq_expr_R(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "="){
		index++;
		if (cmp_expr(index, dataraw, indent)){
			if (eq_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() == "!="){
		index++;
		if (cmp_expr(index, dataraw, indent)){
			if (eq_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() != "=" || "!=") { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool eq_expr(int index, vector<Node> dataraw, int indent){
	if (cmp_expr(index, dataraw, indent)){
		if (eq_expr_R(index, dataraw, indent)) { return true; }
	};
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool and_expr_R(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "&"){
		index++;
		if (cmp_expr(index, dataraw, indent)){
			if (and_expr_R(index, dataraw, indent)) { return true; }
		}
	}
	if (dataraw[index].get_typename() != "|") { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}
bool and_expr(int index, vector<Node> dataraw, int indent){
	if (eq_expr(index, dataraw, indent)){
		if (and_expr_R(index, dataraw, indent))  {return true; }
	};
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}

bool cond_expr_R(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "|"){
		index++;
		if (and_expr(index, dataraw, indent)){
			if (cond_expr_R(index, dataraw, indent)) { return true; }
		} 
	}
	if (dataraw[index].get_typename() != "|") { return true; }
	cout << "Parse error" << endl;
	exit(0); 
	return false;
	
}

bool cond_expr(int index, vector<Node> dataraw, int indent){
	if (and_expr(index,dataraw,indent)){
		if (cond_expr_R(index, dataraw, indent)) { return true; }
	};
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}

bool output_stmt(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "["){
		index++;
		if (expr(index, dataraw, indent)){
			if (dataraw[index].get_typename() == "]"){
				index++;
				if (dataraw[index].get_typename() == ";") index++; {return true; }
			}
		}
	}
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}

bool assign_stmt(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "ID"){
		index++;
		if (dataraw[index].get_typename() == ":="){
			index++;
			if (expr(index, dataraw, indent)) return true;
		}
	}
	cout << "Parse error" << endl;
	exit(0); 
	return false;
}

bool opt_else(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "ELSE"){
		index++;
		if (dataraw[index].get_typename() == "{"){
			index++;
			if (stmt(index, dataraw, indent)){
				if (dataraw[index].get_typename() == "}") index++; return true;				
			}
		}
	}
	return true;//dataraw[index].get_typename() == ""
}

bool if_stmt(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "IF"){
		index++;
		if (dataraw[index].get_typename() == "("){
			index++;
			if (cond_expr(index, dataraw, indent)){
				if (dataraw[index].get_typename() == ")"){
					index++;
					if (dataraw[index].get_typename() == "{")
						index++;
						if (stmt(index, dataraw, indent)){
							if (dataraw[index].get_typename() == "}"){
								index++;
								if (opt_else(index, dataraw, indent)){
									return true;
								}
							}
						}
				}
			}
		}
	}
	cout << "Parse error" << endl;
	exit(0);
	return false;
}

bool opt_assign(int index, vector<Node> dataraw, int indent){
	if (assign_stmt(index, dataraw, indent)) return true;
	if (dataraw[index].get_typename() == ";") return true;  // no need to index ++
	return false;
}

bool opt_cond_expr(int index, vector<Node> dataraw, int indent){
	if (cond_expr(index, dataraw, indent)) return true;
	if (dataraw[index].get_typename() == ";") return true;  // no need to index ++
	return false;
}

bool for_stmt(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "WHILE"){
		index++;
		if (dataraw[index].get_typename() == "("){
			index++;
			if (opt_assign(index, dataraw, indent)){
				if (dataraw[index].get_typename() == ";"){
					index++;
					if (opt_cond_expr(index, dataraw, indent)){
						if (dataraw[index].get_typename() == ";"){
							index++;
							if (opt_assign(index, dataraw, indent)){
								if (dataraw[index].get_typename() == ")"){
									index++;
									if (dataraw[index].get_typename() == "{"){
										index++;
										if (stmt(index, dataraw, indent)){
											if (dataraw[index].get_typename() == "}") index++; return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	cout << "Parse error" << endl;
	exit(0);
	return false;
}
bool while_stmt(int index, vector<Node> dataraw, int indent){
	if (dataraw[index].get_typename() == "WHILE"){
		index++;
		if (dataraw[index].get_typename() == "("){
			index++;
			if (cond_expr(index, dataraw, indent)){
				if (dataraw[index].get_typename() == ")"){
					index++;
					if (dataraw[index].get_typename() == "{"){
						index++;
						if (stmt(index, dataraw, indent)){
							if (dataraw[index].get_typename() == "}") index++; return true;
						}
					}
				}
			}
		}
	}
	cout << "Parse error" << endl;
	exit(0);
	return false;
}
bool stmt(int index, vector<Node> dataraw, int indent){
	//cout << "Parse error" << endl;
	//exit(0);
	if (while_stmt(index, dataraw, indent)){
		stmt(index, dataraw, indent); return true;
	}
	if (for_stmt(index, dataraw, indent)){
		stmt(index, dataraw, indent); return true;
	}
	if (if_stmt(index, dataraw, indent)){
		stmt(index, dataraw, indent); return true;
	}
	if (assign_stmt(index, dataraw, indent)){
		if (dataraw[index].get_typename() == ";"){
			index++;
			stmt(index, dataraw, indent); return true;
		}
	}
	cout << "Parse error" << endl;
	exit(0);
	return false;
}

void check_grammar(vector<Node> dataraw){
	int index = 0;
	int indent = 0;
	stmt(index, dataraw, indent);
}






int main(int argc, char* argv[])
{
//  cout << argc << " " << strcmp(argv[1], "1") << endl;
    int pattern = 0;
    string s = "";
    getstring(&s);
    //if(argc > 2 || (argc == 2 && strcmp(argv[1], "0") && strcmp(argv[1], "1") && strcmp(argv[1], "2")))
    //{
        //cout << "TID: 1, TYPE: 97(ERR1), POS: 0, LEN 0\n\n";
        //cout << "Totals: len = 0, tokens = 1, printed = 1" << endl;
        //return 1;
		// ignore other arguments for hw2
		//morphology_analyze(s, 0);
    //}
//  cout << s << endl;
/*  cout << s.length() << endl;
    for(int i = 0; i < s.length(); i++)
        cout << s[i] << endl; */
    //if(argc == 2 && !strcmp(argv[1], "1")) pattern = 1;
    //if(argc == 2 && !strcmp(argv[1], "2")) pattern = 2;
    vector<Node> dataraw=morphology_analyze(s, 0);
    //return 0;
	check_grammar(dataraw);
} 
