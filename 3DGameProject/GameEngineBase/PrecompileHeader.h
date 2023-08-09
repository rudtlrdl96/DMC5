#pragma once

// Platform

#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <WinSock2.h> // window.h보다 무조건 위쪽에 있어야 한다.
#include <Windows.h>

#include <memory>
#include <iostream>
#include <math.h>
#include <cmath>

// std
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <string_view>
#include <chrono>
#include <filesystem>

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>

#pragma comment (lib, "ws2_32") // <= 윈도우 서버 사용을 위한 라이브러리
