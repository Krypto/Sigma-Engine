#include "AnimatedSprite.h"
#include "SIG_Utilities.h"

sig::AnimatedSprite::AnimatedSprite()
	:	sig::Sprite()
{
	m_rows = 1;
	m_cols = 1;
	m_static = false;
	m_current = -1;
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
	if (m_current < 0) { return; }
	if (m_static) { return; }
	
	Animation *current = GetCurrent();

	current->time += dt;
	
	int frame = 0;
	if (current->time >= current->speed) {
		current->time = 0;
		
		if (current->frames.size() > 0) {
			if (current->currentIndex++ >= current->frames.size()-1) {
				if (current->loop) {
					current->currentIndex = 0;
					current->stopped = false;
				} else {
					current->currentIndex = current->frames.size()-1;
					current->stopped = true;
				}
			}
		} else {
			u32 maxF = m_rows * m_cols;
			if (current->currentIndex++ >= maxF-1) {
				if (current->loop) {
					current->currentIndex = 0;
					current->stopped = false;
				} else {
					current->currentIndex = maxF-1;
					current->stopped = true;
				}
			}
		}
	}
	
	if (current->frames.size() > 0) {
		frame = current->frames[current->currentIndex];
	} else {
		frame = current->currentIndex;
	}
	
	SetUVRectangle(m_frames[frame]);
}

sig::AnimatedSprite *sig::AnimatedSprite::GetInstance(Node* owner)
{
	AnimatedSprite *inst = dynamic_cast<AnimatedSprite*>(Sprite::GetInstance(owner));
	inst->m_current = m_current;
	inst->Configure(m_rows, m_cols);
	inst->m_static = m_static;

	SIG_FOREACH(it, m_animations)
	{
		Animation *anim = *it;
		Animation *new_anim = new Animation();
		new_anim->currentIndex = anim->currentIndex;
		new_anim->frames = anim->frames;
		new_anim->loop = anim->loop;
		new_anim->name = anim->name;
		new_anim->speed = anim->speed;
		new_anim->stopped = anim->stopped;
		new_anim->time = anim->time;

		inst->m_animations.push_back(new_anim);
	}

	return inst;
}

sig::Animation* sig::AnimatedSprite::AddAnimation(string name, float speed, bool loop, std::initializer_list<int> frames)
{
	Animation *ani = new Animation();
	ani->name = name;
	ani->frames = frames;
	ani->loop = loop;
	ani->speed = speed;
	ani->time = ani->speed;
	
	if (GetAnim(name) <= -1) {
		m_animations.push_back(ani);
	} else {
		SIG_FREE(ani);
		return nullptr;
	}
	
	if (m_current <= -1) {
		m_current = 0;
	}
	
	return ani;
}

void sig::AnimatedSprite::SetAnimation(string name)
{
	int ani = GetAnim(name);
	if (ani >= 0) {
		m_current = ani;

		GetCurrent()->currentIndex = 0;
		GetCurrent()->stopped = false;
		GetCurrent()->time = 0;
	}
}

int sig::AnimatedSprite::GetAnim(string name)
{
	int index = 0;
	SIG_FOREACH(it, m_animations)
	{
		if ((*it)->name == name) {
			return index;
		}
		index++;
	}
	return -1;
}

void sig::AnimatedSprite::SetFrame(int f)
{
	if (m_current >= 0) {
		GetCurrent()->currentIndex = f;
		SetUVRectangle(m_frames[f]);
	}
}
