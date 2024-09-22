#include "string"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif



class DUCKENGINE_API DuckEngine_Sound
{
public:
	static void LoadSound(const std::string& soundName, const std::string& filePath);
	static void PlaySound(const std::string& soundName);
	static void StopSound(const std::string& soundName = "");
};