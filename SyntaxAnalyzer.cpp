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

#define PRINT_WITH_TAB(ct,TXT)	\
			{int  tt = (ct); while(tt--)fprintf(xfp,"  "); fprintf(xfp,TXT);}

#define PRINT_IDEN_WITH_TAB(ct,TXT,ID)	\
			{int  tt = (ct); while(tt--)fprintf(xfp,"  "); fprintf(xfp,TXT,ID);}

#define PRINT_TABS(ct)	\
	{int  tt = (ct); while(tt--)fprintf(xfp,"  ");}


//eNUMS
enum  eSYMBOLS
{
	eLeftCurl,
	eRightCurl,
	eLeftBracket,
	eRightBracket,
	eSemiColon,
	eSquareBracketOpen,
	eSquareBracketClose,
	eDot,
	eComma,
	eOpStart,
	ePlus = eOpStart,
	eMinus,
	eMultiply,
	eDivide,
	eLessThan,
	eGreaterThan,
	eEqual,
	eLogicalAnd,
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
	key_IF,
	key_WHILE,
	key_LET,
	key_DO,
	key_RETURN,
	key_STATEMENTS_CNT,
	key_CONST = key_STATEMENTS_CNT,
	key_FUN,
	key_METHOD,
	key_FIELD,
	key_STATIC,
	key_VAR,
	key_INT,
	key_CHAR,
	key_BOOL,
	key_VOID,
	key_KW_CONST_START,
	key_TRUE = key_KW_CONST_START,
	key_FALSE,
	key_NULL,
	key_THIS,
	key_KW_CONST_END = key_THIS,
	key_ELSE,
	key_CLASS,
	key_NONE
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
	{ '&', eLogicalAnd },
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
	FILE* xp;
	string CurLine;
	FILE* ip;
	int idxInLine;
public:
	string CurToken;

	JackTokenizer(FILE* ffp)
	{
		CurToken.clear();
		CurLine.clear();
		xp = NULL;
		ip = ffp;
		idxInLine = 0;
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

	bool hasMoreTokens()
	{
		if (feof(ip) && CurLine.size() == 0)
			return false;
		return true;
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

	void advance()
	{
		char line[100];

		if (CurLine.size() == 0)
		{
			while (fgets(line, 100, ip) != NULL)
			{
				if (line[0] != '\n')//ignore Empty Line
				{
					int i = 0;
					while (i < 99 && (line[i] == ' ' || line[i] == '\t'))i++;
					if (line[i] == '/' || line[i] == '*' || line[i] == '\n')//Ignore line Having only comments
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
						} while (c != '\n' &&  !(c == '/' && line[i + j + 1] == '/'));
						while (ins.back() == ' ')ins.pop_back();
						//objJackTokenizer.ProcessOneLine(ins);
						CurLine = ins;
						idxInLine = 0;
						break;
					}
				}
			}
		}
		ProcessOneLine();
		if (idxInLine == CurLine.size())
			CurLine.clear();
		//Assert(CurToken.size() > 0, "Empty Cur Token");
		printf("%s\n", CurToken.c_str());
	}

	void ProcessOneLine()
	{
		CurToken.clear();
		for (int i = idxInLine; i < CurLine.size(); ++i)
		{
			if (CurLine[i] == '\"')
			{
				//CurToken.push_back('\"');
				while (++i<CurLine.size() && CurLine[i] != '\"')
					CurToken.push_back(CurLine[i]);
				if (CurToken.size() > 0)
				{
					idxInLine = i+1;
					return;
					//ProcessCurrentToken();
				}
				CurToken.clear();
				continue;
			}
			if (CurLine[i] == ' ' || SymbolsMap.find(CurLine[i]) != SymbolsMap.end())
			{
				if (CurToken.size()>0)
				{
					idxInLine = i;
					return;
					//ProcessCurrentToken();
				}
				CurToken.clear();
				if (CurLine[i] != ' ')
				{
					CurToken.push_back(CurLine[i]);
					idxInLine = i+1;
					return;
					ProcessCurrentToken();
					CurToken.clear();
				}
			}
			else
				CurToken.push_back(CurLine[i]);
		}
		CurLine.clear();
		if (CurToken.size() > 0)
		{
			idxInLine = CurToken.size();
			return;
			ProcessCurrentToken();
		}
	}
	void eat(string match)
	{
		if (match == CurToken)
		{
			advance();
		}
		else
		{
			Assert(false, "Compile Error");
		}
	}
};


class CompilationEngine{
	FILE* xfp;
	JackTokenizer * pJackTok;
	int curTab;
	void(CompilationEngine::*pfCompileStatement[key_STATEMENTS_CNT])();
public:
	CompilationEngine(FILE* tfp,char* fn)
	{
		pJackTok = new JackTokenizer(tfp);
		pJackTok->advance();
		xfp = fopen(fn, "w");
		Assert(xfp, "Unable to open XML File");
		//fprintf(xp, "<tokens>\n");
		curTab = 0;
		
	}

