#include "pch.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MathUtils.h"
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
	m_play_state = false;

	// 공간 설정
	m_world.SetTranslation(Vector3(0.0f, 0.0f, 0.0f));

	// 스왑 체인 생성
	RECT rect = GetClientWindowRect();
	m_swap_chain = Engine::GetInstance()->GetGraphics()->CreateSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	// 메쉬 생성
	m_skybox_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\sphere.obj");
	m_plane_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\Assets\\Meshes\\plane.obj");

	// 텍스처 생성
	m_skybox_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\stars_map.jpg");
	m_plane_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\Assets\\Textures\\plane.png");

	// 스프라이트 생성
	m_plane_sprite = Engine::GetInstance()->CreateSprite(L"..\\..\\Assets\\Shaders\\UIVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\UIPixelShader.hlsl");
	assert(m_plane_sprite);
	m_plane_sprite->AddTexture(m_plane_texture);

	// 머티리얼 생성
	m_skybox_material = Engine::GetInstance()->CreateMaterial(L"..\\..\\Assets\\Shaders\\SkyBoxVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\SkyBoxPixelShader.hlsl");
	assert(m_skybox_material);
	m_skybox_material->AddTexture(m_skybox_texture);
	m_skybox_material->SetCullMode(Material::CullMode::Front);

	m_plane_material = Engine::GetInstance()->CreateMaterial(L"..\\..\\Assets\\Shaders\\DirectionalLightVertexShader.hlsl", L"..\\..\\Assets\\Shaders\\DirectionalLightPixelShader.hlsl");
	assert(m_plane_material);
	m_plane_material->AddTexture(m_plane_texture);

	m_materials.reserve(32);
}

