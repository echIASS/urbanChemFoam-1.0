///////////////////////////////////////////////////////////////
//
//  simpleEmissionFvPatchField.H
//  ----------------------------
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
//   Declaration of class simpleEmissionFvPatchField 
//
//	 This boundary condition prescribes a uniform emission rate (in kg/s)
//	 on the cell centroids adjacant to the patch, as commonly found in
//	 CTM codes.  Emission is assumed to be derived from a line source and
//	 is distributed based on patch area.  Support for 
//
//	 Usage
//   Property            | Description             | Required    | Default value
//   sectionRateProfile  | emission rate per meter | yes         |
//                       | section [kg/s/m]        |             |
//   sectionLength       | length of emission      | no          | 1.0
//                       | line source             |             |
//   scaleFactor         | emission scale factor   | no          | 1.0
//   timeToSecondsFactor | scaling factor for time | no          | 1.0 (seconds)
//                       | to seconds              |             |
//   showEmissionMass    | tallies and writes out  | no          | false
//                       | species mass in domain  |             |
//                       | at the beginning of     |             |
//                       | each time step          |             |
//
//   Example of the boundary condition specification:
//
//   <patchName>
//   {
//        type                  simpleEmission;
//        value                 uniform   0.0;    // placeholder
//        sectionRateProfile    table (
//                                  ( 0  1.0e-9  )
//                                  ( 1  5.0e-10 )
//                                  ( 2  8.0e-10 )
//                              )
//        sectionLength         120.0;   // emissions will be multiplied by this
//        scaleFactor             2.5;   // scale emission 2.5x
//        timeToSecondsFactor  3600.0;   // convert hourly data to seconds
//        shwoEmissionMass     true;     // show domain mass per time step
//   }
//
//   Notes
//
//   1) As simpleEmission is derived from fixedValue, a vestigial field "value"
//      is still required.  Any don't-care value will do.
//   2) The sectionRateProfile entry is a Function1 type, able to describe time
//      varying functions.  The example above gives the usage for supplying a
//      constant value.
//   3) simpleEmission uses field density to calculate mass fraction.  If this
//      is unavailable (esp at the first time step), it will be estimated using
//      an initial gas constant of air.  This will cause some inaccuracies
//      (most of the time no more than a few per cent in relative mass imbalance)
//      at the start of the run, which will subside in at most a few minutes.
//
///////////////////////////////////////////////////////////////

#ifndef simpleEmissionFvPatchField_H
#define simpleEmissionFvPatchField_H

#include "fixedValueFvPatchFields.H"
#include "Function1.H"

#include "volFields.H"
#include "commonGlobals.H"
#include "commonPatchEmissions.H"

				///////////////////////////////////////////////

namespace Foam  {

				///////////////////////////////////////////////
				//
				// DECLARATION FOR CLASS
				// simpleEmisisonFvPatchField
				//
				///////////////////////////////////////////////

template<class Type>
class simpleEmissionFvPatchField
: public fixedValueFvPatchField<Type>  {

private   :

				// patch parameters

	autoPtr<Function1<Type>> sectionRateProfile_;	// sectional emission mass rate [kg/s/m]
	scalar sectionLength_;							// section length [m]
	scalar timeToSecondsFactor_;					// convert time to seconds
	scalar emissionScaleFactor_;					// fudge factor to test things quickly

	bool   showEmissionMass_;						// whether emission mass will be printed

				// internal variables

	label iSpecies_;				// index of species corresponding patch
	word  speciesName_;				// name of variable (to prevent _0 ambiguity)
	bool  initiated_;				// has boundary been initialized
	bool  speciesAssigned_;			// if species has been assigned

				// initiation lists

	DynamicList<label>  patchCellIndices;	// indices to internal cells
	DynamicList<scalar> patchCellVolumes; 	// patch cell volumes
	DynamicList<scalar> patchAreaWeights;	// patch cell area weighting
	DynamicList<scalar> patchWeights;		// patch area weight / patch volume 

protected :

				// return patch parameters

	scalar sectionRateProfile     ( scalar timeValue )  const  {
		return ( mag ( sectionRateProfile_->value(
					timeValue / timeToSecondsFactor() )));
	}
	scalar sectionLength       ( )  const  { return sectionLength_;       }
	scalar timeToSecondsFactor ( )  const  { return timeToSecondsFactor_; }
	scalar emissionScaleFactor ( )  const  { return emissionScaleFactor_; }
	bool   showEmissionMass    ( )  const  { return showEmissionMass_;    }

				// return internal variables

	bool   initiated        ( )  const  { return initiated_;        }
	bool   speciesAssigned  ( )  const  { return speciesAssigned_;  }
	word   speciesName      ( )  const  { return speciesName_;      }
	label  iSpecies         ( )  const  { return iSpecies_;         }

				// processing member functions

	void  assignSpecies ( const DynamicList<word> & speciesList, const word & speciesName );
	Foam::volScalarField getCellDensity ( );

	void  initiatePatches ( );
	void  updatePatchField ( );
	void  checkEmissionMass ( scalar timeValue, scalar massReference );

public    :

	TypeName("simpleEmission"); 	// Runtime type information

				////////////////////////////
				//
				// CONSTRUCTORS
				//
				////////////////////////////

	simpleEmissionFvPatchField (	// from patch + internal field
				const fvPatch&,
				const DimensionedField<Type, volMesh>& );

	simpleEmissionFvPatchField (	// from patch + internal field + patch field
				const fvPatch&,
				const DimensionedField<Type, volMesh>&,
				const Field<scalar>& fld );

	simpleEmissionFvPatchField (	// from patch + internal field + dictionary
				const fvPatch&,
				const DimensionedField<Type, volMesh>&,
				const dictionary& );

	simpleEmissionFvPatchField (	// by mapping given instance onto new patch
				const simpleEmissionFvPatchField&,
				const fvPatch&,
				const DimensionedField<Type, volMesh>&,
				const fvPatchFieldMapper& );

	simpleEmissionFvPatchField (	// copy constructor
				const simpleEmissionFvPatchField<Type>& );

	simpleEmissionFvPatchField (	// copy constructor set internal field ref
				const simpleEmissionFvPatchField<Type>&,
				const DimensionedField<Type, volMesh>& );

				// construct and return clone

	virtual tmp<fvPatchField<Type>> clone() const  {
		return tmp<fvPatchField<Type>> (
			new simpleEmissionFvPatchField<Type>(*this) );
	}

				// construct and return a clone set internal field ref

	virtual tmp<fvPatchField<Type>> clone (
				const DimensionedField<Type, volMesh>& iF ) const  {
		return tmp<fvPatchField<Type>> (
			new simpleEmissionFvPatchField<Type>(*this, iF) );
	}

				////////////////////////////
				//
				// DESTRUCTOR
				//
				////////////////////////////

	virtual ~simpleEmissionFvPatchField ( );

				////////////////////////////
				//
				// MEMBER FUNCTIONS
				//
				////////////////////////////

	virtual void evaluate  (
				const Pstream::commsTypes
				commsType = Pstream::commsTypes::blocking );

	virtual void write ( Ostream& ) const;		// write

};

				///////////////////////////////////////////////

} // End namespace Foam

				///////////////////////////////////////////////

#ifdef NoRepository
	#include "simpleEmissionFvPatchField.C"
#endif

				///////////////////////////////////////////////

#endif

				///////////////////////////////////////////////
