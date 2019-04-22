#pragma once

#include "cinder/Cinder.h"
#include <algorithm>
#include <iostream>

namespace cinder {

struct MediaTime {
  public:
	static constexpr int32_t DEFAULT_TIME_BASE = 10000000;

	MediaTime() : value( 0 ), base( 1 ), epoch( 0 ) {}
	MediaTime( int64_t value, int32_t base, int64_t epoch = 0 )
		: value( value ), base( base ), epoch( epoch )
	{}
	/*explicit MediaTime( double seconds, int32_t base = DEFAULT_TIME_BASE, int64_t epoch = 0 )
		: value( (int64_t)( seconds * base ) ), base( base ), epoch( epoch )
	{}*/

	double		getSeconds() const { return value / (double)base; }

	// Scale value to 'newBase' Subject to rounding 
	void		setBase( int32_t newBase ) { value *= newBase; value /= base; base = newBase; }

	int64_t		getValueAsBase( int32_t otherBase ) const { return ( base == otherBase ) ? value : ( value * otherBase / base ); }

	bool operator<( const MediaTime &rhs ) const {
		if( epoch != rhs.epoch ) return epoch < rhs.epoch;
		if( base == rhs.base ) return value < rhs.value;
		else return value * rhs.base < rhs.value * base;
	}

	bool operator>( const MediaTime &rhs ) const {
		if( epoch != rhs.epoch ) return epoch > rhs.epoch;
		if( base == rhs.base ) return value > rhs.value;
		else return value * rhs.base > rhs.value * base;
	}

	bool operator<=( const MediaTime &rhs ) const {
		if( epoch != rhs.epoch ) return epoch <= rhs.epoch;
		if( base == rhs.base ) return value <= rhs.value;
		else return value * rhs.base <= rhs.value * base;
	}

	bool operator>=( const MediaTime &rhs ) const {
		if( epoch != rhs.epoch ) return epoch >= rhs.epoch;
		if( base == rhs.base ) return value >= rhs.value;
		else return value * rhs.base >= rhs.value * base;
	}

	bool operator==( const MediaTime &rhs ) const {
		if( epoch != rhs.epoch ) return false;
		else if( base == rhs.base ) return value == rhs.value;
		else return value * rhs.base == rhs.value * base;
	}

	bool operator!=( const MediaTime &rhs ) const {
		if( epoch != rhs.epoch ) return true;
		else if( base == rhs.base ) return value != rhs.value;
		else return value * rhs.base != rhs.value * base;
	}

	// epoch is max of epochs
	MediaTime operator-( const MediaTime &rhs ) const {
		if( rhs.base == base ) return MediaTime( value - rhs.value, base, std::max( epoch, rhs.epoch ) );
		else return MediaTime( value * rhs.base - rhs.value * base, base * rhs.base, std::max( epoch, rhs.epoch ) );
	}

	// epoch is max of epochs
	MediaTime operator+( const MediaTime &rhs ) const {
		if( rhs.base == base ) return MediaTime( value + rhs.value, base, std::max( epoch, rhs.epoch ) );
		else return MediaTime( value * rhs.base + rhs.value * base, base * rhs.base, std::max( epoch, rhs.epoch ) );
	}

	// ignores epoch
	MediaTime& operator*=( const MediaTime &rhs ) {
		if( rhs.base == base ) { value *= rhs.value; base *= rhs.base; }
		else { value = value * rhs.base * rhs.value * base; base = base * rhs.base * base * rhs.base; }
		return *this;
	}

	// ignores epoch
	MediaTime& operator+=( const MediaTime &rhs ) {
		if( rhs.base == base ) { value += rhs.value; }
		else { value = value * rhs.base + rhs.value * base; base = base * rhs.base; }
		return *this;
	}

	MediaTime operator-() const {
		return MediaTime( -value, base, epoch );
	}

	friend std::ostream& operator<<( std::ostream &os, const MediaTime &mt );

	int64_t		value;
	int32_t		base;
	int64_t		epoch;
};

inline std::ostream& operator<<( std::ostream &os, const MediaTime &mt )
{
	if( mt.epoch == 0 )
		return os << mt.value << "/" << mt.base;
	else
		return os << mt.value << "/" << mt.base << "[" << mt.epoch << "]";
}

inline MediaTime operator"" _sec( long double seconds )
{
	return MediaTime( (int64_t)(seconds * MediaTime::DEFAULT_TIME_BASE ), MediaTime::DEFAULT_TIME_BASE );
}

inline MediaTime operator"" _sec( unsigned long long seconds )
{
	return MediaTime( (int64_t)(seconds * MediaTime::DEFAULT_TIME_BASE ), MediaTime::DEFAULT_TIME_BASE );
}

} // namespace cinder