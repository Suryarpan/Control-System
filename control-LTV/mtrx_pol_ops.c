#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "ctrl_error.h"
#include "mtrx_pol_ops.h"

extern int ctrl_errno;

/*
 * allocates memory for mtrx_p struct.
 * Use row major order for array of poly pointer.
 * If array of poly pointer is not supplied i.e. poly_pointer_arr is NULL
 * a memory block of size row * column is supplied to the poly_elem field.
 * If array is supplied, the address of the array is stored in poly_elem field.
 */
mtrx_p * mtrx_p_alloc( int row, int column, bool owner, poly ** poly_pointer_arr )
{
    if ( !( row > 0 && column > 0 ) ) // ensures row > 0 and col > 0
    {
        int ctrl_errno = CTRL_ERANGE;
        return NULL;
    }

    if( poly_pointer_arr == NULL )
    {
        // allocating mtrx_p
        mtrx_p * ret_mat = ( mtrx_p * ) malloc( sizeof( mtrx_p ) );
        if( ret_mat == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        // allocating pol_elem (array of poly pointers)
        ret_mat->pol_elem = malloc( row * column * sizeof( poly * ) );
        if ( ret_mat->pol_elem == NULL )
        {
            free( ret_mat );
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }
        
        ret_mat->row = row;
        ret_mat->colm = column;
        ret_mat->owner = owner;

        return ret_mat;
    }
    else
    {
        // allocating mtrx_p st
        mtrx_p * ret_mat = ( mtrx_p * ) malloc( sizeof( mtrx_p ) );
        if ( ret_mat == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }
        
        // allocating pol_elem array of poly pointers
        ret_mat->pol_elem = malloc( row * column * sizeof( poly * ) );
        if( ret_mat->pol_elem == NULL )
        {
            free( ret_mat );
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }

        ret_mat->row = row;
        ret_mat->colm = column;
        ret_mat->owner = owner;
        memcpy( ret_mat->pol_elem, poly_pointer_arr, row * column * sizeof( poly * ) );

        return ret_mat;
    }
}

/*
 * It frees the memory associated with a mtrx_p pointer.
 * DONOT CALL free() on a mtrx_p pointer.
 * It will cause a whole array of poly pointers anonymous and thus causing a memory leak.
 */
void mtrx_p_free( mtrx_p * a )
{
    #pragma omp parallel for
    for ( int i = 0; i < ( a->row * a->colm ); i++ )
    {
        free( a->pol_elem[ i ] );
    }
    
    free( a->pol_elem );
    free( a );
}

/*
 * Initialises a mtrx_p  pointer with all rank zero polynomials and owner set to false.
 */
static mtrx_p * mtrx_p_init( int row, int column, bool owner )
{
    mtrx_p * ret_mat = mtrx_p_alloc( row, column, owner, NULL);
    if ( ret_mat == NULL)
    {
        return NULL;
    }

    #pragma omp parallel for
    for (int i = 0; i < row * column; i++)
    {
        ret_mat->pol_elem[ i ] = poly_alloc( 0, false, NULL);
        if ( ret_mat->pol_elem[ i ] == NULL )
        {
            return NULL;
        }
    }
    
    return ret_mat;
}

/*
 * Adds two mtrx_p pointers.
 * It also frees the input matrix if owner is set to false.
 */
mtrx_p * mtrx_p_add( mtrx_p * a, mtrx_p * b )
{
    if ( !( ( a->row == b->row ) && ( a->colm == b->colm ) ) )
    {
        ctrl_errno = CTRL_ERANK;
        return NULL;
    }

    mtrx_p * add_res = mtrx_p_alloc( a->row, a->colm, false, NULL);
    if ( add_res == NULL )
    {
        return NULL;
    }

    #pragma omp parallel for
    for (int i = 0; i < ( add_res->row * add_res->colm ); i++)
    {
        add_res->pol_elem[ i ] = poly_add( a->pol_elem[ i ], b->pol_elem[ i ] );
    }

    if ( !( a->owner ) )
    {
        mtrx_p_free( a );
    }
    
    if ( !( b->owner ) )
    {
        mtrx_p_free( b );
    }
    
    return add_res;
}

/*
 * Subtracts two mtrx_p pointers. [ a - b ]
 * It also frees the input matrix if owner is set to false.
 */
mtrx_p * mtrx_p_sub( mtrx_p * a, mtrx_p * b )
{
    if ( !( ( a->row == b->row ) && ( a->colm == b->colm ) ) )
    {
        ctrl_errno = CTRL_ERANK;
        return NULL;
    }

    mtrx_p * sub_res = mtrx_p_alloc( a->row, a->colm, false, NULL);
    if ( sub_res == NULL )
    {
        return NULL;
    }

    #pragma omp parallel for
    for (int i = 0; i < ( sub_res->row * sub_res->colm ); i++)
    {
        sub_res->pol_elem[ i ] = poly_sub( a->pol_elem[ i ], b->pol_elem[ i ] );
    }
    
    if ( !( a->owner ) )
    {
        mtrx_p_free( a );
    }

    if( !( b->owner ) )
    {
        mtrx_p_free( b );
    }

    return sub_res;
}

/*
 * Multiplies a scalar(x) to the matrix(a).
 * It also frees the input matrix if owner is set to false.
 */
mtrx_p * mtrx_p_nmul( double x, mtrx_p * a )
{
    mtrx_p * mul_res = mtrx_p_alloc( a->row, a->colm, false, a->pol_elem );
    if ( mul_res == NULL )
    {
        return NULL;
    }

    #pragma omp parallel for
    for (int i = 0; i < ( a->row * a->colm ); i++)
    {
        mul_res->pol_elem[ i ] = poly_nmul( x, a );
    }
    
    if( !( a->owner ) )
    {
        mtrx_p_free( a );
    }

    return mul_res;
}

/*
 * Multipies two matrix if multiplication is possible.
 * It also frees the input matrix if owner is set to false.
 * It is implemented in a row-major fashion.
 */
mtrx_p * mtrx_p_mmul( mtrx_p * a, mtrx_p * b )
{
    if ( a->colm != b->row )
    {
        ctrl_errno = CTRL_ERANK;
        return NULL;
    }

    mtrx_p * c = mtrx_p_init( a->row, b->colm, false );
    if ( c == NULL )
    {
        return NULL;
    }

    #pragma omp parallel for
    for ( int i = 0; i < a->row; i++ )
    {
        for ( int j = 0; j < b->colm; j++ )
        {
            for ( int p = 0; p < a->colm; p++ )
            {
                poly * mul_res = poly_pmul( a->pol_elem[ i*a->row + p ], b->pol_elem[ p*b->row + j ] );
                if ( mul_res == NULL )
                {
                    return NULL;
                }
                c->pol_elem[ i*c->row + j ] = poly_add( mul_res, c->pol_elem[ i*c->row + j ] );
                if ( c->pol_elem[ i*c->row + j ] == NULL )
                {
                    return NULL;
                }
            }
        }
    }
    
    if ( !( a->owner ) )
    {
        mtrx_p_free( a );
    }
    if ( !( b->owner ) )
    {
        mtrx_p_free( b );
    }

    return c;
}
