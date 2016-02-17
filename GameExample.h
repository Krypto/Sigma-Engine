#ifndef GAME_EXAMPLE
#define GAME_EXAMPLE

#include "Sigma.h"
#include "Editor.h"
#include "ComponentEditor.h"
#include "SpriteEditor.h"
#include "AnimatedSpriteEditor.h"
#include "GUIGFX.h"
#include "imgui_internal.h"

using namespace sig;
using namespace math;

class GameExample : public BaseGame
{
public:
	GameExample() : BaseGame() {
		selectedNode = nullptr;
		NewResource("asteroid", "asteroid1.png", ResourceType::RESOURCE_TEXTURE);
	}
	
	void Initialize() {
		n = GetCurrentScene()->CreateNode("test");

		ast = new AnimatedSprite(GetResourceData(Texture2D*, "asteroid"), 1, 15);
		ast->AddAnimation("default", 0.05f, true, {});
		
		n->AddComponent(ast);
		
		GetCurrentScene()->AddChild(n);
	}
	
	void Update(float dt) {
		if (Input::GetMouseButtonUp(Input::MOUSE_LEFT_BUTTON)) {
			if (drag) {
				drag = false;
			}
		} else {
			if (drag) {
				Vector2 camPos = Vector2::Zero;
				if (GetCurrentScene()->GetCamera() != nullptr) {
					camPos = GetCurrentScene()->GetCamera()->GetOrigin() - GetCurrentScene()->GetCamera()->GetPosition();
				}
				selectedNode->SetPosition(Input::GetMousePosition() - camPos);
			}
		}
	}
	
	void Render() {
		if (hoveredNode != nullptr) {
			float w = hoveredNode->GetSize().X();
			float h = hoveredNode->GetSize().Y();
			
			glPushMatrix();
			glMultMatrixf(&hoveredNode->GetTransformMatrix()(0, 0));
			
			glColor4f(0.8f, 0.4f, 0.0f, 0.5f);
			glBegin(GL_QUADS);
			
			glVertex2f(0, 0);
			glVertex2f(w, 0);
			glVertex2f(w, h);
			glVertex2f(0, h);
			
			glEnd();
			
			glPopMatrix();
			
			hoveredNode = nullptr;
		}
		
		if (selectedNode != nullptr) {
			float w = selectedNode->GetSize().X();
			float h = selectedNode->GetSize().Y();
			
			glPushMatrix();
			glMultMatrixf(&selectedNode->GetTransformMatrix()(0, 0));
			
			glPushAttrib(GL_LINE_WIDTH | GL_POINT_SIZE);
			glLineWidth(2.0f);
			glColor4f(0.04f, 0.4f, 0.8f, 0.5f);
			glBegin(GL_LINE_LOOP);
			
			glVertex2f(0, 0);
			glVertex2f(w, 0);
			glVertex2f(w, h);
			glVertex2f(0, h);
			
			glEnd();
			
			glEnable(GL_POINT_SMOOTH);
			glPointSize(4.0f);
			glColor4f(0.8, 0.4f, 0.04f, 0.7f);
			glBegin(GL_POINTS);
			glVertex2f(selectedNode->GetOrigin().X() * w, selectedNode->GetOrigin().Y() * h);
			glEnd();
			
			glPopAttrib();
			glPopMatrix();
		}
	}
	
	void OnGUI() {
		static bool show_node_inspector = true;
		
		Vector2 vpos = Vector2::Zero;
		Vector2 vscl = Vector2::Zero;
		Vector2 vori = Vector2::Zero;
		float vrotRad = 0;
		string name = "null";
		static char *nname = "";
		
		if (selectedNode != nullptr) {
			vpos = selectedNode->GetPosition();
			vscl = selectedNode->GetScale();
			vori = selectedNode->GetOrigin();
			name = selectedNode->GetName();
			vrotRad = selectedNode->GetRotation();
			nname = (char*)selectedNode->GetName().c_str();
		}
		float pos[2] = { vpos.X(), vpos.Y() };
		float scal[2] = { vscl.X(), vscl.Y() };
		float ori[2] = { vori.X(), vori.Y() };
		
		/////  NODE PROPERTIES ///////////////////////////////////////////////
		
		if (show_node_inspector) {
			string propTitle = string("Node Inspector (") + name + string(")");	
			ImGui::Begin(propTitle.c_str(), &show_node_inspector);
			
			if (ImGui::InputText("Name", nname, IM_ARRAYSIZE(nname))) {
				if (selectedNode != nullptr) {
					selectedNode->SetName(string(nname));
				}
			}
			
			if (ImGui::CollapsingHeader("Transform")) {
				if (ImGui::InputFloat2("Position", pos, 2)) {
					if (selectedNode != nullptr) {
						selectedNode->SetPosition(Vector2(pos[0], pos[1]));
					}
				}
				
				if (ImGui::InputFloat2("Scale", scal, 2)) {
					if (selectedNode != nullptr) {
						selectedNode->SetScale(Vector2(scal[0], scal[1]));
					}
				}
				
				if (ImGui::InputFloat2("Origin", ori, 2)) {
					if (selectedNode != nullptr) {
						selectedNode->SetOrigin(Vector2(ori[0], ori[1]));
					}
				}
				
				if (ImGui::SliderAngle("Rotation", &vrotRad, 0.0f, 360.0f)) {
					if (selectedNode != nullptr) {
						selectedNode->SetRotation(vrotRad);
					}
				}
			}
			
			if (ImGui::CollapsingHeader("Components")) {
				if (selectedNode != nullptr) {
					auto comps = selectedNode->GetComponents();
					
					for (auto 	it = comps.begin();
								it != comps.end();
								++it)
					{
						Component *comp = *it;
						
						if (ImGui::TreeNode(comp->GetName().c_str())) {
							if (SIG_ISINSTANCE(Component*, comp)) {
								ComponentEditor compedit;
								compedit.GenerateEditor(comp);
							}
							if (SIG_ISINSTANCE(Sprite*, comp)) {
								SpriteEditor spredit;
								spredit.GenerateEditor(comp);
							}
							if (SIG_ISINSTANCE(AnimatedSprite*, comp)) {
								AnimatedSpriteEditor spredit;
								spredit.GenerateEditor(comp);
							}
							ImGui::TreePop();
						}
					}
					ImGui::SetScrollHere();
				}
			}
			
			ImGui::End();			
		}
	}
	
	void OnNodeHover(Node *node, float mx, float my) {
		hoveredNode = node;
		if (Input::GetMouseButtonDown(Input::MOUSE_RIGHT_BUTTON)) {
			if (!drag) {
				if (node != selectedNode) {
					selectedNode = node;
				}
			}
		} else if (Input::GetMouseButtonDown(Input::MOUSE_LEFT_BUTTON)) {
			if (!drag) {
				if (selectedNode != nullptr) {
					drag = true;
				}
			}
		}
	}
	
	bool drag;
	AnimatedSprite *ast;
	Node *n;
	
	Node *selectedNode, *hoveredNode;
};

#endif // GAME_EXAMPLE
