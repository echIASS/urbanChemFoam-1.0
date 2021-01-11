///////////////////////////////////////////////////////////////
//
//   derivedOptions.H
//   ----------------
//
//   ECC / IASS
//   Copyright (C) 2020 Edward C. Chan
//
//   LICENSE
//
//   This source code is licensed under the terms of
//   the GNU General Public License as published by
//   the Free Software Foundation, either version 3 for
//   the License or, at your option, any later version.
//
//   DESCRIPTION
//
//   Defines variables derived from urbanChemFoam dictionary
//	 items
//
///////////////////////////////////////////////////////////////

#ifndef __DERIVEDOPTIONS_H__
#define __DERIVEDOPTIONS_H__

dateTime referenceDate ( referenceTimestamp );
scalar phi0 = D2R * originLatitude;

dimensionedVector g     (
	"g",
	dimensionSet(0,1,-2,0,0,0,0),
	gravityVector  );

dimensionedVector Omega (
	"Omega",
	dimensionSet(0,0,-1,0,0,0,0),
	Foam::cos(phi0)*rotationVector );

Info << " *** derived options *** " << endl;
Info << "       rotation vector at (0, 0) = " << Omega << " rad/s" << endl;
Info << endl;

#endif

				///////////////////////////////////////////////
