#include "AnimatedSpriteEditor.h"
#include "SIG_Utilities.h"
#include "imgui_internal.h"
#include <cstdlib>

void sig::AnimatedSpriteEditor::GenerateEditor(Component* comp)
{
	AnimatedSprite *ani = (AnimatedSprite*) comp;
	
	bool _static = ani->IsStatic();
	if (ImGui::Checkbox("Static", &_static)) {
		ani->SetStatic(_static);
	}
	
	if (_static) {
		int currentFrame = ani->GetFrame();
		int maxFrame = 0;
		if (ani->GetCurrent() != nullptr) {
			maxFrame = ani->GetCurrent()->frames.size();
			if (maxFrame == 0) {
				maxFrame = (ani->GetRows() * ani->GetCols())-1;
			}
		}
		if (ImGui::SliderInt("Current Frame", &currentFrame, 0, maxFrame)) {
			ani->SetFrame(currentFrame);
		}
	}
	
	int row_cols[2] = { ani->GetRows(), ani->GetCols() };
	if (ImGui::InputInt2("Rows/Columns", row_cols)) {
		ani->Configure(row_cols[0], row_cols[1]);
	}
	
	vector<const char*> ani_names;
	auto animations = ani->GetAnimations();
	for (auto it = animations.begin(); it != animations.end(); ++it) {
		ani_names.push_back((*it)->name.c_str());
	}
	
	static int selected_animation = 0;
	if (ImGui::Combo("Current Animation", &selected_animation, ani_names.data(), ani_names.size())) {
		ani->SetAnimation(string(ani_names[selected_animation]));
	}
	
	if (ImGui::Button("New Animation")) {
		ImGui::OpenPopup("New Animation");
	}
	
	if (ImGui::BeginPopupModal("New Animation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char text[128] = "";
		ImGui::InputText("Animation Name", text, IM_ARRAYSIZE(text));
		ImGui::Separator();
		
		if (ImGui::Button("Ok")) {
			ani->AddAnimation(string(text), 0, false, {});
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	
	if (selected_animation > -1) {
		Animation *cani = ani->GetCurrent();
		if (cani != nullptr) {
			ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 2.0f);
            ImGui::BeginChild("Sub2", ImVec2(0,0), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
			string title = string("Animation (") + cani->name + string(")");
			ImGui::Text("%s", title.c_str());
            
			bool _loop = cani->loop;
			if (ImGui::Checkbox("Loop", &_loop)) {
				cani->loop = _loop;
			}
			
			float _speed = cani->speed;
			if (ImGui::InputFloat("Speed", &_speed, 0.01, 0.5f, 3)) {
				cani->speed = _speed;
			}
			
			if (ImGui::Button("Set Frames")) {
				ImGui::OpenPopup("Set Frames");
			}
						
			if (ImGui::BeginPopupModal("Set Frames")) {
				static int fsize = 0;
				static int *v = (int*) std::calloc(fsize, sizeof(int));
				
				ImGui::InputIntN("Frames", v, fsize, ImGuiInputTextFlags_CharsDecimal);
				ImGui::SameLine();
				if (ImGui::Button("+")) {
					fsize++;
				}
				ImGui::SameLine();
				if (ImGui::Button("-")) {
					fsize--;
					if (fsize <= 0) {
						fsize = 0;
					}
				}
		
				if (ImGui::Button("Ok")) {
					cani->frames.clear();
					for (int i = 0; i < fsize; i++) {
						cani->frames.push_back(v[i]);
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			
            ImGui::EndChild();
            ImGui::PopStyleVar();
			
		}
	}
}
