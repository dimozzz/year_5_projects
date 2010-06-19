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
    decoder_t( STANDARD standard )
        : standard_( standard )
    {}

    void operator() ( signal_t const & signal )
    {
        layer_t layer( 8 );
        for ( int state = 0; state != 8; ++state )
        {
            foreach ( int tr, transitions[state] )
            {
                const int new_state = tr % 8;
                const int y = tr / 8;
                const int nearest = find_nearest( signal, y );
                int last_bits;
                switch ( standard_ )
                {
                case V32:
                    last_bits = nearest % 4;
                    break;
                case V32bis:
                    last_bits = nearest % 16;
                    break;
                }
                layer[state].push_back( transition( dist( signal, code[standard_][nearest] ), new_state, last_bits ) );
            }
        }
        lattice_.push_back( layer );
    }

    std::vector< int > get_result() const
    {
        std::vector< double > cost( 8, 0 );

        std::vector< std::vector< int > > prev;
        std::vector< std::vector< int > > last_bits;
        foreach ( layer_t const & layer, lattice_ )
        {
            assert( layer.size() == 8 );
            prev.push_back( std::vector< int >( 8 ) );
            last_bits.push_back( std::vector< int >( 8 ) );
            std::vector< double > next_cost( 8, std::numeric_limits< double >::max() );
            for ( int state = 0; state != 8; ++state )
            {
                foreach( transition const & tr, layer[state] )
                {
                    if ( make_min( next_cost[tr.end_state], cost[state] + tr.cost ) )
                    {
                        prev.back()[tr.end_state] = state;
                        last_bits.back()[tr.end_state] = tr.last_bits;
                    }
                }
            }
            cost.swap( next_cost );
        }
        const size_t N = lattice_.size();
        std::vector< int > state( N + 1 );
        double min_cost = std::numeric_limits< double >::max();
        for ( int s = 0; s != 8; ++s )
        {
            if ( make_min( min_cost, cost[s] ) )
                state.back() = s;
        }
        std::vector< int > lbs( N );
        for ( size_t i = N; i > 0; --i )
        {
            state[i - 1] = prev[i - 1][state[i]];
            lbs[i - 1] = last_bits[i - 1][state[i]];
        }
        std::vector< int > res( N );
        int last_y = state.front();
        for ( size_t i = 0; i != N; ++i )
        {
            int tr = preencoded_symbol[last_y * 64 + state[i] * 8 + state[i + 1]];
            switch ( standard_ )
            {
            case V32:
                res[i] = ( tr % 4 ) * 4 + lbs[i];
                break;
            case V32bis:
                res[i] = ( tr % 4 ) * 16 + lbs[i];
                break;
            }
            last_y = tr / 4;
        }
        return res;
    }

private:
    struct transition
    {
        double cost;
        int end_state;
        int last_bits;

        transition() {}

        transition( double c, int e, int lbs )
            : cost( c )
            , end_state( e )
            , last_bits( lbs )
        {}
    };

    typedef std::vector< std::vector< transition > > layer_t;
    typedef std::vector< layer_t > lattice_t;

    int find_nearest( signal_t const & signal, const int y ) const
    {
        double min_dist = std::numeric_limits< double >::max();
        int res = -1;

        switch ( standard_ )
        {
        case V32:
            for ( int s = y * 4; s != y * 4 + 4; ++s )
            {
                if ( make_min( min_dist, dist( signal, code[V32][s] ) ) )
                    res = s;
            }
            break;
        case V32bis:
            for ( int s = y * 16; s != y * 16 + 16; ++s )
            {
                if ( make_min( min_dist, dist( signal, code[V32bis][s] ) ) )
                    res = s;
            }
            break;
        }

        return res;
    }

private:
    STANDARD  standard_;
    lattice_t lattice_;
};

#endif /* _DECODER_H_ */
