#include <omp.h>
#include <stdbool.h>
#include <string.h>

#include "pmul.h"
#include "fftw3.h"
#include "ctrl_error.h"

// Multiplies two rank 1 poly [ O(1) ].
poly * poly_mul_rank1( poly * a, poly * b)
{
    poly * c = poly_alloc( a->rank + b->rank, false, NULL );
    if ( c == NULL)
    {
        return NULL;
    }

    c->coeff[ 0 ] = a->coeff[ 0 ] * b->coeff[ 0 ];
    c->coeff[ 1 ] = a->coeff[ 0 ] * b->coeff[ 1 ] + a->coeff[ 1 ] * b->coeff[ 0 ];
    c->coeff[ 2 ] = a->coeff[ 1 ] * b->coeff[ 1 ];

    return c;
}

// Multiplies two poly of medium rank (upto 15) [ O(n^2) ].
poly * poly_mul_rank15( poly * a, poly * b )
{
    poly * res = poly_alloc( a->rank + b->rank, false, NULL);
    if (res == NULL)
    {
        return NULL;
    }

    #pragma omp parallel for
    for (int i = 0; i <= a->rank; i++)
    {
        #pragma omp parallel for
        for (int j = 0; i <= b->rank; j++)
        {
            res->coeff[ i + j ] += ( a->coeff[ i ] * b->coeff[ i ] );
        }   
    }
    
    return res;
}

// Multiplies two polynomial of any rank [ O(nlogn) ].
// Uses twice the memrory of the returned polynomial.
poly * poly_mul_genrl( poly * a, poly * b )
{
    int arr_len = a->rank + b->rank + 1;

    // initialise two array to hold the DFTs
    double * p_arr_a = fftw_alloc_real( arr_len );
    if ( p_arr_a == NULL )
    {
        return NULL;
    }
    
    double * p_arr_b = fftw_alloc_real( arr_len );
    if ( p_arr_b == NULL )
    {
        return NULL;
    }

    fftw_plan for_plan1 = fftw_plan_r2r_1d( arr_len, p_arr_a, p_arr_a, FFTW_R2HC, FFTW_ESTIMATE );

    // copy the original coeff data
    memcpy( p_arr_a, a->coeff, ( 1 + a->rank ) * sizeof( double ) );
    memcpy( p_arr_b, b->coeff, ( 1 + b->rank ) * sizeof( double ) );

    fftw_execute( for_plan1 );
    fftw_execute_r2r( for_plan1, p_arr_b, p_arr_b );

    fftw_destroy_plan( for_plan1 );

    // Multiplication of data points from DFTs
    p_arr_a[ 0 ] *= p_arr_b[ 0 ];
    // if arr_len is even
    !(arr_len & 1) ? p_arr_a[ arr_len / 2 ] *= p_arr_b[ arr_len / 2 ] : 0;

    double tmp_i;
    // half complex array for the inverse DFT
    for (int i = 0; i < arr_len / 2; i++)
    {
        tmp_i = p_arr_a[ i ] * p_arr_b[ arr_len - i ] + p_arr_a[ arr_len - i ] * p_arr_b[ i ];
        p_arr_a[ i ] = p_arr_a[ i ] * p_arr_b[ i ] - p_arr_a[ arr_len - i ] * p_arr_b[ arr_len - i ];

        p_arr_a[ arr_len - i ] = tmp_i;
    }

    fftw_free( p_arr_b );

    // inverse DFT
    fftw_plan rev_plan = fftw_plan_r2r_1d(arr_len, p_arr_a, p_arr_a, FFTW_HC2R, FFTW_ESTIMATE);
    fftw_execute( rev_plan );
    fftw_destroy_plan( rev_plan );

    double scale_fac = 1.0 / arr_len;

    #pragma omp parallel for
    for (int i = 0; i < arr_len; i++)
    {
        p_arr_a[ i ] *= scale_fac;
    }

    poly * ret_pol = poly_alloc( arr_len - 1, false, p_arr_a );
    if ( ret_pol == NULL )
    {
        return NULL;
    }

    fftw_free( p_arr_a );

    return ret_pol;
}