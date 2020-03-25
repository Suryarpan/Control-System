#include <stdlib.h>
#include <stdbool.h>

#include "solver_LTI.h"
#include "../ctrl_error.h"
#include "../control-LTI/ss_model.h"

// File Operation

// Solvers
/*
 * x_(n+1) = (I + A.h(I + A.h/2(I + A.h/3(I + A.h/4)))) x_n
 *              +   h(I + A.h/2(I + A.h/3(I + A.h/4)))B u_n
 */
void ss_rk_dsolve_LTI_x( ss_mod * sys, mtrx * x0, mtrx ** u,
                          mtrx ** ret_x, double start, double end, double h )
{
    // copy owner info of x0 and u
    bool x0_owner = x0->owner, u_owner;

    int step = 1 + ( int )( end - start) / h;
    mtrx *coeff_un = NULL;
    mtrx *coeff_xn = NULL;

    // preprocessing
    {
        coeff_un = mtrx_init_unit( sys->A->row, false );

        for ( int i = 4; i > 1; --i )
        {
            coeff_un = mtrx_mmul( sys->A, coeff_un );
            coeff_un = mtrx_nmul( h / i, coeff_un );
            coeff_un = mtrx_add_unity( coeff_un );
        }
        coeff_un = mtrx_nmul( h, coeff_un );

        coeff_un->owner = true;
        coeff_xn = mtrx_mmul( sys->A, coeff_un );
        coeff_xn = mtrx_add_unity( coeff_xn );

        coeff_un->owner = false;
        coeff_un = mtrx_mmul( coeff_un, sys->B );
    }

    // calculation of states
    x0->owner = true;
    for (int i = 1; i < step; i++)
    {
        u_owner = u[ i ]->owner;
        u[ i ]->owner = true;
        ret_x[ i ] = mtrx_add( mtrx_mmul( coeff_xn, ret_x[ i - 1 ] ),
                                mtrx_mmul( coeff_un, u[ i ] ) );
        u[ i ]->owner = u_owner;
    }
    
    // make the x0 and u owner info as previous
    x0->owner = x0_owner;
}

void ss_rk_dsolve_LTI_y( ss_mod * sys, mtrx * x0, mtrx ** u, 
                        mtrx ** ret_y, double start, double end, double h )
{
    // copy owner info of x0 and u
    bool x0_owner = x0->owner, u_owner;

    int step = 1 + ( int )( end - start ) / h;
    mtrx * coeff_xn = NULL;
    mtrx * coeff_un = NULL;
    mtrx * ret_X = NULL;
    mtrx * ret_X_prev = NULL;

    // preprocessing
    {
        coeff_un = mtrx_init_unit( sys->A->row, false );

        for ( int i = 4; i > 1; --i )
        {
            coeff_un = mtrx_mmul( sys->A, coeff_un );
            coeff_un = mtrx_nmul( h / i, coeff_un );
            coeff_un = mtrx_add_unity( coeff_un );
        }
        coeff_un = mtrx_nmul( h, coeff_un );

        coeff_un->owner = true;
        coeff_xn = mtrx_mmul( sys->A, coeff_un );
        coeff_xn = mtrx_add_unity( coeff_xn );

        coeff_un->owner = false;
        coeff_un = mtrx_mmul( coeff_un, sys->B );
    }
    // calculation of output
    x0->owner = true;
    for ( int i = 0; i < step; i++ )
    {
        u_owner = u[ i ]->owner;
        u[ i ]->owner = true;
        ret_X = mtrx_add( mtrx_mmul( coeff_xn, ret_x[ i - 1 ] ),
                                mtrx_mmul( coeff_un, u[ i ] ) );
        u[ i ]->owner = u_owner;
        ret_y[ i ] = ss_y( sys, ret_X, u[ i ] );
    }
}