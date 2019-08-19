#include <omp.h>
#include <stdbool.h>

#include "pmul.h"
#include "fftw3.h"

poly * poly_mul_rank1( poly * a, poly * b)
{

}

poly * poly_mul_rank2( poly * a, poly * b)
{

}

poly * poly_mul_rank3( poly * a, poly * b )
{

}

poly * poly_mul_rank7( poly * a, poly * c )
{

}

poly * poly_mul_genrl( poly * a, poly * b )
{
    int arr_len = a->rank + b->rank + 1;
    double * p_arr_a = fftw_alloc_real( arr_len );
    double * p_arr_b = fftw_alloc_real( arr_len );

    fftw_plan for_plan1 = fftw_plan_r2r_1d( arr_len, p_arr_a, p_arr_a, FFTW_R2HC, FFTW_ESTIMATE );

    #pragma omp parallel for
    for (int i = 0; i < arr_len; i++)
    {
        if ( i <= a->rank )
        {
            p_arr_a[ i ] = *( a->coeff + i );
        }

        if ( i <= b->rank )
        {
            p_arr_b[ i ] = *( b->coeff + i );
        }
        
        p_arr_a[ i ] = p_arr_b[ i ] = 0;
    }
    
    fftw_execute( for_plan1 );
    fftw_execute_r2r( for_plan1, p_arr_b, p_arr_b );

    fftw_destroy_plan( for_plan1 );

    p_arr_a[ 0 ] *= p_arr_b[ 0 ];
    if (!(arr_len & 1))
    {
        p_arr_a[ arr_len / 2 ] *= p_arr_b[ arr_len / 2 ];
    }

    double tmp_i;

    for (int i = 0; i < arr_len / 2; i++)
    {
        tmp_i = p_arr_a[ i ] * p_arr_b[ arr_len - i ] + p_arr_a[ arr_len - i ] * p_arr_b[ i ];
        p_arr_a[ i ] = p_arr_a[ i ] * p_arr_b[ i ] - p_arr_a[ arr_len - i ] * p_arr_b[ arr_len - i ];

        p_arr_a[ arr_len - i ] = tmp_i;
    }

    fftw_free( p_arr_b );

    fftw_plan rev_plan = fftw_plan_r2r_1d(arr_len, p_arr_a, p_arr_a, FFTW_HC2R, FFTW_ESTIMATE);
    fftw_execute( rev_plan );
    fftw_destroy_plan( rev_plan );

    poly * ret_pol = poly_alloc( arr_len - 1, false, p_arr_a );

    fftw_free( p_arr_a );

    return ret_pol;
}