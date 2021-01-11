///////////////////////////////////////////////////////////////
//
//  cyclicZeroGradientFvPatchField.H
//  --------------------------------
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
//   Declaration of class cyclicZeroGradientFvPatchField 
//
//	 This class overrides the cyclic boudnary patch and
//	 applies a zero gradient boundary condition on the
//	 scalar variable.  It can only be applied to a cyclic
//	 patch.
//
///////////////////////////////////////////////////////////////			

#ifndef cyclicZeroGradientFvPatchField_H
#define cyclicZeroGradientFvPatchField_H

				///////////////////////////////////////////////

#include "coupledFvPatchField.H"		// already includes fvPatchField.H
#include "cyclicLduInterfaceField.H"
#include "cyclicFvPatch.H"

				///////////////////////////////////////////////

namespace Foam  {

				///////////////////////////////////////////////
				//
				// DECLARACTION FOR CLASS
				// cyclicZeroGradient
				//
				///////////////////////////////////////////////

template<class Type>
class cyclicZeroGradientFvPatchField
: public coupledFvPatchField<Type>,
  public cyclicLduInterfaceField  {

private    :

	const cyclicFvPatch& cyclicPatch_;

//protected  :
public     :

	TypeName("cyclicZeroGradient");

				///////////////////////////////////////////////
				//
				// CONSTRUCTORS
				//
				///////////////////////////////////////////////

	cyclicZeroGradientFvPatchField  (	// from patch + internal field
				const fvPatch&,
				const DimensionedField<Type,volMesh>& );

	cyclicZeroGradientFvPatchField  (	// from patch + internal field + dictionary
				const fvPatch&,
				const DimensionedField<Type,volMesh>&,
				const dictionary& );

	cyclicZeroGradientFvPatchField  (	// by mapping given instance on new patch
				const cyclicZeroGradientFvPatchField<Type>&,
				const fvPatch&,
				const DimensionedField<Type,volMesh>&,
				const fvPatchFieldMapper& );

	cyclicZeroGradientFvPatchField  (	// copy constructor
				const cyclicZeroGradientFvPatchField<Type>& );

	cyclicZeroGradientFvPatchField  (	// copy constructor set internal field ref
				const cyclicZeroGradientFvPatchField<Type>&,
				const DimensionedField<Type,volMesh>& );

				///////////////////////////////////////////////
				//
				// VIRTUAL MEMBER FUNCTIONS
				//
				///////////////////////////////////////////////

				// construct and return clone

	virtual tmp<fvPatchField<Type>> clone ( ) const  {
		return tmp<fvPatchField<Type>> (
			new cyclicZeroGradientFvPatchField<Type>(*this) );
	}

				// construct and return clone set internal field ref

	virtual tmp<fvPatchField<Type>> clone (
				const DimensionedField<Type,volMesh>& iF ) const  {
		return tmp<fvPatchField<Type>> (
			new cyclicZeroGradientFvPatchField<Type>(*this,iF) );
	}

				// does patchField perform transformation?

	virtual bool doTransform ( ) const  {
		return !(cyclicPatch_.parallel() || pTraits<Type>::rank == 0);
	}

				// return face transformation tensor

	virtual const tensorField& forwardT ( ) const  {
		return cyclicPatch_.forwardT ( );
	}
	
				// return neighbor face transformation tensor

	virtual const tensorField& reverseT ( ) const  {
		return cyclicPatch_.reverseT ( );
	}

				// return ranke of transformation tensor component

	virtual int rank ( ) const  { return pTraits<Type>::rank; }

				// write out patchField

	virtual void write ( Ostream& os ) const;

				// update result field from interface

	virtual void updateInterfaceMatrix  (
				scalarField& result,
				const scalarField& psiInternal,
				const scalarField& coeffs,
				const direction cmpt,
				const Pstream::commsTypes commsType ) const;

	virtual void updateInterfaceMatrix  (
				Field<Type>& result,
				const Field<Type>& psiInternal,
				const scalarField& coeffs,
				const Pstream::commsTypes commsType ) const;

				///////////////////////////////////////////////
				//
				// MEMBER FUNCTIONS
				//
				///////////////////////////////////////////////

	const cyclicFvPatch& cyclicPatch ( ) const  { return cyclicPatch_; }

				// neighbor internal cell data

	tmp<Field<Type>> patchNeighbourField ( ) const;

				// neighbor internal patchField ref

	const cyclicZeroGradientFvPatchField<Type>&
				neighbourPatchField() const;

};

				///////////////////////////////////////////////

}  // namespace Foam

				///////////////////////////////////////////////

#ifdef NoRepository
#include "cyclicZeroGradientFvPatchField.C"
#endif

#endif

				///////////////////////////////////////////////