	void eatPrint(string match,string Type)
	{
		if (match == pJackTok->CurToken)
		{
			pJackTok->advance();
			PRINT_TABS(curTab);
			fprintf(xfp, "<%s> %s </%s>\n", Type.c_str(), match.c_str(), Type.c_str());
		}
		else
		{
			Assert(false, "Compile Error");
		}
	}
	void PrintType()
	{
		if (KeyWordsMap.find(pJackTok->CurToken) != KeyWordsMap.end())
		{
			PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
		}
		else
		{
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
		}
	}
	void CompileClassVarDec()
	{
		while (pJackTok->CurToken == "static" || pJackTok->CurToken == "field")
		{
			PRINT_WITH_TAB(curTab++, "<classVarDec>\n")

			PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			//PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			PrintType();
			pJackTok->advance();
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			while (pJackTok->CurToken == ",")
			{
				eatPrint(",","symbol");
				PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
				pJackTok->advance();
			}
			pJackTok->eat(";");
			PRINT_WITH_TAB(curTab--, "<symbol> ; </symbol>\n");
			PRINT_WITH_TAB(curTab, "</classVarDec>\n")

		}
	}

	void CompileParamList()
	{
		PRINT_WITH_TAB(curTab++, "<parameterList>\n")

		while (pJackTok->CurToken != ")")
		{
			PrintType();
			//PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			if (pJackTok->CurToken == ",")
			{
				eatPrint(",", "symbol");
			}
		}

		PRINT_WITH_TAB(--curTab, "</parameterList>\n")
	}


	void CompileSubRoutineDec()
	{
		
			PRINT_WITH_TAB(curTab++, "<subroutineDec>\n");
			PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			PrintType();
			//PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			eatPrint("(","symbol");
			CompileParamList();
			eatPrint(")", "symbol");
			CompileSubRoutineBody();
			PRINT_WITH_TAB(--curTab, "</subroutineDec>\n");

	}

	void CompileVarDec()
	{
		while (pJackTok->CurToken == "var")
		{
			PRINT_WITH_TAB(curTab++, "<varDec>\n");
			eatPrint("var", "keyword");
			PrintType();
			pJackTok->advance();
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			while (pJackTok->CurToken == ",")
			{
				eatPrint(",", "symbol");
				//PrintType();
				//pJackTok->advance();
				PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
				pJackTok->advance();
			}
			eatPrint(";", "symbol");
			PRINT_WITH_TAB(--curTab, "</varDec>\n");
		}

	}
	
	KEYWORDS isNextStatement()
	{
		KEYWORDS retKW = key_NONE;
		if (KeyWordsMap.find(pJackTok->CurToken) != KeyWordsMap.end())
		{
			KEYWORDS KW_map = KeyWordsMap[pJackTok->CurToken];
			if (KW_map == key_IF || KW_map == key_WHILE || KW_map == key_LET || KW_map == key_DO || KW_map == key_RETURN)
				retKW = KW_map;
		}
		return retKW;
	}

	void CompileTerm()
	{
		PRINT_WITH_TAB(curTab++, "<term>\n");
		string nexTok = pJackTok->CurToken;
		//KeyWordConstant
		if (KeyWordsMap.find(nexTok) != KeyWordsMap.end())
		{
			eatPrint(nexTok, "keyword");
			Assert(KeyWordsMap[nexTok] >= key_KW_CONST_START && KeyWordsMap[nexTok] <= key_KW_CONST_END, "Wrong KeyWord Constant");
		}
		else{
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
		}
		PRINT_WITH_TAB(--curTab, "</term>\n");
	}

	void compileExpression()
	{
		PRINT_WITH_TAB(curTab++, "<expression>\n");
		CompileTerm();
		string nextTok = pJackTok->CurToken;
		while (nextTok.size() == 1 &&
			SymbolsMap.find(nextTok[0]) != SymbolsMap.end() &&
			SymbolsMap[nextTok[0]] >= eOpStart)
		{
			//Op Term
			eatPrint(nextTok, "symbol");
			CompileTerm();
			nextTok = pJackTok->CurToken;
		}
		PRINT_WITH_TAB(-- curTab, "</expression>\n");
	}


	void compileStatements()
	{
		PRINT_WITH_TAB(curTab++, "<statements>\n");
		while (1)
		{
			KEYWORDS eKW = isNextStatement();
			if (eKW == key_NONE)break;
			(this->*pfCompileStatement[eKW])();
		}
		PRINT_WITH_TAB(--curTab, "</statements>\n");
	}

