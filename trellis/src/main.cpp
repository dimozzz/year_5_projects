#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <iostream>

#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

#include <boost/scoped_array.hpp>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>

#include "util.h"
#include "bin2complex.h"
#include "encoder.h"
#include "decoder.h"

using boost::mt19937;
using boost::normal_distribution;
using boost::variate_generator;

struct noise_generator_t
{
    noise_generator_t( double eps )
        : rng_( time( NULL ) )
        , sampler_( rng_, normal_distribution< double >( 1.0, eps ) ) 
    {
    }

    signal_t operator () ( signal_t const & s )
    {
        return s * sampler_(); 
    }

private:
    mt19937 rng_;
    variate_generator< mt19937&, normal_distribution< double > > sampler_;
};

void preprocess()
{
    for ( int state = 0; state != 8; ++state )
    {
        std::set< int > trs;
        for ( int last_y = 0; last_y != 8; ++last_y )
        {
            for ( int q = 0; q != 4; ++q )
            {
                int tmp = encode( q, state, last_y );
                preencoded_symbol[last_y * 64 + state * 8 + tmp % 8] = ( tmp / 8 ) * 4 + q;
                trs.insert( tmp );
            }
        }
        assert( trs.size() == 4 );
        transitions[state].assign( trs.begin(), trs.end() );
    }

    std::cout << "preprocessing finished" << std::endl;
}

std::string simulate( const double eps, const int N )
{
    encoder_t encoder;
    decoder_t decoder;
    noise_generator_t noise_generator( eps );
    
    std::vector< int > in( N );
    for ( size_t i = 0; i != N; ++i )
    {
        in[i] = rand() % 16;
        int encoded_in = encoder( in[i] );
        signal_t const & s = code[encoded_in]; 
        signal_t ns = noise_generator( s );
        decoder( ns );
    }
    
    std::cout << "encoding finished" << std::endl;

    std::vector< int > out = decoder.get_result();
    std::cout << "decoding finished" << std::endl;
    assert( out.size() == N );
    boost::scoped_array< char > res( new char[N + 1] );
    for ( size_t i = 0; i != N; ++i )
    {
        res[i] = in[i] == out[i] ? '_' : '*';
    }
    res[N] = '\0';
    return std::string( res.get() );
}

int main( int argc, char** argv )
{
    assert( argc == 3 );
    int N = boost::lexical_cast< int >( argv[1] );
    double eps = boost::lexical_cast< double >( argv[2] );
    preprocess();
    
    std::cout << simulate( eps, N ) << std::endl;
}
