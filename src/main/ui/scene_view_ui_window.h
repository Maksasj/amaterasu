#ifndef _SCENE_VIEW_UI_WINDOW_H_
#define _SCENE_VIEW_UI_WINDOW_H_

#include <chrono>
#include <thread>

#include "common/common_ui_window.h"
#include "scene/scene.h"
#include "object_create_ui_window.h"

namespace amts {
    using namespace std::chrono_literals;

    class SceneViewUIWindow : public CommonUIWindow {
        private:
            u64 m_selectedObjectIndex;
            f32 m_visibleObjects;

            std::unique_ptr<ObjectCreateUIWindow> m_objectCreateUIWindow;

        public:
            SceneViewUIWindow() 
                : CommonUIWindow("Scene View"),
                  m_selectedObjectIndex(0),
                  m_visibleObjects(5.0f),
                  m_objectCreateUIWindow(nullptr) {
                
                m_objectCreateUIWindow = std::make_unique<ObjectCreateUIWindow>();
            }

            ~SceneViewUIWindow() override {
                
            }

            void run(std::unique_ptr<Scene>& scenePtr, std::unique_ptr<MaterialCollection>& materials) {
                if(!CommonUIWindow::start_window()) return;
                
                Scene& scene = *scenePtr;
                std::vector<std::unique_ptr<Object>>& objects = scene.m_objects;

                m_objectCreateUIWindow->run(materials);

                if(m_objectCreateUIWindow->have_object_to_pop()) {
                    while (scenePtr->is_rendering()) { std::this_thread::sleep_for(1ms); } // Todo, move this code somewhere else
                    
                    objects.emplace_back(m_objectCreateUIWindow->pop_top_object());
                    scene.mark_as_modified();
                }

                if(ImGui::TreeNodeEx("Objects", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if(ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, m_visibleObjects * ImGui::GetTextLineHeightWithSpacing()))) {
                        for(u64 i = 0; i < objects.size(); ++i) {
                            const bool isSelected = (m_selectedObjectIndex == i);

                            std::unique_ptr<Object>& object = objects[i];

                            std::stringstream ss;
                            ss << object->m_name << "## " << (void*) object.get();

                            if(ImGui::Selectable(ss.str().c_str(), isSelected))
                                m_selectedObjectIndex = i;

                            if(isSelected)
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndListBox();
                    }

                    ImGui::TreePop();
                }

                if(ImGui::TreeNodeEx("Actions", ImGuiTreeNodeFlags_DefaultOpen)) {
                        if(ImGui::Button("Add object")) {
                            if(!m_objectCreateUIWindow->is_open()) 
                                m_objectCreateUIWindow->force_open();
                            else
                                m_objectCreateUIWindow->force_close();
                        } ImGui::SameLine();
                        
                        if(ImGui::Button("Delete object")) {
                            while (scenePtr->is_rendering()) { std::this_thread::sleep_for(1ms); } // Todo, move this code somewhere else

                            if(m_selectedObjectIndex < objects.size())
                                objects.erase(objects.begin() + m_selectedObjectIndex);

                            if(m_selectedObjectIndex > 0)
                                --m_selectedObjectIndex;

                            scene.mark_as_modified();
                        } ImGui::SameLine();
                        
                        if(ImGui::Button("Copy object")) {
                            while (scenePtr->is_rendering()) { std::this_thread::sleep_for(1ms); } // Todo, move this code somewhere else
                            
                            if(m_selectedObjectIndex < objects.size()) {
                                objects.emplace_back(objects[m_selectedObjectIndex]->clone());
                            }

                            scene.mark_as_modified();
                        }

                        ImGui::TreePop();
                    }
                    ImGui::Separator();

                    if(ImGui::TreeNodeEx("Sort", ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::Text("Sorting criteria ");
                        ImGui::SameLine();
                        /*
                        if(ImGui::BeginCombo("## sorting criteria", _tileGroupSorters[_selectedSortingAlgorithm]._label.c_str(), ImGuiComboFlags_None)) {
                            for (i32 n = 0; n < _tileGroupSorters.size(); n++) {
                                const bool isSelected = (_selectedSortingAlgorithm == n);

                                if(ImGui::Selectable(_tileGroupSorters[n]._label.c_str(), isSelected))
                                    _selectedSortingAlgorithm = n;

                                if(isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                        */

                        ImGui::Text("Reverse sort");
                        ImGui::SameLine();

                        /*
                        ImGui::Checkbox("## reverse sort checkbox", &_reverseSort);

                        if(ImGui::Button("Sort tile groups"))
                            _tileGroupSorters[_selectedSortingAlgorithm]._sorter->sort(levelData._tileGroups, _reverseSort);
                        */

                        ImGui::TreePop();
                    }

                CommonUIWindow::end_window();
            }
    };
}

#endif