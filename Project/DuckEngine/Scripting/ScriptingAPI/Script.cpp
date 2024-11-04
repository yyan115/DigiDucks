#include "Script.h"

#pragma warning(disable : 4679)

namespace ScriptAPI
{
	void Script::Update()
	{
		System::Diagnostics::Debug::WriteLine("Unoverride Update");
	}
}