#include<iostream>
#include<stdio.h>
#include<string>
#include<unordered_map>
#include<locale>
#include<filesystem>
#include<stack>

using namespace std;

//defines
#define Assert(X,Y) if(!(X)) \
{ printf(Y); while (1); }

//eNUMS
enum  eSYMBOLS
{
	eLeftCurl,
	eRightCurl,
	eLeftBracket,
	eRightBracket,
	eLessThan,
	eGreaterThan,
	eEqual,
	eSemiColon,
	eSquareBracketOpen,
	eSquareBracketClose,
	eDot,
	eComma,
	ePlus,
	eMinus,
	eMultiply,
	eDivide,
	eAmpersAnd,
	eLogicalOr,
	eLogicalNot
};

enum eTOKENTYPE
{
	eTT_KEYWORD,
	eTT_SYMBOL,
	eTT_IDENTIFIER,
	eTT_INT_CONST,
	eTT_STRING_CONST
};

enum KEYWORDS
{
	key_CLASS,
	key_IF,
	key_LET,
	key_CONST,
	key_FUN,
	key_METHOD,
	key_FIELD,
	key_STATIC,
	key_VAR,
	key_INT,
	key_CHAR,
	key_BOOL,
	key_VOID,
	key_TRUE,
	key_FALSE,
	key_NULL,
	key_THIS,
	key_DO,
	key_ELSE,
	key_WHILE,
	key_RETURN,
};

//maps
unordered_map<char, eSYMBOLS> SymbolsMap = {
	{ '{', eLeftCurl },
	{ '}', eRightCurl },
	{ '(', eLeftBracket },
	{ ')', eRightBracket },
	{ '<',eLessThan },
	{ '>' ,eGreaterThan},
	{ '=',eEqual },
	{ ';',eSemiColon },
	{'[', eSquareBracketOpen},
	{ ']', eSquareBracketClose },
	{ '.', eDot },
	{ ',', eComma },
	{ '+', ePlus },
	{ '-', eMinus },
	{ '*', eMultiply },
	{ '/', eDivide },
	{ '&', eAmpersAnd },
	{ '|', eLogicalOr },
	{ '~', eLogicalNot }
};

unordered_map<string, KEYWORDS> KeyWordsMap
{
	{ "class", key_CLASS },
	{ "if", key_IF },
	{ "let", key_LET },
	{"constructor",key_CONST},
	{"function",key_FUN},
	{"method",key_METHOD},
	{"field",key_FIELD},
	{"static",key_STATIC},
	{"var",key_VAR},
	{"int",key_INT},
	{"char",key_CHAR},
	{"boolean",key_BOOL},
	{"void",key_VOID},
	{"true",key_TRUE},
	{"false",key_FALSE},
	{"null",key_NULL},
	{"this",key_THIS},
	{"do",key_DO},
	{"else",key_ELSE},
	{ "while", key_WHILE},
	{ "return", key_RETURN}
};

//classes
class JackTokenizer{
	string CurToken;
	FILE* xp;
public:
	JackTokenizer()
	{
		CurToken.clear();
		xp = NULL;
	}
	~JackTokenizer()
	{
		if (xp)
		{
			fprintf(xp, "</tokens>\n");
			fclose(xp);
		}
	}
	void openXMLFile(char* fn)
	{
		xp = fopen(fn, "w");
		Assert(xp, "Unable to open XML File");
		fprintf(xp, "<tokens>\n");
	}

