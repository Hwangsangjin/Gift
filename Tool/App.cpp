#include "pch.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
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
#include "Material.h"

void App::OnCreate()
{
	// 윈도우
	Window::OnCreate();

	// 입력
	Input::GetInstance()->AddListener(this);
	Input::GetInstance()->ShowCursor(true);

	// 타이머
	Timer::GetInstance()->Initialize();

	// 실행 상태 설정
	m_play_state = true;

	// 공간 설정
	m_world.SetTranslation(Vector3(0.0f, 2.0f, -5.0f));

	// 스왑 체인 생성
	RECT rect = GetClientWindowRect();
	m_swap_chain = Engine::GetInstance()->GetGraphics()->CreateSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	// 상수 버퍼 생성
	Constant constant;
	m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_constant_buffer);
	m_skybox_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_skybox_constant_buffer);

	// 메쉬 생성
	m_skybox_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\sphere.obj");
	m_plane_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\scene.obj");

	// 텍스처 생성
	m_skybox_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\stars_map.jpg");
	m_plane_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\wall.jpg");

	// 머티리얼 생성
	m_skybox_material = Engine::GetInstance()->CreateMaterial(L"..\\..\\Assets\\Shaders\\PointLightVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\SkyBoxShader.hlsl");
	assert(m_skybox_material);
	m_skybox_material->AddTexture(m_skybox_texture);
	m_skybox_material->SetCullMode(CULL_MODE::CULL_MODE_FRONT);

	m_plane_material = Engine::GetInstance()->CreateMaterial(L"..\\..\\Assets\\Shaders\\PointLightVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\PointLightPixelShader.hlsl");
	assert(m_plane_material);
	m_plane_material->AddTexture(m_plane_texture);
	m_plane_material->SetCullMode(CULL_MODE::CULL_MODE_BACK);
}

void App::OnUpdate()
{
	Window::OnUpdate();
	Input::GetInstance()->Update();
	Timer::GetInstance()->Update();
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
	m_upward = 0.0f;
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
	if (!m_play_state)
		return;

	if (key == 'Q')
	{
		m_upward = -1.0f;
	}
	else if (key == 'E')
	{
		m_upward = 1.0f;
	}
	else if (key == 'W')
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
	else if (key == 'O')
	{
		m_light_radius -= 1.0f * Timer::GetInstance()->GetDeltaTime();
	}
	else if (key == 'P')
	{
		m_light_radius += 1.0f * Timer::GetInstance()->GetDeltaTime();
	}
}

void App::OnMouseMove(const Point& point)
{
	//if (!m_play_state)
	//	return;

	//int width = GetClientWindowRect().right - GetClientWindowRect().left;
	//int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	//m_rotation_x += (point.GetY() - (height / 2.0f)) * Timer::GetInstance()->GetDeltaTime() * 0.1f;
	//m_rotation_y += (point.GetX() - (width / 2.0f)) * Timer::GetInstance()->GetDeltaTime() * 0.1f;

	//Input::GetInstance()->SetCursorPosition(Point(width / 2, height / 2));
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
	// 렌더 타겟 지우기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 변환 행렬 계산
	UpdateCamera();
	UpdateSkyBox();
	UpdateLight();

	UpdateModel(Vector3(0.0f, 0.0f, 0.0f), m_plane_material);
	DrawMesh(m_plane_mesh, m_plane_material);

	DrawMesh(m_skybox_mesh, m_skybox_material);
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
	new_position = new_position + camera.GetYDirection() * (m_upward * 0.05f);

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

void App::UpdateModel(Vector3 position, const MaterialPtr& material)
{
	Matrix4x4 light_rotation_matrix;
	light_rotation_matrix.SetIdentity();
	light_rotation_matrix.SetRotationY(m_light_rotation_y);

	Constant constant;
	constant.world.SetIdentity();
	constant.world.SetTranslation(position);
	constant.view = m_view;
	constant.projection = m_projection;
	constant.camera_position = m_world.GetTranslation();
	constant.light_position = m_light_position;
	constant.light_radius = m_light_radius;
	constant.light_direction = light_rotation_matrix.GetZDirection();
	constant.time = Timer::GetInstance()->GetGameTime();

	material->SetData(&constant, sizeof(constant));
}

void App::UpdateSkyBox()
{
	Constant constant;
	constant.world.SetIdentity();
	constant.world.SetScale(Vector3(100.0f, 100.0f, 100.0f));
	constant.world.SetTranslation(m_world.GetTranslation());
	constant.view = m_view;
	constant.projection = m_projection;

	m_skybox_material->SetData(&constant, sizeof(constant));
}

void App::UpdateLight()
{
	m_light_rotation_y += 1.57f * Timer::GetInstance()->GetDeltaTime();

	float dist_from_origin = 3.0f;
	m_light_position = Vector4(std::cos(m_light_rotation_y) * dist_from_origin, 2.0f, std::sin(m_light_rotation_y) * dist_from_origin, 1.0f);
}

void App::Render()
{
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

void App::DrawMesh(const MeshPtr& mesh, const MaterialPtr& material)
{
	// 머티리얼 설정
	Engine::GetInstance()->SetMaterial(material);

	// 정점 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());

	// 인덱스 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	// 삼각형 그리기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}
