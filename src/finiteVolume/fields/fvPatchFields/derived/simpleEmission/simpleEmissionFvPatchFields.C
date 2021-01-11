///////////////////////////////////////////////////////////////
//
//  simpleEmissionFvPatchFields.C
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
//	 Instantiates simpleEmissionFvPatchFields class
//
///////////////////////////////////////////////////////////////		

#include "simpleEmissionFvPatchFields.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"

				///////////////////////////////////////////////

namespace Foam
{

				///////////////////////////////////////////////

makePatchFields(simpleEmission);

				///////////////////////////////////////////////

} // End namespace Foam

				///////////////////////////////////////////////
