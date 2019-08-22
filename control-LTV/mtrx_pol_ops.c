#include <stdlib.h>

#include "ctrl_error.h"

#include "mtrx_pol_ops.h"

extern int ctrl_errno;

mtrx_p * mtrx_p_alloc( int row, int column, bool owner, poly ** poly_pointer_arr)
{
    if ( !(row > 0 && column > 0) ) // ensures row > 0 and col > 0
    {
        int ctrl_errno = CTRL_ERANGE;
        return NULL;
    }

    if( poly_pointer_arr == NULL )
    {
        // allocating mtrx_p
        mtrx_p * ret_mat = ( mtrx_p * ) malloc( sizeof( mtrx_p * ) );
        if( ret_mat == NULL )
        {
            ctrl_errno = CTRL_ENOMEM;
            return NULL;
        }
    }
}