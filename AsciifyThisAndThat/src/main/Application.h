#pragma once

#include "utils/Utils.h"


class Application {
public:
	Application();
	void run();
private:
	std::string* current_ascii;
};