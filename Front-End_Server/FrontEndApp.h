#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <winsock2.h> 

#include "stdafx.h"
#include "TopicParser.h"
#include "RequestTable.h"
#include "TCPSocket.h"

using namespace std;

#pragma comment(lib,"wsock32.lib")
#pragma once

TopicParser *TP;

TNSN_ENTRY TNSNDatagram;