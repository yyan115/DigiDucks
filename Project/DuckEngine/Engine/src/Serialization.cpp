#include "Serialization.h"

json Serialization::jsonData;  // Define jsonData
//std::string Serialization::title;  // Define title
//int Serialization::width = 0;   // Define width and optionally initialize it
//int Serialization::height = 0;  // Define height and optionally initialize it
Window_init W_init;

void Serialization::Init_json() {
    // Open the JSON file
    std::ifstream file("../Resources/windows init.json");

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
    }

    // Parse the JSON data

    file >> jsonData;

    /*init.title = jsonData["title"];
    init.width = jsonData["width"];
    init.height = jsonData["height"];*/

    /*init.height = height;
    init.width = width;
    init.title = title;*/

    W_init.title = jsonData.at("title").get<std::string>();
    W_init.width = jsonData.at("width").get<GLint>();
    W_init.height = jsonData.at("height").get<GLint>();


    // Access and print some values from the JSON object
    //std::cout << "Parsed JSON data: " << jsonData.dump(4) << std::endl;

    // Example: Access specific fields in the JSON file
    /*if (jsonData.contains("name")) {
        std::string name = jsonData["name"];
        std::cout << "Name: " << name << std::endl;
    }*/
   
    file.close();
}


