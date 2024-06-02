#include <iostream>
#include "../msrt/msrt_main.h"

int main() {
	MSRT::InitMSRT("main.Script.txt", false, true); // Loads, parses, tokenises, cleans and analyses the script
	MSRT::Run(); // Runs the script
}