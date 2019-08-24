#ifndef MTRX_POL_OPS_H
#define MTRX_POL_OPS_H

#include <stdbool.h>

#include "poly_ops.h"

struct mtrx_p
{
    int row;
    int colm;
    int owner;
    poly ** pol_elem;
};

typedef struct mtrx_p mtrx_p;

mtrx_p * mtrx_p_alloc( int , int , bool, poly ** );

void mtrx_p_free( mtrx_p * );

mtrx_p * mtrx_p_add( mtrx_p *, mtrx_p * );

mtrx_p * mtrx_p_sub( mtrx_p *, mtrx_p * );

mtrx_p * mtrx_p_nmul( double, mtrx_p * );

mtrx_p * mtrx_p_mmul( mtrx_p *, mtrx_p * );

#endif // !MTRX_POL_OPS_H