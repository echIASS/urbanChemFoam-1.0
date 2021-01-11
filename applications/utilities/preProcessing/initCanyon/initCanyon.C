///////////////////////////////////////////////////////////////
//
//  initCanyon.H
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
//   Utility to introduce initial mean flow field and
//   turbulent perturbation in a street canyon geometry
//
//	 User-defined input are located in constant/initProperties 
//
//   In terms of mean velocity, altitudes below the
//   freestreamLowerHeight (inside canyon) will be zero,
//   while a linear velocity profile will be assigned between
//   freestreamLowerHeight and freestreamUpperHeight, from
//   ( 0, 0, 0 ) at the lower height to the values prescribed
//   by freestreamVelocityVector.
//
//   Random flow perturbations are controlled by the vector
//	 entry turbulentIntensityVector, which indicates fraction
//   with respect to the corresponding freestreamVelocityVector
//   components to be assigned as turbulent intensity
//
//	 The updated initial velocity profile will be written to
//   the "0" folder (or whatever time for initial time step)
//
///////////////////////////////////////////////////////////////

#include "fvCFD.H"
#include "Random.H"
#include "commonConstants.H"

				////////////////////////////////////////////////

const int randSeedPhi =   34592000;
const int randSeedLam = -106365200;

int main ( int argc, char *argv[] )  {

	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"

				// dictionary (constant > initProperties)

	IOobject initPropertiesIO (
		"initProperties",
		runTime.constant( ),
		mesh,
		IOobject::MUST_READ,
		IOobject::NO_WRITE
	);

				// velocity field

	IOobject Ufield (
		"U",
		runTime.timeName ( ),
		mesh,
		IOobject::MUST_READ,
		IOobject::NO_WRITE
	);

				// read dictionary

	Info << " *** reading dictionary " << initPropertiesIO.name() << " ... ";

	dictionary initProperties = IOdictionary ( initPropertiesIO );

	scalar freestreamLowerHeight ( readScalar (
					initProperties.lookup("freestreamLowerHeight") ) );
	scalar freestreamUpperHeight ( readScalar (
					initProperties.lookup("freestreamUpperHeight") ) );
	
	vector freestreamVelocityVector (
					initProperties.lookup("freestreamVelocityVector") );
	vector turbulentIntensityVector (
					initProperties.lookup("turbulentIntensityVector") );

				// correct dictionary values

	scalar z0 = freestreamLowerHeight;
	scalar z1 = freestreamUpperHeight;

	if ( z0 > z1 )  {
		z0 = freestreamUpperHeight;
		z1 = freestreamLowerHeight;
	}

	scalar dz = z1 - z0;
	if ( dz < vSmall )  dz = 1.0;

	vector Uinf  = freestreamVelocityVector;
	vector dUmax = vector (
						max(1.0,mag(Uinf.x()))*mag(turbulentIntensityVector.x()),
						max(1.0,mag(Uinf.y()))*mag(turbulentIntensityVector.y()),
						max(1.0,mag(Uinf.z()))*mag(turbulentIntensityVector.z()) );

	Info << "done" << endl;

				// read mesh centroids

	Info << " *** reading mesh data at time " << runTime.timeName() << " ... ";
	const vectorField& cellCentroids(mesh.C());
	Info << "done" << endl;

				// read initial U field

	Info << " *** reading velocity field at time " << runTime.timeName() << " ... ";
	volVectorField U(Ufield, mesh);
	Info << "done" << endl;

				// calculate velocities
	
	Info << "     generating velocity perturbation ... ";

	label i;
	i = i + 0;	// gets rid of unused variable warning

	Random rand(randSeedPhi+randSeedLam);		// RIP Hank and Gomez

	forAll ( cellCentroids, i )  {

		U[i] = vector ( rand.scalarAB ( -dUmax.x(), dUmax.x() ),
						rand.scalarAB ( -dUmax.y(), dUmax.y() ),
						rand.scalarAB ( -dUmax.z(), dUmax.z() ) );

		if ( (cellCentroids[i].z()>z0) && (cellCentroids[i].z()<z1) )  {
			scalar dUdz = ( cellCentroids[i].z() - z0 ) / dz;
			U[i] += dUdz * Uinf;
		}

	}

    U.write();
	Info << "done" << endl;
	return 0;

}

				////////////////////////////////////////////////
