///////////////////////////////////////////////////////////////
//
//   commonGlobals.H
//   ---------------
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
//   Global variables 
//
///////////////////////////////////////////////////////////////

#ifndef __COMMONGLOBALS_H__
#define __COMMONGLOBALS_H__

				///////////////////////////////////////////////
				// solver state indicators
				///////////////////////////////////////////////				

bool firstSolverIteration;		// indicate first solver iteration
bool externalPatchCallOverride;	// solver override to call external data

				///////////////////////////////////////////////
				// solar state
				///////////////////////////////////////////////				

Foam::scalar zenithAngle;		// solar zenith angle
Foam::scalar cosZenithAngle;	// cosine of above (for convenience)

#endif

				///////////////////////////////////////////////