	void compileIfStatement()
	{
		PRINT_WITH_TAB(curTab++, "<ifStatement>\n");
		eatPrint("if", "keyword");
		eatPrint("(", "symbol");
		compileExpression();
		eatPrint(")", "symbol");
		eatPrint("{", "symbol");
		compileStatements();
		eatPrint("}", "symbol");
		//else part
		if (pJackTok->CurToken == "else")
		{
			eatPrint("else", "keyword");
			eatPrint("{", "symbol");
			compileStatements();
			eatPrint("}", "symbol");
		}
		PRINT_WITH_TAB(--curTab, "</ifStatement>\n");
	}

	void compileWhileStatement()
	{
		PRINT_WITH_TAB(curTab++, "<whileStatement>\n");
		eatPrint("while", "keyword");
		eatPrint("(", "symbol");
		compileExpression();
		eatPrint(")", "symbol");
		eatPrint("{", "symbol");
		compileStatements();
		eatPrint("}", "symbol");
		PRINT_WITH_TAB(--curTab, "</whileStatement>\n");

	}

	void compileLetStatement()
	{
		PRINT_WITH_TAB(curTab++, "<letStatement>\n");
		eatPrint("let", "keyword");
		PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
		pJackTok->advance();
		if (pJackTok->CurToken == "[")
		{
			eatPrint("[", "symbol");
			compileExpression();
			eatPrint("]", "symbol");
		}
		eatPrint("=", "symbol");
		compileExpression();
		eatPrint(";", "symbol");
		PRINT_WITH_TAB(--curTab, "</letStatement>\n");

	}

	void compileExpList()
	{
		PRINT_WITH_TAB(curTab++, "<expressionList>\n");
		while (pJackTok->CurToken != ")")
		{
			compileExpression();
			while (pJackTok->CurToken == ",")
			{
				eatPrint(",", "symbol");
				compileExpression();
			}
		}
		PRINT_WITH_TAB(--curTab, "</expressionList>\n");

	}


	void CompileSubRoutineCall()
	{
		//Sub Routine Name
		PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
		pJackTok->advance();
		if (pJackTok->CurToken == ".")
		{
			eatPrint(".", "symbol");
			//Sub Routine Name
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
		}
		eatPrint("(", "symbol");
		compileExpList();
		eatPrint(")", "symbol");

	}


	void compileDoStatement()
	{
		PRINT_WITH_TAB(curTab++, "<doStatement>\n");
		eatPrint("do", "keyword");
		CompileSubRoutineCall();
		eatPrint(";", "symbol");
		PRINT_WITH_TAB(--curTab, "</doStatement>\n");

	}

	void compileReturnStatement()
	{
		PRINT_WITH_TAB(curTab++, "<returnStatement>\n");
		eatPrint("return", "keyword");
		if (pJackTok->CurToken != ";")
			compileExpression();
		eatPrint(";", "symbol");
		PRINT_WITH_TAB(--curTab, "</returnStatement>\n");

	}

	void initializeFP()
	{
		pfCompileStatement[0] = &CompilationEngine::compileIfStatement;
		pfCompileStatement[1] = &CompilationEngine::compileWhileStatement;
		pfCompileStatement[2] = &CompilationEngine::compileLetStatement;
		pfCompileStatement[3] = &CompilationEngine::compileDoStatement;
		pfCompileStatement[4] = &CompilationEngine::compileReturnStatement;
	}



	void CompileSubRoutineBody()
	{
		PRINT_WITH_TAB(curTab++, "<subroutineBody>\n");
		eatPrint("{","symbol");
		CompileVarDec();
		compileStatements();
		eatPrint("}", "symbol");
		PRINT_WITH_TAB(--curTab, "</subroutineBody>\n");

	}

	void CompileClass()
	{
		initializeFP();
		pJackTok->eat("class");
		PRINT_WITH_TAB(curTab++, "<class>\n");
		PRINT_WITH_TAB(curTab, "<keyword> class </keyword>\n");
		PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
		pJackTok->advance();
		eatPrint("{","symbol");
		CompileClassVarDec();
		while (pJackTok->CurToken == "constructor" || pJackTok->CurToken == "function" || pJackTok->CurToken == "method")
			CompileSubRoutineDec();
		eatPrint("}", "symbol");
		PRINT_WITH_TAB(--curTab, "</class>\n");
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
	strcat(hackFn, "Pok.xml");
	/*FILE *hp;
	hp = fopen(hackFn, "w");
	Assert(hp,"\nUnable to Create xml File");
	*/
	//JackTokenizer objJackTokenizer(fp);
	//objJackTokenizer.openXMLFile(hackFn);
	//while(objJackTokenizer.hasMoreTokens())
	//{
	//	objJackTokenizer.advance();
	//}
	CompilationEngine objCompiler(fp,hackFn);
	objCompiler.CompileClass();
#if 0
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
#endif
	if (feof(fp))
		printf("End of File");

	fclose(fp);

}
