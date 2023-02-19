 #pragma once

// lib
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "DirectXTK")
#pragma comment(lib, "DirectXTex")

#include <cassert>
#include <exception>
#include <windows.h>
#include <iostream>
#include <comdef.h>
#include <chrono>
#include <fstream>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Matrix4x4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

#include "DirectXTex/DirectXTex.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

// class
class SwapChain;
class DeviceContext;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class Graphics;
class Engine;
class Resource;
class ResourceManager;
class Texture;
class TextureManager;
class Mesh;
class MeshManager;

// typedef
typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Mesh> MeshPtr;

// Vertex
struct Vertex
{
	Vector3 position;
	Vector2 texcoord;
};

// Constant
__declspec(align(16))
struct Constant
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector4 light_direction;
	Vector4 camera_position;
	float time = 0.0f;
};