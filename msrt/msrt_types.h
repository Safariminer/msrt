#pragma once
#include <iostream>
#include <vector>
namespace MSRT {
	enum class VarType{VOID, INTEGER, REAL, BOOLEAN, TEXT, VEC2, VEC3, INT3, IDENT};
	
	class FunctionArgument {
	public:
		VarType type;
		std::string data;
	};

	class Function {
	public:
		virtual void Execute(std::vector<FunctionArgument> args) = 0;
	}; 
}