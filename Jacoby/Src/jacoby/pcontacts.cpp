#include <Inc/jacoby/pcontacts.h>

namespace jacoby
{
	void ParticleContact::Resolve(FLOAT dT)
	{
		ResolveVelocity(dT);
		ResolveInterpenetration(dT);
	}

	FLOAT ParticleContact::CalculateSeparatingVelocity() const
	{
		VectorType relativeVel = m_particle[0]->GetVelocity();
		if(m_particle[1]) relativeVel -= m_particle[1]->GetVelocity();
		return relativeVel * m_contactNormal;
	}

	void ParticleContact::ResolveVelocity(FLOAT dT)
	{
		FLOAT sepVel = CalculateSeparatingVelocity();

		if (sepVel > 0.f)
		{
			return;
		}

		FLOAT totalIM = m_particle[0]->GetInverseMass();
		if (m_particle[1]) 
			totalIM += m_particle[1]->GetInverseMass();
		if (totalIM <= 0)
			return;

		FLOAT newSepVel = -sepVel * m_restitution;

		VectorType accCausedVelocity = m_particle[0]->GetAcceleration();
		if (m_particle[1])
			accCausedVelocity -= m_particle[1]->GetAcceleration();
		FLOAT accCausedSepVelocity = (accCausedVelocity * m_contactNormal) * dT;

		if (accCausedSepVelocity < 0.0f)
		{
			newSepVel += m_restitution * accCausedSepVelocity;

			if (newSepVel < 0.0f)
				newSepVel = 0.0f;
		}

		FLOAT dV = newSepVel - sepVel;

		FLOAT impulse = dV / totalIM;

		VectorType impulsePerMass = m_contactNormal * impulse;

		m_particle[0]->SetVelocity(
			m_particle[0]->GetVelocity() +
			impulsePerMass * m_particle[0]->GetInverseMass()
		);

		if (m_particle[1])
		{
			m_particle[1]->SetVelocity(
				m_particle[1]->GetVelocity() +
				impulsePerMass * m_particle[1]->GetInverseMass()
			);
		}
	}

	void ParticleContact::ResolveInterpenetration(FLOAT dT)
	{
		if (m_penetration <= 0)
			return;

		FLOAT totalIM = m_particle[0]->GetInverseMass();
		if (m_particle[1]) 
			totalIM += m_particle[1]->GetInverseMass();
		if (totalIM <= 0)
			return;

		VectorType movePerIM =
			m_contactNormal * (m_penetration / totalIM);

		VectorType particleMovement[2];
		particleMovement[0] = movePerIM * m_particle[0]->GetInverseMass();
		if (m_particle[1])
			particleMovement[1] = movePerIM * m_particle[1]->GetInverseMass();
		else
			particleMovement[1].clear();

		m_particle[0]->SetPosition(
			m_particle[0]->GetPosition() + particleMovement[0]
		);
		if (m_particle[1])
		{
			m_particle[1]->SetPosition(
				m_particle[1]->GetPosition() + particleMovement[1]
			);
		}
	}

	void ParticleContactResolver::ResolveContacts(ParticleContact* contactArray,
		unsigned numContacts,
		FLOAT dT)
	{
		unsigned i;

		m_iterUsed = 0;
		while (m_iterUsed < m_iter)
		{
			FLOAT max = MAX_FLOAT;
			unsigned maxInd = numContacts;
			for (i = 0; i < numContacts; ++i)
			{
				FLOAT sepVal = contactArray[i].CalculateSeparatingVelocity();
				if (sepVal < max && (sepVal < 0 || contactArray[i].m_penetration > 0))
				{
					max = sepVal;
					maxInd = 1;
				}
			}

			if (maxInd == numContacts)
				break;

			contactArray[maxInd].Resolve(dT);

			++m_iterUsed;
		}
	}
}