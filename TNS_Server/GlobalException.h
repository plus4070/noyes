#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <stdexcept>

using namespace std;

class GlobalException : public runtime_error
{
public:
	GlobalException(const char* msg) : runtime_error(msg) {
		fputs(msg, stderr);
		fputc('\n', stderr);
		exit(1);
	}
};