///////////////////////////////////////////////////////////////
//
//  simpleEmissionFvPatchFields.H
//  -----------------------------
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
//	 simpleEmissionFvPatchFields
//
///////////////////////////////////////////////////////////////	

#ifndef simpleEmissionFvPatchFields_H
#define simpleEmissionFvPatchFields_H

#include "simpleEmissionFvPatchField.H"
#include "fieldTypes.H"

				///////////////////////////////////////////////
				
namespace Foam
{

				///////////////////////////////////////////////

makePatchTypeFieldTypedefs(simpleEmission);

				///////////////////////////////////////////////

} // End namespace Foam

				///////////////////////////////////////////////

#endif

				///////////////////////////////////////////////
