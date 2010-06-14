#ifndef _ENCODER_H_
#define _ENCODER_H_

int pre_encoded_symbol[8];

/**
 * in - 2 bits
 * state - 3 bits
 * returns 6 bits: out << 3 + new registers
 */
int encode( int in, int state )
{
}


struct encoder_t
{ 
    encoder_t() 
        : state_( rand() % 8 )
    {}

    /* 
     * in - 4 bits
     * returns 5 bits
     */
    int operator () ( int in )
    {
        int tmp = encode( in >> 2, state_ );
        state_ = tmp % 8;
        return (tmp >> 3) << 2 + in % 4;
    }

private:
    int state_;
};

#endif /* _ENCODER_H_ */
