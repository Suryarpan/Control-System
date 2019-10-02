#ifndef OPENBLAS_INTERFACE_H
#define OPENBLAS_INTERFACE_H

#include <stdbool.h>

#include "cblas.h"

struct mtrx
{
    int row;
    int col;
    bool owner;
    double *elem;
};

typedef struct mtrx mtrx;

mtrx * mtrx_alloc( int, int, bool, double * );

mtrx * mtrx_init_unit( int, bool );

void mtrx_free( mtrx * );

mtrx * mtrx_add( mtrx *, mtrx * );

mtrx * mtrx_add_unit( mtrx * );

mtrx * mtrx_nmul( double, mtrx * );

mtrx * mtrx_mmul( mtrx *, mtrx * );

mtrx * mtrx_vmul( mtrx *, mtrx * );

#endif // !OPENBLAS_INTERFACE_H