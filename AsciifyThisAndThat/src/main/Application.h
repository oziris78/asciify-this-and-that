#pragma once

#include "Utils.h"
#include "Constants.h"


class Application {
public:
	Application();
	void run();
private:
	std::string* current_ascii;
	std::string* current_path;
	ImFont* font;
	ImFont* def_font;
};