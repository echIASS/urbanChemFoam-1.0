///////////////////////////////////////////////////////////////
//
//   commonPatchEmissions.H
//   ----------------------
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
//   Global variables and methods for passing patch emissions
//   data from simpleEmissions boundary condition to OpenFOAM
//   solver
//
///////////////////////////////////////////////////////////////

#ifndef __COMMONPATCHEMISSIONS_H__
#define __COMMENPATCHEMISSIONS_H__

				///////////////////////////////////////////////

namespace commonPatchEmissions  {

				///////////////////////////////////////////////

				///////////////////////////////////////////////
				// global variables specific to emission BC
				///////////////////////////////////////////////

Foam::DynamicList<Foam::word>                 speciesNamesList(0);
Foam::DynamicList<Foam::Field<Foam::scalar>>  densitySourceTerms(0);

				///////////////////////////////////////////////
				// find index of patchEmissionSpecies
				///////////////////////////////////////////////

Foam::label getSpeciesIndex (
				const Foam::DynamicList<Foam::word> & speciesList,
				const Foam::word speciesName,
				Foam::label pos = 0 )  {

	const Foam::label listSize = speciesList.size();

	if ( listSize && ! ( pos < 0 ) )  {
		while ( pos < listSize )  {
			if ( speciesList[pos] == speciesName )  return ( pos );
			pos++;
		}
	}

	return (-1);

}

				///////////////////////////////////////////////
				// allocate space for global patch
				// emission data
				///////////////////////////////////////////////

void allocateSpecies ( const Foam::word speciesName,
					   const Foam::label meshSize,
					   bool  speciesAllocated = true )  {
	if ( speciesAllocated )  return;
	speciesNamesList.append   ( speciesName );
	Foam::Field<Foam::scalar> densitySourceTerm ( meshSize, Foam::Zero );
	densitySourceTerms.append   ( densitySourceTerm );
}

				///////////////////////////////////////////////
				// clean up all dynamic lists at the end
				///////////////////////////////////////////////

void cleanup ( )  {
	densitySourceTerms.clearStorage();
	speciesNamesList.clearStorage();
}

				///////////////////////////////////////////////

}	// namespace commonPatchEmissions

				///////////////////////////////////////////////

#endif

				///////////////////////////////////////////////
