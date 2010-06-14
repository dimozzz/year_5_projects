#ifndef _UTIL_H_
#define _UTIL_H_

template< typename T >
bool make_min( T & a, T const & b )
{
	if ( b < a )
	{
		a = b;
		return true;
	}
	return false;
}

#endif /* _UTIL_H_ */
