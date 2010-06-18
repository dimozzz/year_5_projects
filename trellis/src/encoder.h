#ifndef _ENCODER_H_
#define _ENCODER_H_

int preencoded_symbol[1 << 9];

void subdiv( int a, int* x, size_t n )
{
    for ( ; n > 0; --n )
    {
        x[n-1] = a % 2;
        a /= 2;
    }
}

int unite( int* x, size_t n )
{
    int res = 0;
    for ( size_t i = 0; i != n; ++i )
    {
        res *= 2;
        res += x[i];
    }
    return res;
}

/**
 * in - 2 bits
 * state - 3 bits
 * last_y - 3 bits
 * returns 6 bits: (out << 3) + new state
 */
int encode( int in, int state, int last_y )
{
    int ly[3];
    subdiv( last_y, ly, 3 );
    int ls[3];
    subdiv( state, ls, 3 );

    int q[2];
    subdiv( in, q, 2 );

    int y[3];
    y[0] = ls[0];
    y[1] = ( q[0] + ly[1]) % 2;
    y[2] = ( (q[0] & ly[1]) + ly[2] + q[1] ) % 2;

    int s[3];
    s[2] = y[0];
    s[1] = ( y[1] + y[2] + ls[2] + 
                ( ( ( ls[1] + y[2] ) % 2 ) & ls[0] )
           ) % 2;
    s[0] = ( ls[1] + y[2] + (y[1] & ls[0]) ) % 2;
	
    return unite( y, 3 ) * 8 + unite( s, 3 );
}


struct encoder_t
{ 
    encoder_t() 
        : state_( rand() % 8 )
        , last_y_( state_ )
    {}

    /* 
     * in - 4 bits
     * returns 5 bits
     */
    int operator () ( int in )
    {
        int tmp = encode( in / 4, state_, last_y_ );
        state_ = tmp % 8;
        last_y_ = tmp / 8;
        return last_y_ * 4 + in % 4;
    }

private:
    int state_;
    int last_y_;
};

#endif /* _ENCODER_H_ */
