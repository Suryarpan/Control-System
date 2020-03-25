#ifndef SS_MODEL_H
#define SS_MODEL_H

#include "openblas_interface.h"

struct ss_mod
{
    mtrx *A, *B, *C, *D;
};

typedef struct ss_mod ss_mod;

ss_mod * ss_alloc( mtrx *, mtrx *, mtrx *, mtrx * );

void ss_free( ss_mod * );

void ss_free_all( ss_mod * );

mtrx * ss_dx( ss_mod *, mtrx *, mtrx * );

mtrx * ss_y( ss_mod *, mtrx *, mtrx * );

#endif // !SS_MODEL_H