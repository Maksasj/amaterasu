#ifndef _MATERIALS_UI_WINDOW_H_
#define _MATERIALS_UI_WINDOW_H_

#include "common/common_ui_window.h"
#include "materials/material_collection.h"

namespace amts {
    class MaterialsUIWindow : public CommonUIWindow {
        private:
            u64 m_selectedMaterial;

        public:
            MaterialsUIWindow() : CommonUIWindow("Materials") {

            }

            ~MaterialsUIWindow() override {
                
            }

            static Color color_picker_widget(const std::string& label, const Color& color) {
                std::stringstream ss;
                ss << label << " ## " << (void*) &color;
                const std::string completeLabel = ss.str();

                ImVec4 temporaryColor = ImVec4(color.r(), color.g(), color.b(), color.a());
                bool open_popup = ImGui::ColorButton(completeLabel.c_str(), temporaryColor, ImGuiColorEditFlags_None);
                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                open_popup |= ImGui::Button((std::string("Pallete ##") + completeLabel).c_str());

                if (open_popup)
                    ImGui::OpenPopup(completeLabel.c_str());

                if (ImGui::BeginPopup(completeLabel.c_str())) {
                    ImGui::Text("%s", label.c_str());
                    ImGui::Separator();
                    ImGui::ColorPicker4("## picker", (float*)&temporaryColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    ImGui::Text("Current");
                    ImGui::ColorButton("## current", temporaryColor, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
                    
                    ImGui::EndGroup();
                    ImGui::EndPopup();
                }

                return Color(Vec4f(temporaryColor.x, temporaryColor.y, temporaryColor.z, temporaryColor.w));
            }

            void run(std::unique_ptr<MaterialCollection>& materialsPtr, std::unique_ptr<Scene>& scenePtr) {
                if(!CommonUIWindow::start_window()) return;

                MaterialCollection& materialCollection = *materialsPtr;
                std::vector<std::unique_ptr<Material>>& materials = materialCollection.m_materials;

                if(ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if(ImGui::BeginCombo("## Materials", materials[m_selectedMaterial]->m_name.c_str())) {
                        for(u64 i = 0; i < materials.size(); ++i) {
                            const bool isSelected = (m_selectedMaterial == i);

                            std::stringstream ss;
                            ss << materials[i]->m_name << " ## " << (void*) materials[i].get();
                            const std::string materialName = ss.str();

                            if(ImGui::Selectable(materialName.c_str(), isSelected))
                                m_selectedMaterial = i;

                            if(isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::TreePop();
                }

                if(ImGui::TreeNodeEx("Actions", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if(ImGui::Button("Add material")) {
                        materials.emplace_back(Material::create_default_material());
                    } ImGui::SameLine();
                    
                    if(ImGui::Button("Delete material")) {
                        if(materials.size() > 1) {
                            if(m_selectedMaterial < materials.size())
                                materials.erase(materials.begin() + m_selectedMaterial);

                            if(m_selectedMaterial > 0)
                                --m_selectedMaterial;
                        }
                    } ImGui::SameLine();
                    
                    if(ImGui::Button("Copy material")) {
                        if(m_selectedMaterial < materials.size())
                            materials.emplace_back(materials[m_selectedMaterial]->clone());
                    }

                    ImGui::TreePop();
                }
                
                ImGui::Separator();

                if(ImGui::TreeNodeEx("Selected Material", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if(m_selectedMaterial < materials.size()) {
                        Material& material = *materials[m_selectedMaterial];

                        ImGui::Text("Name");
                        char buffer[256] = { 0 };
                        material.m_name.copy(buffer,  material.m_name.size());
                        ImGui::InputText("## value", buffer, 256);
                        material.m_name = std::string(buffer);

                        ImVec4 temporaryColor = ImVec4(
                            material.m_albedo.r(), 
                            material.m_albedo.g(), 
                            material.m_albedo.b(), 
                            material.m_albedo.a());

                        ImGui::Text("Albedo Color");
                        const Color newAlbedo = color_picker_widget("Albedo Color Picker", material.m_albedo);

                        if(material.m_albedo != newAlbedo) {
                            material.m_albedo = newAlbedo;
                            scenePtr->mark_as_modified();
                        }

                        ImGui::Text("Emission Color");
                        const Color newEmissionColor = color_picker_widget("Emission Color Picker", material.m_emissionColor);

                        if(material.m_emissionColor != newEmissionColor) {
                            material.m_emissionColor = newEmissionColor;
                            scenePtr->mark_as_modified();
                        }

                        ImGui::Text("Emission Strength");
                        f32 emissionStrength = material.m_emissionStrength;
                        ImGui::DragFloat("## Emission Strength", &emissionStrength, 0.005f, 0.0f, std::numeric_limits<f32>::max());

                        if(material.m_emissionStrength != emissionStrength) {
                            material.m_emissionStrength = emissionStrength;
                            scenePtr->mark_as_modified();
                        }

                        ImGui::Text("Metallic");
                        f32 metallic = material.m_metallic;
                        ImGui::DragFloat("## Metallic", &metallic, 0.005f, 0.0f, 1.0f);

                        if(material.m_metallic != metallic) {
                            material.m_metallic = metallic;
                            scenePtr->mark_as_modified();
                        }
                    }

                    ImGui::TreePop();
                }

                CommonUIWindow::end_window();
            }
    };
}

#endif