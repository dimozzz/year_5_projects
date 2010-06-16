#include <complex>

typedef std::complex< double > signal_t;

signal_t code[32] =
{
    signal_t( -4, 1  ),
    signal_t( 0 , -3 ),
    signal_t( 0 , 1  ),
    signal_t( 4 , 1  ),
    signal_t( 4 , -1 ),
    signal_t( 0 , 3  ),
    signal_t( 0 , -1 ),
    signal_t( -4, -1 ),
    signal_t( -2, 3  ),
    signal_t( -2, -1 ),
    signal_t( 2 , 3  ),
    signal_t( 2 , -1 ),
    signal_t( 2 , -3 ),
    signal_t( 2 , 1  ),
    signal_t( -2, -3 ),
    signal_t( -2, 1  ),
    signal_t( -3, -2 ),
    signal_t( 1 , -2 ),
    signal_t( -3,  2 ),
    signal_t( 1 ,  2 ),
    signal_t( 3,   2 ),
    signal_t( -1,  2 ),
    signal_t( 3,  -2 ),
    signal_t( -1, -2 ),
    signal_t( 1 ,  4 ),
    signal_t( -3,  0 ),
    signal_t( 1 ,  0 ),
    signal_t( 1 , -4 ),
    signal_t( -1, -4 ),
    signal_t( 3 ,  0 ),
    signal_t( -1,  0 ),
    signal_t( -1,  4 )
};

std::vector< int > transitions[8];
