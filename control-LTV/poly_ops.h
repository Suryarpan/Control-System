#ifndef POLY_OPS_H
#define POLY_OPS_H

#include <stdbool.h>

struct poly
{
    int rank;
    int owner;
    double * coeff;
};

typedef struct poly poly;

poly * poly_alloc( int, bool, const double * );

void poly_free( poly * );

double poly_eval( double, poly * );

poly * poly_add( poly *, poly * );

poly * poly_sub( poly *, poly * );

poly * poly_nmul( double, poly * );

poly * poly_pmul( poly *, poly * );

#endif // !POLY_OPS_H