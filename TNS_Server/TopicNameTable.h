#pragma once

#include "stdafx.h"

class TopicNameTable
{
public:
	TopicNameTable();

	virtual		~TopicNameTable();
	void		resetTable();


	bool		isEntryExist(TN_ENTRY entry);
	bool		addEntry(TN_ENTRY entry);
	bool		deleteEntry(TN_ENTRY entry);
	bool		getEntry(TN_ENTRY *p_entry);
	bool		setEntry(TN_ENTRY *p_entry);

	void		testShowAll();

	PTN_NODE TN_head;
};

