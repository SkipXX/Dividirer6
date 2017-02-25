#pragma once

class Vec2
{
public:
	Vec2() = default;
	Vec2( double x_in,double y_in );
	Vec2 operator+( const Vec2& rhs ) const;
	Vec2& operator+=( const Vec2& rhs );
	Vec2 operator*( double rhs ) const;
	Vec2& operator*=( double rhs );
	Vec2 operator-( const Vec2& rhs ) const;
	Vec2& operator-=( const Vec2& rhs );
	double GetLength() const;
	double GetLengthSq() const;
	Vec2& Normalize();
	Vec2 GetNormalized() const;
public:
	double x;
	double y;
};