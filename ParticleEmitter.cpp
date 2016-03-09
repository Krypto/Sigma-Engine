#include "ParticleEmitter.h"
#include "Node.h"
#include "SpriteBatch.h"

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

	for (auto it = m_particlePool.begin(); it != m_particlePool.end(); ++it) {
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
		for (auto it = m_particlePool.begin(); it != m_particlePool.end(); ++it) {
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
	for (auto it = m_particlePool.begin(); it != m_particlePool.end(); ++it) {
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

	p->accel = m_particleAccel;

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
