#include "pch.h"
#include "Tool.h"
#include "World.h"
#include "Entity.h"
#include "GameObject.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

void Tool::OnCreate()
{
	App::OnCreate();
	m_entity = GetWorld()->CreateEntity<GameObject>();
}

void Tool::OnUpdate(float delta_time)
{
	App::OnUpdate(delta_time);
}
