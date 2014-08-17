/* File : example.i */

#ifndef tws_contract_i
#define tws_contract_i

%module twsPython

%{
/*#include "Contract.h"*/
%}

%{
/*using IB::LegOpenClose;
using IB::ComboLeg¨*/
%}

/* pick the struct from Contract.h */
enum LegOpenClose { 

	SAME_POS,
	OPEN_POS, 
	CLOSE_POS,
	UNKNOWN_POS

};

/*struct ComboLeg {

	ComboLeg()
		: conId(0)
		, ratio(0) {}

	long		conId;
	long		ratio;
	
};*/

#endif
