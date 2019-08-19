#include <stdio.h>
#include "/home/pupu/C_Programs/fftw/include/fftw3.h"

#define N1 4
#define N2 3
#define N (N1 + N2 + 1)

int main( void )
{
    int ret_rank = N;
    double a_coeff[] = {5, 4, 3, 2, 1}, b_coeff[] = {4, 3, 2, 1};
    double * p_arr_a = fftw_alloc_real( ret_rank );
    double * p_arr_b = fftw_alloc_real( ret_rank );

    fftw_plan for_plan1 = fftw_plan_r2r_1d(ret_rank, p_arr_a, p_arr_a, FFTW_R2HC, FFTW_ESTIMATE);
    fftw_plan for_plan2 = fftw_plan_r2r_1d(ret_rank, p_arr_b, p_arr_b, FFTW_R2HC, FFTW_ESTIMATE);

    #pragma omp parallel for
    for (int i = 0; i < ret_rank; i++)
    {
        p_arr_a[ i ] = p_arr_b[ i ] = 0;

        if ( i <= N1 )
        {
            p_arr_a[ i ] = *( a_coeff + i );
        }

        if ( i <= N2 )
        {
            p_arr_b[ i ] = *( b_coeff + i );
        }
    }
    
    fftw_execute( for_plan1 );
    fftw_execute_r2r( for_plan1, p_arr_b, p_arr_b );

    fftw_destroy_plan( for_plan1 );
    fftw_destroy_plan( for_plan2 );

    p_arr_a[ 0 ] *= p_arr_b[ 0 ];
    if (!(ret_rank & 1))
    {
        p_arr_a[ ret_rank / 2 ] *= p_arr_b[ ret_rank / 2 ];
    }

    double tmp_r, tmp_i;

    for (int i = 1; i < ret_rank / 2; i++)
    {
        tmp_i = p_arr_a[ i ] * p_arr_b[ ret_rank - i ] + p_arr_a[ ret_rank - i ] * p_arr_b[ i ];
        p_arr_a[ i ] = p_arr_a[ i ] * p_arr_b[ i ] - p_arr_a[ ret_rank - i ] * p_arr_b[ ret_rank - i ];

        p_arr_a[ ret_rank - i ] = tmp_i;
    }

    fftw_free( p_arr_b );

    fftw_plan rev_plan = fftw_plan_r2r_1d(ret_rank, p_arr_a, p_arr_a, FFTW_HC2R, FFTW_ESTIMATE);
    fftw_execute( rev_plan );

    fftw_destroy_plan( rev_plan );

    // poly * ret_pol = poly_alloc( ret_rank, false, p_arr_a );

    fftw_free( p_arr_a );

    return 0;
}