///////////////////////////////////////////////////////////////
//
//  simpleEmissionFvPatchField.C
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
//   Definition of simpleEmissionFvPatchField class
//	 properties and members
//
///////////////////////////////////////////////////////////////

#include "simpleEmissionFvPatchField.H"
#include "objectRegistry.H"
#include <type_traits>

#define DEFAULT_SECTION_LENGTH          1.0
#define DEFAULT_TIME_TO_SECONDS_FACTOR  1.0
#define DEFAULT_EMISSION_SCALE_FACTOR   1.0
#define DEFAULT_SHOW_EMISSION_MASS      false

				///////////////////////////////////////////////
				//
				// CONSTRUCTORS
				//
				///////////////////////////////////////////////

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::simpleEmissionFvPatchField (
		const fvPatch& p,
		const DimensionedField<Type, volMesh>& iF
	) :
		fixedValueFvPatchField<Type>(p, iF),
		sectionRateProfile_(),
		sectionLength_(DEFAULT_SECTION_LENGTH),
		timeToSecondsFactor_(DEFAULT_TIME_TO_SECONDS_FACTOR), 
		emissionScaleFactor_(DEFAULT_EMISSION_SCALE_FACTOR),
		showEmissionMass_(DEFAULT_SHOW_EMISSION_MASS),
		iSpecies_(-1),
		speciesName_(this->internalField().name()),
		initiated_(false),
		speciesAssigned_(false),
		patchCellIndices(0),
		patchAreaWeights(0),
		patchWeights(0)
{ }

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::simpleEmissionFvPatchField (
		const fvPatch& p,
		const DimensionedField<Type, volMesh>& iF,
		const Field<scalar>& fld
	) :
		fixedValueFvPatchField<Type>(p, iF, fld),
		sectionRateProfile_(),
		sectionLength_(DEFAULT_SECTION_LENGTH),
		timeToSecondsFactor_(DEFAULT_TIME_TO_SECONDS_FACTOR), 
		emissionScaleFactor_(DEFAULT_EMISSION_SCALE_FACTOR),
		showEmissionMass_(DEFAULT_SHOW_EMISSION_MASS),
		iSpecies_(-1),
		speciesName_(this->internalField().name()),
		initiated_(false),
		speciesAssigned_(false),
		patchCellIndices(0),
		patchAreaWeights(0),
		patchWeights(0)
{ }

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::simpleEmissionFvPatchField (
		const fvPatch& p,
		const DimensionedField<Type, volMesh>& iF,
		const dictionary& dict
	) :
		fixedValueFvPatchField<Type>(p, iF, dict, false),
		sectionRateProfile_(Function1<Type>::New("sectionRateProfile", dict)),
		sectionLength_(dict.lookupOrDefault("sectionLength", DEFAULT_SECTION_LENGTH)),
		timeToSecondsFactor_(dict.lookupOrDefault("timeToSecondsFactor", DEFAULT_TIME_TO_SECONDS_FACTOR)),
		emissionScaleFactor_(dict.lookupOrDefault("emissionScaleFactor", DEFAULT_EMISSION_SCALE_FACTOR)),
		showEmissionMass_(dict.lookupOrDefault("showEmissionMass", DEFAULT_SHOW_EMISSION_MASS)),
		iSpecies_(-1),
		speciesName_(this->internalField().name()),
		initiated_(false),
		speciesAssigned_(false),
		patchCellIndices(0),
		patchCellVolumes(0),
		patchAreaWeights(0),
		patchWeights(0)
{
	this->evaluate();
}

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::simpleEmissionFvPatchField (
		const simpleEmissionFvPatchField<Type>& ptf,
		const fvPatch& p,
		const DimensionedField<Type, volMesh>& iF,
		const fvPatchFieldMapper& mapper
	) :
		fixedValueFvPatchField<Type>(ptf, p, iF, mapper, false),	// don't map
		sectionRateProfile_(ptf.sectionRateProfile_, false),
		sectionLength_(ptf.sectionLength_),
		timeToSecondsFactor_(ptf.timeToSecondsFactor_),
		emissionScaleFactor_(ptf.emissionScaleFactor_),
		showEmissionMass_(ptf.showEmissionMass_),
		iSpecies_(ptf.iSpecies_),
		speciesName_(ptf.speciesName_),
		initiated_(ptf.initiated_),
		speciesAssigned_(ptf.speciesAssigned_),
		patchCellIndices(ptf.patchCellIndices),
		patchCellVolumes(ptf.patchCellVolumes),
		patchAreaWeights(ptf.patchAreaWeights),
		patchWeights(ptf.patchWeights)
{
	this->evaluate();	// evaluate since value not mapped
}

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::simpleEmissionFvPatchField (
		const simpleEmissionFvPatchField<Type>& ptf
	) :
		fixedValueFvPatchField<Type>(ptf),
		sectionRateProfile_(ptf.sectionRateProfile_, false),
		sectionLength_(ptf.sectionLength_),
		timeToSecondsFactor_(ptf.timeToSecondsFactor_),
		showEmissionMass_(ptf.showEmissionMass_),
		iSpecies_(ptf.iSpecies_),
		speciesName_(ptf.speciesName_),
		initiated_(ptf.initiated_),
		speciesAssigned_(ptf.speciesAssigned_),
		patchCellIndices(ptf.patchCellIndices),
		patchCellVolumes(ptf.patchCellVolumes),
		patchAreaWeights(ptf.patchAreaWeights),
		patchWeights(ptf.patchWeights)
{ }

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::simpleEmissionFvPatchField (
		const simpleEmissionFvPatchField<Type>& ptf,
		const DimensionedField<Type, volMesh>& iF
	) :
		fixedValueFvPatchField<Type>(ptf, iF),
		sectionRateProfile_(ptf.sectionRateProfile_, false),
		sectionLength_(ptf.sectionLength_),
		timeToSecondsFactor_(ptf.timeToSecondsFactor_),
		emissionScaleFactor_(ptf.emissionScaleFactor_),
		showEmissionMass_(ptf.showEmissionMass_),
		iSpecies_(ptf.iSpecies_),
		speciesName_(ptf.speciesName_),
		initiated_(ptf.initiated_),
		speciesAssigned_(ptf.speciesAssigned_),
		patchCellIndices(ptf.patchCellIndices),
		patchCellVolumes(ptf.patchCellVolumes),
		patchAreaWeights(ptf.patchAreaWeights),
		patchWeights(ptf.patchWeights)
{
	if (ptf.sectionRateProfile_.valid())  { this->evaluate(); }	 // evaluate if defined
}

				///////////////////////////////////////////////
				//
				// DESTRUCTOR
				//
				///////////////////////////////////////////////

