#ifndef PARTICLE_FORCE_GENERATOR
#define PARTICLE_FORCE_GENERATOR

#include <Inc/jacoby/types.h>
#include <Inc/jacoby/particle.h>
#include <vector>

namespace jacoby
{
	typedef Particle<FLOAT> ParticleType;
	typedef Vector3< FLOAT > VectorType;

	/*
	* Template for different force types
	*/
	class ParticleForceGenerator
	{
	public:
		virtual void UpdateForce(ParticleType*, FLOAT) = 0;
	};

	/*
	* Manager that keeps trach of forces, their type and particles
	* on which they act
	TODO - make it a singleton
	*/
	class ParticleForceManager
	{
	protected:
		struct ParticleForceRegistration
		{
			ParticleType* p_particle;
			ParticleForceGenerator* p_fg;
		};

		typedef std::vector<ParticleForceRegistration> RegistryType;
		RegistryType m_registry;

	public:
		void Add(ParticleType* particle, ParticleForceGenerator* fg);

		void Remove(ParticleType* particle, ParticleForceGenerator* fg);

		void Clear();

		void UpdateForces(FLOAT dT);
	};

	class ParticleGravity : public ParticleForceGenerator
	{
		VectorType m_gravity;

	public:
		ParticleGravity(const VectorType& gravity);

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);
	};

	class ParticleDrag : public ParticleForceGenerator
	{
		FLOAT m_k1, m_k2;

	public:
		ParticleDrag(FLOAT k1, FLOAT k2) : m_k1(k1), m_k2(k2) {};

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);
	};

	class ParticleSpring : public ParticleForceGenerator
	{
		ParticleType* m_other;

		FLOAT m_springConstant;

		FLOAT m_restLength;

	public:
		ParticleSpring(ParticleType* other, FLOAT springConstant, FLOAT restLength) :
			m_other(other),
			m_springConstant(springConstant),
			m_restLength(restLength)
		{}

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);
	};

	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	protected:
		VectorType* m_anchor;

		FLOAT m_springConstant;

		FLOAT m_restLength;

	public:

		ParticleAnchoredSpring(VectorType* anchor, FLOAT springConstant, FLOAT restLength) :
			m_anchor(anchor),
			m_springConstant(springConstant),
			m_restLength(restLength)
		{}

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);

	};

	class ParticleBungee : public ParticleForceGenerator
	{
		ParticleType* m_other;

		FLOAT m_springConstant;

		FLOAT m_restLength;

	public:
		ParticleBungee(ParticleType* other, FLOAT springConstant, FLOAT restLength) :
			m_other(other),
			m_springConstant(springConstant),
			m_restLength(restLength)
		{}

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);
	};

	class ParticleBuoyancy : public ParticleForceGenerator
	{
		FLOAT m_maxDepth;

		FLOAT m_volume;

		FLOAT m_waterHeight;

		FLOAT m_liquidDensity;

	public:
		ParticleBuoyancy(FLOAT maxDepth, FLOAT volume, FLOAT waterHeight, FLOAT liquidDensity) :
			m_maxDepth(maxDepth),
			m_volume(volume),
			m_waterHeight(waterHeight),
			m_liquidDensity(liquidDensity)
		{}

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);
	};

	class ParticleFakeSpring : public ParticleForceGenerator
	{
		VectorType* m_anchor;

		FLOAT m_springConstant;

		FLOAT m_damping;

		ParticleFakeSpring(VectorType* anchor, FLOAT springConstant, FLOAT damping) :
			m_anchor(anchor),
			m_springConstant(springConstant),
			m_damping(damping)
		{}

		virtual void UpdateForce(ParticleType* particle, FLOAT dT);
	};

}

#endif //PARTICLE_FORCE_GENERATOR