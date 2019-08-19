#ifndef POLY_OPS_H
#define POLY_OPS_H

struct poly
{
    int rank;
    int owner;
    double *coeff;
};

typedef struct poly poly;

poly * poly_alloc( int, bool, const double * );

void poly_free( poly * );

poly * poly_add( poly *, poly * );

poly * poly_sub( poly *, poly * );

poly * poly_nmul( double, poly * );

poly * poly_pmul( poly *, poly * );

#endif // !POLY_OPS_H