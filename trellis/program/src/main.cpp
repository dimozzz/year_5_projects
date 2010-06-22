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
#include "preprocessing.h"
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

std::string simulate( const double eps, const int N, STANDARD standard )
{
    encoder_t encoder( standard );
    decoder_t decoder( standard );
    noise_generator_t noise_generator( eps );
    
    std::vector< int > in( N );
    for ( size_t i = 0; i != N; ++i )
    {
        switch ( standard )
        {
        case V32:
            in[i] = rand() % 16;
            break;
        case V32bis:
            in[i] = rand() % 64;
            break;
        }
        int encoded_in = encoder( in[i] );
        signal_t const & s = code[standard][encoded_in]; 
        signal_t ns = noise_generator( s );
        decoder( ns );
    }
    
    //std::cout << "encoding finished" << std::endl;

    std::vector< int > out = decoder.get_result();
    //std::cout << "decoding finished" << std::endl;
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
    assert( argc == 4 );
    int N = boost::lexical_cast< int >( argv[1] );
    double eps = boost::lexical_cast< double >( argv[2] );
    preprocess();
    STANDARD standard;
    if ( argv[3] == std::string( "V32" ) )
        standard = V32;
    else if ( argv[3] == std::string( "V32bis" ) )
        standard = V32bis;
    else
    {
        std::cerr << "erroneous standard!" << std::endl;
        return 1;
    }
   
	freopen("output.txt", "w", stdout);
	for(double eps2 = 0.01;eps2 < 1.5;eps2 += 0.04){
		std::string s = simulate(eps2, N, standard);
		int res = 0;
		for(int i = 0;i < s.length();i ++){
			if(s[i] == '_')res ++;
		}
		std::cout << eps2 << " " << 1.0 * res / N << std::endl;
	}
	
    //std::cout << simulate( eps, N, standard ) << std::endl;
}
