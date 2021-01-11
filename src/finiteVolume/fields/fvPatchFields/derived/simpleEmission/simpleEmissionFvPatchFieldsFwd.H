///////////////////////////////////////////////////////////////
//
//  simpleEmissionFvPatchFieldsFwd.H
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
//	 Forward declaration for class
//	 cyclicFixedValueFvPatchField
//
///////////////////////////////////////////////////////////////

#ifndef simpleEmissionFvPatchFieldsFwd_H
#define simpleEmissionFvPatchFieldsFwd_H

#include "fieldTypes.H"

				///////////////////////////////////////////////

namespace Foam
{

				///////////////////////////////////////////////

template<class Type> class simpleEmissionFvPatchField;

makePatchTypeFieldTypedefs(uniform);

				///////////////////////////////////////////////

} // End namespace Foam

				///////////////////////////////////////////////

#endif

				///////////////////////////////////////////////
