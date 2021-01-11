///////////////////////////////////////////////////////////////
//
//  cyclicFixedValueFvPatchFields.H
//  -------------------------------
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

#ifndef cyclicFixedValueFvPatchFields_H
#define cyclicFixedValueFvPatchFields_H

				///////////////////////////////////////////////

#include "cyclicFixedValueFvPatchField.H"
#include "fieldTypes.H"

				///////////////////////////////////////////////

namespace Foam  {

makePatchTypeFieldTypedefs(cyclicFixedValue);

}  // namespace Foam

				///////////////////////////////////////////////

#endif

				///////////////////////////////////////////////
