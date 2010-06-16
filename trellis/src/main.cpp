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

std::string simulate( const double eps, const int N )
{
    std::set< int > incoming_y[8];
    for ( int state = 0; state != 8; ++ state )
    {
        for ( int last_y = 0; last_y != 8; ++last_y )
        {
            for ( int q = 0; q != 4; ++q )
            {
                int tmp = encode( q, state, last_y );
                int new_state = tmp % 8;
                int y = tmp / 8;
                incoming_y[new_state].insert( y );
            }
        }
    }
    for ( size_t i = 0; i != 64; ++i )
    {
        preencoded_symbol[i] = -1;
    }
    for ( int state = 0; state != 8; ++state )
    {
        assert( incoming_y[state].size() == 4 );
        std::set< int > trs;
        std::map< int, int > tr2y;
        foreach ( int last_y, incoming_y[state] )
        {
            for ( int q = 0; q != 4; ++q )
            {
                int tmp = encode( q, state, last_y );
                int tr = (tmp / 8) * 8 + state;
                if ( preencoded_symbol[tr] != -1 )
                {
                    if( preencoded_symbol[tr] != q )
                    {
                        std::cout   << "state = " << state 
                                    << ":\t{ (" << tr2y[tr] << ", " << preencoded_symbol[tr] 
                                    << "), (" << last_y << ", " << q << ") }" << std::endl;
                    }
                }
                tr2y[tr] = last_y;
				preencoded_symbol[tr] = q;
                trs.insert( tmp );
            }
        }
        assert( trs.size() == 4 );
        transitions[state].assign( trs.begin(), trs.end() );
    }

    std::cout << "preprocessing finished" << std::endl;

    encoder_t encoder;
    decoder_t decoder;
    noise_generator_t noise_generator( eps );
    
    std::vector< int > in( N );
    std::vector< int > encoder_state( N );
    for ( size_t i = 0; i != N; ++i )
    {
        in[i] = rand() % 16;
        encoder_state[i] = encoder.state_;
        int encoded_in = encoder( in[i] );
        signal_t const & s = code[encoded_in]; 
        decoder( s );
    }
    //std::copy( encoder_state.rbegin(), encoder_state.rend(), 
    //            std::ostream_iterator< int >( std::cout, " " ) );
    
    std::cout << "coding finished" << std::endl;

    std::vector< int > out = decoder.get_result();
    assert( out.size() == N );
    boost::scoped_array< char > res( new char[N + 1] );
    for ( size_t i = 0; i != N; ++i )
    {
        res[i] = in[i] == out[i] ? '_' : '*';
        assert( in[i] % 4 == out[i] % 4 );
        //std::cout << in[i] << "\t" << out[i] << std::endl;
    }
    res[N] = '\0';
    return std::string( res.get() );
}

int main()
{
    std::ifstream conf( "config.txt" );
    double eps;
    int N;
    conf >> eps >> N;
    std::cout << simulate( eps, N );
}
