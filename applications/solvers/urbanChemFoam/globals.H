///////////////////////////////////////////////////////////////
//
//   globals.H
//   ---------
//
//   ECC / IASS
//   Copyright (C) 2020 Edward C. Chan
//
//   LICENSING
//
//   This source code is licensed under the terms of
//   the GNU General Public License as published by
//   the Free Software Foundation, either version 3 for
//   the License or, at your option, any later version.
//
//   DESCRIPTION
//
//   Global variables used in urbanChemFoam
//
///////////////////////////////////////////////////////////////

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

				// macros and magic numbers

				// datum values for potential temperature

const dimensionedScalar thetaP0 (
	"thetaP0",
	dimensionSet (1,-1,-2,0,0,0,0),
	100000.0
);

const dimensionedScalar thetaR (
	"thetaR",
	dimensionSet (0,0,0,0,0,0,0),
	0.286
);

#endif

				///////////////////////////////////////////////
