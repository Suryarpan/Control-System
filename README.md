# control-system

A state space based approach to solving computations of control system. The development is currently under progress. A short description of the files are given.

## control-LTI
This folder is currently being developed. All of the functions(to be implemented) here require BLAS support. An interface to OpenBLAS is under development and will soon be available. This creates the required matrix functions to be used for solving state space model. A 4-th order Runge Kutta method will be used for solving the state space equations.

## control-LTV
This folder is currently being developed. A polynomial struct is used for the time varying components in the A, B, C and D matrices. Addition, subtraction, multiplication of polynomials are implemented. Implementation of matrix with polynomial elements and subsequently required mathematical operations will be implemented. Support for vector instruction for such operations will be added later.

#### polynomial operations:
 - *Addition*: for loop based addition with OpenMP based parallelisation.
 - *Subtraction*: for loop based subtraction with OpenMp based parallelisation.
 - *Multiplication*: for loop based multiplication(*O(n<sup>2</sup>)*) or direct multiplication for small sized polynomials. DFT based multiplication(*O(nlogn)*) for larger polynomials with `double` coefficient. Cost of initialisation would be more for small polynomials. **FFTW-3.3.8** is used for computing the DFTs. An alternative much faster method based on Kronecker Trick is used for larger polynomials with `int` or `long int`. **GMP** is used for this purpose.

## ctrl_error
This file defines all of the error conditions and *errno* values. A capable error handler for required operations is under development.
