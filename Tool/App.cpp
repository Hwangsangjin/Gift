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
#include "Sprite.h"

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
	m_world.SetTranslation(Vector3(0.0f, 5.0f, -20.0f));

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
	m_plane_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\plane.obj");
	m_skybox_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\sphere.obj");
	m_terrain_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\terrain.obj");
	m_house_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\house.obj");

	// 텍스처 생성
	m_plane_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\plane.png");
	m_skybox_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\sky.jpg");
	m_sand_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\sand.jpg");
	m_barrel_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\barrel.jpg");
	m_brick_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\house_brick.jpg");
	m_windows_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\house_windows.jpg");
	m_wood_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\house_wood.jpg");
	m_start_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\start.png");
	m_map_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\map.png");
	m_exit_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\exit.png");

	// 머티리얼 생성
	m_plane_material = Engine::GetInstance()->CreateMaterial(L"..\\..\\Assets\\Shaders\\PointLightVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\PointLightPixelShader.hlsl");
	assert(m_plane_material);
	m_plane_material->AddTexture(m_plane_texture);

	m_terrain_material = Engine::GetInstance()->CreateMaterial(m_plane_material);
	assert(m_terrain_material);
	m_terrain_material->AddTexture(m_sand_texture);

	m_barrel_material = Engine::GetInstance()->CreateMaterial(m_plane_material);
	assert(m_barrel_material);
	m_barrel_material->AddTexture(m_barrel_texture);

	m_brick_material = Engine::GetInstance()->CreateMaterial(m_plane_material);
	assert(m_brick_material);
	m_brick_material->AddTexture(m_brick_texture);

	m_windows_material = Engine::GetInstance()->CreateMaterial(m_plane_material);
	assert(m_windows_material);
	m_windows_material->AddTexture(m_windows_texture);

	m_wood_material = Engine::GetInstance()->CreateMaterial(m_plane_material);
	assert(m_wood_material);
	m_wood_material->AddTexture(m_wood_texture);

	m_skybox_material = Engine::GetInstance()->CreateMaterial(L"..\\..\\Assets\\Shaders\\PointLightVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\SkyBoxShader.hlsl");
	assert(m_skybox_material);
	m_skybox_material->AddTexture(m_skybox_texture);
	m_skybox_material->SetCullMode(Material::CullMode::Front);

	m_materials.reserve(32);
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

	//UpdateModel(Vector3(0.0f, 0.0f, 0.0f), m_plane_material);
	//DrawMesh(m_plane_mesh, m_plane_material);

	if (m_plane_sprite)
	{
		UpdateUI(Vector3(0.0f, 0.0f, 0.0f), m_plane_sprite);
		DrawSprite(m_plane_sprite);
	}

	if (m_start_sprite)
	{
		UpdateUI(Vector3(2.0f, 2.0f, 0.0f), m_start_sprite);
		DrawSprite(m_start_sprite);
	}

	if (m_map_sprite)
	{
		UpdateUI(Vector3(2.0f, 0.0f, 0.0f), m_map_sprite);
		DrawSprite(m_map_sprite);
	}

	if (m_exit_sprite)
	{
		UpdateUI(Vector3(2.0f, -2.0f, 0.0f), m_exit_sprite);
		DrawSprite(m_exit_sprite);
	}

	// 집
	m_materials.clear();
	m_materials.push_back(m_barrel_material);
	m_materials.push_back(m_brick_material);
	m_materials.push_back(m_windows_material);
	m_materials.push_back(m_wood_material);

	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			UpdateModel(Vector3(-14.0f + 14.0f * i, 0.0f, -14.0f + 14.0f * j), m_materials);
			DrawMesh(m_house_mesh, m_materials);
		}
	}

	// 지형
	m_materials.clear();
	m_materials.push_back(m_terrain_material);
	UpdateModel(Vector3(0.0f, 0.0f, 0.0f), m_materials);
	DrawMesh(m_terrain_mesh, m_materials);

	// 스카이박스
	m_materials.clear();
	m_materials.push_back(m_skybox_material);

	DrawMesh(m_skybox_mesh, m_materials);
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

	if (m_projection_state)
	{
		int width = GetClientWindowRect().right - GetClientWindowRect().left;
		int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

		m_projection.SetPerspectiveProjection(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
	}
	else
	{
		m_projection.SetOrthographicProjection
		(
			(GetClientWindowRect().right - GetClientWindowRect().left) / 100.0f,
			(GetClientWindowRect().bottom - GetClientWindowRect().top) / 100.0f,
			-4.0f,
			4.0f
		);
	}
}

