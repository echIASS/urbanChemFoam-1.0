///////////////////////////////////////////////////////////////
//
//  solarUtils.H
//  ------------
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
//	 Functions for calculating solar state
//
///////////////////////////////////////////////////////////////

#ifndef __SOLARUTILS_H__
#define __SOLARUTILS_H__

#include "fvCFD.H"

				// calculate zenith angle given lat/lon and JD

scalar solarGetZenithAngle ( scalar jd, scalar phi, scalar lam );

#endif
