#include <omp.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ss_model.h"
#include "ctrl_error.h"
#include "openblas_interface.h"

extern int ctrl_errno;

ss_mod * ss_alloc( mtrx * A, mtrx * B, mtrx * C, mtrx *D )
{
    ss_mod * sys = ( ss_mod * ) malloc( sizeof( ss_mod ) );
    if ( sys == NULL )
    {
        ctrl_errno = CTRL_ENOMEM;
        return NULL;
    }

    sys->A = A;
    sys->B = B;
    sys->C = C;
    sys->D = D;

    ctrl_errno = CTRL_SUCCESS;
    return sys;
}

void ss_free( ss_mod * sys )
{
    free( sys );
    ctrl_errno = CTRL_SUCCESS;
}

void ss_free_all( ss_mod * sys )
{
    mtrx_free( sys->A );
    mtrx_free( sys->B );
    mtrx_free( sys->C );
    mtrx_free( sys->D );

    free( sys );
    ctrl_errno = CTRL_SUCCESS;
}

mtrx * ss_dx( ss_mod * sys, mtrx * x, mtrx * u )
{
    // if u has only one element
    if ( u->col == 1 && u->row == 1 )
    {
        return mtrx_add( mtrx_vmul( x, sys->A ), mtrx_nmul( u->elem[0], sys->B ) );
    }
    // if u is a vector
    if ( u->col == 1 )
    {
        return mtrx_add( mtrx_vmul( x, sys->A ), mtrx_vmul( u, sys->B ) );
    }
    // general case
    return mtrx_add( mtrx_vmul( x, sys->A ), mtrx_mmul( sys->B, u ) );
}

mtrx * ss_y( ss_mod * sys, mtrx * x, mtrx * u )
{
    // if u has only one element
    if ( u->col == 1 && u->row == 1 )
    {
        return mtrx_add( mtrx_vmul( x, sys->C ), mtrx_nmul( u->elem[0], sys->D ) );
    }
    // if u is a vector
    if ( u->col == 1 )
    {
        return mtrx_add( mtrx_vmul( x, sys->C ), mtrx_vmul( u, sys->D ) );
    }
    // general case
    return mtrx_add( mtrx_vmul( x, sys->C ), mtrx_mmul( sys->D, u ) );
}
