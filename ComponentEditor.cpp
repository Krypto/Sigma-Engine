#include "ComponentEditor.h"

void sig::ComponentEditor::GenerateEditor(Component *comp)
{
	bool enabled = comp->IsEnabled();
	if (ImGui::Checkbox("Enabled", &enabled)) {
		comp->SetEnabled(enabled);
	}
}
