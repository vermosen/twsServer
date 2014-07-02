#include "example.h"

// accessors
double example::a() const { return a_; };
double example::b() const { return b_; };

void example::a(const double & val){ a_ = val; };
void example::b(const double & val){ b_ = val; };