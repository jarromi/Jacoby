#pragma once
#include <utility>
#include <cmath>
#include <Inc/jacoby/types.h>

#ifndef CORE_JACOBY
#define CORE_JACOBY

namespace jacoby
{
	template< typename PrecType = FLOAT>
	class Vector3
	{
		PrecType x, y, z, pad;
	public:
		// default constructor
		Vector3() :
			x(PrecType(0)),
			y(PrecType(0)),
			z(PrecType(0)),
			pad(PrecType(0))
		{};

		// constructor from PrecType
		Vector3(const PrecType& x_, const PrecType& y_, const PrecType& z_):
			x(x_),
			y(y_),
			z(z_)
		{}

		// move constructor from PrecType
		Vector3(PrecType&& x_, PrecType&& y_, PrecType&& z_) :
			x(std::move(x_)),
			y(std::move(y_)),
			z(std::move(z_))
		{}

		// copy constructor
		Vector3(Vector3< PrecType>& rVec) :
			x(rVec.x),
			y(rVec.y),
			z(rVec.z),
			pad(rVec.pad)
		{};

		// copy constructor
		Vector3(const Vector3< PrecType> & rVec) :
			x(rVec.x),
			y(rVec.y),
			z(rVec.z),
			pad(rVec.pad)
		{};

		// move constructor
		Vector3(Vector3< PrecType>&& rVec) :
			x(std::move(rVec.x)),
			y(std::move(rVec.y)),
			z(std::move(rVec.z))
		{};

		// =========== Operators ===============
		// assignment operator
		Vector3< PrecType >& operator = (Vector3< PrecType >& rVec)
		{
			x = rVec.x;
			y = rVec.y;
			z = rVec.z;
			return *this;
		}

		Vector3< PrecType >& operator = (const Vector3< PrecType >& rVec)
		{
			x = rVec.x;
			y = rVec.y;
			z = rVec.z;
			return *this;
		}

		// move assignment operator
		Vector3< PrecType >& operator = (Vector3< PrecType >&& rVec)
		{
			x = std::move(rVec.x);
			y = std::move(rVec.y);
			z = std::move(rVec.z);
			return *this;
		}

		// addition
		Vector3< PrecType > operator + (const Vector3< PrecType >& rVec) const
		{
			PrecType xOut = x + rVec.x;
			PrecType yOut = y + rVec.y;
			PrecType zOut = z + rVec.z;
			return Vector3< PrecType >(xOut, yOut, zOut);
		}

		Vector3< PrecType >& operator += (const Vector3< PrecType >& rVec)
		{
			x += rVec.x;
			y += rVec.y;
			z += rVec.z;
			return *this;
		}

		// subtraction
		Vector3< PrecType > operator - (const Vector3< PrecType >& rVec) const
		{
			PrecType xOut = x - rVec.x;
			PrecType yOut = y - rVec.y;
			PrecType zOut = z - rVec.z;
			return Vector3< PrecType >(xOut, yOut, zOut);
		}

		Vector3< PrecType >& operator -= (const Vector3< PrecType >& rVec)
		{
			x -= rVec.x;
			y -= rVec.y;
			z -= rVec.z;
			pad -= rVec.pad;
			return *this;
		}

		// multiplication by number
		Vector3< PrecType > operator * (const PrecType& rVal) const
		{
			PrecType xOut = x * rVal;
			PrecType yOut = y * rVal;
			PrecType zOut = z * rVal;
			return Vector3< PrecType >(xOut, yOut, zOut);
		}

		Vector3< PrecType >& operator *= (const PrecType& rVal)
		{
			x *= rVal;
			y *= rVal;
			z *= rVal;
			return *this;
		}

		// division by number
		Vector3< PrecType > operator / (const PrecType& rVal) const
		{
			PrecType xOut = x / rVal;
			PrecType yOut = y / rVal;
			PrecType zOut = z / rVal;
			return Vector3< PrecType >(xOut, yOut, zOut);
		}

		Vector3< PrecType >& operator /= (const PrecType& rVal)
		{
			x /= rVal;
			y /= rVal;
			z /= rVal;
			return *this;
		}

		// scalar product
		PrecType operator * (const Vector3< PrecType >& rVec)
		{
			return x * rVec.x + y * rVec.y + z * rVec.z;
		}

		// vector product
		Vector3< PrecType > operator % (const Vector3<PrecType>& rVec) const
		{
			return Vector3<PrecType>(
				y * rVec.z - z * rVec.y,
				z * rVec.x - x * rVec.z,
				x * rVec.y - y * rVec.x
				);
		}

		void operator %= (const Vector3<PrecType>& rVec)
		{
			*this = VectorProduct(*this, rVec);
		}

		friend BOOL operator == (const Vector3<PrecType>& rVec, const Vector3<PrecType>& lVec)
		{
			return (rVec.x== lVec.x && rVec.y== lVec.y & rVec.z== lVec.z );
		}

		BOOL operator != (const Vector3<PrecType>& rVec)
		{
			return !(*this == rVec);
		}

		// =========== Methods ===============
		// invert
		void Invert()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		PrecType Magnitude() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		PrecType SquareMagnitude() const
		{
			return x * x + y * y + z * z;
		}

		void Normalize()
		{
			PrecType norm = Magnitude();
			if (norm > 0)
			{
				x /= norm;
				y /= norm;
				z /= norm;
			}
		}

		void AddScaledVector(const Vector3< PrecType >& rVec, const PrecType& rVal)
		{
			x += rVec.x * rVal;
			y += rVec.y * rVal;
			z += rVec.z * rVal;
		}

		static PrecType ScalarProduct(const Vector3<PrecType>& lVec, const Vector3<PrecType>& rVec)
		{
			return lVec.x * rVec.x + lVec.y * rVec.y + lVec.z * rVec.z;
		}

		static Vector3< PrecType > ComponentProduct(const Vector3<PrecType>& lVec, const Vector3<PrecType>& rVec)
		{
			return Vector3< PrecType >(lVec.x * rVec.x, lVec.y * rVec.y, lVec.z * rVec.z);
		}

		static Vector3< PrecType > VectorProduct(const Vector3<PrecType>& lVec, const Vector3<PrecType>& rVec)
		{
			return Vector3<PrecType>(
				lVec.y * rVec.z - lVec.z * rVec.y,
				lVec.z * rVec.x - lVec.x * rVec.z,
				lVec.x * rVec.y - lVec.y * rVec.x
				);
		}

		static void MakeOrthonromalBasis(Vector3< PrecType >& e1, Vector3< PrecType >& e2, Vector3< PrecType >& e3)
		{
			e1.Normalize();
			e3 = e1 % e2;
			if (e3.SquareMagnitude() == 0.0)
			{
				throw;
			}
		}

		PrecType& getX() 
		{
			return x;
		}

		PrecType& getY()
		{
			return y;
		}

		PrecType& getZ()
		{
			return z;
		}

		void clear()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
	};
}

#endif