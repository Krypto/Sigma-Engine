#ifndef SIGMA_PARTICLE_EMITTER
#define SIGMA_PARTICLE_EMITTER

#include "Color.h"
#include "Vector.h"
#include "Component.h"
#include "Texture2D.h"
#include "SIG_Utilities.h"

#include <vector>
#include <functional>
using namespace std;

namespace sig {
	using namespace math;

	typedef struct Particle {
		float ang_speed, angle, size, delta_size, life_time, original_life_time;
		Vector2 velocity, position, accel;
		Color color, delta_color;
		bool is_dead;

		Particle();
		Particle(const Vector2& pos, const Vector2& vel,
				 float ang, float sz, float life,
				 const Color& col);

		void Update(float d);

	} Particle;

	enum class BlendMode;
	class ParticleEmitter : public Component
	{
	public:
		ParticleEmitter(u32 max_particles=100);

		void Initialize();
		void Update(float dt);
		void Render(SpriteBatch *batch);

		void EmitParticle();
		void EmitParticles(u32 count);

		Color& GetStartColor() { return m_startColor; }
		Color& GetEndColor() { return m_endColor; }
		float GetParticleSpread() const { return m_particleSpread; }
		float GetParticleLife() const { return m_particleLife; }
		float GetParticleSpeed() const { return m_particleSpeed; }
		Texture2D* GetTexture() { return m_texture; }
		BlendMode& GetParticleBlend() { return m_particleBlend; }
		Vector2& GetParticleAcceleration() { return m_particleAccel; }
		float GetParticleAngularVelocity() { return m_particleAngularVelocity; }
		float GetParticleStartSize() const { return m_particleStartSize; }
		float GetParticleEndSize() const { return m_particleEndSize; }
		float GetEmitRate() const { return m_emitRate; }
		bool IsEnabled() const { return m_enabled; }

		void SetStartColor(const Color& col) { m_startColor = col; }
		void SetEndColor(const Color& col) { m_endColor = col; }
		void SetParticleSpread(float v) { m_particleSpread = v; }
		void SetParticleLife(float v) { m_particleLife = v; }
		void SetParticleSpeed(float v) { m_particleSpeed = v; }
		void SetTexture(Texture2D* tex) { m_texture = tex; }
		void SetParticleBlend(BlendMode mode) { m_particleBlend = mode; }
		void SetParticleAcceleration(const Vector2& v) { m_particleAccel = v; }
		void SetParticleAngularVelocity(float v) { m_particleAngularVelocity = v; }
		void SetParticleStartSize(float v) { m_particleStartSize = v; }
		void SetParticleEndSize(float v) { m_particleEndSize = v; }
		void SetEmitRate(float v) { m_emitRate = v; }
		void SetEnabled(bool v) { m_enabled = v; }

		void SetPositionCallback(function<Vector2(float, float)> cb) { m_posFun = cb; }
	private:
		u32 m_maxParticles, m_liveParticles;
		Color m_startColor, m_endColor;
		float m_particleSpread, m_particleLife, m_particleSpeed, m_particleAngularVelocity;
		float m_particleStartSize, m_particleEndSize;
		float m_emitRate, m_time;
		Texture2D *m_texture;
		BlendMode m_particleBlend;
		Vector2 m_particleAccel;
		bool m_enabled;

		function<Vector2(float, float)> m_posFun;

		vector<Particle*> m_particlePool;
	};
}

#endif // SIGMA_PARTICLE_EMITTER
