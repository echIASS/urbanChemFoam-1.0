///////////////////////////////////////////////////////////////
//
//   planetaryPropertiesOptions.H
//   ----------------------------
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
//   input dictionary constant/planetaryProperties
//
///////////////////////////////////////////////////////////////

#ifndef __PLANETARYPROPERTIESOPTIONS_H__
#define __PLANETARYPROPERTIESOPTIONS_H__

				// define (optional) dictionary

IOobject planetaryPropertiesIO (
	"planetaryProperties",
	runTime.constant ( ),
	mesh,
	IOobject::MUST_READ_IF_MODIFIED,
	IOobject::NO_WRITE
);

				// read dictionary entries or
				// take default value

dictionary planetaryProperties = IOdictionary ( planetaryPropertiesIO );

scalar semiMajorAxis ( planetaryProperties.lookupOrDefault<scalar> (
						"semiMajorAxis",  6378137.0 ) );

scalar flattening    ( planetaryProperties.lookupOrDefault<scalar> (
						"inverseFlattening",  298.257223563 ) );

scalar centralMeridianScaleFactor (planetaryProperties.lookupOrDefault<scalar> (
						"centralMeridianScaleFactor",  0.9996 ) );

vector gravityVector  ( planetaryProperties.lookupOrDefault<vector> (
						"gravityVector",  vector(0.0, 0.0, -9.80665) ) );

vector rotationVector ( planetaryProperties.lookupOrDefault<vector> (
						"rotationVector", vector(7.29212e-5, 0.0, 0.0 ) ) );

Info << " *** planetary properties *** " << endl;
Info << "       semi major axis               = " << semiMajorAxis << " m" << endl;
Info << "       inverse flattening            = " << flattening    << endl;
Info << "       central meridian scale factor = " << centralMeridianScaleFactor << endl;
Info << "       gravitation vector            = " << gravityVector << " m/s^2" << endl;
Info << "       rotation vector               = " << rotationVector << " rad/s" << endl;
Info << endl;

#endif

				///////////////////////////////////////////////
