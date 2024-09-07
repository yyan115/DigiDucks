#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GraphicsManager.h"

bool GraphicsManager::CloseWindow() {
    if (glfwWindowShouldClose(ptr_window))
        return true;

    return false;
}

//void GraphicsManager::Render() {
//
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // for ( auto & sprites : spritesList){
//    // render or something
//    // }
//
//    glfwSwapBuffers(ptr_window);
//}


void GraphicsManager::Shutdown() {
    glfwDestroyWindow(ptr_window);
    glfwTerminate();
}

// Vertex and Fragment shader sources
const char* vertexShaderSource = R"(
#version 450 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 450 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.5, 0.8, 0.2, 1.0);  // Green-ish color
}
)";

GLuint GraphicsManager::CreateShaderProgram(const char* vertexSource, const char* fragmentSource) {
    // Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compile errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders to create a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shaders (they're linked into the program now and can be deleted)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void GraphicsManager::Render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the shader program and bind the VAO to render the rectangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Draw the rectangle (6 vertices = 2 triangles)
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbind the VAO
    glBindVertexArray(0);

    // Swap buffers (assuming glfwSwapBuffers is handled elsewhere)
    glfwSwapBuffers(ptr_window);

    glfwPollEvents();
}


/*  ____________________________________tas_____________________________________ */
/*! init

@param GLint width
@param GLint height
Dimensions of window requested by program

@param std::string title_str
String printed to window's title bar

@return bool
true if OpenGL context and GLEW were successfully initialized.
false otherwise.

Uses GLFW to create OpenGL context. GLFW's initialization follows from here:
http://www.glfw.org/docs/latest/quick.html
a window of size width x height pixels
and its associated OpenGL context that matches a core profile that is
compatible with OpenGL 4.5 and doesn't support "old" OpenGL, has 32-bit RGBA,
double-buffered color buffer, 24-bit depth buffer and 8-bit stencil buffer
with each buffer of size width x height pixels
*/
bool GraphicsManager::Initialize(GLint width, GLint height, std::string title) {
    width = width;
    height = height;
    title = title;

    // Check if glfw init success
    if (!glfwInit()) {
        std::cout << "GLFW init has failed - abort program!!!" << std::endl;
        return false;
    }

    // In case a GLFW function fails, an error is reported to callback function
    //glfwSetErrorCallback(error_cb);

    // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
    // in that context:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);

    // Check if glfw context created successfully
    ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!ptr_window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(ptr_window);

    //setup_event_callbacks();

    // this is the default setting ...
    glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Part 2: Initialize entry points to OpenGL functions and extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return false;
    }
    if (GLEW_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Warning: The driver may lack full compatibility with OpenGL 4.5, potentially limiting access to advanced features." << std::endl;
    }

    // Define the rectangle vertices (two triangles making up a rectangle)
    float vertices[] = {
        // First triangle
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,  // bottom right
         0.5f,  0.5f, 0.0f,  // top right
         // Second triangle
         -0.5f, -0.5f, 0.0f,  // bottom left
          0.5f,  0.5f, 0.0f,  // top right
         -0.5f,  0.5f, 0.0f   // top left
    };

    // Create and compile shaders, link them into a program
    shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Generate and bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the vertex attributes (only position for now)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO for now
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void GraphicsManager::InsertShader(std::string shdr_pgm_name,
    std::string vtx_shdr,
    std::string frg_shdr) {

    std::map<std::string, GLSLShader>::iterator it =
        shaders.find(shdr_pgm_name);

    if (it != shaders.end()) return;

    std::vector<std::pair<GLenum, std::string>> shdr_files{
    std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
    std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
    };

    GLSLShader shdr_pgm;

    // Automation hook. [!WARNING!] Do not alter/remove this!
    AUTOMATION_HOOK_SHADER(shdr_pgm, shdr_files);

    shdr_pgm.CompileLinkValidate(shdr_files);

    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs\n";
        std::cout << shdr_pgm.GetLog() << "\n";
        std::exit(EXIT_FAILURE);
    }

    // add compiled, linked, and validated shader program to
    // std::map container GLApp::shdrpgms
    shaders[shdr_pgm_name] = shdr_pgm;
}



