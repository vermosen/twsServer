/* File : example.i */

#ifndef tws_example_i
#define tws_example_i

%module twsPython

%{
#include "example.h"
%}

/* Let's just grab the original header file here */
%include "example.h"

#endif