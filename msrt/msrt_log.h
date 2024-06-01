#pragma once
#include <iostream>

namespace MSRT {
	enum class LogType {NONE, INFO, WARNING, ERROR};
	void Log(LogType type, std::string message);
}