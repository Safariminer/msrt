#pragma once
#include <iostream>
#include <vector>

namespace MSRT {
	enum class TokenType{ UNDEFINED, VARIABLE, TYPE, MAGIC, OPERATOR, KEYWORD, FUNCTION, FUNCTION_DECLARATION };
	class Token {
	public:
		std::string tokenContent;
		TokenType type;
	};
	
	class Sentence {
	public:
		std::vector<Token> tokens;
		void RunSentence();
		void CleanSentence();
		void AnalyseSentence();
	};
	
}