template<class Type>
Foam::simpleEmissionFvPatchField<Type>::~simpleEmissionFvPatchField ( )  { }

				///////////////////////////////////////////////
				//
				// VIRTUAL MEMBER FUNCTIONS
				//
				///////////////////////////////////////////////

				///////////////////////////////////////////////
				// evaluate boundary conditions
				///////////////////////////////////////////////

template<class Type>
void Foam::simpleEmissionFvPatchField<Type>::evaluate (
		const Pstream::commsTypes commsType )  {

	initiatePatches ( ); 	// initiate (start of simulation)
	updatePatchField ( ); 	// get source terms (start of time step)

				// update boundary patch values
				// same as zero gradient to prevent
				// mass flux out of patch surface

	fvPatchField<Type>::operator==(this->patchInternalField());
	fvPatchField<Type>::evaluate();

}

				///////////////////////////////////////////////
				// write boundary condition
				///////////////////////////////////////////////

template<class Type>
void Foam::simpleEmissionFvPatchField<Type>::write(Ostream& os) const  {
	fvPatchField<Type>::write(os);
	writeEntry ( os, sectionRateProfile_() );
	writeEntry ( os, "sectionLength", sectionLength_ );
	writeEntry ( os, "timeToSecondsFactor", timeToSecondsFactor_ );
	writeEntry ( os, "showEmissionMass", showEmissionMass_ );
	writeEntry ( os, "value", *this );
}

				///////////////////////////////////////////////
				//
				// MEMBER FUNCTIONS
				//
				///////////////////////////////////////////////

				///////////////////////////////////////////////
				// assign species indices
				///////////////////////////////////////////////

template<class Type>
void Foam::simpleEmissionFvPatchField<Type>::assignSpecies (
				const DynamicList<word> & speciesList, const word & speciesName )  {
	if ( speciesAssigned () )  return;
	iSpecies_ = commonPatchEmissions::getSpeciesIndex ( speciesList, speciesName );
	speciesAssigned_ = true;
}

				///////////////////////////////////////////////
				// get cell densities
				///////////////////////////////////////////////

template<class Type>
Foam::volScalarField Foam::simpleEmissionFvPatchField<Type>::getCellDensity ( )  {

				// placeholder	

	Foam::volScalarField cellDensity = this->db().template
			lookupObject<volScalarField>("thermo:rho");
	
				// if field exists, take it, otherwise estimate
				// using gas constant

	if ( this->db().template foundObject<volScalarField>("rho") )  {
		cellDensity = this->db().template lookupObject<volScalarField>("rho");
	}
	else  {
		const dimensionedScalar RAIR (      // specific gas constant for air
					"RAIR", dimensionSet(0,2,-2,-1,0,0,0), scalar(287.04) );
		const volScalarField & pressure    = this->db().template lookupObject<volScalarField>("p");
		const volScalarField & temperature = this->db().template lookupObject<volScalarField>("T");
		cellDensity = pressure / ( temperature * RAIR );
	}

	return (cellDensity);

}

				///////////////////////////////////////////////
				// initiate patches (at start of simulation)
				///////////////////////////////////////////////

