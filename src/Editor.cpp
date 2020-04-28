#include "Editor.h"
#include <iostream>
#include <fstream>
#include <limits>

void Editor::loop(void)
{
	std::string input_s;
	
	// Read input file
	while (std::getline(std::cin, input_s)) {
		std::cin >> std::noskipws , input_s;
		document->input(input_s);
	}
}