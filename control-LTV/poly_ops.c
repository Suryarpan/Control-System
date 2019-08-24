#include <omp.h>
#include <stdlib.h>
#include <string.h>

#include "../ctrl_error.h"
#include "poly_ops.h"
#include "pmul.h"

extern int ctrl_errno;

/*
 * If coeff is NULL a blank polynomial is returned with all the elements initialized to zero.
 * The owner of the polynomial will be as supplied to poly_alloc().
 * 
 * If coeff is supplied it will be copied to the field coeff of the returned poly.
 * Number of coeff = 1 + rank
 */
poly * poly_alloc( int rank, bool owner, const double * coeff )
{
    if( rank < 0 )
    {
        ctrl_errno = CTRL_EDOM;
        return NULL;
    }

    if ( coeff == NULL )
    {
        poly * ret_pol = ( poly * ) malloc( sizeof( poly ) );
        if ( ret_pol == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->coeff = calloc( rank + 1, sizeof( double ) );
        if ( ret_pol->coeff == NULL)
        {
            free( ret_pol );
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->rank = rank;
        ret_pol->owner = owner;

        return ret_pol;
    }
    else
    {
        poly * ret_pol = ( poly * ) malloc( sizeof( poly ) );

        if( ret_pol == NULL ) // checks memory allocation
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->coeff = malloc( ( rank + 1 ) * sizeof( double ) );
        if ( ret_pol->coeff == NULL )
        {
            free( ret_pol );
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_pol->rank = rank;
        ret_pol->owner = owner;
        memcpy( ret_pol->coeff, coeff, ( 1 + rank ) * sizeof( double ) );

        return ret_pol;
    }
}

/*
 * ALWAYS CALL poly_free() TO FREE THE POLY POINTER.
 * DO NOT call free() on a poly pointer.
 */
void poly_free( poly * a )
{
    free( a->coeff );
    free( a );
}

double poly_eval( double x, poly * a )
{
    double val = a->coeff[ a->rank ];
    
    for ( int i = a->rank - 1; i >= 0; i-- )
    {
        val *= x;
        val += a->coeff[ i ];
    }

    return val;
}

/*
 * Adds two poly struct
 */
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
        return NULL;
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

/*
 * Subtracts poly b from poly a.
 * Order of subtraction is important.
 */
poly * poly_sub( poly * a, poly *b )
{
    int large = a->rank > b->rank ? a->rank : b->rank;

    poly * sub_res = poly_alloc( large, false, NULL);
    if ( sub_res == NULL)
    {
        return NULL;
    }
    
    #pragma omp parallel for
    for (int i = 0; i <= a->rank; i++)
    {
        sub_res->coeff[ i ] += a->coeff[ i ];
    }
    
    #pragma omp parallel for
    for (int i = 0; i <= b->rank; i++)
    {
        sub_res->coeff[ i ] -= b->coeff[ i ];
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

/*
 * Multiplies a double number to a polynomial
 */
poly * poly_nmul( double x, poly * a )
{
    if ( x == 1.0 )
    {
        return a;
    }

    poly * res = poly_alloc( a->rank, false, a->coeff );
    if ( res == NULL )
    {
        return NULL;
    }
    
    #pragma omp parallel for
    for (int i = 0; i <= a->rank; i++)
    {
        res->coeff[ i ] *= x;
    }

    if (!a->owner)
    {
        poly_free( a );
    }
    
    return res;
}

/*
 * Multiplies two polynomial. Refer to pmul.c for more information.
 */
poly * poly_pmul( poly * a, poly * b )
{
    poly * ret_pol = NULL;

    if ( a->rank == 0 )
    {
        ret_pol = poly_nmul( a->coeff[ 0 ], b );
    }
    else if ( b->rank == 0 )
    {
        ret_pol = poly_nmul( b->coeff[ 0 ], a );
    }
    else if ( a->rank == 1 && b->rank == 1 )
    {
        ret_pol = poly_mul_rank1( a, b );
    }
    else if ( a->rank < 16 && b->rank < 16 )
    {
        ret_pol = poly_mul_rank15( a, b );
    }
    else
    {
        ret_pol = poly_mul_genrl( a, b );
    }
    
    if ( !( a->owner ) )
    {
        poly_free( a );
    }
    if ( !( b->owner ) )
    {
        poly_free( b );
    }

    return ret_pol;
}