/// OLD HELPER CODE THAT DEALS WITH I/O. Not sure if should fall under graphicsmanager, copied over as reference for now.







///*  _________________________________________________________________________*/
///*! key_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param int
//the keyboard key that was pressed or released
//
//@parm int
//Platform-specific scancode of the key
//
//@parm int
//GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
//action will be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it,
//for example E-mail and Play keys.
//
//@parm int
//bit-field describing which modifier keys (shift, alt, control)
//were held down
//
//@return none
//
//This function is called when keyboard buttons are pressed.
//When the ESC key is pressed, the close flag of the window is set.
//*/
//void GLHelper::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
//    if (GLFW_PRESS == action) {
//#ifdef _DEBUG
//        std::cout << "Key pressed" << std::endl;
//#endif
//    }
//    else if (GLFW_REPEAT == action) {
//#ifdef _DEBUG
//        std::cout << "Key repeatedly pressed" << std::endl;
//#endif
//    }
//    else if (GLFW_RELEASE == action) {
//#ifdef _DEBUG
//        std::cout << "Key released" << std::endl;
//#endif
//    }
//
//    // key state changes from released to pressed
//    if (GLFW_PRESS == action) {
//        if (GLFW_KEY_ESCAPE == key) {
//            glfwSetWindowShouldClose(pwin, GLFW_TRUE);
//        }
//
//        // less buggy, but doesnt mimic sample as well
//        //if (key == GLFW_KEY_V)
//        //    GLApp::camera2d.camtype_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_Z)
//        //    GLApp::camera2d.zoom_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_H)
//        //    GLApp::camera2d.left_turn_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_K)
//        //    GLApp::camera2d.right_turn_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_U)
//        //    GLApp::camera2d.move_flag = GL_TRUE;
//
//        // set accordingly
//        GLApp::camera2d.camtype_flag = (key == GLFW_KEY_V) ? GL_TRUE : GL_FALSE;
//        GLApp::camera2d.zoom_flag = (key == GLFW_KEY_Z) ? GL_TRUE : GL_FALSE;
//        GLApp::camera2d.left_turn_flag = (key == GLFW_KEY_H) ? GL_TRUE : GL_FALSE;
//        GLApp::camera2d.right_turn_flag = (key == GLFW_KEY_K) ? GL_TRUE : GL_FALSE;
//        GLApp::camera2d.move_flag = (key == GLFW_KEY_U) ? GL_TRUE : GL_FALSE;
//    }
//    else if (GLFW_REPEAT == action) {
//        // key state was and is being pressed (comment out to allow holding)
//        //GLApp::camera2d.camtype_flag    = GL_FALSE;
//        //GLApp::camera2d.zoom_flag       = GL_FALSE;
//        //GLApp::camera2d.left_turn_flag  = GL_FALSE;
//        //GLApp::camera2d.right_turn_flag = GL_FALSE;
//        //GLApp::camera2d.move_flag       = GL_FALSE;
//    }
//    else if (GLFW_RELEASE == action) {
//        // key start changes from pressed to released
//        GLApp::camera2d.camtype_flag = GL_FALSE;
//        GLApp::camera2d.zoom_flag = GL_FALSE;
//        GLApp::camera2d.left_turn_flag = GL_FALSE;
//        GLApp::camera2d.right_turn_flag = GL_FALSE;
//        GLApp::camera2d.move_flag = GL_FALSE;
//    }
//}
//
///*  _________________________________________________________________________*/
///*! mousebutton_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param int
//the mouse button that was pressed or released
//GLFW_MOUSE_BUTTON_LEFT and GLFW_MOUSE_BUTTON_RIGHT specifying left and right
//mouse buttons are most useful
//
//@parm int
//action is either GLFW_PRESS or GLFW_RELEASE
//
//@parm int
//bit-field describing which modifier keys (shift, alt, control)
//were held down
//
//@return none
//
//This function is called when mouse buttons are pressed.
//*/
//void GLHelper::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod) {
//    switch (button) {
//    case GLFW_MOUSE_BUTTON_LEFT:
//#ifdef _DEBUG
//        std::cout << "Left mouse button ";
//#endif
//        break;
//    case GLFW_MOUSE_BUTTON_RIGHT:
//#ifdef _DEBUG
//        std::cout << "Right mouse button ";
//#endif
//        break;
//    }
//    switch (action) {
//    case GLFW_PRESS:
//#ifdef _DEBUG
//        std::cout << "pressed!!!" << std::endl;
//#endif
//        break;
//    case GLFW_RELEASE:
//#ifdef _DEBUG
//        std::cout << "released!!!" << std::endl;
//#endif
//        break;
//    }
//}
//
///*  _________________________________________________________________________*/
///*! mousepos_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param double
//new cursor x-coordinate, relative to the left edge of the client area
//
//@param double
//new cursor y-coordinate, relative to the top edge of the client area
//
//@return none
//
//This functions receives the cursor position, measured in screen coordinates but
//relative to the top-left corner of the window client area.
//*/
//void GLHelper::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
//#ifdef _DEBUG
//    std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
//#endif
//}
//
///*  _________________________________________________________________________*/
///*! mousescroll_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param double
//Scroll offset along X-axis
//
//@param double
//Scroll offset along Y-axis
//
//@return none
//
//This function is called when the user scrolls, whether with a mouse wheel or
//touchpad gesture. Although the function receives 2D scroll offsets, a simple
//mouse scroll wheel, being vertical, provides offsets only along the Y-axis.
//*/
//void GLHelper::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset) {
//#ifdef _DEBUG
//    std::cout << "Mouse scroll wheel offset: ("
//        << xoffset << ", " << yoffset << ")" << std::endl;
//#endif
//}
//
///*  _________________________________________________________________________ */
///*! error_cb
//
//@param int
//GLFW error code
//
//@parm char const*
//Human-readable description of the code
//
//@return none
//
//The error callback receives a human-readable description of the error and
//(when possible) its cause.
//*/
//void GLHelper::error_cb(int error, char const* description) {
//#ifdef _DEBUG
//    std::cerr << "GLFW error: " << description << std::endl;
//#endif
//}
//
///*  _________________________________________________________________________ */
///*! fbsize_cb
//
//@param GLFWwindow*
//Handle to window that is being resized
//
//@parm int
//Width in pixels of new window size
//
//@parm int
//Height in pixels of new window size
//
//@return none
//
//This function is called when the window is resized - it receives the new size
//of the window in pixels.
//*/
//void GLHelper::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
//#ifdef _DEBUG
//    std::cout << "fbsize_cb getting called!!!" << std::endl;
//#endif
//    GLHelper::width = width;
//    GLHelper::height = height;
//}
//
///*  _________________________________________________________________________*/
///*! update_time
//
//@param double
//fps_calc_interval: the interval (in seconds) at which fps is to be
//calculated
//
//This function must be called once per game loop. It uses GLFW's time functions
//to compute:
//1. the interval in seconds between each frame
//2. the frames per second every "fps_calc_interval" seconds
//*/
//void GLHelper::update_time(double fps_calc_interval) {
//    // get elapsed time (in seconds) between previous and current frames
//    static double prev_time = glfwGetTime();
//    double curr_time = glfwGetTime();
//    delta_time = curr_time - prev_time;
//    prev_time = curr_time;
//
//    // fps calculations
//    static double count = 0.0; // number of game loop iterations
//    static double start_time = glfwGetTime();
//    // get elapsed time since very beginning (in seconds) ...
//    double elapsed_time = curr_time - start_time;
//
//    ++count;
//
//    // update fps at least every 10 seconds ...
//    fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
//    fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
//    if (elapsed_time > fps_calc_interval) {
//        GLHelper::fps = count / elapsed_time;
//        start_time = curr_time;
//        count = 0.0;
//    }
//}
