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

static bool WidgetComparator(sig::Widget* a, sig::Widget* b)
{
	return a->GetZIndex() < b->GetZIndex();
}

sig::GUI::GUI()
{
	m_clickHandle = m_keyHandle = false;
	m_focused = nullptr;
	handled = false;
	DEFAULT_FONT = nullptr;
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

void sig::GUI::AddWidget(Widget *w)
{	
	auto pos = std::find(m_widgets.begin(), m_widgets.end(), w);
	if (pos == m_widgets.end()) {
		w->m_guiManager = this;
		w->m_zindex = zindex;

		m_widgets.push_back(w);

		if (m_currentBox) {
			m_currentBox->AddWidget(w);
		}

		zindex++;
	}
}

void sig::GUI::AddLabel(const string &text)
{
	Label *lbl = new Label();
	lbl->SetText(text);
	lbl->Fit();

	AddWidget(dynamic_cast<Widget*>(lbl));
}

sig::Button *sig::GUI::AddButton(const string &text)
{
	Button *btn = new Button();
	btn->SetText(text);
	btn->Fit();

	AddWidget(dynamic_cast<Widget*>(btn));

	return btn;
}

void sig::GUI::AddSeparator()
{
	//TODO: Create proper Separator class
	Widget *w = new Widget();
	Rect b = w->GetBounds();
	b.height = b.width = 1;
	w->SetBounds(b);

	AddWidget(w);
}

void sig::GUI::AddParam(const string &text, int *value, int vmin, int vmax)
{
	AddLabel(text);

	Slider *sld = new Slider();
	sld->SetIncrement(1.0f);
	sld->SetValue(*value);
	sld->SetMin(vmin);
	sld->SetMax(vmax);

	sld->SetCallback([sld, value]() {
		*value = sld->GetValue();
	});

	AddWidget(dynamic_cast<Widget*>(sld));
}

void sig::GUI::AddParam(const string &text, float *value, float vmin, float vmax, float increment)
{
	AddLabel(text);

	Slider *sld = new Slider();
	sld->SetIncrement(increment);
	sld->SetValue(*value);
	sld->SetMin(vmin);
	sld->SetMax(vmax);

	sld->SetCallback([sld, value]() {
		*value = sld->GetValue();
	});

	AddWidget(dynamic_cast<Widget*>(sld));
}

void sig::GUI::AddParam(const string &text, sig::Color *value, const sig::Color &col)
{
	AddLabel(text);

	Color v = *value;

	Slider *sr = new Slider();
	sr->SetMin(0.0f);
	sr->SetMax(1.0f);
	sr->SetIncrement(0.1f);
	sr->SetValue(v.r);
	sr->SetCallback([&v, sr, value]() {
		v.r = sr->GetValue();
		*value = v;
	});

	Slider *sg = new Slider();
	sg->SetMin(0.0f);
	sg->SetMax(1.0f);
	sg->SetIncrement(0.1f);
	sg->SetValue(v.g);
	sr->SetCallback([&v, sg, value]() {
		v.g = sg->GetValue();
		*value = v;
	});

	Slider *sb = new Slider();
	sb->SetMin(0.0f);
	sb->SetMax(1.0f);
	sb->SetIncrement(0.1f);
	sb->SetValue(v.b);
	sr->SetCallback([&v, sb, value]() {
		v.b = sb->GetValue();
		*value = v;
	});

	Slider *sa = new Slider();
	sa->SetMin(0.0f);
	sa->SetMax(1.0f);
	sa->SetIncrement(0.1f);
	sa->SetValue(v.a);
	sr->SetCallback([&v, sa, value]() {
		v.a = sa->GetValue();
		*value = v;
	});

	Box *colBox = new Box();
	colBox->AddWidget(sr);
	colBox->AddWidget(sg);
	colBox->AddWidget(sb);
	colBox->AddWidget(sa);

	AddWidget(dynamic_cast<Widget*>(colBox));
}

void sig::GUI::AddParam(const string &text, string *value, bool masked)
{
	AddLabel(text);

	Entry *txt = new Entry();
	txt->SetMasked(masked);
	txt->SetText(*value);
	txt->SetCallback([txt, value]() {
		*value = txt->GetText();
	});

	AddWidget(dynamic_cast<Widget*>(txt));
}

void sig::GUI::AddParam(const string &text, bool *value)
{
	Boolean *bol = new Boolean();
	bol->SetSelected(*value);
	bol->SetText(text);
	bol->Fit();

	bol->SetCallback([bol, value]() {
		*value = bol->IsSelected();
	});

	AddWidget(dynamic_cast<Widget*>(bol));
}

void sig::GUI::AddParam(const string &text, float *value)
{
	AddLabel(text);

	NumberEntry *ent = new NumberEntry();
	ent->SetValue(*value);
	ent->Fit();

	ent->SetValueCallback([ent, value]() {
		*value = ent->GetValue();
	});

	AddWidget(dynamic_cast<Widget*>(ent));
}

void sig::GUI::AddParam(const string &text, sig::math::Vector2 *value)
{
	Vector2 val = *value;

	AddLabel(text);

	BeginBox(Rect(0, 0, 0, 0));
	GetCurrentBox()->SetTabWidgets(true);

	AddLabel("x");
	NumberEntry *x = new NumberEntry();
	x->SetValue(val.X());
	x->Fit();
	x->SetValueCallback([x, value, &val]() {
		val.SetX(x->GetValue());
		*value = val;
	});
	AddWidget(dynamic_cast<Widget*>(x));

	AddLabel("y");
	NumberEntry *y = new NumberEntry();
	y->SetValue(val.Y());
	y->Fit();
	y->SetValueCallback([y, value, &val]() {
		val.SetY(y->GetValue());
		*value = val;
	});
	AddWidget(dynamic_cast<Widget*>(y));

	EndBox();
}

void sig::GUI::BeginBox(const sig::Rect &bounds)
{
	Box *b = new Box();
	b->m_zindex = 999 + box_id;
	b->SetBounds(bounds);

	m_boxStack.push_back(b);

	AddWidget(dynamic_cast<Widget*>(b));

	m_currentBox = b;
	box_id++;
}

void sig::GUI::EndBox()
{
	if (m_boxStack.size() > 0) {
		m_boxStack.pop_back();
		m_currentBox = m_boxStack.back();
	} else {
		m_currentBox = nullptr;
	}
}
