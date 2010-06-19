#include <complex>

typedef std::complex< double > signal_t;

std::vector< signal_t > code[2];
std::vector< int > transitions[8];

int bin2dec( const char * bin )
{
    int res = 0;
    for ( int i = 0; i != 7; ++i )
    {
        res *= 2;
        res += ( bin[i] - '0' );
    }
    return res;
}

enum STANDARD
{
    V32 = 0, V32bis
};  

#define BIN_2_COMPLEX( bin, re, im ) code[V32bis][bin2dec( bin )] = signal_t( re, im );

void init_code()
{
    signal_t v32[32] = 
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
    code[V32].assign( v32, v32 + 32 );

    code[V32bis].assign( 128, signal_t( 239, 239 ) );
    
    BIN_2_COMPLEX( "0110000", -2,  9 );
    BIN_2_COMPLEX( "0111000",  2,  9 );

    BIN_2_COMPLEX( "1100000", -3,  8 );
    BIN_2_COMPLEX( "1111001", -1,  8 );
    BIN_2_COMPLEX( "1101000",  1,  8 );
    BIN_2_COMPLEX( "1110001",  3,  8 );

    BIN_2_COMPLEX( "0010011", -4,  7 );
    BIN_2_COMPLEX( "0101001", -2,  7 );
    BIN_2_COMPLEX( "0010111",  0,  7 );
    BIN_2_COMPLEX( "0100001",  2,  7 );
    BIN_2_COMPLEX( "0010101",  4,  7 );

    BIN_2_COMPLEX( "1011101", -5,  6 );
    BIN_2_COMPLEX( "1000011", -3,  6 );
    BIN_2_COMPLEX( "1011111", -1,  6 );
    BIN_2_COMPLEX( "1000111",  1,  6 );
    BIN_2_COMPLEX( "1011011",  3,  6 );
    BIN_2_COMPLEX( "1000101",  5,  6 );

    BIN_2_COMPLEX( "0110101", -6,  5 );
    BIN_2_COMPLEX( "0001101", -4,  5 );
    BIN_2_COMPLEX( "0110100", -2,  5 );
    BIN_2_COMPLEX( "0001111",  0,  5 );
    BIN_2_COMPLEX( "0111100",  2,  5 );
    BIN_2_COMPLEX( "0001011",  4,  5 );
    BIN_2_COMPLEX( "0111101",  6,  5 );

    BIN_2_COMPLEX( "1100101", -7,  4 );
    BIN_2_COMPLEX( "1111011", -5,  4 );
    BIN_2_COMPLEX( "1100100", -3,  4 );
    BIN_2_COMPLEX( "1111010", -1,  4 );
    BIN_2_COMPLEX( "1101100",  1,  4 );
    BIN_2_COMPLEX( "1110010",  3,  4 );
    BIN_2_COMPLEX( "1101101",  5,  4 );
    BIN_2_COMPLEX( "1110011",  7,  4 );

    BIN_2_COMPLEX( "0010001", -8,  3 );
    BIN_2_COMPLEX( "0101011", -6,  3 );
    BIN_2_COMPLEX( "0010010", -4,  3 );
    BIN_2_COMPLEX( "0101010", -2,  3 );
    BIN_2_COMPLEX( "0010110",  0,  3 );
    BIN_2_COMPLEX( "0100010",  2,  3 );
    BIN_2_COMPLEX( "0010100",  4,  3 );
    BIN_2_COMPLEX( "0100011",  6,  3 );
    BIN_2_COMPLEX( "0010000",  8,  3 );

    BIN_2_COMPLEX( "1011000", -9,  2 );
    BIN_2_COMPLEX( "1000001", -7,  2 );
    BIN_2_COMPLEX( "1011100", -5,  2 );
    BIN_2_COMPLEX( "1000010", -3,  2 );
    BIN_2_COMPLEX( "1011110", -1,  2 );
    BIN_2_COMPLEX( "1000110",  1,  2 );
    BIN_2_COMPLEX( "1011010",  3,  2 );
    BIN_2_COMPLEX( "1000100",  5,  2 );
    BIN_2_COMPLEX( "1011001",  7,  2 );
    BIN_2_COMPLEX( "1000000",  9,  2 );

    BIN_2_COMPLEX( "0001000", -8,  1 );
    BIN_2_COMPLEX( "0110111", -6,  1 );
    BIN_2_COMPLEX( "0001100", -4,  1 );
    BIN_2_COMPLEX( "0110110", -2,  1 );
    BIN_2_COMPLEX( "0001110",  0,  1 );
    BIN_2_COMPLEX( "0111110",  2,  1 );
    BIN_2_COMPLEX( "0001010",  4,  1 );
    BIN_2_COMPLEX( "0111111",  6,  1 );
    BIN_2_COMPLEX( "0001001",  8,  1 );

    BIN_2_COMPLEX( "1100111", -7,  0 );
    BIN_2_COMPLEX( "1111111", -5,  0 );
    BIN_2_COMPLEX( "1100110", -3,  0 );
    BIN_2_COMPLEX( "1111110", -1,  0 );
    BIN_2_COMPLEX( "1101110",  1,  0 );
    BIN_2_COMPLEX( "1110110",  3,  0 );
    BIN_2_COMPLEX( "1101111",  5,  0 );
    BIN_2_COMPLEX( "1110111",  7,  0 );

    BIN_2_COMPLEX( "0011001", -8, -1 );
    BIN_2_COMPLEX( "0101111", -6, -1 );
    BIN_2_COMPLEX( "0011010", -4, -1 );
    BIN_2_COMPLEX( "0101110", -2, -1 );
    BIN_2_COMPLEX( "0011110",  0, -1 );
    BIN_2_COMPLEX( "0100110",  2, -1 );
    BIN_2_COMPLEX( "0011100",  4, -1 );
    BIN_2_COMPLEX( "0100111",  6, -1 );
    BIN_2_COMPLEX( "0011000",  8, -1 );

    BIN_2_COMPLEX( "1010000", -9, -2 );
    BIN_2_COMPLEX( "1001001", -7, -2 );
    BIN_2_COMPLEX( "1010100", -5, -2 );
    BIN_2_COMPLEX( "1001010", -3, -2 );
    BIN_2_COMPLEX( "1010110", -1, -2 );
    BIN_2_COMPLEX( "1001110",  1, -2 );
    BIN_2_COMPLEX( "1010010",  3, -2 );
    BIN_2_COMPLEX( "1001100",  5, -2 );
    BIN_2_COMPLEX( "1010001",  7, -2 );
    BIN_2_COMPLEX( "1001000",  9, -2 );

    BIN_2_COMPLEX( "0000000", -8, -3 );
    BIN_2_COMPLEX( "0110011", -6, -3 );
    BIN_2_COMPLEX( "0000100", -4, -3 );
    BIN_2_COMPLEX( "0110010", -2, -3 );
    BIN_2_COMPLEX( "0000110",  0, -3 );
    BIN_2_COMPLEX( "0111010",  2, -3 );
    BIN_2_COMPLEX( "0000010",  4, -3 );
    BIN_2_COMPLEX( "0111011",  6, -3 );
    BIN_2_COMPLEX( "0000001",  8, -3 );

    BIN_2_COMPLEX( "1100011", -7, -4 );
    BIN_2_COMPLEX( "1111101", -5, -4 );
    BIN_2_COMPLEX( "1100010", -3, -4 );
    BIN_2_COMPLEX( "1111100", -1, -4 );
    BIN_2_COMPLEX( "1101010",  1, -4 );
    BIN_2_COMPLEX( "1110100",  3, -4 );
    BIN_2_COMPLEX( "1101011",  5, -4 );
    BIN_2_COMPLEX( "1110101",  7, -4 );

    BIN_2_COMPLEX( "0101101", -6, -5 );
    BIN_2_COMPLEX( "0011011", -4, -5 );
    BIN_2_COMPLEX( "0101100", -2, -5 );
    BIN_2_COMPLEX( "0011111",  0, -5 );
    BIN_2_COMPLEX( "0100100",  2, -5 );
    BIN_2_COMPLEX( "0011101",  4, -5 );
    BIN_2_COMPLEX( "0100101",  6, -5 );

    BIN_2_COMPLEX( "1010101", -5, -6 );
    BIN_2_COMPLEX( "1001011", -3, -6 );
    BIN_2_COMPLEX( "1010111", -1, -6 );
    BIN_2_COMPLEX( "1001111",  1, -6 );
    BIN_2_COMPLEX( "1010011",  3, -6 );
    BIN_2_COMPLEX( "1001101",  5, -6 );

    BIN_2_COMPLEX( "0000101", -4, -7 );
    BIN_2_COMPLEX( "0110001", -2, -7 );
    BIN_2_COMPLEX( "0000111",  0, -7 );
    BIN_2_COMPLEX( "0111001",  2, -7 );
    BIN_2_COMPLEX( "0000011",  4, -7 );

    BIN_2_COMPLEX( "1100001", -3, -8 );
    BIN_2_COMPLEX( "1111000", -1, -8 );
    BIN_2_COMPLEX( "1101001",  1, -8 );
    BIN_2_COMPLEX( "1110000",  3, -8 );

    BIN_2_COMPLEX( "0101000", -2, -9 );
    BIN_2_COMPLEX( "0100000",  2, -9 );

    for ( size_t i = 0; i != code[V32bis].size(); ++i )
    {
        if ( code[V32bis][i] == signal_t( 239, 239 ) )
        {
            std::cout << i << std::endl;
            assert( false );
        }
    }
}

int encode( int, int, int );

int preencoded_symbol[1 << 9];

void preprocess()
{
    init_code();

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