template<class Type>
void Foam::simpleEmissionFvPatchField<Type>::initiatePatches ( )  {

				// no need to initiate again

	if ( initiated() )  return;

				// obtain mesh information

	const label       & meshSize    = this->internalField().size();		// cell count
	const scalarField & cellVolumes = this->internalField().mesh().V();	// cell volumes

				// patch information ( total surface emissions are
				// split according to patch area )

	const fvPatch     & boundaryPatches = this->patch();			// patch reference
	const scalarField & patchAreas      = boundaryPatches.magSf();	// patch areas
	scalar boundaryArea = gSum ( patchAreas );						// boundary area

	labelUList & patchIndices = const_cast<labelUList &>(			// patch cell indices
								boundaryPatches.faceCells( ) );

				// append name and scalar fields to global emission lists

	commonPatchEmissions::allocateSpecies ( speciesName(), meshSize, initiated() );

				// assign mesh indices and patch weights

	forAll ( boundaryPatches, iPatch )  {
		label  iCell      = patchIndices[iPatch];
		patchCellIndices.append ( iCell );
		patchCellVolumes.append ( cellVolumes[iCell] );
		patchAreaWeights.append ( patchAreas[iPatch] / boundaryArea );
		patchWeights.append     ( patchAreaWeights[iPatch] / patchCellVolumes[iPatch] );
	}

	initiated_ = true;	// initiation is now complete

}

				///////////////////////////////////////////////
				// update patch field values
				///////////////////////////////////////////////

template<class Type>
void Foam::simpleEmissionFvPatchField<Type>::updatePatchField ( )  {

	if ( ! firstSolverIteration )  return;
	assignSpecies ( commonPatchEmissions::speciesNamesList, speciesName() );

				// time and emission rate information

	const scalar & timeValue       = this->db().time().timeOutputValue();
	const scalar   emissionRate = emissionScaleFactor() * sectionLength() *
									sectionRateProfile ( timeValue );

				// get cell densities

	Foam::volScalarField density = getCellDensity ( );

				// clear old terms (just to be sure)

	forAll ( commonPatchEmissions::densitySourceTerms[iSpecies()], iCell )
		commonPatchEmissions::densitySourceTerms[iSpecies()][iCell] = Zero;

				// accumulate internal field values

	forAll ( patchCellVolumes, iPatch )  {
		const label  & iCell              = patchCellIndices[iPatch];
		const scalar   cellAirMass        = density[iCell] * patchCellVolumes[iPatch];
		const scalar   cellEmissionSource = (emissionRate/cellAirMass) *
													patchAreaWeights[iPatch];
		commonPatchEmissions::densitySourceTerms[iSpecies()][iCell] =
													density[iCell] * cellEmissionSource;
	}

				// check domain mass balance

	checkEmissionMass ( timeValue, emissionRate );

}

				///////////////////////////////////////////////
				// check mass balance
				///////////////////////////////////////////////

template<class Type>
void Foam::simpleEmissionFvPatchField<Type>::checkEmissionMass (
							scalar timeValue, scalar massRate )  {

	if ( ! showEmissionMass() )  return;

				// basic field information

	const DimensionedField<Type,volMesh> & cellMassFractions = this->internalField();
	const fvMesh      & meshCells   = cellMassFractions.mesh();	
	const scalarField & cellVolumes = meshCells.V ( );
	const volScalarField density    = getCellDensity ( );

	DynamicList<scalar> cellMass(0);			// mass of entire mixture in cell
	DynamicList<scalar> cellSpeciesMass(0);		// mass of emission species in cell

				// calculate cell masses

	forAll ( cellMassFractions, iCell )  {
		scalar thisCellMass        = cellVolumes[iCell] * density[iCell];
		scalar thisCellSpeciesMass = thisCellMass * mag(cellMassFractions[iCell]);
		cellMass.append(thisCellMass);
		cellSpeciesMass.append(thisCellSpeciesMass);
	}

				// sum up masses across all domains

	scalar domainMass        = gSum ( cellMass );
	scalar domainSpeciesMass = gSum ( cellSpeciesMass );

				// output

	Info << " *** simpleEmission " << speciesName() << " :  "
		 << "t = " << timeValue << "  "
		 << "tSeries = " << timeValue / timeToSecondsFactor() << "  "
		 << "m = " << domainSpeciesMass <<  " / " << domainMass << "  "
		 << " r = " << massRate << "  "
		 << " rSection = " << massRate / sectionLength() << endl;

}

				///////////////////////////////////////////////
