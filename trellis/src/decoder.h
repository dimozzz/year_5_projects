#ifndef _DECODER_H_
#define _DECODER_H_

namespace
{
    double dist( signal_t const & a, signal_t const & b )
    {
        return abs( a - b );
    }
}


struct decoder_t
{
    void operator() ( signal_t const & signal )
    {
        layer_t layer( 8 );
        for ( int state = 0; state != 8; ++state )
        {
            for ( int q = 0; q != 4; ++q )
            {
                int tmp = encode( q, state ); 
                int new_state = tmp % 8;
                int y = tmp >> 3;
                int nearest_point = find_nearest( signal, y );
                layer[state].push_back( transition( dist( signal, code[nearest_point] ), new_state, pre_encoded_symbol[nearest_point >> 2] << 2 + nearest_point % 4 ) );
            }
        }
        lattice_.push_back( layer );
    }

    std::vector< int > get_result() const
    {
        std::vector< double > cost( 8 );

        std::vector< std::vector< int > > prev;
        std::vector< std::vector< int > > symbol;
        foreach ( layer_t const & layer, lattice_ )
        {
            prev.push_back( std::vector< int >() );
            symbol.push_back( std::vector< int >() );
            std::vector< double > next_cost( 8 );
            for ( int i = 0; i != 8; ++i )
                next_cost[i] = std::numeric_limits< double >::max();
            for ( int state = 0; state != 8; ++state )
            {
                foreach( transition const & tr, layer[state] )
                {
                    if ( make_min( next_cost[tr.end_state], cost[state] + tr.cost ) )
                    {
                        prev.back()[tr.end_state] = state;
                        symbol.back()[tr.end_state] = tr.symbol;
                    }
                }
            }
            cost.swap( next_cost );
        }
        int state = -1;
        double min_cost = std::numeric_limits< double >::max();
        for ( int s = 0; s != 8; ++s )
        {
            if ( make_min( min_cost, cost[s] ) )
                state = s;
        }
        std::vector< int > res;
        for ( size_t i = lattice_.size() - 1; i >= 0; --i )
        {
            res.push_back( symbol[i][state] );
            state = prev[i][state];
        }
        return res;
    }

private:
    struct transition
    {
        double cost;
        int end_state;
        int symbol;

        transition() {}

        transition( double c, int e, int s )
            : cost( c )
            , end_state( e )
            , symbol( s )
        {}
    };

    typedef std::vector< std::vector< transition > > layer_t;
    typedef std::vector< layer_t > lattice_t;

    int find_nearest( signal_t const & signal, int y ) const
    {
        double min_dist = std::numeric_limits< double >::max();
        int res = -1;

        foreach( int s, corresponding_signals[y] )
        {
            if ( make_min( min_dist, dist( signal, code[s] ) ) )
                res = s;
        }

        return res;
    }

private:
    lattice_t lattice_;
};

#endif /* _DECODER_H_ */
