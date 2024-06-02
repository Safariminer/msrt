#pragma once

/*

MSRT: Maniascript Runtime
Written by Safariminer

*/


#include <iostream>
#include "msrt_tokens.h"
namespace MSRT {

	void InitMSRT(std::string file, bool dataread = false, bool verboseparsing = false); // All you truly need.
	void Run();

	void InitKeywordMap();
	void InitVarTypeMap();
	void InitFunctionMap();
	std::vector<MSRT::Sentence> Parse(std::string data, bool verboseparsing);
}