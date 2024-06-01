#include "msrt_main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include "msrt_tokens.h"
#include "msrt_log.h"
#include "msrt_keywords.h"
#include "msrt_types.h"

#ifndef MSRT_VERSION
#define MSRT_VERSION "0.0"
#endif


std::map<std::string, MSRT::Keyword> keywordMap;
std::map<std::string, MSRT::VarType> vartypeMap;
std::map<std::string, MSRT::Function*> functionMap;

std::vector<MSRT::Sentence> program;
std::vector<std::string> tempVarNames; // only for analysis
std::map<std::string, MSRT::VarType> variablesTypes;

std::map<std::string, std::string> varText;
std::map<std::string, int> varInteger;
std::map<std::string, double> varReal;


class MSRTLogFunction : public MSRT::Function {
public:
	void Execute(std::vector<MSRT::FunctionArgument> args) {
		for (int i = 0; i < args.size(); i++) {
			// std::cout << args.at(i).data.replace(args.at(i).data.find("\\n"), std::string("\\n").length(), "\n");
			for (int j = 0; j < args.at(i).data.size(); j++) {
				if (args.at(i).data.at(j) == '\\') {
					if (args.at(i).data.at(j + 1) == 'n') std::cout << "\n";
					j += 1;
				}
				else std::cout << args.at(i).data.at(j);
			}
		}
	}
};


void MSRT::InitKeywordMap() {
	keywordMap.insert_or_assign("declare", MSRT::Keyword::DECLARE);
}

void MSRT::InitVarTypeMap() {
	vartypeMap.insert_or_assign("Void", MSRT::VarType::VOID);
	vartypeMap.insert_or_assign("Integer", MSRT::VarType::INTEGER);
	vartypeMap.insert_or_assign("Real", MSRT::VarType::REAL);
	vartypeMap.insert_or_assign("Boolean", MSRT::VarType::BOOLEAN);
	vartypeMap.insert_or_assign("Text", MSRT::VarType::TEXT);
	vartypeMap.insert_or_assign("Vec2", MSRT::VarType::VEC2);
	vartypeMap.insert_or_assign("Vec3", MSRT::VarType::VEC3);
	vartypeMap.insert_or_assign("Int3", MSRT::VarType::INT3);
	vartypeMap.insert_or_assign("Ident", MSRT::VarType::IDENT);
}

void MSRT::InitFunctionMap() {
	functionMap.insert_or_assign("log", new MSRTLogFunction);
}

