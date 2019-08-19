#ifndef PMUL_H
#define PMUL_H

#include "poly_ops.h"

poly * poly_mul_rank1( poly *, poly * );

poly * poly_mul_rank2( poly *, poly * );

poly * poly_mul_rank3( poly *, poly * );

poly * poly_mul_rank7( poly *, poly * );

poly * poly_mul_genrl( poly *, poly * );

#endif // !PMUL_H