	eTOKENTYPE getTokenType()
	{
		if (CurToken.size() == 1 && SymbolsMap.find(CurToken[0]) != SymbolsMap.end())
		{
			return eTT_SYMBOL;
		}
		else
		{
			if (KeyWordsMap.find(CurToken) != KeyWordsMap.end())
			{
				return eTT_KEYWORD;
			}
			if (CurToken[0] == '\"')
				return eTT_STRING_CONST;
			if (CurToken[0] >= '0' && CurToken[0] <= '9')
				return eTT_INT_CONST;
			Assert(CurToken[0] >= 'a' && CurToken[0] <= 'z' || CurToken[0] >= 'A' && CurToken[0] <= 'Z' || CurToken[0] == '_', "Wrong Token Type");
			return eTT_IDENTIFIER;
		}
	}
	string getSymbolPrint()
	{
		string retStr;
		Assert(CurToken.size() == 1, "Wrong Symbol Token");
		switch (CurToken[0])
		{
		case '<':
			retStr = "&lt;";
			break;
		case '>':
			retStr = "&gt;";
			break;
		case '"':
			retStr = "&quot;";
			break;
		case '&':
			retStr = "&amp;";
			break;
		default:
			retStr = CurToken;
			break;
		}
		return retStr;
	}
	void ProcessCurrentToken()
	{
		eTOKENTYPE eTT = getTokenType();
		switch (eTT)
		{
		case eTT_KEYWORD:
			printf("keyword %s\n", CurToken.c_str());
			fprintf(xp, "<keyword> %s </keyword>\n", CurToken.c_str());
			break;
		case eTT_SYMBOL:
			printf("Symbol %s\n", CurToken.c_str());
			{
				string strSymbol = getSymbolPrint();
				fprintf(xp, "<symbol> %s </symbol>\n", strSymbol.c_str());
			}
			break;
		case eTT_IDENTIFIER:
			printf("Identifier %s\n", CurToken.c_str());
			fprintf(xp, "<identifier> %s </identifier>\n", CurToken.c_str());
			break;
		case eTT_INT_CONST:
			printf("Integer Constant %s\n", CurToken.c_str());
			Assert(CurToken[0] >= '0' && CurToken[0] <= '9', "Wrong Int Constant");
			fprintf(xp, "<integerConstant> %s </integerConstant>\n", CurToken.c_str());
			break;
		case eTT_STRING_CONST:
			printf("String COnst %s\n", CurToken.c_str());
			Assert(CurToken[0] == '\"', "Wrong String Constant");
			//CurToken.pop_back();
			fprintf(xp, "<stringConstant> %s </stringConstant>\n", CurToken.substr(1).c_str());

			break;
		}
	}
	void ProcessOneLine(string Line)
	{
		CurToken.clear();
		for (int i = 0; i < Line.size(); ++i)
		{
			if (Line[i] == '\"')
			{
				CurToken.push_back('\"');
				while (++i<Line.size() && Line[i] != '\"')
					CurToken.push_back(Line[i]);
				if (CurToken.size()>0)
					ProcessCurrentToken();
				CurToken.clear();
				continue;
			}
			if (Line[i] == ' ' || SymbolsMap.find(Line[i]) != SymbolsMap.end())
			{
				if (CurToken.size()>0)
				ProcessCurrentToken();
				CurToken.clear();
				if (Line[i] != ' ')
				{
					CurToken.push_back(Line[i]);
					ProcessCurrentToken();
					CurToken.clear();
				}
			}
			else
				CurToken.push_back(Line[i]);
		}
		if (CurToken.size()>0)
			ProcessCurrentToken();
	}
};

int main(int argc, char* argv[])
{
	system("cd");
	Assert(argc >= 2, "Give file name to open");
	char hackFn[50];
	FILE* fp = fopen(argv[1], "r");
	Assert(fp, "\nFile Not found");
	char *fn = strtok(argv[1], ".");
	strcpy(hackFn, fn);
	strcat(hackFn, "Tok.xml");
	/*FILE *hp;
	hp = fopen(hackFn, "w");
	Assert(hp,"\nUnable to Create xml File");
	*/
	JackTokenizer objJackTokenizer;
	objJackTokenizer.openXMLFile(hackFn);
	char line[100];
	//read each line
	while (fgets(line, 100, fp) != NULL)
	{
		if (line[0] != '\n')//ignore Empty Line
		{
			int i = 0;
			while (i < 99 && (line[i] == ' ' || line[i] == '\t'))i++;
			if (line[i] == '/' || line[i] == '*' || line[i]=='\n')//Ignore line Having only comments
				continue;
			else
			{
				int j = 0;
				string ins;
				char c = line[i + j];
				do
				{
					ins.push_back(c);
					j++;
					c = line[i + j];
				} while (c != '\n' &&  !(c == '/' && line[i+j+1]=='/'));
				while (ins.back() == ' ')ins.pop_back();
				objJackTokenizer.ProcessOneLine(ins);
			}
		}
	}


	fclose(fp);

}
