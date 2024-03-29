#include "ParticleEmitter.h"
#include "Node.h"
#include "SpriteBatch.h"
#include "SIG_Utilities.h"

#include <cmath>
#include <cstdlib>

sig::ParticleEmitter::ParticleEmitter(u32 max_particles)
	:	m_maxParticles(max_particles),
		m_liveParticles(0),
		m_startColor(Color::WHITE),
		m_endColor(Color::BLACK),
		m_particleSpread(0.5f),
		m_particleLife(2.0f),
		m_particleSpeed(200.0f),
		m_particleAngularVelocity(0),
		m_particleStartSize(1.0f),
		m_particleEndSize(0.0f),
		m_emitRate(0.08f),
		m_texture(0),
		m_particleBlend(BlendMode::NORMAL),
		m_particleAccel(Vector2::Zero),
		m_enabled(true)
{
	m_time = 0.0f;
}

void sig::ParticleEmitter::Initialize()
{
	for (u32 i = 0; i < m_maxParticles; i++) {
		m_particlePool.push_back(new Particle());
	}
	m_liveParticles = 0;
}

void sig::ParticleEmitter::Update(float dt)
{
	if (m_enabled) {
		m_time += dt;
		if (m_time >= m_emitRate) {
			EmitParticle();
			m_time = 0.0f;
		}
	}

	SIG_FOREACH(it, m_particlePool)
	{
		Particle *current = (*it);
		if (!current->is_dead) {
			current->Update(dt);
		} else { continue; }
	}
}

void sig::ParticleEmitter::Render(sig::SpriteBatch *batch)
{
	if (batch == nullptr) { return; }
	if (batch->IsDrawing()) {
		SIG_FOREACH(it, m_particlePool)
		{
			Particle *current = (*it);
			if (!current->is_dead) {
				batch->Draw(int(current->position.Y()),
							current->position.X(),
							current->position.Y(),
							current->size,
							current->angle,
							m_texture,
							current->color,
							m_particleBlend);
			} else { continue; }
		}
	}
}

void sig::ParticleEmitter::EmitParticle()
{
	Particle *p = nullptr;

	// Get the first dead particle
	SIG_FOREACH(it, m_particlePool)
	{
		Particle *current = (*it);
		if (current->is_dead) {
			p = current;
			break;
		}
	}
	if (p == nullptr) { return; }

	p->is_dead = false;
	p->life_time = m_particleLife - RandRange<float>(0.0f, 0.2f);
	p->original_life_time = p->life_time;

	p->angle = RandRange<float>(-m_particleSpread, m_particleSpread);
	p->angle += GetOwner()->GetRotation() + PI/2;
	p->ang_speed = m_particleAngularVelocity;

	float life = p->original_life_time;

	p->delta_color.r = (m_endColor.r - m_startColor.r) / life;
	p->delta_color.g = (m_endColor.g - m_startColor.g) / life;
	p->delta_color.b = (m_endColor.b - m_startColor.b) / life;
	p->delta_color.a = (m_endColor.a - m_startColor.a) / life;
	p->color = m_startColor;

	float startSize = m_particleStartSize - RandRange<float>(0.0f, 0.2f);
	p->size = startSize;
	p->delta_size = (m_particleEndSize - startSize) / life;

	p->position = GetOwner()->GetPosition();
	if (m_posFun) {
		p->position = p->position + m_posFun(p->angle, life);
	}

	p->accel = Vector2(m_particleAccel.X(), -m_particleAccel.Y());

	float speed = m_particleSpeed - RandRange<float>(0.0f, 0.2f);
	p->velocity = Vector2(
		std::cos(p->angle) * speed,
		std::sin(p->angle) * -speed
	);
}

void sig::ParticleEmitter::EmitParticles(u32 count)
{
	if (count > m_maxParticles) { return; }
	if (count <= 0) { return; }
	for (u32 i = 0; i < count; i++) {
		EmitParticle();
	}
}

sig::ParticleEmitter *sig::ParticleEmitter::GetInstance(Node *owner)
{
	ParticleEmitter *inst = new ParticleEmitter(m_maxParticles);
	inst->m_emitRate = m_emitRate;
	inst->m_enabled = m_enabled;
	inst->m_endColor = Color(m_endColor.r, m_endColor.g, m_endColor.b, m_endColor.a);
	inst->m_liveParticles = m_liveParticles;
	inst->m_owner = owner;
	inst->m_particleAccel = Vector2(m_particleAccel.X(), m_particleAccel.Y());
	inst->m_particleAngularVelocity = m_particleAngularVelocity;
	inst->m_particleBlend = m_particleBlend;
	inst->m_particleEndSize = m_particleEndSize;
	inst->m_particleLife = m_particleLife;
	inst->m_particleSpeed = m_particleSpeed;
	inst->m_particleSpread = m_particleSpread;
	inst->m_particleStartSize = m_particleStartSize;
	inst->m_posFun = m_posFun;
	inst->m_startColor = Color(m_startColor.r, m_startColor.g, m_startColor.b, m_startColor.a);
	inst->m_texture = m_texture;
	inst->m_time = m_time;

	return inst;
}

sig::Particle::Particle()
	:	angle(0),
		size(0),
		life_time(0),
		velocity(Vector2::Zero),
		position(Vector2::Zero),
		color(Color::WHITE)
{
	is_dead = true;
	original_life_time = 0;
}

sig::Particle::Particle(const sig::math::Vector2 &pos,
						const sig::math::Vector2 &vel, float ang, float sz,
						float life, const sig::Color &col)
	:	ang_speed(0),
		angle(ang),
		size(sz),
		life_time(life),
		velocity(vel),
		position(pos),
		color(col)
{
	original_life_time = life;
	if (life <= 0) {
		is_dead = true;
	}
	accel = Vector2::Zero;
}

void sig::Particle::Update(float d) {
	life_time -= d;
	velocity = velocity + accel;
	position = position + velocity * d;
	angle += ang_speed * d;

	color.r += delta_color.r * d;
	color.g += delta_color.g * d;
	color.b += delta_color.b * d;
	color.a += delta_color.a * d;

	size += delta_size * d;

	is_dead = life_time <= 0.0f;
}
