#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <concurrent_queue.h>
#include <concurrent_vector.h>
#include <optional>
#include <memory_resource>

#include <iostream>
using namespace std;
using namespace Concurrency;

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "ObjectPool.h"
#include "TypeCast.h"
#include "SendBuffer.h"
#include "SendBufferChunk.h"
#include "SendBufferManager.h"