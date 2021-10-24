#pragma once

#include <Inc/jacoby/types.h>
#include <Inc/jacoby/particle.h>

namespace jacoby
{
	typedef Particle<FLOAT> ParticleType;
	typedef Vector3< FLOAT > VectorType;

	class ParticleContact
	{
	public:
		ParticleType* m_particle[2];

		FLOAT m_restitution;

		VectorType m_contactNormal;

	protected:
		void Resolve(FLOAT dT);

		FLOAT CalculateSeparatingVelocity() const;

	private:
		void ResolveVelocity(FLOAT dT);

		FLOAT m_penetration;

		void ResolveInterpenetration(FLOAT dT);
	};
}