/**
 *	Filename: TVector.h
 *	Date Created: 30/03/25
 *	Purpose: The Base class for vector types.
 ***/
#pragma once

#include "glm/glm/glm.hpp"

namespace Sengine::Math
{
	template<typename T, size_t N>
	struct TVector
	{
		T data[N];
	};

    /* 2D vector */
    template <typename T>
    struct TVector<T, 2>
    {
        union { T X; };
        union { T Y; };

        TVector() : X(0), Y(0) {}
        TVector(T x, T y) : X(x), Y(y) {}

        T& operator[](size_t index)
    	{
            if (index == 0)
            {
                return X;
            }
            if (index == 1)
            {
                return Y;
            }
            throw std::out_of_range("TVector2 index out of range");
        }

        const T& operator[](size_t index) const
        {
            if (index == 0)
            {
                return X;
            }
            if (index == 1)
            {
                return Y;
            }
            throw std::out_of_range("TVector2 index out of range");
        }

        TVector<T, 2> operator+(const TVector<T, 2>& other) const
    	{
            return TVector<T, 2>(X + other.X, Y + other.Y);
        }

        TVector<T, 2> operator-(const TVector<T, 2>& other) const
    	{
            return TVector<T, 2>(X - other.X, Y - other.Y);
        }

        TVector<T, 2> operator*(T scalar) const
    	{
            return TVector<T, 2>(X * scalar, Y * scalar);
        }

        T Dot(const TVector<T, 2>& other) const
    	{
            return X * other.X + Y * other.Y;
        }

        T MagnitudeSquared() const
    	{
            return X * X + Y * Y;
        }

        [[nodiscard]] glm::vec2 ToGLM() const
        {
            return {X, Y};
        }

        [[nodiscard]] std::string ToString() const
        {
            return "[" + std::to_string(X) + ", " + std::to_string(Y) + "]";
        }
    };

    /* 3D Vector */
    template <typename T>
    struct TVector<T, 3>
    {
        union { T X; };
        union { T Y; };
        union { T Z; };

        TVector() : X(0), Y(0), Z(0) {}
        TVector(T x, T y, T z) : X(x), Y(y), Z(z) {}

        T& operator[](size_t index)
        {
            if (index == 0)
            {
                return X;
            }
            if (index == 1)
            {
                return Y;
            }
            if (index == 2)
            {
                return Z;
            }
            throw std::out_of_range("TVector3 index out of range");
        }

        const T& operator[](size_t index) const
        {
            if (index == 0)
            {
                return X;
            }
            if (index == 1)
            {
                return Y;
            }
            if (index == 2) 
            {
                return Z;
            }
            throw std::out_of_range("TVector3 index out of range");
        }

        TVector<T, 3> operator+(const TVector<T, 3>& other) const
        {
            return TVector<T, 3>(X + other.X, Y + other.Y, Z + other.Z);
        }

        TVector<T, 3> operator-(const TVector<T, 3>& other) const
        {
            return TVector<T, 3>(X - other.X, Y - other.Y, Z - other.Z);
        }

        TVector<T, 3> operator*(T scalar) const
        {
            return TVector<T, 3>(X * scalar, Y * scalar, Z * scalar);
        }

        T Dot(const TVector<T, 3>& other) const
        {
            return X * other.X + Y * other.Y + Z * other.Z;
        }

        T MagnitudeSquared() const
        {
            return X * X + Y * Y + Z * Z;
        }

        [[nodiscard]] glm::vec3 ToGLM() const
        {
            return { X, Y, Z };
        }

        [[nodiscard]] std::string ToString() const
        {
            return "[" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + "]";
        }
    };

    /* 4D Vector */
    template <typename T>
    struct TVector<T, 4>
    {
        union { T X; T R;};
        union { T Y; T G; };
        union { T Z; T B; };
        union { T W; T A; };

        TVector() : X(0), R(0), Y(0), G(0), Z(0), B(0), W(0), A(0) {}
        TVector(T x, T y, T z, T w) : X(x), R(x), Y(y), G(y), Z(z), B(z), W(w), A(w) {}

        T& operator[](size_t index)
        {
            if (index == 0)
            {
                return X;
            }
            if (index == 1)
            {
                return Y;
            }
            if (index == 2)
            {
                return Z;
            }
            if (index == 3)
            {
                return W;
            }
            throw std::out_of_range("TVector3 index out of range");
        }

        const T& operator[](size_t index) const
        {
            if (index == 0)
            {
                return X;
            }
            if (index == 1)
            {
                return Y;
            }
            if (index == 2)
            {
                return Z;
            }
            if (index == 3)
            {
                return W;
            }
            throw std::out_of_range("TVector3 index out of range");
        }

        TVector<T, 4> operator+(const TVector<T, 4>& other) const
        {
            return TVector<T, 4>(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
        }

        TVector<T, 4> operator-(const TVector<T, 4>& other) const
        {
            return TVector<T, 4>(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
        }

        TVector<T, 4> operator*(T scalar) const
        {
            return TVector<T, 4>(X * scalar, Y * scalar, Z * scalar, W * scalar);
        }

        T Dot(const TVector<T, 4>& other) const
        {
            return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
        }

        T MagnitudeSquared() const
        {
            return X * X + Y * Y + Z * Z + W * W;
        }

        [[nodiscard]] std::string ToString() const
        {
            return "[" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ", " + std::to_string(W) + "]";
        }
    };
}//namespace Sengine::Math
