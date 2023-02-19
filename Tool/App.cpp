#include "pch.h"
#include "App.h"
#include "Input.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Plane.h"

void App::OnCreate()
{
	// 윈도우
	Window::OnCreate();

	// 입력
	Input::GetInstance()->AddListener(this);
	Input::GetInstance()->ShowCursor(true);

	// 스왑 체인 생성
	RECT rect = GetClientWindowRect();
	m_swap_chain = Engine::GetInstance()->GetGraphics()->CreateSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	// 실행 상태
	m_play_state = false;

	// 공간 설정
	m_world.SetTranslation(Vector3(0.0f, 0.0f, -5.0f));

	// 에셋 로드
	m_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\sphere_hq.obj");
	m_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\brick.png");

	m_skybox_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\sphere.obj");
	m_skybox_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\stars_map.jpg");

	// 정점 셰이더 생성
	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(L"..\\..\\Assets\\Shaders\\VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// 픽셀 셰이더 생성
	Engine::GetInstance()->GetGraphics()->CompilePixelShader(L"..\\..\\Assets\\Shaders\\PixelShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	Engine::GetInstance()->GetGraphics()->CompilePixelShader(L"..\\..\\Assets\\Shaders\\SkyBoxShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_skybox_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_skybox_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// 상수 버퍼 생성
	Constant constant;
	m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_constant_buffer);
	m_skybox_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_skybox_constant_buffer);
}

void App::OnUpdate()
{
	Window::OnUpdate();
	Input::GetInstance()->Update();
	Update();
	Render();
}

void App::OnSize()
{
	RECT rect = GetClientWindowRect();
	m_swap_chain->Resize(rect.right, rect.bottom);
	Update();
	Render();
}

void App::OnFocus()
{
	Input::GetInstance()->AddListener(this);
}

void App::OnKillFocus()
{
	Input::GetInstance()->RemoveListener(this);
}

void App::OnDestroy()
{
	Window::OnDestroy();
	m_swap_chain->SetFullScreen(false, 1, 1);
}

void App::OnKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G')
	{
		m_play_state = m_play_state ? false : true;
		Input::GetInstance()->ShowCursor(!m_play_state);
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = GetScreenSize();

		m_swap_chain->SetFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
	}
}

void App::OnKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
}

void App::OnMouseMove(const Point& point)
{
	/*if (!m_play_state)
		return;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	m_rotation_x += (point.GetY() - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rotation_y += (point.GetX() - (width / 2.0f)) * m_delta_time * 0.1f;

	Input::GetInstance()->SetCursorPosition(Point(width / 2, height / 2));*/
}

void App::OnLeftButtonUp(const Point& point)
{
}

void App::OnLeftButtonDown(const Point& point)
{
}

void App::OnRightButtonUp(const Point& point)
{
}

void App::OnRightButtonDown(const Point& point)
{
}

void App::Update()
{
	// 프레임 설정
	m_old_delta = m_new_delta;
	m_new_delta = static_cast<float>(::GetTickCount64());
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000.0f : 0.0f;

	// 렌더 타겟 지우기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 변환 행렬 계산
	UpdateCamera();
	UpdateModel();
	UpdateSkyBox();

	if (m_plane)
	{
		m_plane->Update();
	}
}

void App::UpdateCamera()
{
	Matrix4x4 camera, temp;
	camera.SetIdentity();

	temp.SetIdentity();
	temp.SetRotationX(m_rotation_x);
	camera *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rotation_y);
	camera *= temp;

	Vector3 new_position = m_world.GetTranslation() + camera.GetZDirection() * (m_forward * 0.05f);

	new_position = new_position + camera.GetXDirection() * (m_rightward * 0.05f);

	camera.SetTranslation(new_position);

	m_world = camera;

	camera.Inverse();

	m_view = camera;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	m_projection.SetPerspectiveProjection(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	/*m_projection.SetOrthographicProjection
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 100.0f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 100.0f,
		-4.0f,
		4.0f
	);*/
}

void App::UpdateModel()
{
	Matrix4x4 light_rotation_matrix;
	light_rotation_matrix.SetIdentity();
	light_rotation_matrix.SetRotationY(m_light_rotation_y);
	m_light_rotation_y += 0.785f * m_delta_time;

	Constant constant;
	constant.world.SetIdentity();
	constant.view = m_view;
	constant.projection = m_projection;
	constant.camera_position = m_world.GetTranslation();
	constant.light_direction = light_rotation_matrix.GetZDirection();

	m_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), &constant);
}

void App::UpdateSkyBox()
{
	Constant constant;
	constant.world.SetIdentity();
	constant.world.SetScale(Vector3(100.0f, 100.0f, 100.0f));
	constant.world.SetTranslation(m_world.GetTranslation());
	constant.view = m_view;
	constant.projection = m_projection;

	m_skybox_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), &constant);
}

void App::UpdateUI()
{
}

void App::Render()
{
	// 렌더링
	Engine::GetInstance()->GetGraphics()->SetRasterizerState(false);
	DrawMesh(m_mesh, m_texture, m_vertex_shader, m_pixel_shader, m_constant_buffer);
	Engine::GetInstance()->GetGraphics()->SetRasterizerState(true);
	DrawMesh(m_skybox_mesh, m_skybox_texture, m_vertex_shader, m_skybox_pixel_shader, m_skybox_constant_buffer);

	if (m_plane)
	{
		m_plane->Render();
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	// Create ImGui Window
	ImGui::Begin("Inspector");

	if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Button("Button"))
	{
		m_plane = new Plane;
	}

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	ImGui::End();

	// Assemble Together Draw Data
	ImGui::Render();
	// Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// 페이지 플리핑
	m_swap_chain->Present(true);
}

void App::DrawMesh(const MeshPtr& mesh, const TexturePtr& texture, const VertexShaderPtr& vertex_shader, const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer)
{
	// 상수 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(vertex_shader, constant_buffer);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(pixel_shader, constant_buffer);

	// 셰이더 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexShader(vertex_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetPixelShader(pixel_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetTexture(pixel_shader, texture);

	// 정점 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());

	// 인덱스 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	// 삼각형 그리기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}
