/*
 * mtrx->elem uses row major order
 */

#include <omp.h>
#include <string.h>
#include <stdlib.h>

#include "ctrl_error.h"
#include "openblas_interface.h"

extern int ctrl_errno;
/*
 * 
 */
mtrx * mtrx_alloc( int row, int col, bool owner, double * elem_arr )
{
    if ( !( row > 0 && col > 0 ) )
    {
        int ctrl_errno = CTRL_ERANGE;
        return NULL;
    }

    if( elem_arr == NULL )
    {
        mtrx * ret_mat = ( mtrx * ) malloc( sizeof( mtrx ) );
        if ( ret_mat == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_mat->elem = malloc( row * col * sizeof( double ) );
        if ( ret_mat->elem == NULL )
        {
            free( ret_mat );
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_mat->row = row;
        ret_mat->col = col;
        ret_mat->owner = owner;
        ctrl_errno = CTRL_SUCCESS;

        return ret_mat;
    }
    else
    {
        mtrx * ret_mat = ( mtrx * ) malloc( sizeof( mtrx ) );
        if ( ret_mat == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_mat->elem = malloc( row * col * sizeof( double ) );
        if ( ret_mat->elem == NULL )
        {
            free( ret_mat );
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_mat->row = row;
        ret_mat->col = col;
        ret_mat->owner = owner;
        memcpy( ret_mat->elem, elem_arr, row * col * sizeof( double ) );
        ctrl_errno = CTRL_SUCCESS;

        return ret_mat;
    }
}

mtrx * mtrx_init_unit( int dim, bool owner )
{
    if ( dim <= 0 )
    {
        ctrl_errno = CTRL_ERANGE;
        return NULL;
    }

    mtrx * ret_unit = mtrx_alloc( dim, dim, owner, NULL );
    if ( ret_unit == NULL )
    {
        return NULL;
    }

    #pragma omp parallel for
    for ( int i = 0; i < dim; ++i )
    {
        for ( int j = 0; j < dim; ++j )
        {
            ret_unit->elem[ j + i*dim ] = ( (i == j)? 1 : 0 );
        }
    }

    ctrl_errno = CTRL_SUCCESS;
    return ret_unit;
}

void mtrx_free( mtrx * a)
{
    free( a->elem );
    free( a );
    ctrl_errno = CTRL_SUCCESS;
}

mtrx * mtrx_add( mtrx * a, mtrx * b )
{
    if ( !( (a->row == b->row) && ( a->col == b->col ) ) )
    {
        ctrl_errno = CTRL_ESIZE;
        return NULL;
    }

    mtrx *add_res = mtrx_alloc( a->row, a->col, false, a->elem );
    if ( add_res == NULL )
    {
        return NULL;
    }

    #pragma omp prallel for
    for ( int i = 0; i < a->row * a->col; ++i )
    {
        add_res->elem[ i ] += b->elem[ i ];
    }

    if ( !( a->owner ) )
    {
        mtrx_free( a );
    }
    if( !( b->owner ) )
    {
        mtrx_free( b );
    }

    ctrl_errno = CTRL_SUCCESS;
    return add_res;
}

mtrx * mtrx_add_unit( mtrx * a )
{
    if ( a->row != a->col )
    {
        ctrl_errno = CTRL_ESIZE;
        return NULL;
    }

    mtrx * add_res = mtrx_alloc( a->row, a->row, false, a->elem );
    if ( add_res == NULL )
    {
        return NULL;
    }

    #pragma omp parallel for
    for ( int i = 0; i < a->row; ++i )
    {
        add_res->elem[ i + i*a->col ] += 1;
    }
    
    if ( !( a->owner ) )
    {
        mtrx_free( a );
    }

    ctrl_errno = CTRL_SUCCESS;
    return add_res;
}

mtrx * mtrx_nmul( double x, mtrx * a )
{
    mtrx * mul_res = mtrx_alloc( a->row, a->col, false, a->elem );
    if ( mul_res == NULL )
    {
        return NULL;
    }

    if ( x == 1 )
    {
        if ( !( a->owner ) )
        {
            mtrx_free( a );
        }
        return mul_res;
    }

    #pragma omp parallel for
    for (int i = 0; i < a->row * a->col; i++)
    {
        mul_res->elem[ i ] *= x;
    }
    
    if( !( a->owner ) )
    {
        mtrx_free( a );
    }

    ctrl_errno = CTRL_SUCCESS;
    return mul_res;
}

mtrx * mtrx_mmul( mtrx * a, mtrx * b )
{
    if ( a->col != b->row )
    {
        ctrl_errno = CTRL_ESIZE;
        return NULL;
    }

    mtrx * mul_res = mtrx_alloc( a->row, b->col, false, NULL );
    if ( mul_res == NULL )
    {
        return NULL;
    }

    cblas_dgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                a->row, b->col, a->col, 1, 
                a->elem, a->col, 
                b->elem, b->col, 0, 
                mul_res->elem, mul_res->col );
    
    if ( !( a->owner ) )
    {
        mtrx_free( a );
    }
    if ( !( b->owner ) )
    {
        mtrx_free( b );
    }

    ctrl_errno = CTRL_SUCCESS;
    return mul_res;
}

mtrx * mtrx_vmul( mtrx * vec, mtrx * mat )
{
    if ( vec->col != 1 )
    {
        ctrl_errno = CTRL_ESIZE;
        return NULL;
    }
    if ( mat->col != vec->row )
    {
        ctrl_errno = CTRL_ESIZE;
        return NULL;
    }

    mtrx * mul_res = mtrx_alloc( mat->row, 1, false, NULL );
    if ( mul_res == NULL )
    {
        return NULL;
    }

    cblas_dgemv( CblasRowMajor, CblasNoTrans, 
                mat->row, mat->col, 1.0, 
                mat->elem, mat->col, 
                vec->elem, 1, 0.0, mul_res->elem, 1 );
    
    if ( !( mat->owner ) )
    {
        mtrx_free( mat );
    }
    if ( !( vec->owner ) )
    {
        mtrx_free( vec );
    }

    ctrl_errno = CTRL_SUCCESS;
    return mul_res;
}