void App::UpdateModel(Vector3 position, const std::vector<MaterialPtr>& materials)
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

	for (size_t m = 0; m < materials.size(); m++)
	{
		materials[m]->SetData(&constant, sizeof(constant));
	}
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

	const float dist_from_origin = 3.0f;
	//m_light_position = Vector4(std::cos(m_light_rotation_y) * dist_from_origin, 2.0f, std::sin(m_light_rotation_y) * dist_from_origin, 1.0f);
	m_light_position = Vector4(180.0f, 140.0f, 70.0f, 1.0f);
}

void App::UpdateUI(Vector3 position, const SpritePtr& sprite)
{
	Constant constant;
	constant.world.SetIdentity();
	constant.world.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	constant.world.SetTranslation(position);
	constant.view = m_view;
	constant.projection = m_projection;

	sprite->SetData(&constant, sizeof(constant));
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

	//ImGuiMainMenuBar();
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Button("Plane"))
	{
		m_plane_sprite = Engine::GetInstance()->CreateSprite(L"..\\..\\Assets\\Shaders\\PlaneVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\PlanePixelShader.hlsl");
		assert(m_plane_sprite);
		m_plane_sprite->AddTexture(m_plane_texture);
	}

	if (ImGui::Button("Start"))
	{
		m_start_sprite = Engine::GetInstance()->CreateSprite(L"..\\..\\Assets\\Shaders\\PlaneVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\PlanePixelShader.hlsl");
		assert(m_start_sprite);
		m_start_sprite->AddTexture(m_start_texture);
	}

	if (ImGui::Button("Map"))
	{
		m_map_sprite = Engine::GetInstance()->CreateSprite(L"..\\..\\Assets\\Shaders\\PlaneVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\PlanePixelShader.hlsl");
		assert(m_map_sprite);
		m_map_sprite->AddTexture(m_map_texture);
	}

	if (ImGui::Button("Exit"))
	{
		m_exit_sprite = Engine::GetInstance()->CreateSprite(L"..\\..\\Assets\\Shaders\\PlaneVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\PlanePixelShader.hlsl");
		assert(m_plane_sprite);
		m_exit_sprite->AddTexture(m_exit_texture);
	}

	if (ImGui::Button("Wireframe"))
	{
		m_plane_material->SetFillMode(Material::FillMode::Wireframe);
		m_plane_sprite->SetFillMode(Sprite::FillMode::Wireframe);
		m_start_sprite->SetFillMode(Sprite::FillMode::Wireframe);
		m_map_sprite->SetFillMode(Sprite::FillMode::Wireframe);
		m_exit_sprite->SetFillMode(Sprite::FillMode::Wireframe);
	}

	if (ImGui::Button("Solid"))
	{
		m_plane_material->SetFillMode(Material::FillMode::Solid);
		m_plane_sprite->SetFillMode(Sprite::FillMode::Solid);
		m_start_sprite->SetFillMode(Sprite::FillMode::Solid);
		m_map_sprite->SetFillMode(Sprite::FillMode::Solid);
		m_exit_sprite->SetFillMode(Sprite::FillMode::Solid);
	}

	if (ImGui::Button("Orthographic"))
	{
		m_projection_state = false;
	}

	if (ImGui::Button("Perspective"))
	{
		m_projection_state = true;
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

void App::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
{
	// 정점 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());

	// 인덱스 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	// 머티리얼 슬롯의 크기 만큼 반복
	for (size_t m = 0; m < mesh->GetMaterialSlotSize(); m++)
	{
		if (m >= materials.size())
			break;

		MaterialSlot material_slot = mesh->GetMaterialSlot(m);

		// 머티리얼 설정
		Engine::GetInstance()->SetMaterial(materials[m]);

		// 삼각형 그리기
		Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(material_slot.index_size, 0, material_slot.start_index);
	}
}

void App::DrawSprite(const SpritePtr& sprite)
{
	// 스프라이트 설정
	Engine::GetInstance()->SetSprite(sprite);
	
	// 정점 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(sprite->GetVertexBuffer());

	// 인덱스 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(sprite->GetIndexBuffer());

	// 삼각형 그리기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(sprite->GetIndexBuffer()->GetIndexCount(), 0, 0);
}