void MSRT::InitMSRT(std::string file, bool dataread, bool verboseparsing)
{
	std::cout << "MSRT: ManiaScript Runtime\nWritten by Safariminer\nsafari.is-probably.gay - github.com/safariminer\nVersion " + (std::string)MSRT_VERSION + "\n\n";
	MSRT::InitKeywordMap();
	MSRT::InitVarTypeMap();
	MSRT::InitFunctionMap();
	std::fstream script(file);
	std::string line;
	std::string data = "";
	if (!script.is_open()) {
		MSRT::Log(MSRT::LogType::ERROR, "Couldn't load file " + file + "!");
		abort();
	}
	while (std::getline(script, line)) data += line + " \n";

	if(dataread){
		std::cout << data + "\n---\n";
	}
	

	bool inComment = false;
	bool inSingleLineComment = false;
	bool inMagic = false;
	MSRT::Sentence newSentence;
	MSRT::Token newToken;
	newToken.type = TokenType::UNDEFINED;
	for (int i = 0; i < data.size(); i++) {

		if (data.at(i) == ';') {
			std::cout << "inComment: " << inComment << "\n";
			std::cout << "inSingleLineComment: " << inSingleLineComment << "\n";
			std::cout << "inMagic: " << inMagic << "\n";
			if (i + 1 < data.size()) std::cout << "Next character: " << data.at(i + 1);
			std::cout << "---\n\n";
		}

		if (!inComment && !inSingleLineComment && !inMagic) {
			
			if (data.at(i) == '/') {
				if(data.at(i + 1) == '*'){
					inComment = true;
					if(verboseparsing) MSRT::Log(MSRT::LogType::INFO, "Crossed comment.");
					i++;
				}
				else if (data.at(i + 1) == '/') {
					inSingleLineComment = true;
					if (verboseparsing) MSRT::Log(MSRT::LogType::INFO, "Crossed single line comment.");
				}
				
			}

			if ((data.at(i) == '+' || data.at(i) == '-' || data.at(i) == '*' || data.at(i) == '/' || data.at(i) == '^' || data.at(i) == '=') && !inComment && !inSingleLineComment) {
				if (newToken.tokenContent.size() != 0) newSentence.tokens.push_back(newToken);
				newToken.tokenContent = data.at(i);
				newToken.type = MSRT::TokenType::OPERATOR;
				newSentence.tokens.push_back(newToken);
				newToken.tokenContent = "";
				newToken.type = MSRT::TokenType::UNDEFINED;
			}
			else if(!inComment && !inSingleLineComment && !inMagic) {
				/*if ((data.at(i) == ';' || data.at(i) == ' ' || data.at(i) == '\n') && newToken.tokenContent != "") {
					newSentence.tokens.push_back(newToken);
					newToken.tokenContent = "";
					newToken.type = MSRT::TokenType::UNDEFINED;
					if (data.at(i) == ';' || data.at(i) == '\n') program.push_back(newSentence);
				}
				else if(data.at(i) == '"'){
					newToken.type = TokenType::MAGIC;
					inMagic = true;
					// i++;
				}
				else {
					newToken.tokenContent += data.at(i);
				}*/
				if ((data.at(i) == ';' || data.at(i) == ' ' || data.at(i) == '\n' || data.at(i) == '"' || data.at(i) == '(' || data.at(i) == ')') && newToken.tokenContent != "") {
					if (data.at(i) == '"') {
						newToken.type = TokenType::MAGIC;
						inMagic = true;
						// i++;
					}
					if (data.at(i) == ';' || data.at(i) == ' ' || data.at(i) == '\n' || data.at(i) == '(' || data.at(i) == ')') {
						if (data.at(i) == '(') newToken.type = TokenType::FUNCTION;
						newSentence.tokens.push_back(newToken);
						newToken.tokenContent = "";
						newToken.type = MSRT::TokenType::UNDEFINED;
						if ((data.at(i) == ';' && data.at(i+1) == '\n')|| data.at(i) == '\n') {
							program.push_back(newSentence);
							newSentence.tokens.clear();
						}
						
					}
					// if (data.at(i) == '"') i++;
				}
				else {
					if(data.at(i) != ';') newToken.tokenContent += data.at(i);
				}
				if (data.at(i) == '"') i++;
			}
			
		}
		if(inComment){
			if (data.at(i) == '/' && data.at(i - 1) == '*') inComment = false;
			// i += 1;
		}
		if (inSingleLineComment) {
			if (data.at(i) == '\n') inSingleLineComment = false;
		}
		if (inMagic) {
			/*if (data.at(i) == '\\') {
				if (data.at(i + 1) == 'n') {
					newToken.tokenContent += "\n";
				}
				newToken.tokenContent += data.at(i + 1);
				i++;
			}*/
			// else{
				if (data.at(i) == '"') {
					newToken.tokenContent.erase(0, 1);
					newSentence.tokens.push_back(newToken);
					newToken.tokenContent = "";
					newToken.type = TokenType::UNDEFINED;
					inMagic = false;
				}
				else newToken.tokenContent += data.at(i);
			// }
		}

	}
	std::cout << "Sentences before cleanup: " << program.size() << "\n";
	for (int s = 0; s < program.size(); s++) {
		program.at(s).CleanSentence();
		if (program.at(s).tokens.size() == 0) {
			// program.erase(program.begin() + s, program.begin() + (s + 1));
			// s--;
		}

		program.at(s).AnalyseSentence();
	}

	std::cout << "Sentences after cleanup: " << program.size() << "\n";
	for (int s = 0; s < program.size(); s++) {
		if(program.at(s).tokens.size() != 0){
		if (program.at(s).tokens.at(0).tokenContent.at(0) == '\n') program.at(s).tokens.at(0).tokenContent.erase(0,1);
		// program.at(s).CleanSentence();
		// program.at(s).AnalyseSentence();
		if (verboseparsing) std::cout << "\n\nSentence " << s << ":\n";
		if (verboseparsing) for (int t = 0; t < program.at(s).tokens.size(); t++) {
			std::cout << "\"" + program.at(s).tokens.at(t).tokenContent + "\" ";
			switch (program.at(s).tokens.at(t).type) {
			case TokenType::UNDEFINED: std::cout << "UNDEFINED"; break;
			case TokenType::VARIABLE: std::cout << "VARIABLE"; break;
			case TokenType::TYPE: std::cout << "TYPE"; break;
			case TokenType::MAGIC: std::cout << "MAGIC"; break;
			case TokenType::OPERATOR: std::cout << "OPERATOR"; break;
			case TokenType::KEYWORD: std::cout << "KEYWORD"; break;
			case TokenType::FUNCTION: std::cout << "FUNCTION"; break;
			case TokenType::FUNCTION_DECLARATION: std::cout << "FUNCTION_DECLARATION"; break;
			}
			std::cout << " | ";
		}
		}
		if (verboseparsing) std::cout << "\n";
	}
	std::cout << "\n\n---\n\n";
}


