#include "GUI.h"
#include "SIG_Utilities.h"
#include "Input.h"
#include "Boolean.h"
#include "Entry.h"
#include "NumberEntry.h"
#include "Slider.h"
#include "Label.h"
#include "Button.h"
#include "Box.h"
#include "SIG_OpenGL.h"
#include <algorithm>

sig::Texture2D* sig::GUI::DEFAULT_FONT = 0;
int sig::GUI::zindex = 0;

sig::GUI::GUI()
{
	m_clickHandle = m_keyHandle = false;
	m_focused = nullptr;
	handled = false;
	DEFAULT_FONT = new Texture2D("mono.png");
	m_currentBox = nullptr;
}

sig::GUI::~GUI()
{
	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget* w = (*it);

		SIG_FREE(w);
	}
	SIG_FREE(DEFAULT_FONT);
}

void sig::GUI::Initialize()
{
	
}

void sig::GUI::Update(float dt)
{
	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget* w = (*it);
		w->Update(dt);
	}
}

void sig::GUI::Render()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
	
	glLoadIdentity();
	std::stable_sort(m_widgets.begin(), m_widgets.end(), WidgetComparator);
	
	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget* w = (*it);
		w->Render();
	}
	
	glPopMatrix();
}

sig::Widget* sig::GUI::CreateWidget(const string &name, Widget *w)
{
	w->m_guiManager = this;
	w->m_name = name;
	w->m_zindex = zindex;
	
	m_widgets.push_back(w);

	zindex++;
	return w;
}

sig::Label* sig::GUI::CreateLabel(const string &name, const string &text, float font_scale, float char_spacing)
{
	Label *l = new Label();
	l->SetText(text);
	l->SetCharSpacing(char_spacing);
	l->SetFontScale(font_scale);

	return (Label*)CreateWidget(name, (Widget*) l);
}

sig::Button* sig::GUI::CreateButton(const string &name, const string& text, float font_scale, float char_spacing)
{
	Button *btn = new Button();
	btn->SetText(text);
	btn->SetCharSpacing(char_spacing);
	btn->SetFontScale(font_scale);

	return (Button*)CreateWidget(name, (Widget*) btn);
}

sig::Box* sig::GUI::CreateBox(const string &name)
{
	Box* b = new Box();
	return (Box*)CreateWidget(name, (Widget*) b);
}

sig::Entry* sig::GUI::CreateEntry(const string &name, const string& text, float font_scale, float char_spacing)
{
	Entry *e = new Entry();
	e->SetText(text);
	e->SetCharSpacing(char_spacing);
	e->SetFontScale(font_scale);

	return (Entry*)CreateWidget(name, (Widget*) e);
}

sig::NumberEntry* sig::GUI::CreateNumberEntry(const string &name, float font_scale, float char_spacing)
{
	NumberEntry *e = new NumberEntry();
	e->SetCharSpacing(char_spacing);
	e->SetFontScale(font_scale);

	return (NumberEntry*)CreateWidget(name, (Widget*) e);
}

sig::Boolean* sig::GUI::CreateBoolean(const string& name, float font_scale, float char_spacing)
{
	Boolean* b = new Boolean();
	b->SetCharSpacing(char_spacing);
	b->SetFontScale(font_scale);
	
	return (Boolean*)CreateWidget(name, (Widget*) b);
}

sig::Slider* sig::GUI::CreateSlider(const string& name, float _min, float _max, float font_scale, float char_spacing)
{
	Slider* s = new Slider();
	s->SetCharSpacing(char_spacing);
	s->SetFontScale(font_scale);
	s->SetMin(_min);
	s->SetMax(_max);
	
	return (Slider*)CreateWidget(name, (Widget*) s);
}

sig::Box* sig::GUI::BeginBox(const Rect &rect)
{
	m_currentBox = CreateBox(string("_box") + ToString(pid));
	m_currentBox->m_zindex = 999+pid;
	m_currentBox->SetBounds(rect);
	
	pid++;
	return m_currentBox;
}

void sig::GUI::RangeProperty(const string& title, function<float()> getter, function<void(float)> setter, float _min, float _max)
{
	if (m_currentBox == nullptr) { return; }

	Slider *range = CreateSlider(string("_range") + ToString(pid) + string("_") + title, _min, _max);
	range->SetValue(getter ? getter() : 0);
	range->SetIncrement(0.05f);
	range->SetPrefix("");
	
	range->SetCallback([setter, range]() {
		if (setter) {
			setter(range->GetValue());
		}
	});
	
	Label *ltitle = CreateLabel("_label_title_" + ToString(pid) + title, title);
	ltitle->Fit();

	m_currentBox->AddWidget(ltitle);
	m_currentBox->AddWidget(range);
}

void sig::GUI::VectorProperty(const string& title, function<Vector2()> getter, function<void(Vector2)> setter)
{
	if (m_currentBox == nullptr) { return; }
	
	NumberEntry *x = CreateNumberEntry(string("_number_entry_x_") + ToString(pid) + title);
	x->SetWidth(60);
	NumberEntry *y = CreateNumberEntry(string("_number_entry_y_") + ToString(pid) + title);
	y->SetWidth(60);
	
	x->SetCallback([getter, setter](float val) {
		if (setter) {
			Vector2 v = getter ? getter() : Vector2(0.0f);
			v.SetX(val);
			setter(v);
		}
	});
	y->SetCallback([getter, setter](float val) {
		if (setter) {
			Vector2 v = getter ? getter() : Vector2(0.0f);
			v.SetY(val);
			setter(v);
		}
	});
	
	Label *ltitle = CreateLabel("_label_title_" + ToString(pid) + title, title);
	ltitle->Fit();
	
	Box *inner = CreateBox("_box_" + ToString(pid) + title);
	inner->SetPadding(2);
	inner->SetOrientation(Box::ORIENTATION_HORIZONTAL);
	
	Rect b = inner->GetBounds();
	b.height = 22;
	inner->SetBounds(b);
	
	inner->AddWidget(x);
	inner->AddWidget(y);
	
	m_currentBox->AddWidget(ltitle);
	m_currentBox->AddWidget(inner);
}

void sig::GUI::EndBox()
{
	m_currentBox = nullptr;
}

bool sig::GUI::WidgetComparator(Widget* a, Widget* b)
{
	return a->m_zindex > b->m_zindex;
}
