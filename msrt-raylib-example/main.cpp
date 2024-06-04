#define MSRT_CUSTOM_FUNCTIONS_NEEDED

#include "../msrt/msrt_main.h"
#include "../msrt/msrt_types.h"
#include "../msrt/msrt_log.h"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

class RaylibInitWindowFunction : public MSRT::Function {
public:
	void Execute(std::vector<MSRT::FunctionArgument> args) {
		if (args.size() < 3) {
			MSRT::Log(MSRT::LogType::ERROR, "InitWindow: arguments missing...");
			abort();
		}
		InitWindow(std::stoi(args.at(0).data), std::stoi(args.at(1).data), args.at(2).data.c_str());
	}
};

class RaylibSetTargetFPSFunction : public MSRT::Function {
public:
	void Execute(std::vector<MSRT::FunctionArgument> args) {
		if (args.size() < 1) {
			MSRT::Log(MSRT::LogType::ERROR, "SetTargetFPS: arguments missing...");
			abort();
		}
		SetTargetFPS(std::stoi(args.at(0).data));
	}
};

class RaylibBeginDrawingFunction : public MSRT::Function {
public:
	void Execute(std::vector<MSRT::FunctionArgument> args) {
		BeginDrawing();
	}
};

class RaylibEndDrawingFunction : public MSRT::Function {
public:
	void Execute(std::vector<MSRT::FunctionArgument> args) {
		EndDrawing();
	}
};

class RaylibClearBackgroundFunction : public MSRT::Function {
public:
	void Execute(std::vector<MSRT::FunctionArgument> args) {
		if (args.size() < 1) {
			MSRT::Log(MSRT::LogType::ERROR, "ClearBackground: arguments missing...");
			abort();
		}
		std::string color = args.at(0).data;
		Color colorProcessed = BLACK;
		if (color == "White") colorProcessed = WHITE;
		if (color == "Yellow") colorProcessed = YELLOW;
		if (color == "Red") colorProcessed = RED;
		if (color == "Green") colorProcessed = GREEN;
		ClearBackground(colorProcessed);

	}
};

int main() {
	functionMap.insert_or_assign("InitWindow", new RaylibInitWindowFunction);
	functionMap.insert_or_assign("SetTargetFPS", new RaylibSetTargetFPSFunction);
	functionMap.insert_or_assign("BeginDrawing", new RaylibBeginDrawingFunction);
	functionMap.insert_or_assign("ClearBackground", new RaylibClearBackgroundFunction);
	functionMap.insert_or_assign("EndDrawing", new RaylibEndDrawingFunction);

	MSRT::InitMSRT("main.Script.txt", true, true, true);
	MSRT::Run();
}