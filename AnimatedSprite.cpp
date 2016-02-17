#include "AnimatedSprite.h"
#include "SIG_Utilities.h"

sig::AnimatedSprite::AnimatedSprite()
	:	sig::Sprite()
{
	m_rows = 1;
	m_cols = 1;
	m_static = false;
	m_current = nullptr;
}

sig::AnimatedSprite::AnimatedSprite(Texture2D* texture, int rows, int cols)
	: AnimatedSprite()
{
	m_static = false;
	m_texture = texture;
	
	Configure(rows, cols);
}

void sig::AnimatedSprite::Configure(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
	
	int fcount = m_rows * m_cols;
	m_frames.clear();
	
	float fw = 1.0f / float(m_cols);
	float fh = 1.0f / float(m_rows);
	
	for (int i = 0; i < fcount; i++) {
		Rect f = Rect();		
		f.x = (i % m_cols) * fw;
		f.y = int(i / m_cols) * fh;
		f.width = fw;
		f.height = fh;
		
		m_frames.push_back(f);
	}
}

void sig::AnimatedSprite::Update(float dt)
{
	if (m_animations.size() == 0) { return; }
	if (m_current == nullptr) { return; }
	if (m_static) { return; }
	
	m_current->time += dt;
	
	int frame = 0;
	if (m_current->time >= m_current->speed) {
		m_current->time = 0;
		
		if (m_current->frames.size() > 0) {
			if (m_current->currentIndex++ >= m_current->frames.size()-1) {
				if (m_current->loop) {
					m_current->currentIndex = 0;
					m_current->stopped = false;
				} else {
					m_current->currentIndex = m_current->frames.size()-1;
					m_current->stopped = true;
				}
			}
		} else {
			u32 maxF = m_rows * m_cols;
			if (m_current->currentIndex++ >= maxF-1) {
				if (m_current->loop) {
					m_current->currentIndex = 0;
					m_current->stopped = false;
				} else {
					m_current->currentIndex = maxF-1;
					m_current->stopped = true;
				}
			}
		}
	}
	
	if (m_current->frames.size() > 0) {
		frame = m_current->frames[m_current->currentIndex];
	} else {
		frame = m_current->currentIndex;
	}
	
	SetUVRectangle(m_frames[frame]);
}

sig::Animation* sig::AnimatedSprite::AddAnimation(string name, float speed, bool loop, std::initializer_list<int> frames)
{
	Animation *ani = new Animation();
	ani->name = name;
	ani->frames = frames;
	ani->loop = loop;
	ani->speed = speed;
	ani->time = ani->speed;
	
	if (GetAnim(name) == nullptr) {
		m_animations.push_back(ani);
	} else {
		SIG_FREE(ani);
		return nullptr;
	}
	
	if (m_current == nullptr) {
		m_current = ani;
	}
	
	return ani;
}

void sig::AnimatedSprite::SetAnimation(string name)
{
	Animation* ani = GetAnim(name);
	if (ani != nullptr) {
		ani->currentIndex = 0;
		ani->stopped = false;
		ani->time = 0;
		
		m_current = ani;
	}
}

sig::Animation* sig::AnimatedSprite::GetAnim(string name)
{
	for (auto it = m_animations.begin(); it != m_animations.end(); ++it) {
		if ((*it)->name == name) {
			return (*it);
		}
	}
	return nullptr;
}

void sig::AnimatedSprite::SetFrame(int f)
{
	if (m_current != nullptr) {
		m_current->currentIndex = f;
		SetUVRectangle(m_frames[f]);
	}
}