void MSRT::Run() {
	for (int i = 0; i < program.size(); i++) {
		if(program.at(i).tokens.size() > 0)
			program.at(i).RunSentence();
	}
	std::cout << "\n\n";
}

void MSRT::Sentence::RunSentence(){
	if (tokens.at(0).type == TokenType::KEYWORD) {
		switch (keywordMap.at(tokens.at(0).tokenContent)) {
		case MSRT::Keyword::DECLARE:
			if (variablesTypes.contains(tokens.at(2).tokenContent)) {
				MSRT::Log(LogType::ERROR, "Redefining variable " + tokens.at(2).tokenContent);
				abort();
			}
			switch (vartypeMap.at(tokens.at(1).tokenContent)) {
			case VarType::TEXT:
				variablesTypes.insert_or_assign(tokens.at(2).tokenContent, VarType::TEXT);
				if (tokens.at(3).tokenContent == "=") {
					// newText.data = tokens.at(4).tokenContent;
					varText.insert_or_assign(tokens.at(2).tokenContent, tokens.at(4).tokenContent);
				}
				break;
			case VarType::INTEGER:
				variablesTypes.insert_or_assign(tokens.at(2).tokenContent, VarType::INTEGER);
				if (tokens.at(3).tokenContent == "=") {
					// newText.data = tokens.at(4).tokenContent;
					varInteger.insert_or_assign(tokens.at(2).tokenContent, std::stoi(tokens.at(4).tokenContent));
				}
				break;
			case VarType::REAL:
				variablesTypes.insert_or_assign(tokens.at(2).tokenContent, VarType::REAL);
				if (tokens.at(3).tokenContent == "=") {
					// newText.data = tokens.at(4).tokenContent;
					varReal.insert_or_assign(tokens.at(2).tokenContent, std::stod(tokens.at(4).tokenContent));
				}
				break;
			}
			break;
		}
	}
	if (tokens.at(0).type == TokenType::FUNCTION) {
		std::vector<FunctionArgument> arguments;
		for (int i = 1; i < tokens.size(); i++) {
			std::string argdata = "";
			MSRT::VarType varType = VarType::TEXT;
			if (tokens.at(i).type == TokenType::VARIABLE) {
				varType = variablesTypes.at(tokens.at(i).tokenContent);
				switch (variablesTypes.at(tokens.at(i).tokenContent)) {
				case VarType::TEXT:
					argdata = varText.at(tokens.at(i).tokenContent);
					break;
				case VarType::INTEGER:
					argdata = std::to_string(varInteger.at(tokens.at(i).tokenContent));
					break;
				case VarType::REAL:
					argdata = std::to_string(varReal.at(tokens.at(i).tokenContent));
					break;
				}
			}
			if (tokens.at(i).type == TokenType::MAGIC) {
				varType = VarType::TEXT;
				if (std::regex_match(tokens.at(i).tokenContent, std::regex("[0-9]+([\\.][0-9]+)?"))) {
					varType = VarType::INTEGER;
					if ((double)std::stoi(tokens.at(i).tokenContent) != std::stod(tokens.at(i).tokenContent)) {
						varType = VarType::REAL;
					}
				}
				argdata = tokens.at(i).tokenContent;
			}
			if (i + 1 != tokens.size()) {
				if (tokens.at(i + 1).type == TokenType::OPERATOR) {
					if (tokens.at(i + 1).tokenContent == "+" || tokens.at(i + 1).tokenContent == "^") {
						switch (varType) {
						case VarType::TEXT:
							if (tokens.at(i+2).type == TokenType::VARIABLE) {
								// varType = variablesTypes.at(tokens.at(i).tokenContent);
								switch (variablesTypes.at(tokens.at(i + 2).tokenContent)) {
								case VarType::TEXT:
									argdata += varText.at(tokens.at(i + 2).tokenContent);
									break;
								case VarType::INTEGER:
									argdata += std::to_string(varInteger.at(tokens.at(i + 2).tokenContent));
									break;
								case VarType::REAL:
									argdata += std::to_string(varReal.at(tokens.at(i + 2).tokenContent));
									break;
								}
							}
							if (tokens.at(i + 2).type == TokenType::MAGIC) {
								// varType = VarType::TEXT;
								// if (std::regex_match(tokens.at(i).tokenContent, std::regex("[0-9]+([\\.][0-9]+)?"))) {
								// 	varType = VarType::INTEGER;
								// 	if ((double)std::stoi(tokens.at(i).tokenContent) != std::stod(tokens.at(i).tokenContent)) {
								// 		varType = VarType::REAL;
								// 	}
								// }
								argdata += tokens.at(i + 2).tokenContent;
							}
							break;
						}
					}
					i += 2;
				}
			}
			arguments.push_back({ varType, argdata });
		}
		functionMap.at(tokens.at(0).tokenContent)->Execute(arguments);
	}
}

