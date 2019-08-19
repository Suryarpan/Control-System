#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../ctrl_error.h"
#include "poly_ops.h"
#include "pmul.h"

extern int ctrl_errno;

poly * poly_alloc( int rank, bool owner, const double * coeff )
{
    if( rank < 0 )
    {
        ctrl_errno = CTRL_EDOM;
        return NULL;
    }

    if( owner )
    {
        poly * ret_pol = ( poly * ) malloc( sizeof( poly ) );

        if( ret_pol == NULL ) // checks memory allocation
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->rank = rank;
        ret_pol->owner = true;
        ret_pol->coeff = coeff;

        return ret_pol;
    }

    else if ( coeff == NULL )
    {
        // struct hacking
        poly * ret_pol = ( poly * ) malloc( sizeof( poly ) + ( 1 + rank ) * sizeof( double ) );
        
        if( ret_pol == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->rank = rank;
        ret_pol->owner = false;

        return ret_pol;
    }

    else
    {
        // struct hacking
        poly * ret_pol = ( poly * ) malloc( sizeof( poly ) + ( 1 + rank ) * sizeof( double ) );

        if( ret_pol == NULL ) // checks memory allocation
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->rank = rank;
        ret_pol->owner = false;
        memcpy( ret_pol->coeff, coeff, ( 1 + rank ) * sizeof( double ) );

        return ret_pol;
    }
}

void poly_free( poly * a )
{
    free( a );
}

poly * poly_add( poly * a, poly * b )
{
    poly * large, * small;

    if( a->rank > b->rank )
    {
        large = a;
        small = b;
    }
    else
    {
        large = b;
        small = a;
    }
    
    poly * add_res = poly_alloc( large->rank, false, large->coeff );
    if ( add_res == NULL)
    {
        int errnum = ctrl_errno;
        error_handle( errnum );
    }
    

    #pragma omp parallel for
    for ( int i = 0; i <= small->rank; i++ )
    {
        add_res->coeff[ i ] += small->coeff[ i ];
    }

    if ( !a->owner )
    {
        poly_free( a );
    }

    if( !b->owner )
    {
        poly_free( b );
    }

    return add_res;
}

poly * poly_sub( poly * a, poly *b )
{
    poly * sub_res = NULL;

    if ( a->rank >= b->rank )
    {
        sub_res = poly_alloc( a->rank, false, a->coeff );
        if ( sub_res == NULL )
        {
            int errnum = ctrl_errno;
            error_handle( errnum );
        }

        #pragma omp parallel for
        for ( int i = 0; i <= b->rank; i++ )
        {
            sub_res->coeff[ i ] -= b->coeff[ i ];
        }
    }

    else
    {
        sub_res = poly_alloc( b->rank, false, b->coeff);
        if ( sub_res == NULL )
        {
            int errnum = ctrl_errno;
            error_handle( errnum );
        }
        
        #pragma omp parallel for
        for (int i = 0; i <= a->rank; i++)
        {
            sub_res->coeff[ i ] = a->coeff[ i ] - sub_res->coeff[ i ];
        }

        #pragma omp parallel for
        for (int i = a->rank + 1; i <= b->rank; i++)
        {
            sub_res->coeff[ i ] *= -1.0;
        }
    }

    if ( !a->owner )
    {
        poly_free( a );
    }
    
    if (!b->owner)
    {
        poly_free( b );
    }
    
    return sub_res;
}

poly * poly_nmul( double x, poly * a )
{
    if ( x == 1.0 )
    {
        return a;
    }

    poly * res = poly_alloc( a->rank, false, a->coeff );
    if ( res == NULL )
    {
        int errnum = ctrl_errno;
        error_handle( errnum );
    }
    
    #pragma omp parallel for
    for (int i = 0; i <= a->rank; i++)
    {
        res->coeff[ i ] *= x;
    }
    
}

poly * poly_pmul( poly * a, poly * b )
{
    poly * c;
}