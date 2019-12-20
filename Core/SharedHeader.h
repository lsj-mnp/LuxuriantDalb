#pragma once

#include <Windows.h>
#include <string>
#include <d3d11.h>
#include <wrl.h>
#include <cassert>
#include <d3dcompiler.h>
#include <vector>
#include <memory>
#include "../DirectXTK/DirectXTK.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

enum class EShaderType
{
	VertexShader,
	PixelShader
};