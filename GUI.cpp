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
#include "Image.h"
#include "ListView.h"
#include "SIG_OpenGL.h"

#include <algorithm>

sig::Texture2D* sig::GUI::DEFAULT_FONT = nullptr;
int sig::GUI::zindex = 0;

static bool WidgetComparator(sig::Widget* a, sig::Widget* b)
{
	return a->GetZIndex() < b->GetZIndex();
}

sig::GUI::GUI()
{
	m_clickHandle = m_keyHandle = handled = false;
	m_focused = nullptr;
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
	glPushMatrix();
	
	glLoadIdentity();
	std::stable_sort(m_widgets.begin(), m_widgets.end(), WidgetComparator);

	for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
		Widget* w = (*it);

		// TODO: Move this to the GFX class
		Rect b = w->GetBounds();
		int rect[4];
		int viewport[4];

		glGetIntegerv(GL_SCISSOR_BOX, rect);
		glGetIntegerv(GL_VIEWPORT, viewport);

		glEnable(GL_SCISSOR_TEST);
		glScissor(b.x, viewport[3] - b.y - b.height, b.width, b.height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_SCISSOR_BIT);

		w->Render();
		glScissor(rect[0], rect[1], rect[2], rect[3]);

		glDisable(GL_SCISSOR_TEST);
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

		if (m_currentBox != nullptr) {
			m_currentBox->AddWidget(w);
		}

		zindex++;
	}
}

sig::Label *sig::GUI::AddLabel(const string &text)
{
	if (text.empty()) { return nullptr; }

	Label *lbl = new Label();
	lbl->SetText(text);
	lbl->Fit(-4);

	AddWidget(dynamic_cast<Widget*>(lbl));

	return lbl;
}

sig::Button *sig::GUI::AddButton(const string &text)
{
	Button *btn = new Button();
	btn->SetText(text);
	btn->Fit(0);

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
	Slider *sld = new Slider();
	sld->SetIncrement(1.0f);
	sld->SetValue(*value);
	sld->SetMin(vmin);
	sld->SetMax(vmax);

	sld->SetCallback([sld, value]() {
		*value = sld->GetValue();
	});

	AddWidget<Slider>(text, sld);
}

void sig::GUI::AddParam(const string &text, float *value, float vmin, float vmax, float increment)
{
	Slider *sld = new Slider();
	sld->SetIncrement(increment);
	sld->SetValue(*value);
	sld->SetMin(vmin);
	sld->SetMax(vmax);

	sld->SetCallback([sld, value]() {
		*value = sld->GetValue();
	});

	AddWidget<Slider>(text, sld);
}

void sig::GUI::AddParam(const string &text, sig::Color *value)
{
	Widget *wcol = new Widget();
	wcol->SetHeight(6);
	wcol->SetBackColor((*value));

	AddLabel(text);

	BeginBox(Rect(0, 0, 1, 1));

	Slider *sr = new Slider();
	sr->SetMin(0.0f);
	sr->SetMax(1.0f);
	sr->SetIncrement(0.05f);
	sr->SetValue((*value).r);
	sr->SetCallback([wcol, sr, value]() {
		(*value).r= sr->GetValue();
		wcol->SetBackColor(*value);
	});
	AddWidget<Slider>("R", sr);

	Slider *sg = new Slider();
	sg->SetMin(0.0f);
	sg->SetMax(1.0f);
	sg->SetIncrement(0.05f);
	sg->SetValue((*value).g);
	sg->SetCallback([wcol, sg, value]() {
		(*value).g = sg->GetValue();
		wcol->SetBackColor(*value);
	});
	AddWidget<Slider>("G", sg);

	Slider *sb = new Slider();
	sb->SetMin(0.0f);
	sb->SetMax(1.0f);
	sb->SetIncrement(0.05f);
	sb->SetValue((*value).b);
	sb->SetCallback([wcol, sb, value]() {
		(*value).b = sb->GetValue();
		wcol->SetBackColor(*value);
	});
	AddWidget<Slider>("B", sb);

	Slider *sa = new Slider();
	sa->SetMin(0.0f);
	sa->SetMax(1.0f);
	sa->SetIncrement(0.05f);
	sa->SetValue((*value).a);
	sa->SetCallback([wcol, sa, value]() {
		(*value).a = sa->GetValue();
		wcol->SetBackColor(*value);
	});
	AddWidget<Slider>("A", sa);

	AddWidget(wcol);

	EndBox();
}

void sig::GUI::AddParam(const string &text, string *value, bool masked)
{
	Entry *txt = new Entry();
	txt->SetMasked(masked);
	txt->SetText(*value);
	txt->SetCallback([txt, value]() {
		(*value) = txt->GetText();
	});
	txt->Fit(0);

	AddWidget<Entry>(text, txt);
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
	NumberEntry *ent = new NumberEntry();
	ent->SetValue(*value);
	ent->Fit(0);

	ent->SetValueCallback([ent, value]() {
		(*value) = ent->GetValue();
	});

	AddWidget<NumberEntry>(text, ent);
}

void sig::GUI::AddParam(const string &text, sig::math::Vector2 *value)
{
	AddLabel(text);

	BeginBox(Rect(0, 0, 0, 0));

	NumberEntry *x = new NumberEntry();
	x->SetValue((*value).X());
	x->Fit(0);
	x->SetValueCallback([x, value]() {
		(*value).SetX(x->GetValue());
	});
	AddWidget<NumberEntry>("x", x);

	NumberEntry *y = new NumberEntry();
	y->SetValue((*value).Y());
	y->Fit(0);
	y->SetValueCallback([y, value]() {
		(*value).SetY(y->GetValue());
	});
	AddWidget<NumberEntry>("y", y);

	EndBox();
}

sig::Image *sig::GUI::AddParam(const string &text, sig::Texture2D *img)
{
	AddLabel(text);

	Image *tex_view = new Image();
	tex_view->SetImage(img);

	AddWidget(dynamic_cast<Widget*>(tex_view));

	return tex_view;
}

sig::ListView* sig::GUI::AddParam(const string &text, int *selected)
{
	AddLabel(text);

	ListView *lst = new ListView();
	lst->SetCallback([selected, lst]() {
		(*selected) = lst->GetSelected();
	});

	AddWidget(dynamic_cast<Widget*>(lst));
	return lst;
}

void sig::GUI::BeginBox(const sig::Rect &bounds)
{
	Box *b = new Box();
	b->m_zindex = 999 + box_id;
	b->SetBounds(bounds);

	m_boxStack.push(b);

	AddWidget(dynamic_cast<Widget*>(b));

	m_currentBox = b;
	box_id++;
}

void sig::GUI::EndBox()
{
	m_boxStack.pop();
	if (m_boxStack.size() > 0) {
		m_currentBox = m_boxStack.top();
	} else {
		m_currentBox = nullptr;
	}
}

sig::Rect sig::GUI::GetWindowDimensions()
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	return Rect(0, 0, vp[2], vp[3]);
}

template <typename WidgetType>
void sig::GUI::AddWidget(const string &text, WidgetType *widget)
{
	if (widget == nullptr) { return; }

	BeginBox(Rect(0, 0, 0, 18));
	GetCurrentBox()->SetPadding(1);
	GetCurrentBox()->SetDrawBackground(false);
	GetCurrentBox()->SetFitWidgets(true);
	GetCurrentBox()->SetOrientation(Box::ORIENTATION_HORIZONTAL);

	AddLabel(text)->SetAlign(Label::ALIGN_RIGHT);
	AddWidget(dynamic_cast<Widget*>(widget));

	EndBox();
}
