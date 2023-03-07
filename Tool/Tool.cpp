#include "pch.h"
#include "Tool.h"
#include "Input.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Display.h"
#include "SwapChain.h"
#include "World.h"
#include "Object.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Audio.h"
#include "Component.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "LightComponent.h"
#include "AudioComponent.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

void Tool::OnCreate()
{
	App::OnCreate();

	auto title_audio = GetResourceManager()->CreateResourceFromFile<Audio>(L"..\\..\\Assets\\Audios\\The Crows.mp3");
	auto attack_audio = GetResourceManager()->CreateResourceFromFile<Audio>(L"..\\..\\Assets\\Audios\\Player Attack.ogg");

	auto sphere_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\sphere.obj");
	auto terrain_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\terrain.obj");
	auto sponza_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\sponza_basic.obj");

	auto sky_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\stars_map.jpg");
	auto brick_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_bricks_a_diff.jpg");
	auto arch_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_arch_diff.jpg");
	auto colA_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_column_a_diff.jpg");
	auto colB_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_column_b_diff.jpg");
	auto colC_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_column_c_diff.jpg");
	auto flagPole_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_flagpole_diff.jpg");
	auto floor_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Sponza\\sponza_floor_a_diff.jpg");

	auto brick_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	brick_material->AddTexture(brick_texture);
	auto arch_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	arch_material->AddTexture(arch_texture);
	auto colA_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	colA_material->AddTexture(colA_texture);
	auto colB_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	colB_material->AddTexture(colB_texture);
	auto colC_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	colC_material->AddTexture(colC_texture);
	auto flagPole_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	flagPole_material->AddTexture(flagPole_texture);

	auto floor_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Material.hlsl");
	floor_material->AddTexture(floor_texture);
	auto sky_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\SkyBox.hlsl");
	sky_material->AddTexture(sky_texture);
	sky_material->SetCullMode(CullMode::Front);

	// skybox
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		auto mesh_component = entity->CreateComponent<MeshComponent>();
		auto transform = entity->GetTransform();
		mesh_component->SetMesh(sphere_mesh);
		mesh_component->AddMaterial(sky_material);

		transform->SetScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	}

	//// terrain
	//{
	//	auto entity = GetWorld()->CreateEntity<Entity>();
	//	auto mesh_component = entity->CreateComponent<MeshComponent>();
	//	mesh_component->SetMesh(terrain_mesh);
	//	mesh_component->AddMaterial(floor_material);
	//}

	//// sponza
	//{
	//	auto entity = GetWorld()->CreateEntity<Entity>();
	//	auto mesh_component = entity->CreateComponent<MeshComponent>();
	//	mesh_component->SetMesh(sponza_mesh);
	//	mesh_component->AddMaterial(arch_material);
	//	mesh_component->AddMaterial(brick_material);
	//	mesh_component->AddMaterial(colA_material);
	//	mesh_component->AddMaterial(colB_material);
	//	mesh_component->AddMaterial(colC_material);
	//	mesh_component->AddMaterial(flagPole_material);
	//}

	//// light
	//{
	//	m_entity = GetWorld()->CreateEntity<Entity>();
	//	auto light_component = m_entity->CreateComponent<LightComponent>();
	//	light_component->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//	m_entity->GetTransform()->SetRotation(Vector3(-0.785f, 0.785f, 0.0f));
	//}

	// audio
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		m_audio_component = entity->CreateComponent<AudioComponent>();
		m_audio_component->SetAudio(title_audio, L"Title", true);
		m_audio_component->PlayAudio(L"Title");
	}
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		m_audio_component2 = entity->CreateComponent<AudioComponent>();
		m_audio_component2->SetAudio(attack_audio, L"Attack", false);
	}

	GetWorld()->CreateEntity<Object>();

	GetInput()->LockCursor(m_locked);
}

void Tool::OnUpdate(float delta_time)
{
	App::OnUpdate(delta_time);
	m_rotation += 1.57f * delta_time;

	//m_entity->GetTransform()->SetRotation(Vector3(-0.785f, m_rotation, 0.0f));

	if (GetInput()->IsKeyUp(Key::Escape))
	{
		m_locked = !m_locked;
		GetInput()->LockCursor(m_locked);
	}

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

	if (ImGui::Button("BGM Pause"))
	{
		if (m_audio_component)
			m_audio_component->PauseAudio(L"Title");
	}
	if (ImGui::Button("Attack"))
	{
		if (m_audio_component2)
			m_audio_component2->PlayAudio(L"Attack");
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
}
