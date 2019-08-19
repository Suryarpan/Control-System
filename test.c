#include <stdio.h>
#include <stdio.h>
#include "/home/pupu/C_Programs/fftw/include/fftw3.h"

#define N1 5
#define N2 4
#define N (N1 + N2 - 1)

int main( void )
{
    double real_in_1[N]={5, 4, 3, 2, 1, }, real_in_2[N]={4, 3, 2, 1, }, real_out[N];

    fftw_complex * complex_out_1 = fftw_alloc_complex( 1 + N/2 );
    fftw_complex * complex_out_2 = fftw_alloc_complex( 1 + N/2 );
    fftw_complex * complex_in = fftw_alloc_complex( 1 + N/2 );

    fftw_plan for_plan_1 = fftw_plan_dft_r2c_1d(N, real_in_1, complex_out_1, FFTW_MEASURE);
    fftw_plan for_plan_2 = fftw_plan_dft_r2c_1d(N, real_in_2, complex_out_2, FFTW_MEASURE);
    
    fftw_execute( for_plan_1 );
    fftw_execute( for_plan_2 );

    for (int i = 0; i < 1 + N/2; i++)
    {
        complex_in[i][0] = complex_out_1[i][0]*complex_out_2[i][0] - complex_out_1[i][1]*complex_out_2[i][1];
        complex_in[i][1] = complex_out_1[i][0]*complex_out_2[i][1] + complex_out_1[i][1]*complex_out_2[i][0];
    }
    

    fftw_plan rev_plan = fftw_plan_dft_c2r_1d(N, complex_in, real_out, FFTW_MEASURE);

    fftw_execute(rev_plan);

    for (int i = 0; i < N; i++)
    {
        printf("out[ %d ] = %g\n", i, real_out[i]/N);
    }
    
    fftw_free( complex_out_1 );
    fftw_free( complex_out_2 );
    fftw_free(complex_in);
    fftw_destroy_plan( for_plan_1 );
    fftw_destroy_plan( for_plan_2 );
    fftw_destroy_plan( rev_plan );

    return 0;
}