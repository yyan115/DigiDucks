#include "DuckEngine_Input.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

bool DuckEngine_Input::WKeyDown = false;
bool DuckEngine_Input::AKeyDown = false;
bool DuckEngine_Input::SKeyDown = false;
bool DuckEngine_Input::DKeyDown = false;

// Define key constants for letters (A-Z)
const int DuckEngine_Input::KEY_A = GLFW_KEY_A;
const int DuckEngine_Input::KEY_B = GLFW_KEY_B;
const int DuckEngine_Input::KEY_C = GLFW_KEY_C;
const int DuckEngine_Input::KEY_D = GLFW_KEY_D;
const int DuckEngine_Input::KEY_E = GLFW_KEY_E;
const int DuckEngine_Input::KEY_F = GLFW_KEY_F;
const int DuckEngine_Input::KEY_G = GLFW_KEY_G;
const int DuckEngine_Input::KEY_H = GLFW_KEY_H;
const int DuckEngine_Input::KEY_I = GLFW_KEY_I;
const int DuckEngine_Input::KEY_J = GLFW_KEY_J;
const int DuckEngine_Input::KEY_K = GLFW_KEY_K;
const int DuckEngine_Input::KEY_L = GLFW_KEY_L;
const int DuckEngine_Input::KEY_M = GLFW_KEY_M;
const int DuckEngine_Input::KEY_N = GLFW_KEY_N;
const int DuckEngine_Input::KEY_O = GLFW_KEY_O;
const int DuckEngine_Input::KEY_P = GLFW_KEY_P;
const int DuckEngine_Input::KEY_Q = GLFW_KEY_Q;
const int DuckEngine_Input::KEY_R = GLFW_KEY_R;
const int DuckEngine_Input::KEY_S = GLFW_KEY_S;
const int DuckEngine_Input::KEY_T = GLFW_KEY_T;
const int DuckEngine_Input::KEY_U = GLFW_KEY_U;
const int DuckEngine_Input::KEY_V = GLFW_KEY_V;
const int DuckEngine_Input::KEY_W = GLFW_KEY_W;
const int DuckEngine_Input::KEY_X = GLFW_KEY_X;
const int DuckEngine_Input::KEY_Y = GLFW_KEY_Y;
const int DuckEngine_Input::KEY_Z = GLFW_KEY_Z;

// Define key constants for numbers (0-9)
const int DuckEngine_Input::KEY_0 = GLFW_KEY_0;
const int DuckEngine_Input::KEY_1 = GLFW_KEY_1;
const int DuckEngine_Input::KEY_2 = GLFW_KEY_2;
const int DuckEngine_Input::KEY_3 = GLFW_KEY_3;
const int DuckEngine_Input::KEY_4 = GLFW_KEY_4;
const int DuckEngine_Input::KEY_5 = GLFW_KEY_5;
const int DuckEngine_Input::KEY_6 = GLFW_KEY_6;
const int DuckEngine_Input::KEY_7 = GLFW_KEY_7;
const int DuckEngine_Input::KEY_8 = GLFW_KEY_8;
const int DuckEngine_Input::KEY_9 = GLFW_KEY_9;

// Define key constants for common keys
const int DuckEngine_Input::KEY_SPACE = GLFW_KEY_SPACE;
const int DuckEngine_Input::KEY_ENTER = GLFW_KEY_ENTER;
const int DuckEngine_Input::KEY_ESCAPE = GLFW_KEY_ESCAPE;
const int DuckEngine_Input::KEY_LEFT = GLFW_KEY_LEFT;
const int DuckEngine_Input::KEY_RIGHT = GLFW_KEY_RIGHT;
const int DuckEngine_Input::KEY_UP = GLFW_KEY_UP;
const int DuckEngine_Input::KEY_DOWN = GLFW_KEY_DOWN;
const int DuckEngine_Input::KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const int DuckEngine_Input::KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
const int DuckEngine_Input::KEY_BACKSPACE = GLFW_KEY_BACKSPACE;