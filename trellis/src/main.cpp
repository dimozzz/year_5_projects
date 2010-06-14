#include <vector>
#include <cstdlib>
#include <limits>
#include <iostream>

#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

#include <boost/scoped_array.hpp>

#include "util.h"
#include "bin2complex.h"
#include "encoder.h"
#include "decoder.h"

signal_t noise( signal_t const & s )
{
    return s * ( ((double) rand()) / RAND_MAX );
}

const int N = 239;

std::string simulate()
{
    for ( int in = 0; in != 4; ++in )
    {
        for ( int state = 0; state != 8; ++state )
        {
            pre_encoded_symbol[encode( in, state ) >> 3] = in; 
        }
    }
    encoder_t encoder;
    decoder_t decoder;
    
    std::vector< int > in( N );
    for ( size_t i = 0; i != N; ++i )
    {
        in[i] = rand() % 16;
        decoder( noise( code[encoder( in[i] )] ) );
    }
    std::vector< int > out = decoder.get_result();
    assert( out.size() == N );
    boost::scoped_array< char > res( new char[N + 1] );
    for ( size_t i = 0; i != N; ++i )
        res[i] = in[i] == out[i] ? '_' : '*';
    res[N] = '\0';
    return std::string( res.get() );
}

int main()
{
    std::cout << simulate();
}
