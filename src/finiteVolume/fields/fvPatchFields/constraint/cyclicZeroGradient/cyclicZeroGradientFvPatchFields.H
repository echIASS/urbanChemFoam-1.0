///////////////////////////////////////////////////////////////
//
//  cyclicZeroGraidentFvPatchFields.H
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
//	 Instantiates typedef for class
//	 cyclicFixedValueFvPatchField
//
///////////////////////////////////////////////////////////////	

#ifndef cyclicZeroGradientFvPatchFields_H
#define cyclicZeroGradientFvPatchFields_H

				///////////////////////////////////////////////

#include "cyclicZeroGradientFvPatchField.H"
#include "fieldTypes.H"

				///////////////////////////////////////////////

namespace Foam  {

makePatchTypeFieldTypedefs(cyclicZeroGradient);

}  // namespace Foam

				///////////////////////////////////////////////

#endif

				///////////////////////////////////////////////
