#ifndef SOLVER_LTI_H
#define SOLVER_LTI_H

// File operations


// Solvers (Runge-Kutta 4th order only)

void ss_rk_dsolve_LTI_x( ss_mod *, mtrx *, mtrx **, mtrx **, double, double, double );

void ss_rk_dsolve_LTI_y( ss_mod *, mtrx *, mtrx **, mtrx **, double, double, double );


#endif // !SOLVER_LTI_H