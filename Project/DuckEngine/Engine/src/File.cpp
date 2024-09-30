#include "File.h"
#include <iostream>
#include <string.h>
#include <filesystem>




std::string FilePath::GetFilePath()
{
	return std::filesystem::current_path().string();
}

void FilePath::PrintPath()
{
	FilePath fp;
	std::cout << "Current file path: " << fp.GetFilePath() << std::endl;
}
