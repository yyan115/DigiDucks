//#include "Entity.h"
//#include <cstring>
//#include <string>
//
//struct Entity::Impl
//{
//    std::string name;
//
//    Impl() : name("") {}
//};
//
//Entity::Entity(int id, const char* name)
//    : entityID(id), impl(new Impl())
//{
//    if (name) 
//    {
//        impl->name = name;
//    }
//    else 
//    {
//        impl->name = "";
//    }
//}
//
//
//const char* Entity::GetName() const
//{
//    return impl->name.c_str();
//}
//
//void Entity::SetName(const char* name)
//{
//    if (name) 
//    {
//        impl->name = name;
//    }
//    else {
//        impl->name.clear();
//    }
//}
//
//bool Entity::IsName(const char* compareName) const
//{
//    if (compareName == nullptr)
//        return false;  
//    return std::strcmp(impl->name.c_str(), compareName) == 0;
//}
//
//void Entity::Release()
//{
//    if (impl)
//    {
//        delete impl;
//    }
//}
