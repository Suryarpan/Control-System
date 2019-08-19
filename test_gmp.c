#include <gmp.h>
#include <stdio.h>

#define N1 5
#define N2 4
#define LEXP 7

int main( void )
{
    int p_arr1[] = {5, 4, 3, 2, 1}, p_arr2[] = {4, 3, 2, 1};
    
    mpz_t val1, val2, large_val;
    mp_bitcnt_t exp = LEXP;

    mpz_init( val1 );
    mpz_set_ui( val1, p_arr1[ 0 ] );
    
    mpz_init( val2 );
    mpz_set_ui( val2, p_arr2[ 0 ] );

    mpz_init( large_val );
    mpz_set_ui( large_val, 0 );

    for (int i = 1; i < N1; i++)
    {
        mpz_mul_2exp(val1, val1, exp );
        mpz_add_ui(val1, val1, p_arr1[ i ]);
    }

    for (int i = 1; i < N2; i++)
    {
        mpz_mul_2exp(val2, val2, exp);
        mpz_add_ui(val2, val2, p_arr2[ i ]);
    }
    
    mpz_out_str( stdout, 10, val1 );
    printf("\n");
    mpz_out_str( stdout, 10, val2 );
    printf("\n");

    mpz_mul(large_val, val1, val2);
    mpz_out_str( stdout, 10, large_val );
    printf("\n");

    mpz_t rem;
    mpz_init_set_ui(rem, 0);

    for (int i = 0; mpz_cmp_ui(large_val, 0); i++)
    {
        mpz_tdiv_r_2exp(rem, large_val, exp);
        mpz_tdiv_q_2exp(large_val, large_val, exp);

        printf("[%d] = ", i);
        mpz_out_str( stdout, 10, rem );
        printf("\n");
    }

    mpz_clear( val1 );
    mpz_clear( val2 );
    mpz_clear( large_val );
    mpz_clear( rem );

    return 0;
}