void MSRT::Sentence::CleanSentence() {
	for (int i = 0; i < tokens.size(); i++) {
		if(tokens.at(i).tokenContent!="" && tokens.at(i).type != TokenType::MAGIC) if (tokens.at(i).tokenContent.at(0) == '\n' || tokens.at(i).tokenContent.at(0) == ' ') tokens.at(i).tokenContent.erase(0, 1);
		bool hasErased = false;
		if ((tokens.at(i).tokenContent == " " || tokens.at(i).tokenContent == ")" || tokens.at(i).tokenContent == "" || tokens.at(i).tokenContent == "\n") && tokens.at(i).type != TokenType::MAGIC) {
			tokens.erase(tokens.begin() + i, tokens.begin() + i + 1); 
			hasErased = true;
			i--;
		}
		// if(hasErased) i--;
	}
}



void MSRT::Sentence::AnalyseSentence() {
	for (int i = 0; i < tokens.size(); i++) {
		// check for keyword
		if (keywordMap.contains(tokens.at(i).tokenContent)) tokens.at(i).type = TokenType::KEYWORD;

		// check for type
		if (vartypeMap.contains(tokens.at(i).tokenContent)) tokens.at(i).type = TokenType::TYPE;

		// check for declarations and variables and stuff like that
		if (i > 0) {
			if(tokens.at(i - 1).type == TokenType::TYPE){
				if (tokens.at(i).type == TokenType::FUNCTION) tokens.at(i).type = TokenType::FUNCTION_DECLARATION;
				else {
					tokens.at(i).type = TokenType::VARIABLE;
					tempVarNames.push_back(tokens.at(i).tokenContent);
				}
			}
		}
		if (std::find(tempVarNames.begin(), tempVarNames.end(), tokens.at(i).tokenContent) != tempVarNames.end()) tokens.at(i).type = TokenType::VARIABLE;
	}
}

