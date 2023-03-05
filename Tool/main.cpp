#include "pch.h"
#include "Tool.h"

int main()
{
	try
	{
		Tool tool;
		tool.Run();
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}