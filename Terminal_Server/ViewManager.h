#include <stdio.h>

#pragma once

class ViewManager
{
public:
	ViewManager();
	~ViewManager();

	void	printMessage(char * s);
	
	//DB Manager
	void	setDBInfo();

	void	defineDDSTopicCommand();
};

