**********************
Error Reporting in SSA
**********************

This chapter describes how functions in SSA reports and handles error. All public facing functions defined within this library reports an error code whenever the requested task is unfulfilled. Also, users can define their own error handling routines to modify the default behavior.

The functions listed here are defined in :file:`ssa_error.h`.

Error Checking
==============

The library follows the thread-safe error reporting conventions of the POSIX Threads library. A function returns a non-zero status code to indicate an error had taken place during computation. Otherwise, a status code of zero (**0**) is returned to indicate success. The library uses following type definition as return code for most of the functions whenever possible.

.. c:type:: ssa_error_t

It is a :code:`const` type which helps to manage such status codes without the risk of accidental changes.

.. tip::

   A general error checking strategy is illustrated below.
   
   .. code-block:: c

      ssa_error_t status = ssa_func(...);

      if (status) /* An error occurred in ssa_func */
      {
         /* Check status value for specific type of error */
      }

The status code is equivalent to :code:`errno` in C library, but provides a more robust error checking facility.

The error reporting and handling interface is divided in three main parts and these are discussed below.

Error Codes and String Mapping
==============================

The library has a few error status codes defined in :file:`ssa_error.h`. Each of these are listed in their gory details in `Details of Error Codes`_. A function is provided which maps these error codes with a short string describing what has happened. 

.. function:: const char *ssa_strerror (ssa_error_t errnum)

   :param errnum: The number of the error status
   :return: A string having a short description about the error

It is similar to the :c:func:`strerror` as defined in C library. The output string is not modifiable. Following example illustrates intended usage of this function.

.. code-block:: c

   ssa_error_t status = ssa_func(...);
   const char *str = ssa_strerror(status);
   /* print str to a standard or project specific file */


Error Handling
==============

The functions described in this section are useful for error handling. A handler type is defined to make the process of error handling much more modular and nonrigid.

Handlers Provided by SSA
--------------------------

A default handler is provided along with a null handler. Behavior of these are documented below.

No Handler
^^^^^^^^^^

This is used to turn off all type of error handling. This is set by calling the following function.

.. function:: ssa_error_handler_t *ssa_set_error_handler_off (void)

   :return: Returns pointer to previous error handler. If none was set previously, then :c:macro:`NULL` is returned.

Default Handler
^^^^^^^^^^^^^^^

This error handler is set by default and is called if no other handlers are set. It can be turned off with the function detailed in `No Handler`_. It prints a detailed message about the error along with location to the stream (see `Error Printing`_) and aborts the program.

.. caution::

   The default handler aborts the program.

This behavior may not be desired always and needs to be changed. The mechanism is detailed below.

Handler Type
------------

The handler type has the following name.

.. type:: ssa_error_handler_t

A user defined handler function should conform to this standard handler type used by SSA. The detailed function signature for using as an error handler is described below.

.. function:: void example_handler (const char *reason, const char *file, int line, ssa_error_t ssa_errno)
   
   :param reason: A string describing the cause of error.
   :param file: Name of the file where error occurred (:c:macro:`__FILE__` can be used).
   :param line: Number of line where error occurred (:c:macro:`__LINE__` can be used).
   :param ssa_errno: Code of the error as described in `Details of Error Codes`_.
   :return: Since it may be an exit point, the function must not return.

Registering User Defined Handlers
---------------------------------

Once such an error handler is defined, the default handler can be changed with the help of following function.

.. function:: ssa_error_handler_t *ssa_set_error_handler(ssa_error_handler_t *new_handler)

   :param new_handler: pointer to the user defined error handler
   :return: pointer to previous error handler. If none was set previously, then :c:macro:`NULL` is returned.

Error Printing
==============

The functions described in this section are useful for logging purposes and help to generalize the process of printing the error messages to a particular file. The default error stream is set to :c:macro:`stderr` but can be changed with following function.

.. function:: FILE *ssa_set_stream(FILE *new_stream)

   :param new_stream: A file where the error reports will be printed
   :return: Previous stream file. If none was set previously, :c:macro:`stderr` is returned.

One other function is provided to change the default printing function. It is set up similar to error handler. The stream handler type has the following name.

.. type:: ssa_stream_handler_t

Such a function should have the following signature in order to work.

.. function:: void example_stream (const char *label, const char *file, int line, const char *reason)

Once such a function is defined, the pointer of that function can be passed to the following function in order to register the same as default error printing function.

.. function:: ssa_stream_handler_t *ssa_set_stream_handler (ssa_stream_handler_t *new_handler)

Previous handler is returned by this function. If none was set :c:macro:`NULL` is returned.

Now all the error codes along with their approximate meaning are listed below.

Details of Error Codes
======================

This section details the various error codes available to the user and their meaning. All of them start with ``SSA_`` to distinguish from other libraries. The details will also help users incorporate the same error codes.

.. enumerator:: SSA_SUCCESS
   
   Indicates successful execution.

.. enumerator:: SSA_FAILURE

   Indicates general failure (For the sake of completeness).

.. enumerator:: SSA_ERANGE

   Similar to :c:macro:`ERANGE` in the C library. It indicates output of a computation is not representable (e.g. :c:var:`NaN`, :c:var:`INF`).

.. enumerator:: SSA_EDOM

   Similar to :c:macro:`EDOM` in the C library. It indicates input/argument to a function is not invalid (``sqrt(-2)``).

.. enumerator:: SSA_EFAULT

   Pointer provided as argument has (reasonably) testable invalidity.

.. enumerator:: SSA_EINVAL
   
   Argument(s) provided to the function is invalid. Similar to :c:enumerator:`SSA_EDOM` but more specific.
   
.. enumerator:: SSA_ESANITY

   Sanity checking failed, generally indicates inconsistent state of certain data.

.. enumerator:: SSA_ENOMEM

   Error due to out of memory condition. Similar to :c:macro:`ENOMEM` in C library.

.. enumerator:: SSA_ERUNAWAY
      
   Iteration out of control, generally indicates :c:macro:`NaN` or :c:macro:`INF` was encountered or a maximum limit was violated.

.. enumerator:: SSA_EMAXITER
   
   Computation has exceeded maximum number of iterations specified by user.

.. enumerator:: SSA_EZERODIV
      
   A division by zero has occurred.

.. enumerator:: SSA_EBADTOL
   
   Tolerance specified by user is not valid.

.. enumerator:: SSA_ETOL
      
   Computation failed to reach the specified tolerance.
      
.. enumerator:: SSA_EBADLEN
      
   Matrix and vector lengths are nonconforming.

.. enumerator:: SSA_ENOTSQR
   
   Matrix is not square.

.. enumerator:: SSA_EOF
   
   End of file has been reached.
