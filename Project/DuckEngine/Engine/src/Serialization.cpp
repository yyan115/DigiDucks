//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <stdexcept>
//#include "Serialization.h"
//
//class ISerialization {
//public:
//    virtual void WriteInt(const std::string& name, int value) = 0;
//    virtual void WriteFloat(const std::string& name, float value) = 0;
//    virtual void WriteString(const std::string& name, const std::string& value) = 0;
//
//    virtual void ReadInt(const std::string& name, int& value) = 0;
//    virtual void ReadFloat(const std::string& name, float& value) = 0;
//    virtual void ReadString(const std::string& name, std::string& value) = 0;
//
//    virtual ~ISerialization() = default;
//};
//
//class JSONSerializer : public ISerialization {
//private:
//    std::stringstream jsonStream;
//    std::ifstream jsonFileIn;
//    std::ofstream jsonFileOut;
//    bool readValue;
//
//    // Helper function for writing a key-value pair
//    template<typename T>
//    void WriteKeyValue(const std::string& name, const T& value) {
//        jsonStream << "\t\"" << name << "\": " << value << ",\n";
//    }
//
//    // Helper function to remove trailing comma from the last element
//    void CleanTrailingComma() {
//        std::string result = jsonStream.str();
//        size_t lastCommaPos = result.find_last_of(",");
//        if (lastCommaPos != std::string::npos) {
//            result.erase(lastCommaPos, 1);
//        }
//        jsonStream.str(result);
//    }
//
//public:
//    // Constructor for writing or reading based on readValue bool flag
//    JSONSerializer(const std::string& filePath, bool isRead = false) : readValue(isRead) {
//        if (readValue) {
//            jsonFileIn.open(filePath);
//            if (!jsonFileIn.is_open()) {
//                throw std::runtime_error("Failed to open file for reading.");
//            }
//        }
//        else {
//            jsonFileOut.open(filePath);
//            if (!jsonFileOut.is_open()) {
//                throw std::runtime_error("Failed to open file for writing.");
//            }
//            jsonStream << "{\n";
//        }
//    }
//
//    // Destructor to finalize JSON writing and close the file
//    ~JSONSerializer() {
//        if (!readValue) {
//            CleanTrailingComma();  // Clean up the trailing comma for valid JSON
//            jsonStream << "\n}";
//            jsonFileOut << jsonStream.str();
//            jsonFileOut.close();
//        }
//        else {
//            jsonFileIn.close();
//        }
//    }
//
//    void WriteInt(const std::string& name, int value) override {
//        WriteKeyValue(name, value);
//    }
//
//    void WriteFloat(const std::string& name, float value) override {
//        WriteKeyValue(name, value);
//    }
//
//    void WriteString(const std::string& name, const std::string& value) override {
//        jsonStream << "\t\"" << name << "\": \"" << value << "\",\n";
//    }
//
//    // Helper function for reading and finding a value in the JSON string
//    std::string FindValue(const std::string& name) {
//        std::string line;
//        while (std::getline(jsonFileIn, line)) {
//            if (line.find(name) != std::string::npos) {
//                size_t colonPos = line.find(":");
//                return line.substr(colonPos + 1);
//            }
//        }
//        throw std::runtime_error("Key not found in JSON: " + name);
//    }
//
//    void ReadInt(const std::string& name, int& value) override {
//        std::string result = FindValue(name);
//        value = std::stoi(result);
//    }
//
//    void ReadFloat(const std::string& name, float& value) override {
//        std::string result = FindValue(name);
//        value = std::stof(result);
//    }
//
//    void ReadString(const std::string& name, std::string& value) override {
//        std::string result = FindValue(name);
//        value = result.substr(2, result.length() - 4);  // Remove quotes
//    }
//};
//
//class GameObject {
//public:
//    //whatever variables we using
//
//    // Serialization function
//    void Serialize(ISerialization& serializer) {
//        serializer.WriteInt("");
//        serializer.WriteFloat("");
//        serializer.WriteString("");
//    }
//
//    // Deserialization function
//    void Deserialize(ISerialization& serializer) {
//        serializer.ReadInt("");
//        serializer.ReadFloat("");
//        serializer.ReadString("");
//    }
//};