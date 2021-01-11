///////////////////////////////////////////////////////////////
//
//   urbanChemFoamDictOptions.H
//   --------------------------
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
//   Defines items and applicable default values for
//   input dictionary system/urbanChemFoamDict
//
///////////////////////////////////////////////////////////////

#ifndef __URBANCHEMFOAMDICTOPTIONS_H__
#define __URBANCHEMFOAMDICTOPTIONS_H__

				// define dictionary

IOobject urbanChemFoamDictIO  (
	"urbanChemFoamDict",
	runTime.system ( ),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
);

dictionary urbanChemFoamDict = IOdictionary ( urbanChemFoamDictIO );

				// read dictionary entries or
				// take default value

scalar originLatitude  ( readScalar ( urbanChemFoamDict.lookup ( "originLatitude"  ) ) );
scalar originLongitude ( readScalar ( urbanChemFoamDict.lookup ( "originLongitude" ) ) );
Foam::string referenceTimestamp ( urbanChemFoamDict.lookup ( "referenceTimestamp"  ) );

bool   fixedSolarState  ( urbanChemFoamDict.lookupOrDefault<Switch> ( "fixedSolarState",  false ) );
scalar fixedZenithAngle ( urbanChemFoamDict.lookupOrDefault<scalar> ( "fixedZenithAngle", 0.0   ) );

Info << " *** urbanChemFoam dictionary *** " << endl;
Info << "       Latitude at  x = (0, 0)   = " << originLatitude     << " deg N" << endl;
Info << "       Longitude at x = (0, 0)   = " << originLongitude    << " deg E" << endl;
Info << "       Timestamp at t = 0        = " << referenceTimestamp << endl;

if ( fixedSolarState )  {
	Info << "       Solar zenith angle is fixed at " << fixedZenithAngle << "deg" << endl;
}
else
	Info << "       Solar angles will be calculated based on simulation date and time" << endl;

Info << endl;

#endif

				///////////////////////////////////////////////
