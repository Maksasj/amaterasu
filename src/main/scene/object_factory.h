#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include <string>

#include "common/common.h"
#include "object.h"

#include "sphere_object.h"
#include "plane_object.h"
#include "marching_sphere_object.h"

namespace amts {
    class ObjectTypeFactory {
        public:
            struct ObjectTypeInfo {
                ObjectType m_enumValue;
                std::string m_name;
            };

        private:
            std::vector<ObjectTypeInfo> m_objectTypeInfos;
        
        public:
            ObjectTypeFactory() {
                m_objectTypeInfos.emplace_back(ObjectTypeInfo{SPHERE, "Sphere"});
                m_objectTypeInfos.emplace_back(ObjectTypeInfo{PLANE, "Plane"});
                m_objectTypeInfos.emplace_back(ObjectTypeInfo{MARCHING_SPHERE, "Marching Sphere"});
            }

            std::string to_string(const ObjectType& objectType) {
                switch (objectType) {
                    case ObjectType::SPHERE: return "Sphere";
                    case ObjectType::PLANE: return "Plane";
                    case ObjectType::MARCHING_SPHERE: return "Marching Sphere";
                    default: throw std::runtime_error("Object type is not implemented");
                }

                return "";
            }

            auto begin() {
                return m_objectTypeInfos.begin();
            }

            auto end() {
                return m_objectTypeInfos.end();
            }
    };
}

#endif