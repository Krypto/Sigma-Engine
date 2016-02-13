#include "SpriteEditor.h"

void sig::SpriteEditor::GenerateEditor(Component* comp)
{
	Sprite *spr = (Sprite*) comp;
	
	Color vcol = spr->GetColor();
	Rect vrec = spr->GetUVRectangle();
	
	float col[4] = { vcol.r, vcol.g, vcol.b, vcol.a };
	float rec[4] = { vrec.x, vrec.y, vrec.width, vrec.height };
	
	if (ImGui::InputFloat4("Tex Coord", rec, 2)) {
		spr->SetUVRectangle(Rect(rec[0], rec[1], rec[2], rec[3]));
	}
	
	if (ImGui::ColorEdit4("Color", col)) {
		spr->SetColor(Color(col[0], col[1], col[2], col[3]));
	}
	
	int tex = 0;
	if (spr->GetTexture() != nullptr) {
		tex = spr->GetTexture()->GetBindCode();
	}
	
	ImGui::Text("Texture");
	ImGui::Image(&tex, ImVec2(64, 64));
	if (ImGui::Button("...")) {
		
	}
}
