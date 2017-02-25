#include "Vec2.h"
#include <cmath>

Vec2::Vec2( double x_in,double y_in )
	:
	x( x_in ),
	y( y_in )
{
}

Vec2 Vec2::operator+( const Vec2& rhs ) const
{
	return Vec2( x + rhs.x,y + rhs.y );
}

Vec2& Vec2::operator+=( const Vec2& rhs )
{
	return *this = *this + rhs;
}

Vec2 Vec2::operator*( double rhs ) const
{
	return Vec2( x * rhs,y * rhs );
}

Vec2& Vec2::operator*=( double rhs )
{
	return *this = *this * rhs;
}

Vec2 Vec2::operator-( const Vec2& rhs ) const
{
	return Vec2( x - rhs.x,y - rhs.y );
}

Vec2& Vec2::operator-=( const Vec2& rhs )
{
	return *this = *this - rhs;
}

double Vec2::GetLength() const
{
	return std::sqrt( GetLengthSq() );
}

double Vec2::GetLengthSq() const
{
	return x * x + y * y;
}

Vec2& Vec2::Normalize()
{
	return *this = GetNormalized();
}

Vec2 Vec2::GetNormalized() const
{
	const double len = GetLength();
	if( len != 0.0f )
	{
		return *this * (1.0f / len);
	}
	return *this;
}
