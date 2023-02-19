#include "pch.h"
#include "App.h"

int main()
{
	try
	{
		Engine::Create();
	}
	catch (...) { return -1; }

	{
		try
		{
			App app;
			while (app.IsRunning());
		}
		catch (...)
		{
			Engine::Release();
			return -1;
		}
	}

	Engine::Release();
	return 0;
}