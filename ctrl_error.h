#ifndef CTRL_ERROR_H
#define CTRL_ERROR_H

int ctrl_errno;

enum errnos{
    CTRL_FAILURE = -1,
    CTRL_SUCCESS = 0,
    CTRL_EDOM    = 1,
    CTRL_ERANGE  = 2,
    CTRL_ENOMEM  = 3,
};

void error_handle( int );

#endif // !CTRL_ERROR_H