#include "pch.h"
#include "Tool.h"
#include "World.h"
#include "Object.h"
#include "Entity.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

void Tool::OnCreate()
{
	App::OnCreate();
	m_entity = GetWorld()->CreateEntity<Object>();
}

void Tool::OnUpdate(float delta_time)
{
	App::OnUpdate(delta_time);
}
