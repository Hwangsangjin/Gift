#include "pch.h"
#include "Tool.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "RenderSystem.h"
#include "Display.h"
#include "SwapChain.h"
#include "World.h"
#include "Object.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "VertexMesh.h"
#include "Audio.h"
#include "Component.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "TerrainComponent.h"
#include "WaterComponent.h"
#include "LightComponent.h"
#include "AudioComponent.h"
#include "Timer.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

void Tool::OnCreate()
{
	Engine::OnCreate();

	auto title_audio = GetResourceManager()->CreateResourceFromFile<Audio>(L"..\\..\\Assets\\Audios\\The Crows.mp3");

	auto sphere_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\sphere.obj");
	auto plane_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\plane.obj");

	auto sky_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\stars_map.jpg");
	auto height_map_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\height_map.png");
	auto wave_height_map_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\waveHeightMap.png");
	auto grass_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\grass.jpg");
	auto ground_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\ground.jpg");

	auto black_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\black.png");
	auto plane_texture = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\plane.png");

	m_ui_texture[0] = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Icon_Bomb.png");
	m_ui_texture[1] = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Icon_Fireball.png");
	m_ui_texture[2] = GetResourceManager()->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\Icon_Hook.png");

	m_ui_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Default.hlsl");
	m_ui_material->AddTexture(m_ui_texture[0]);
	for (UINT i = 0; i < 3; i++)
		m_indices.push_back(i);

	m_plane_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Default.hlsl");
	m_plane_material->AddTexture(black_texture);

	m_sphere_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\Default.hlsl");
	m_sphere_material->AddTexture(plane_texture);

	auto sky_material = GetResourceManager()->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\SkyBox.hlsl");
	sky_material->AddTexture(sky_texture);
	sky_material->SetCullMode(CullMode::Front);

	// skybox
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		auto mesh_component = entity->CreateComponent<MeshComponent>();
		mesh_component->SetMesh(sphere_mesh);
		mesh_component->AddMaterial(sky_material);

		auto transform = entity->GetTransform();
		transform->SetScale(Vector3(5000.0f, 5000.0f, 5000.0f));
	}

	// terrain
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		auto terrain_component = entity->CreateComponent<TerrainComponent>();
		terrain_component->SetHeightMap(height_map_texture);
		terrain_component->SetGroundMap(grass_texture);
		terrain_component->SetCliffMap(ground_texture);

		auto transform = entity->GetTransform();
		transform->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		transform->SetPosition(Vector3(10.0f, 0.0f, 0.0f));
	}

	// sea
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		auto water_component = entity->CreateComponent<WaterComponent>();
		water_component->SetWaveHeightMap(wave_height_map_texture);

		auto transform = entity->GetTransform();
		transform->SetPosition(Vector3(-768.0f, 15.0f, -768.0f));
	}

	// sphere
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		auto mesh_component = entity->CreateComponent<MeshComponent>();
		mesh_component->SetMesh(sphere_mesh);
		mesh_component->AddMaterial(m_sphere_material);

		auto transform = entity->GetTransform();
		entity->GetTransform()->SetPosition(Vector3(-2.0f, 2.0f, 0.0f));
	}

	// plane
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		auto mesh_component = entity->CreateComponent<MeshComponent>();
		mesh_component->SetMesh(plane_mesh);
		mesh_component->AddMaterial(m_plane_material);

	}

	// light
	{
		m_entity = GetWorld()->CreateEntity<Entity>();
		auto light_component = m_entity->CreateComponent<LightComponent>();
		light_component->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_entity->GetTransform()->SetRotation(Vector3(-0.785f, 0.785f, 0.0f));
	}

	// audio
	{
		auto entity = GetWorld()->CreateEntity<Entity>();
		m_audio_component = entity->CreateComponent<AudioComponent>();
		m_audio_component->SetAudio(GetResourceManager()->GetResource<Audio>((L"The Crows")));
		GetSoundSystem()->PlaySound(m_audio_component);
	}

	GetWorld()->CreateEntity<Object>();
}

void Tool::OnUpdate(float delta_time)
{
	Engine::OnUpdate(delta_time);

	if (GetInputSystem()->IsKeyUp(Key::Escape))
	{
		m_locked = !m_locked;
		GetInputSystem()->LockCursor(m_locked);
	}

	m_entity->GetTransform()->SetRotation(Vector3(0.785f, -3.14f, 0.0f));

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

	if (ImGui::Button("BGM"))
	{
		GetSoundSystem()->PauseSound(m_audio_component);
	}

	if (ImGui::Button("Animation"))
	{
		VertexMesh quad_vertices[] =
		{
			VertexMesh(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f), Vector3(), Vector3(), Vector3()),
			VertexMesh(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f), Vector3(), Vector3(), Vector3()),
			VertexMesh(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f), Vector3(), Vector3(), Vector3()),
			VertexMesh(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector3(), Vector3(), Vector3())
		};

		UINT quad_indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		MaterialSlot quad_material_slots[] =
		{
			{ 0, 6, 0 }
		};

		Mesh* mesh = new Mesh(quad_vertices, 4, quad_indices, 6, quad_material_slots, 1, GetResourceManager());
		assert(mesh);
		MeshPtr mesh_ptr(mesh);
		assert(mesh_ptr);

		{
			m_ui = GetWorld()->CreateEntity<Entity>();
			m_ui->GetTransform()->SetPosition(Vector3(2.0f, 2.0f, 0.0f));
			m_sprite_component = m_ui->CreateComponent<SpriteComponent>();
			m_sprite_component->SetMesh(mesh_ptr);
			m_sprite_component->AddMaterial(m_ui_material);
		}
	}

	if (m_ui)
	{
		m_render_time = 1.0f / m_indices.size();

		m_life_time += delta_time;
		if (m_life_time >= m_render_time)
		{
			if (++m_apply_index >= m_indices.size())
				m_apply_index = 0;

			m_life_time = 0.0f;
		}

		if (m_indices.size())
		{
			m_ui_material->RemoveTexture(0);
			m_ui_material->AddTexture(m_ui_texture[m_apply_index]);
		}
	}

	if (ImGui::Button("Wireframe"))
	{
		if (FillMode::Solid == m_ui_material->GetFillMode())
		{
			m_ui_material->SetFillMode(FillMode::Wireframe);
			m_sphere_material->SetFillMode(FillMode::Wireframe);
		}
		else
		{
			m_ui_material->SetFillMode(FillMode::Solid);
			m_sphere_material->SetFillMode(FillMode::Solid);
		}
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
