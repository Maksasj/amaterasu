#ifndef _OBJECT_CREATE_UI_WINDOW_H_
#define _OBJECT_CREATE_UI_WINDOW_H_

#include "common/common_ui_window.h"
#include "scene/object_factory.h"

namespace amts {
    class ObjectCreateUIWindow : public CommonUIWindow {
        private:
            std::vector<std::unique_ptr<Object>> m_creatingObjectQueue;
            std::unique_ptr<ObjectTypeFactory> m_objectTypeFactory;

            ObjectType m_selectedObjectType;

            std::string m_objectName;
            Vec3f m_objectPosition;
            u64 m_objectMaterialId;

            f32 m_objectSphereRadius;
            Vec3f m_objectPlaneNormal;

        public:
            ObjectCreateUIWindow() 
                : CommonUIWindow("Create Object", false),
                  m_objectTypeFactory(nullptr),
                  m_selectedObjectType(SPHERE) {
                
                m_objectTypeFactory = std::make_unique<ObjectTypeFactory>();

                m_objectName = "Unnamed object";
                m_objectPosition = Vec3f::splat(0.0f);
                m_objectMaterialId = 0;

                m_objectSphereRadius = 1.0f;
                m_objectPlaneNormal = Vec3f(0.0f, -1.0f, 0.0f);
            }

            ~ObjectCreateUIWindow() override {
                m_objectTypeFactory = nullptr;
            }

            void run(std::unique_ptr<MaterialCollection>& materials) {
                if(!CommonUIWindow::start_window()) return;

                // Object type
                ImGui::Text("Object Type");
                if(ImGui::BeginCombo("## Object Type", m_objectTypeFactory->to_string(m_selectedObjectType).c_str())) {
                    for(auto& objectType : *m_objectTypeFactory) {
                        const bool isSelected = (m_selectedObjectType == objectType.m_enumValue);

                        if(ImGui::Selectable(objectType.m_name.c_str(), isSelected))
                            m_selectedObjectType = objectType.m_enumValue;

                        if(isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                // Object m_objectName
                ImGui::Text("Name");
                char buffer[256] = { 0 };
                m_objectName.copy(buffer, m_objectName.size());
                ImGui::InputText("## value", buffer, 256);
                m_objectName = std::string(buffer);
                ImGui::SameLine();

                if(ImGui::Button("Clear"))
                    m_objectName.clear();

                ImGui::Text("Material");
                if(ImGui::BeginCombo("## Material", materials->m_materials[m_objectMaterialId]->m_name.c_str())) {
                    for(u64 i = 0; i < materials->m_materials.size(); ++i) {
                        const bool isSelected = (m_objectMaterialId == i);

                        if(ImGui::Selectable(materials->m_materials[i]->m_name.c_str(), isSelected))
                            m_objectMaterialId = i;

                        if(isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }
                
                // Object position
                ImGui::Text("Position");
                ImGui::InputFloat3("## Position", static_cast<f32*>(&m_objectPosition.x));

                ImGui::Separator();

                switch (m_selectedObjectType) {
                    case ObjectType::SPHERE: {
                        ImGui::Text("Sphere Radius");
                        ImGui::DragFloat("## Sphere Radius", &m_objectSphereRadius, 0.005f, 0.0f, std::numeric_limits<f32>::max());

                        if(ImGui::Button("Create Object"))
                            m_creatingObjectQueue.emplace_back(std::make_unique<SphereObject>(m_objectName, m_objectPosition, m_objectMaterialId, m_objectSphereRadius));
                        break;
                    };
                    case ObjectType::PLANE: {
                        ImGui::Text("Normal");
                        ImGui::InputFloat3("## Normal", static_cast<f32*>(&m_objectPlaneNormal.x));

                        if(ImGui::Button("Create Object"))
                            m_creatingObjectQueue.emplace_back(std::make_unique<PlaneObject>(m_objectName, m_objectPosition, m_objectMaterialId, m_objectPlaneNormal));
                        break;
                    };
                    default: throw std::runtime_error("Object type is not implemented");
                }

                CommonUIWindow::end_window();
            }

            bool have_object_to_pop() const {
                return m_creatingObjectQueue.size() != 0;
            }

            std::unique_ptr<Object> pop_top_object() {
                std::unique_ptr<Object> object = std::move(m_creatingObjectQueue.back());
                m_creatingObjectQueue.pop_back();
                return std::move(object);
            } 
    };
}

#endif