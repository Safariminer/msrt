// MSRT: ManiaScript Runtime
// Example program

#include <iostream>
#include "msrt_main.h"

int main() {
	MSRT::InitMSRT("test.Script.txt", false, true); // Loads, parses, tokenises, cleans and analyses the script
	MSRT::Run(); // Runs the script
}