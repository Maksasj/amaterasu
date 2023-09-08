#ifndef _RENDERING_PROFILE_EDITOR_UI_WINDOW_H_
#define _RENDERING_PROFILE_EDITOR_UI_WINDOW_H_

#include "common/common_ui_window.h"
#include "renderer/ray_renderer.h"
#include "renderer/ray_renderer_present_mode.h"

namespace amts {
    class RenderingProfileEditorUIWindow : public CommonUIWindow {
        private:
            std::vector<RayRendererProfile> m_profiles;
            u64 m_selectedProfile;

        public:
            RenderingProfileEditorUIWindow() 
                : CommonUIWindow("Rendering Profile"),
                m_selectedProfile(0) {

                m_profiles.push_back(RayRendererProfile::create_default_renderer_profile());
            }   

            ~RenderingProfileEditorUIWindow() override {
                
            }

            void run(std::unique_ptr<Scene>& scene, std::unique_ptr<RayRenderer>& rayRenderer) {
                if(!CommonUIWindow::start_window()) return;

                if(ImGui::TreeNodeEx("Profiles", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if(ImGui::BeginCombo("Profiles ", m_profiles[m_selectedProfile].m_name.c_str())) {
                        for(u64 i = 0; i < m_profiles.size(); ++i) {
                            const bool isSelected = (m_selectedProfile == i);

                            std::stringstream ss;
                            ss << m_profiles[i].m_name << " ## " << (void*) &m_profiles[i];
                            const std::string profileName = ss.str();

                            if(ImGui::Selectable(profileName.c_str(), isSelected))
                                m_selectedProfile = i;

                            if(isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    if(ImGui::Button("Add profile")) {
                        m_profiles.push_back(RayRendererProfile::create_default_renderer_profile());
                    } ImGui::SameLine();
                        
                    if(ImGui::Button("Delete profile")) {
                        if(m_profiles.size() > 1) {
                            if(m_selectedProfile < m_profiles.size())
                                m_profiles.erase(m_profiles.begin() + m_selectedProfile);

                            if(m_selectedProfile > 0)
                                --m_selectedProfile;
                        }
                    } ImGui::SameLine();
                        
                    if(ImGui::Button("Copy profile")) {
                        if(m_selectedProfile < m_profiles.size())
                            m_profiles.push_back(m_profiles[m_selectedProfile]);
                    }
                    
                    ImGui::TreePop();
                }

                if(m_selectedProfile >= m_profiles.size()) 
                    return;

                ImGui::Separator();

                RayRendererProfile& profile = m_profiles[m_selectedProfile];

                char buffer[256] = { 0 };
                profile.m_name.copy(buffer,  profile.m_name.size());
                ImGui::InputText("Name", buffer, 256);
                profile.m_name = std::string(buffer);

                if(ImGui::TreeNodeEx("Rendering", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputScalar("Max Bounces", ImGuiDataType_U64, &profile.m_maxBounces);
                    ImGui::InputFloat("Reflection Normal Offset", &profile.m_reflectionNormalOffset, 0.00001f);
                    ImGui::Checkbox("Enable Reflection", &profile.m_enableReflection);
                    ImGui::Checkbox("Enable Emmission", &profile.m_enableEmission);
                    ImGui::Checkbox("Enable Skybox", &profile.m_enableSkybox);
                    ImGui::Checkbox("Enable Ray Direction Caching", &profile.m_enableRayDirectionCaching);
                    ImGui::TreePop();
                }

                if(ImGui::TreeNodeEx("Ray Marching", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputScalar("Max Steps", ImGuiDataType_U64, &profile.m_rayMarchingMaxSteps);
                    ImGui::InputScalar("Max Distance", ImGuiDataType_U64, &profile.m_rayMarchingMaxDist);
                    ImGui::InputFloat("Epsilon", &profile.m_rayMarchingEpsilon);
                    ImGui::TreePop();
                }

                if(ImGui::TreeNodeEx("Anti Alliasing", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Text("Anti Alliasing");
                    ImGui::Checkbox("Enable Anti Aliasing", &profile.m_enableAntiAliasing);
                    ImGui::InputFloat("Aliasing Factor", &profile.m_antiAliasingFactor);
                    ImGui::TreePop();
                }

                if(ImGui::TreeNodeEx("Present", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if(ImGui::BeginCombo("Present Mode ", present_mode_to_string(profile.m_presentMode).c_str())) {
                        {
                            const bool isModeSelected = profile.m_presentMode == IMMEDIATE_ACCUMULATION;
                            if(ImGui::Selectable(present_mode_to_string(IMMEDIATE_ACCUMULATION).c_str(), isModeSelected))
                                profile.m_presentMode = IMMEDIATE_ACCUMULATION;

                            if(isModeSelected) ImGui::SetItemDefaultFocus();
                        }

                        {
                            const bool isModeSelected = profile.m_presentMode == TILED_ACCUMULATION;
                            if(ImGui::Selectable(present_mode_to_string(TILED_ACCUMULATION).c_str(), isModeSelected)) 
                                profile.m_presentMode = TILED_ACCUMULATION;

                            if(isModeSelected) ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }

                    ImGui::Checkbox("Enable Multi Threading", &profile.m_multiThreading);
                    ImGui::Checkbox("Enable Immediate Accumulation", &profile.m_immediateAccumulation);
                    ImGui::InputScalar("Samples Per Pixel", ImGuiDataType_U64, &profile.m_samplesPerPixel);
                    ImGui::InputScalar("Tiles in Row", ImGuiDataType_U64, &profile.m_tilesInRow);
                    ImGui::InputScalar("Tiles in Collum", ImGuiDataType_U64, &profile.m_tilesInCollum);

                    ImGui::TreePop();
                }

                if(ImGui::Button("Apply Profile")) {
                    while (scene->is_rendering()) { std::this_thread::sleep_for(1ms); }

                    rayRenderer->apply_profile(profile);
                }

                CommonUIWindow::end_window();
            }
    };
}

#endif