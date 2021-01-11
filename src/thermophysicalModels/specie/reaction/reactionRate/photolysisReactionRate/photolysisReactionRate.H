///////////////////////////////////////////////////////////////
// 
//	 photolysisReactionRate.H
//	 ------------------------
//
//	 ECC / IASS
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
//   Declaration of class Foam::photolysisReactionRate 
//
//	 Photolysis reaction rate is given by:
//
//	 	k = J0 x cos(theta)^m x exp [ -n / cos(theta) ]
//
//		where	J0, m, and n are user defined single-valued
//				constants supplied by the reaction dictionary
//
//				theta is the solar zenith angle.  The value of
//				cos(theta) is can be obtained from global
//				variable cosZenithAngle, updated
//				at the start of each iteration
//
///////////////////////////////////////////////////////////////

#ifndef __PHOTOLYSISREACTIONRATE_H__
#define __PHOTOLYSISREACTIONRATE_H__

#include "scalarField.H"
#include "typeInfo.H"

#include "commonGlobals.H"	// where solar zenith angle is declared

				////////////////////////////////////////////////

namespace Foam  {

				// Forward declaration of friend functions and operators

class photolysisReactionRate;
Ostream& operator << ( Ostream&, const photolysisReactionRate& );

				////////////////////////////////////////////////
				//
				//  class photolysisRate Declaration
				//
				////////////////////////////////////////////////

class photolysisReactionRate  {

	scalar J0_;
	scalar m_;
	scalar n_;

public:

		        // constructors

	inline photolysisReactionRate (
				const scalar J0, const scalar m, const scalar n );
	inline photolysisReactionRate (
				const speciesTable& species, const dictionary& dict );

				// member functions

	static word type ( ) { return "photolysis"; }
	inline scalar operator ( ) (
				const scalar p, const scalar T, const scalarField& c ) const;

				// reaction terms

				// reaction rate
	inline scalar ddT   ( const scalar p,
						  const scalar T,
						  const scalarField& c ) const;

				// third body coefficients (1 - alpha)
	inline const List<Tuple2<label, scalar>>& beta() const;

				// concentration derivative of pressure dependent term
	inline scalar dcidc ( const scalar p,
						  const scalar T,
						  const scalarField& c,
						  scalarField& dcidc ) const;

				// temperature derivative of pressure dependent term
	inline scalar dcidT ( const scalar p,
						  const scalar T,
						  const scalarField& c ) const;

		        // Output

	inline void write ( Ostream& os ) const;
	inline friend Ostream& operator << ( Ostream&, const photolysisReactionRate& );

};

} // End namespace Foam

				////////////////////////////////////////////////
				
#include "photolysisReactionRateI.H"

#endif

				////////////////////////////////////////////////
