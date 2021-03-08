# State Space Analysis (SSA)

## Introduction

A state space based approach to simulating various control systems. Currently, LTI and LTV type systems are being implemented. NL type systems will be added later.

The main focus is to create a C library which can be used within a C program to produce the simulation. A brief run down of the necessary dependencies is provided below.

**Dependencies**:

1. Mathematics library, such as provided by libc,
2. [CBLAS](https://www.netlib.org/blas/) conforming library ([ATLAS](http://math-atlas.sourceforge.net/), [OpenBLAS](https://www.openblas.net/)).

A brief documentation of the various types of systems and usages are given below.
