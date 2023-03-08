#pragma once

#include "Vector4.h"
#include "Matrix4x4.h"
#include "Texture2D.h"

__declspec(align(16))
struct LightData
{
	Vector4 color;
	Vector4 direction;
};

__declspec(align(16))
struct ConstantData
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector4 camera_position;
	LightData light;
};

class RenderSystem
{
public:
	RenderSystem(Engine* engine);
	~RenderSystem();

	void Update();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	SwapChainPtr CreateSwapChain(HWND hwnd, UINT width, UINT height);
	ConstantBufferPtr CreateConstantBuffer(void* buffer, UINT buffer_size);
	IndexBufferPtr CreateIndexBuffer(void* index_list, UINT index_count);
	VertexBufferPtr CreateVertexBuffer(void* vertex_list, UINT vertex_size, UINT vertex_count);
	VertexShaderPtr CreateVertexShader(const wchar_t* file_path);
	PixelShaderPtr CreatePixelShader(const wchar_t* file_path);
	Texture2DPtr CreateTexture(const wchar_t* file_path);
	Texture2DPtr CreateTexture(const Rect& size, Texture2D::Type type);

	/*void CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShader();*/

	ID3D11Device* GetD3DDevice() const;
	ID3D11DeviceContext* GetImmediateContext() const;
	IDXGIFactory* GetDXGIFactory() const;
	DeviceContextPtr GetDeviceContext() const;

	const void* GetMeshLayoutByteCode() const;
	size_t GetMeshLayoutSize() const;

	void SetCullMode(const CullMode& cull_mode);
	void SetFillMode(const FillMode& fill_mode);

private:
	void InitializeRasterizerState();
	void CompilePrivateShader();

	DeviceContextPtr m_device_context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d_device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immediate_context = nullptr;
	Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgi_device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgi_adapter = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgi_factory = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertex_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixel_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_none = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_front = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_back = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_fill_wireframe = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_fill_solid = nullptr;

	UCHAR m_mesh_layout_byte_code[1024] = {};
	size_t m_mesh_layout_size = 0;

	Engine* m_engine = nullptr;

	std::set<MeshComponent*> m_meshes;
	std::set<SpriteComponent*> m_sprites;
	std::set<CameraComponent*> m_cameras;
	std::set<LightComponent*> m_lights;
};

