#pragma once
#include "domain.h"
#include "DynamicVector.h"
#include "repository.h"
#include "controller.h"
#include <string>

class UI {
private:
	Controller& ctrl;

public:
	UI(Controller& c);
	void startUI();
	void administratorMode();
	void userMode();
};