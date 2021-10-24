#pragma once
#include <utility>
#include <cmath>
#include <string>
#include <stdio.h>
#include <iostream>
#include <Inc/jacoby/types.h>
#include <Inc/jacoby/core.h>

#ifndef PARTICLE_JACOBY
#define PARTICLE_JACOBY

namespace jacoby
{
	template< typename PrecType = FLOAT>
	class Particle
	{
	public:
		typedef Vector3< PrecType > VectorType;
	private:

		VectorType m_position;
		VectorType m_velocity;
		VectorType m_acceleration;

		PrecType m_inverseMass;
		PrecType m_damping;

		VectorType m_forceAccumulator;
	public:
		// default constructor
		Particle() :
			m_position(VectorType()),
			m_velocity(VectorType()),
			m_acceleration(VectorType()),
			m_inverseMass(PrecType(1.0)),
			m_damping(PrecType(0.999))
		{};

		// constructor from PrecType
		Particle(const VectorType& pos_,
			const VectorType& vel_ = VectorType(),
			const VectorType& acc_ = VectorType(),
			const FLOAT& invM_ = PrecType(1.0),
			const FLOAT& damp_ = PrecType(0.999)
		) :
			m_position(pos_),
			m_velocity(vel_),
			m_acceleration(acc_)
		{
			if (invM_ != PrecType(0.0))
			{
				m_inverseMass = invM_;
			}
			else
			{
				if (sizeof(PrecType) == 32)
					m_inverseMass = PrecType(std::nextafterf(0.0, 1.0));
				else if (sizeof(PrecType) == 64)
					m_inverseMass = std::nextafter(0.0, 1.0);
				else
					throw;
			}
			if (damp_ < PrecType(1.0) && damp_ > PrecType(0.0))
			{
				m_damping = damp_;
			}
			else
			{
				m_damping = PrecType(0.999);
			}
		};

		// move constructor from PrecType
		Particle(const VectorType&& pos_,
			const VectorType&& vel_ = VectorType(),
			const VectorType&& acc_ = VectorType(),
			const FLOAT&& invM_ = 1.0,
			const FLOAT&& damp_ = 0.999
		) :
			m_position(std::move(pos_)),
			m_velocity(std::move(vel_)),
			m_acceleration(std::move(acc_))
		{
			if (invM_ != PrecType(0.0))
			{
				m_inverseMass = std::move(invM_);
			}
			else
			{
				if (sizeof(PrecType) == 32)
					m_inverseMass = PrecType(std::nextafterf(0.0, 1.0));
				else if (sizeof(PrecType) == 64)
					m_inverseMass = std::nextafter(0.0, 1.0);
				else
					throw std::_Xruntime_error("Wrong size of FLOAT");
			}
			if (damp_ < PrecType(1.0) && damp_ > PrecType(0.0))
			{
				m_damping = (std::move(damp_));
			}
			else
			{
				m_damping = PrecType(0.999);
			}

		};

		// copy constructor
		Particle(const Particle&) = default;

		// move constructor
		Particle(Particle&&) = default;

		// =========== Operators ===============
		// assignment operator
		Particle& operator = (const Particle&) = default;

		// move assignment operator
		Particle& operator = (Particle&& ) = default;


		// =========== Methods ===============
		// position
		VectorType Position() const
		{
			return m_position;
		}

		VectorType& GetPosition()
		{
			return m_position;
		}

		VectorType SetPosition(const VectorType& newPos)
		{
			m_position = newPos;
			return m_position;
		}

		// velocity
		VectorType Velocity() const
		{
			return m_velocity;
		}

		VectorType& GetVelocity()
		{
			return m_velocity;
		}

		VectorType SetVelocity(const VectorType& newVel)
		{
			m_velocity = newVel;
			return m_velocity;
		}

		// acceleration
		VectorType Acceleration() const
		{
			return m_acceleration;
		}

		VectorType& GetAcceleration()
		{
			return m_acceleration;
		}

		VectorType SetAcceleration(const VectorType& newAcc)
		{
			m_acceleration = newAcc;
			return m_acceleration;
		}

		VectorType AddToAcceleration(const VectorType& newAcc)
		{
			m_acceleration += newAcc;
			return m_acceleration;
		}

		BOOL UpdateAcceleration()
		{
			// cannot calculate acceleration with mass less than zero
			// reset acceleration
			if (m_inverseMass <= 0.0f)
			{
				m_acceleration = VectorType();
				return false;
			}

			m_acceleration = m_forceAccumulator * m_inverseMass;
			return true;
		}

		VectorType Integrate(const PrecType& dT)
		{
			assert(dT > 0);
			if (UpdateAcceleration())
			{
				m_position += m_velocity * dT + m_acceleration * dT * dT / PrecType(2.0);
				m_velocity += m_acceleration * dT;
			}
			else
			{
				m_position += m_velocity * dT;
			}
			m_velocity = m_velocity * std::powf(m_damping, dT);

			ClearAccumulator();

			return m_position;
		}

		PrecType Mass() const
		{
			return PrecType(1.) / m_inverseMass;
		}

		PrecType SetMass(const PrecType& newMass) const
		{
			if (newMass != PrecType(0.0))
			{
				m_inverseMass = PrecType(1.) / newMass;
				return PrecType(1.) / m_inverseMass;
			}
			else
			{
				throw std::_Xruntime_error("Cannot set mass to 0");
			}

		}

		PrecType InverseMass() const
		{
			return m_inverseMass;
		}

		PrecType SetInverseMass(const PrecType& newInvMass) const
		{
			if (newInvMass != PrecType(0.0))
			{
				m_inverseMass = newInvMass;
				return m_inverseMass;
			}
			else
			{
				if (sizeof(PrecType) == 32)
					m_inverseMass = PrecType(std::nextafterf(0.0, 1.0));
				else if (sizeof(PrecType) == 64)
					m_inverseMass = std::nextafter(0.0, 1.0);
				else
					throw std::_Xruntime_error("Wrong size of FLOAT");

			}
		}

		PrecType& GetInverseMass()
		{
			return m_inverseMass;
		}

		// damping
		PrecType Damping() const
		{
			return m_damping;
		}

		PrecType& GetDamping()
		{
			return m_damping;
		}

		PrecType SetDamping(const PrecType& newDamp)
		{

			if (newDamp < PrecType(1.0) && newDamp > PrecType(0.0))
			{
				m_damping = newDamp;
			}
			else
			{
				m_damping = PrecType(0.999);
			}
			return m_damping;
		}

		void ClearAccumulator()
		{
			m_forceAccumulator.clear();
		}

		void AddForce(const VectorType& forceToAdd)
		{
			m_forceAccumulator += forceToAdd;
		}
	};
}

#endif //PARTICLE_JACOBY