void App::OnUpdate()
{
	Window::OnUpdate();
	Input::GetInstance()->Update();
	Timer::GetInstance()->Update();
	Update();
	Render();

	m_delta_mouse_x = 0.0f;
	m_delta_mouse_y = 0.0f;
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

	if (key == VK_ESCAPE)
	{
		if (m_play_state)
		{
			m_play_state = m_play_state ? false : true;
			Input::GetInstance()->ShowCursor(!m_play_state);
		}
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
	if (!m_play_state)
		return;

	int width = (GetClientWindowRect().right - GetClientWindowRect().left);
	int height = (GetClientWindowRect().bottom - GetClientWindowRect().top);

	m_delta_mouse_x = static_cast<float>(height / 2.0f) * Timer::GetInstance()->GetDeltaTime() * 0.1f;
	m_delta_mouse_y = static_cast<float>(width / 2.0f) * Timer::GetInstance()->GetDeltaTime() * 0.1f;

	Input::GetInstance()->SetCursorPosition(Point(width / 2, height / 2));
}

void App::OnLeftButtonUp(const Point& point)
{
	if (m_play_state)
	{
		m_play_state = true;
		Input::GetInstance()->ShowCursor(!m_play_state);
	}
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
	// 변환 행렬 계산
	UpdateCamera();
	UpdateLight();
	UpdateSkyBox();
}

void App::UpdateThirdPersonCamera()
{
	Matrix4x4 world, temp;
	world.SetIdentity();

	m_camera_rotation.SetX(m_delta_mouse_y * Timer::GetInstance()->GetDeltaTime() * 0.1f);
	m_camera_rotation.SetY(m_delta_mouse_x * Timer::GetInstance()->GetDeltaTime() * 0.1f);

	if (m_camera_rotation.GetX() >= 1.57f)
		m_camera_rotation.SetX(1.57f);
	else if (m_camera_rotation.GetX() <= -1.57f)
		m_camera_rotation.SetX(-1.57f);

	m_current_camera_rotation = Vector3::LinearInterpolation(m_current_camera_rotation, m_camera_rotation, 3.0f * Timer::GetInstance()->GetDeltaTime());

	temp.SetIdentity();
	temp.SetRotationX(m_current_camera_rotation.GetX());
	world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_current_camera_rotation.GetY());
	world *= temp;

	m_camera_distance = 2.0f;
	m_current_camera_distance = LinearInterpolation(m_current_camera_distance, m_camera_distance, 2.0f * Timer::GetInstance()->GetDeltaTime());
	m_camera_position = Vector3();

	Vector3 new_position = m_camera_position + world.GetZDirection() * (-m_current_camera_distance);
	new_position = new_position + world.GetYDirection() * (5.0f);

	world.SetTranslation(new_position);

	m_world = world;

	world.Inverse();

	m_view = world;

	if (m_projection_state)
	{
		int width = GetClientWindowRect().right - GetClientWindowRect().left;
		int height = GetClientWindowRect().bottom - GetClientWindowRect().top;
		m_projection.SetPerspectiveProjection(1.57f, ((float)width / (float)height), 0.1f, 5000.0f);
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

void App::UpdateCamera()
{
	Matrix4x4 world, temp;
	world.SetIdentity();

	temp.SetIdentity();
	temp.SetRotationX(m_camera_rotation.GetX());
	world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_camera_rotation.GetY());
	world *= temp;

	Vector3 new_position = m_world.GetTranslation() + world.GetZDirection() * (m_forward * 0.05f);
	new_position = new_position + world.GetXDirection() * (m_rightward * 0.05f);
	new_position = new_position + world.GetYDirection() * (m_upward * 0.05f);

	world.SetTranslation(new_position);
	
	m_world = world;

	world.Inverse();

	m_view = world;

	if (m_projection_state)
	{
		int width = GetClientWindowRect().right - GetClientWindowRect().left;
		int height = GetClientWindowRect().bottom - GetClientWindowRect().top;
		m_projection.SetPerspectiveProjection(1.57f, ((float)width / (float)height), 0.1f, 5000.0f);
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

void App::UpdateLight()
{
	Matrix4x4 temp;
	temp.SetIdentity();
	m_light_rotation.SetIdentity();

	temp.SetRotationX(-0.785f);
	m_light_rotation *= temp;

	temp.SetRotationY(0.785f);
	m_light_rotation *= temp;
}

void App::UpdateSkyBox()
{
	Constant constant;
	constant.world.SetIdentity();
	constant.world.SetScale(Vector3(500.0f, 500.0f, 500.0f));
	constant.world.SetTranslation(m_world.GetTranslation());
	constant.view = m_view;
	constant.projection = m_projection;

	m_skybox_material->SetData(&constant, sizeof(Constant));
}

void App::UpdateModel(Vector3 position, Vector3 rotation, Vector3 scale, const std::vector<MaterialPtr>& materials)
{
	Constant constant;
	constant.world.SetIdentity();

	Matrix4x4 temp;
	temp.SetIdentity();
	temp.SetScale(scale);
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationX(rotation.GetX());
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(rotation.GetY());
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationZ(rotation.GetZ());
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetTranslation(position);
	constant.world *= temp;

	constant.view = m_view;
	constant.projection = m_projection;
	constant.camera_position = m_world.GetTranslation();
	constant.light_position = m_light_position;
	constant.light_radius = m_light_radius;
	constant.light_direction = m_light_rotation.GetZDirection();
	constant.time = Timer::GetInstance()->GetGameTime();

	for (unsigned int m = 0; m < materials.size(); m++)
	{
		materials[m]->SetData(&constant, sizeof(constant));
	}
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
	// 렌더 타겟 지우기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 평면
	m_materials.clear();
	m_materials.push_back(m_plane_material);
	UpdateModel(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), m_materials);
	DrawMesh(m_plane_mesh, m_materials);

	// 스카이박스
	m_materials.clear();
	m_materials.push_back(m_skybox_material);
	DrawMesh(m_skybox_mesh, m_materials);

	// UI
	UpdateUI(Vector3(0.0f, 0.0f, 0.0f), m_plane_sprite);
	DrawSprite(m_plane_sprite);

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

	if (ImGui::Button("Wireframe"))
	{
		m_plane_material->SetFillMode(Material::FillMode::Wireframe);
		m_plane_sprite->SetFillMode(Sprite::FillMode::Wireframe);
	}

	if (ImGui::Button("Solid"))
	{
		m_plane_material->SetFillMode(Material::FillMode::Solid);
		m_plane_sprite->SetFillMode(Sprite::FillMode::Solid);
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
	// 머티리얼 슬롯의 크기 만큼 반복
	for (unsigned int m = 0; m < mesh->GetMaterialSlotSize(); m++)
	{
		if (m == materials.size())
			break;

		MaterialSlot material_slot = mesh->GetMaterialSlot(m);

		// 머티리얼 설정
		Engine::GetInstance()->SetMaterial(materials[m]);

		// 정점 버퍼 설정
		Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());

		// 인덱스 버퍼 설정
		Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

		// 삼각형 그리기
		Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(static_cast<UINT>(material_slot.index_size), 0, static_cast<UINT>(material_slot.start_index));
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
