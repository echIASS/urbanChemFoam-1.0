///////////////////////////////////////////////////////////////
//
//  cyclicZeroGradientFvPatchField.C
//  ---------------------------------
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
//   Definition of cyclicZeroGradientFvPatchField class
//	 properties and members
//
///////////////////////////////////////////////////////////////

#include "cyclicZeroGradientFvPatchField.H"
#include "transformField.H"

				///////////////////////////////////////////////
				//
				// CONSTRUCTORS
				//
				///////////////////////////////////////////////

template<class Type>
Foam::cyclicZeroGradientFvPatchField<Type>::cyclicZeroGradientFvPatchField  (
				const fvPatch& p,
				const DimensionedField<Type,volMesh>& iF
		) :
				coupledFvPatchField<Type>(p, iF),
				cyclicPatch_( refCast<const cyclicFvPatch>(p) )
{ }

				///////////////////////////////////////////////

template<class Type>
Foam::cyclicZeroGradientFvPatchField<Type>::cyclicZeroGradientFvPatchField  (
				const fvPatch& p,
				const DimensionedField<Type,volMesh>& iF,
				const dictionary& dict
		) :
				coupledFvPatchField<Type>( p, iF, dict, false ),
				cyclicPatch_( refCast<const cyclicFvPatch>(p) )
{

	if ( !isA<cyclicFvPatch>(p) )  {
		FatalIOErrorInFunction ( dict )
			<< "    patch type '"        << p.type()
			<< "' not constraint type '" << typeName << "'\n"
			<< "    for patch "          << p.name()
			<< " of field "              << this->internalField().name()
			<< " in file "               << this->internalField().objectPath()
			<< exit ( FatalIOError );
	}

	this->evaluate ( Pstream::commsTypes::blocking );

}

				///////////////////////////////////////////////

template<class Type>
Foam::cyclicZeroGradientFvPatchField<Type>::cyclicZeroGradientFvPatchField  (
				const cyclicZeroGradientFvPatchField<Type>& ptf,
				const fvPatch& p,
				const DimensionedField<Type,volMesh>& iF,
				const fvPatchFieldMapper& mapper
		) :
				coupledFvPatchField<Type>(ptf, p, iF, mapper),
				cyclicLduInterfaceField(),
				cyclicPatch_( refCast<const cyclicFvPatch>(p) )
{
	
	if ( !isA<cyclicFvPatch>(this->patch()) )  {
		FatalErrorInFunction 
			<< "    patch type '"        << p.type()
			<< "' not constraint type '" << typeName << "'\n"
			<< "    for patch "          << p.name()
			<< " of field "              << this->internalField().name()
			<< " in file "               << this->internalField().objectPath()
			<< exit ( FatalIOError );
	}
}

				///////////////////////////////////////////////

template<class Type>
Foam::cyclicZeroGradientFvPatchField<Type>::cyclicZeroGradientFvPatchField  (
				const cyclicZeroGradientFvPatchField<Type>& ptf
		) :
				coupledFvPatchField<Type>(ptf),
				cyclicLduInterfaceField(),
				cyclicPatch_( ptf.cyclicPatch_ )
{ }

				///////////////////////////////////////////////

template<class Type>
Foam::cyclicZeroGradientFvPatchField<Type>::cyclicZeroGradientFvPatchField  (
				const cyclicZeroGradientFvPatchField<Type>& ptf,
				const DimensionedField<Type,volMesh>& iF
		) :
				coupledFvPatchField<Type>(ptf),
				cyclicLduInterfaceField(),
				cyclicPatch_( ptf.cyclicPatch_ )
{ }

				///////////////////////////////////////////////
				//
				// VIRTUAL MEMBER FUNCTIONS
				//
				///////////////////////////////////////////////

template<class Type>
void Foam::cyclicZeroGradientFvPatchField<Type>::write ( Ostream& os ) const  {
	fvPatchField<Type>::write(os);
	writeEntry ( os, "value", *this );
}

				///////////////////////////////////////////////

template<class Type>
void Foam::cyclicZeroGradientFvPatchField<Type>::updateInterfaceMatrix  (
				scalarField& result,
				const scalarField& psiInternal,
				const scalarField& coeffs,
				const direction cmpt,
				const Pstream::commsTypes ) const  {

				// does nothing; interface matrix will not be modified
				// based on the neighboring patch

}

				///////////////////////////////////////////////

template<class Type>
void Foam::cyclicZeroGradientFvPatchField<Type>::updateInterfaceMatrix  (
				Field<Type>& result,
				const Field<Type>& psiInternal,
				const scalarField& coeffs,
				const Pstream::commsTypes ) const  {

				// does nothing; interface matrix will not be modified
				// based on the neighboring patch

}

				///////////////////////////////////////////////
				//
				// MEMBER FUNCTIONS
				//
				///////////////////////////////////////////////

				// note, despite the name, this function returns
				// the originating patch field instead of the
				// neighbor patch field

template<class Type>
Foam::tmp<Foam::Field<Type>>
Foam::cyclicZeroGradientFvPatchField<Type>::patchNeighbourField() const  {

	const Field<Type> & internalField   = this->primitiveField ( );
	const fvPatch     & boundaryPatches = this->patch ( );
	const labelUList  & patchIndices    = const_cast<labelUList &>(
												boundaryPatches.faceCells ( ) );
	
	tmp<Field<Type>> tpnf( new Field<Type>(this->size()) );
	Field<Type> & pnf = tpnf.ref();

	forAll ( pnf, iPatch )  {
		label iCell = patchIndices[iPatch];
		pnf[iPatch] = internalField[iCell];
	}

	return tpnf;

}

				///////////////////////////////////////////////

				// note, despite the name, this function returns
				// the originating patch field instead of the
				// neighbor patch field

template<class Type>
const Foam::cyclicZeroGradientFvPatchField<Type>&
Foam::cyclicZeroGradientFvPatchField<Type>::neighbourPatchField() const  {

	const GeometricField<Type,fvPatchField,volMesh> & fld =
		static_cast<const GeometricField<Type,fvPatchField,volMesh>&> (
			this->primitiveField( ) );

	return refCast<const cyclicZeroGradientFvPatchField<Type>> (
		fld.boundaryField()[this->patch().index()] );

}

				///////////////////////////////////////////////
