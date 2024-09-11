
#include <string>
#include <sstream>

class UIDebugConsole {
public:
    // Add a message to the log
    void AddLog(const char* message);

    // Clear the log
    void Clear();

    // Render the console in ImGui
    void Render(bool* p_open);    

private:
    std::string buffer;  // Buffer to hold the log
};
