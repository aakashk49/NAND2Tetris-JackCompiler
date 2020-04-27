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
enum eVarKind{
	eStaticVar,
	eFieldVar,
	eArgVar,
	eLocalVar,
	eVarKindCnt,
	eUndefVar = eVarKindCnt
};
//maps
unordered_map<eVarKind, string> SegmentMap = {
	{ eStaticVar, "static" },
	{eFieldVar, "this"},
	{eArgVar, "argument"},
	{eLocalVar, "local"}
};

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

unordered_map<char, string> OpSymbMap = {
	{ '+', "add" },
	{ '-', "sub" },
	{ '*', "call Math.multiply 2" },
	{ '/', "call Math.divide 2" },
	{ '=', "eq" },
	{ '<', "lt" },
	{ '>', "gt" },
	{ '~', "not" },
	{ '&', "and"},
	{ '|', "or"},
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

unordered_map<string, eVarKind>VarKindMap =
{
	{"static",eStaticVar },
	{"field",eFieldVar},
	{"var",eArgVar},
	//{"",eLocalVar},
};
//Structs
typedef struct
{
	string type;
	eVarKind eKind;
	int nKindCnt;
	string name;
}VarP;

//unordered_map<string, VarP> SybmolTable;
//classes

class SymbolTable{
	unordered_map<string, VarP> Table;
	int anKindCnt[eVarKindCnt];
public:
	SymbolTable()
	{
		memset(anKindCnt, 0, sizeof(anKindCnt));
	}
	void StartSubRoutine()
	{
		Table.clear();
		memset(anKindCnt, 0, sizeof(anKindCnt));

	}
	bool isVarDec(string name)
	{
		return (Table.find(name) != Table.end());
	}
	void Define(string name,string type,eVarKind eKind)
	{
		VarP stTemp = { type, eKind, anKindCnt[eKind]++,name };
		Table[name] = stTemp;
	}
	void Define(VarP stVar)
	{
		stVar.nKindCnt = anKindCnt[stVar.eKind]++;
		//VarP stTemp = { stVar.type, stVar.eKind, anKindCnt[stVar.eKind]++, stVar.name };
		Table[stVar.name] = stVar;
	}
	int VarCount(eVarKind eKind)
	{
		return anKindCnt[eKind];
	}
	eVarKind KindOf(string name)
	{
		if (Table.find(name) != Table.end())
		{
			return Table[name].eKind;
		}
		else
			Assert(false, "No Var of this name");
	}
	string TypeOf(string name)
	{
		if (Table.find(name) != Table.end())
		{
			return Table[name].type;
		}
		else
			Assert(false, "No Var of this name");
	}
	int IndexOf(string name)
	{
		if (Table.find(name) != Table.end())
		{
			return Table[name].nKindCnt;
		}
		else
			Assert(false, "No Var of this name");
	}
};

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
				CurToken.push_back('\"');
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
	string ClassName;
	FILE* xfp;
	FILE* fpVM;
	bool LHSTerm;
	int nLabelCnt;
	JackTokenizer * pJackTok;
	SymbolTable objClassLevelST;
	SymbolTable objRoutineLevelST;
	int curTab;
	void(CompilationEngine::*pfCompileStatement[key_STATEMENTS_CNT])();
public:
	CompilationEngine(FILE* tfp,char* fn,char* tVmf)
	{
		pJackTok = new JackTokenizer(tfp);
		pJackTok->advance();
		xfp = fopen(fn, "w");
		Assert(xfp, "Unable to open XML File");
		fpVM = fopen(tVmf, "w");
		Assert(fpVM, "Unable to open Output VM File");
		//fprintf(xp, "<tokens>\n");
		curTab = 0;
		LHSTerm = false;
		nLabelCnt = 0;
	}
	~CompilationEngine()
	{
		if (xfp)
			fclose(xfp);
		if (fpVM)
			fclose(fpVM);
	}
	void PrintVar(string PushPop, string varName)
	{
		SymbolTable* pObjST = NULL;
		if (objRoutineLevelST.isVarDec(varName))
		{
			pObjST = &objRoutineLevelST;
		}
		else if (objClassLevelST.isVarDec(varName))
		{
			pObjST = &objClassLevelST;
		}
		else
		{
		//	return;
			Assert(false, "Variable Not present");
		}
		//SegmentMap
		fprintf(fpVM, "%s %s %d\n", PushPop.c_str(), SegmentMap[pObjST->KindOf(varName)].c_str(),pObjST->IndexOf(varName));
	}

	void PrintVarProperties(string varName,bool bDec)
	{
		if (objRoutineLevelST.isVarDec(varName))
		{
			PRINT_TABS(curTab);
			fprintf(xfp, "kind = %d, type = %s,index = %d\n", objRoutineLevelST.KindOf(varName), objRoutineLevelST.TypeOf(varName).c_str(), objRoutineLevelST.IndexOf(varName));
		}
		else if (objClassLevelST.isVarDec(varName))
		{
			PRINT_TABS(curTab);
			fprintf(xfp, "kind = %d, type = %s,index = %d\n", objClassLevelST.KindOf(varName),objClassLevelST.TypeOf(varName).c_str(),objClassLevelST.IndexOf(varName));
		}
		else
		{
			return;
			Assert(false, "Variable Not present");
		}
		if (bDec)
			PRINT_WITH_TAB(curTab, "<VarWasDec></VarWasDec>\n")
		else
		PRINT_WITH_TAB(curTab, "<VarWasUsed></VarWasUsed>\n")
	}


	void eatPrint(string match,string Type)
	{
		if (match == pJackTok->CurToken)
		{
			PRINT_TABS(curTab);
			if (Type == "symbol")
			{
				fprintf(xfp, "<%s> %s </%s>\n", Type.c_str(), pJackTok->getSymbolPrint().c_str(), Type.c_str());
			}
			else
				fprintf(xfp, "<%s> %s </%s>\n", Type.c_str(), match.c_str(), Type.c_str());
			pJackTok->advance();
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
			VarP stVar;
			string kind = pJackTok->CurToken;
			stVar.eKind = VarKindMap[pJackTok->CurToken];
			Assert(stVar.eKind == eStaticVar || stVar.eKind == eFieldVar, "Wrong Class Variables");
			PRINT_WITH_TAB(curTab++, "<classVarDec>\n")

			PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			//PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			stVar.type = pJackTok->CurToken;
			PrintType();
			pJackTok->advance();
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			//ST

			PRINT_IDEN_WITH_TAB(curTab, "<kind> %s </kind>\n", kind.c_str());
			stVar.name = pJackTok->CurToken;
			objClassLevelST.Define(pJackTok->CurToken, stVar.type, stVar.eKind);
			PrintVarProperties(stVar.name, true);

			pJackTok->advance();
			while (pJackTok->CurToken == ",")
			{
				eatPrint(",","symbol");
				PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
				//ST
				PRINT_IDEN_WITH_TAB(curTab, "<kind> %s </kind>\n", kind.c_str());
				stVar.name = pJackTok->CurToken;
				objClassLevelST.Define(pJackTok->CurToken, stVar.type, stVar.eKind);
				PrintVarProperties(stVar.name, true);
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
			VarP stVar;
			stVar.eKind = eArgVar;
			stVar.type = pJackTok->CurToken;
			PrintType();
			//PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
			stVar.name = pJackTok->CurToken;
			objRoutineLevelST.Define(stVar);
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			PrintVarProperties(stVar.name, true);
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
		objRoutineLevelST.StartSubRoutine();
		PRINT_WITH_TAB(curTab++, "<subroutineDec>\n");
		PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
		if (pJackTok->CurToken == "method")
		{
			PRINT_WITH_TAB(curTab, "<this></this>\n");
			objRoutineLevelST.Define("this", ClassName, eArgVar);
			PrintVarProperties("this", true);
		}
		pJackTok->advance();
		PrintType();
		//PRINT_IDEN_WITH_TAB(curTab, "<keyword> %s </keyword>\n", pJackTok->CurToken.c_str());
		pJackTok->advance();
		PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
		fprintf(fpVM, "function %s.%s ", ClassName.c_str(), pJackTok->CurToken.c_str());

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
			VarP stVar;
			stVar.eKind = eLocalVar;
			PRINT_WITH_TAB(curTab++, "<varDec>\n");
			eatPrint("var", "keyword");
			PrintType();
			stVar.type = pJackTok->CurToken;
			pJackTok->advance();
			stVar.name = pJackTok->CurToken;
			objRoutineLevelST.Define(stVar);
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			PrintVarProperties(stVar.name, true);
			pJackTok->advance();
			while (pJackTok->CurToken == ",")
			{
				eatPrint(",", "symbol");
				//PrintType();
				//pJackTok->advance();
				stVar.name = pJackTok->CurToken;
				objRoutineLevelST.Define(stVar);
				PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
				PrintVarProperties(stVar.name, true);
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

		if (nexTok == "(")
		{
			eatPrint("(", "symbol");
			compileExpression();
			eatPrint(")", "symbol");
		}
		else if (KeyWordsMap.find(nexTok) != KeyWordsMap.end())//KeyWordConstant
		{
			if (nexTok == "false" || nexTok == "null")
				fprintf(fpVM, "push constant 0\n");
			else if (nexTok == "true")
				fprintf(fpVM, "push constant 1\nneg\n");
			eatPrint(nexTok, "keyword");
			Assert(KeyWordsMap[nexTok] >= key_KW_CONST_START && KeyWordsMap[nexTok] <= key_KW_CONST_END, "Wrong KeyWord Constant");
		}
		else if (nexTok == "-" || nexTok == "~") //unary Op
		{
			eatPrint(nexTok, "symbol");
			CompileTerm();
			if (nexTok == "-")
			fprintf(fpVM, "neg\n");
			else 
				fprintf(fpVM, "not\n");

		}
		else if (nexTok[0] == '\"')//string constant
		{
			PRINT_IDEN_WITH_TAB(curTab, "<stringConstant> %s </stringConstant>\n", nexTok.substr(1).c_str());
			pJackTok->advance();
		}
		else if (nexTok[0] >= '0' && nexTok[0] <= '9')//integer Constant
		{
			PRINT_IDEN_WITH_TAB(curTab, "<integerConstant> %s </integerConstant>\n", nexTok.c_str());
			fprintf(fpVM, "push constant %s\n", nexTok.c_str());
			pJackTok->advance();
		}
		else{
			pJackTok->advance();
			string nnTok = pJackTok->CurToken;
			if (nnTok == "(" || nnTok == ".")
			{
				CompileSubRoutineCall(nexTok);
			}
			else if (nnTok == "[")
			{
				PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", nexTok.c_str());
				PrintVarProperties(nexTok, false);
				eatPrint("[", "symbol");
				compileExpression();
				eatPrint("]", "symbol");
			}
			else
			{
				PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", nexTok.c_str());
				PrintVarProperties(nexTok, false);
				PrintVar("push", nexTok);
				//pJackTok->advance();
			}
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
			Assert((OpSymbMap.find(nextTok[0]) != OpSymbMap.end()), "Wrong Operation");
			fprintf(fpVM, "%s\n", OpSymbMap[nextTok[0]].c_str());
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
		int lclLblCnt = ++nLabelCnt;
		PRINT_WITH_TAB(curTab++, "<ifStatement>\n");
		eatPrint("if", "keyword");
		eatPrint("(", "symbol");
		compileExpression();
		eatPrint(")", "symbol");
		fprintf(fpVM, "not\nif-goto IF_ELSE%d\n", lclLblCnt);
		eatPrint("{", "symbol");
		compileStatements();
		eatPrint("}", "symbol");
		fprintf(fpVM, "goto IF_EXIT%d\nlabel IF_ELSE%d\n", lclLblCnt, lclLblCnt);
		//else part
		if (pJackTok->CurToken == "else")
		{
			eatPrint("else", "keyword");
			eatPrint("{", "symbol");
			compileStatements();
			eatPrint("}", "symbol");
		}
		fprintf(fpVM, "label IF_EXIT%d\n", lclLblCnt);
		PRINT_WITH_TAB(--curTab, "</ifStatement>\n");
	}

	void compileWhileStatement()
	{
		int lclLblCnt = ++nLabelCnt;
		fprintf(fpVM, "label WHILE_TRUE%d\n", lclLblCnt);
		PRINT_WITH_TAB(curTab++, "<whileStatement>\n");
		eatPrint("while", "keyword");
		eatPrint("(", "symbol");
		compileExpression();
		fprintf(fpVM, "not\nif-goto WHILE_FALSE%d\n", lclLblCnt);
		eatPrint(")", "symbol");
		eatPrint("{", "symbol");
		compileStatements();
		fprintf(fpVM, "goto WHILE_TRUE%d\nlabel WHILE_FALSE%d\n", lclLblCnt, lclLblCnt);
		eatPrint("}", "symbol");
		PRINT_WITH_TAB(--curTab, "</whileStatement>\n");
	}

	void compileLetStatement()
	{
		PRINT_WITH_TAB(curTab++, "<letStatement>\n");
		eatPrint("let", "keyword");
		string LHSVar = pJackTok->CurToken;
		PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
		PrintVarProperties(pJackTok->CurToken,false);
		pJackTok->advance();
		if (pJackTok->CurToken == "[")
		{
			eatPrint("[", "symbol");
			compileExpression();
			eatPrint("]", "symbol");
		}
		eatPrint("=", "symbol");
		compileExpression();
		PrintVar("pop", LHSVar);
		eatPrint(";", "symbol");
		PRINT_WITH_TAB(--curTab, "</letStatement>\n");

	}

	int compileExpList()
	{
		int ArgCnt = 0;
		PRINT_WITH_TAB(curTab++, "<expressionList>\n");
		while (pJackTok->CurToken != ")")
		{
			ArgCnt++;
			compileExpression();
			while (pJackTok->CurToken == ",")
			{
				ArgCnt++;
				eatPrint(",", "symbol");
				compileExpression();
			}
		}
		PRINT_WITH_TAB(--curTab, "</expressionList>\n");
		return ArgCnt;
	}


	void CompileSubRoutineCall(string fTok)
	{
		string funcName = fTok;
		bool bMethodCall = (objClassLevelST.isVarDec(fTok) || objRoutineLevelST.isVarDec(fTok));
		//Sub Routine Name
		PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", fTok.c_str());
		if (!(fTok == "Math" || fTok == "Array" || fTok == "Output" || fTok == "Screen" || fTok == "Keyboard" || fTok == "Memory"
			|| fTok == "Sys"))
			PrintVarProperties(fTok, false);

		//pJackTok->advance();
		if (pJackTok->CurToken == ".")
		{
			eatPrint(".", "symbol");
			funcName.push_back('.');
			funcName.append(pJackTok->CurToken);
			//Sub Routine Name
			PRINT_IDEN_WITH_TAB(curTab, "<identifier> %s </identifier>\n", pJackTok->CurToken.c_str());
			pJackTok->advance();
		}
		eatPrint("(", "symbol");
		int argCnt = compileExpList() + (bMethodCall?1:0);
		eatPrint(")", "symbol");
		fprintf(fpVM, "call %s %d\n", funcName.c_str(), argCnt);

	}


	void compileDoStatement()
	{
		PRINT_WITH_TAB(curTab++, "<doStatement>\n");
		eatPrint("do", "keyword");
		string tempTok = pJackTok->CurToken;
		pJackTok->advance();
		CompileSubRoutineCall(tempTok);
		fprintf(fpVM, "pop temp 0\n");
		eatPrint(";", "symbol");
		PRINT_WITH_TAB(--curTab, "</doStatement>\n");

	}

	void compileReturnStatement()
	{
		PRINT_WITH_TAB(curTab++, "<returnStatement>\n");
		eatPrint("return", "keyword");
		if (pJackTok->CurToken != ";")
			compileExpression();
		else{
			fprintf(fpVM, "push constant 0\n");
		}
		fprintf(fpVM, "return\n");
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
		fprintf(fpVM,"%d\n",objRoutineLevelST.VarCount(eLocalVar));
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
		ClassName = pJackTok->CurToken;
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
	char Vmfp[50];
	FILE* fp = fopen(argv[1], "r");
	Assert(fp, "\nFile Not found");
	char *fn = strtok(argv[1], ".");
	strcpy(hackFn, fn);
	strcpy(Vmfp, fn);
	strcat(hackFn, "Pok.xml");
	strcat(Vmfp, ".vm");
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
	CompilationEngine objCompiler(fp,hackFn,Vmfp);
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
