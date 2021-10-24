#include <Inc/jacoby/pfgen.h>
#include <algorithm>
#include <iterator>
#include <math.h>

namespace jacoby
{
	void ParticleForceManager::UpdateForces(FLOAT dT)
	{
		RegistryType::iterator it = m_registry.begin();
		for (; it != m_registry.end(); it++)
		{
			it->p_fg->UpdateForce(it->p_particle, dT);
		}
	}

	void ParticleForceManager::Add(ParticleType* prt, ParticleForceGenerator* fg)
	{
		ParticleForceRegistration newEntry = { prt, fg };
		m_registry.push_back(std::move(newEntry));
	}

	void ParticleForceManager::Remove(ParticleType* prt, ParticleForceGenerator* fg)
	{
		ParticleForceRegistration toRemove{ prt , fg };
		auto is_equal = [toRemove](ParticleForceRegistration &element)
		{
			return toRemove.p_particle == element.p_particle && toRemove.p_fg == element.p_fg;
		};
		auto searchResult = std::find_if(m_registry.begin(), m_registry.end(), is_equal);
		m_registry.erase(searchResult);
	}

	void ParticleForceManager::Clear()
	{
		m_registry.clear();
	}

	ParticleGravity::ParticleGravity(const VectorType& gravity)
	{
		m_gravity = gravity;
	}

	void ParticleGravity::UpdateForce(ParticleType* particle, FLOAT dT)
	{
		if (particle->Mass() <= 0.f || particle->InverseMass() <= 0.f)
		{
			return;
		}

		particle->AddForce(m_gravity * particle->Mass());
	}

	void ParticleDrag::UpdateForce(ParticleType *particle, FLOAT dT)
	{
		VectorType force = particle->GetVelocity();
		
		FLOAT dragCoeff = force.Magnitude();
		dragCoeff = dragCoeff * m_k1 + dragCoeff * dragCoeff * m_k2;

		force.Normalize();
		force *= -dragCoeff;
		particle->AddForce(force);
	}

	void ParticleSpring::UpdateForce(ParticleType* particle, FLOAT dT)
	{
		VectorType force = particle->GetPosition();
		force -= m_other->GetPosition();

		FLOAT magnitude = force.Magnitude();
		magnitude = fabsf(magnitude - m_restLength);
		magnitude *= m_springConstant;

		force.Normalize();
		force *= -magnitude;
		particle->AddForce(force);
	}

	void ParticleAnchoredSpring::UpdateForce(ParticleType* particle, FLOAT dT)
	{
		VectorType force = particle->GetPosition();
		force -= *m_anchor;

		FLOAT magnitude = force.Magnitude();
		magnitude = (m_restLength - magnitude)* m_springConstant;

		force.Normalize();
		force *= magnitude;
		particle->AddForce(force);
	}

	void ParticleBungee::UpdateForce(ParticleType* particle, FLOAT dT)
	{
		VectorType force = particle->GetPosition();
		force -= m_other->GetPosition();

		FLOAT magnitude = force.Magnitude();
		if(magnitude <= m_restLength)
			return;

		magnitude = m_springConstant * (m_restLength - magnitude);

		force.Normalize();
		force *= -magnitude;
		particle->AddForce(force);
	}

	void ParticleBuoyancy::UpdateForce(ParticleType* particle, FLOAT dT)
	{
		FLOAT depth = particle->GetPosition().getY();

		if (depth >= m_waterHeight + m_maxDepth)
			return;

		VectorType force(0, 0, 0);
		if (depth <= m_waterHeight - m_maxDepth)
		{
			force.getY() = m_liquidDensity * m_volume;
			particle->AddForce(force);
			return;
		}

		force.getY() = m_liquidDensity * m_volume
			* (depth - m_maxDepth - m_waterHeight) / 2* m_maxDepth;
		particle->AddForce(force);
	}

	void ParticleFakeSpring::UpdateForce(ParticleType* particle, FLOAT dT)
	{
		if (particle->Mass() == 0.0f || particle->InverseMass() == 0.0f)
			return;

		VectorType position = particle->GetPosition();
		position -= *m_anchor;

		FLOAT gamma = 0.5f * sqrtf(4.0f * m_springConstant - m_damping * m_damping);
		if (gamma == 0.0f)
			return;

		VectorType c = position * (m_damping / (2.0f * gamma));
		c += particle->GetVelocity() * (1.0f / gamma);

		VectorType target = position * cosf(gamma * dT) + c * sinf(gamma * dT);

		target *= expf(-0.5f * dT * m_damping);

		VectorType accel = (target - position) * (1.0f / (dT * dT)) - particle->GetVelocity() * dT;

		particle->AddForce(accel * particle->Mass());
	}
}