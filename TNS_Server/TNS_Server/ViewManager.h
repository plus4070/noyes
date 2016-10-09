#include "stdafx.h"
#include <stdio.h>
#include "TopicNameTable.h"
#pragma once

class ViewManager
{
public:
	TopicNameTable	*TNTable;

	ViewManager();
	~ViewManager();

	void	initialize();
	void	inputDummyEntryToTNTable();
};