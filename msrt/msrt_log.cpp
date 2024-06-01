#include "msrt_log.h"
#include <iostream>

void MSRT::Log(LogType type, std::string message)
{
	switch (type) {
	case MSRT::LogType::INFO:
		std::cout << "[MSRT::LogType::INFO]     |";
		break;
	case MSRT::LogType::WARNING:
		std::cout << "[MSRT::LogType::WARNING]  |";
		break;
	case MSRT::LogType::ERROR:
		std::cout << "[MSRT::LogType::ERROR]    |";
		break;
	}
	std::cout << message + "\n";
}
