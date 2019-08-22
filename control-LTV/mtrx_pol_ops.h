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

#endif // !MTRX_POL_OPS_H