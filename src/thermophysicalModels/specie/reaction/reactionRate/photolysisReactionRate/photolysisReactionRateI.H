///////////////////////////////////////////////////////////////
// 
//	 photolysisReactionRateI.H
//	 -------------------------
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
//   Definition of Foam::photolysisReactionRate class
//	 properties and members
//
///////////////////////////////////////////////////////////////

#include "commonGlobals.H"	// where solar zenith angle is declared

				////////////////////////////////////////////////
				//
				// constructors
				//
				//////////////////////////////////////////////// 

inline Foam::photolysisReactionRate::photolysisReactionRate  (
			const scalar J0,
			const scalar m,
			const scalar n ) :
			J0_ (J0),
			m_ (m),
			n_ (n)
			{ }

inline Foam::photolysisReactionRate::photolysisReactionRate  (
			const speciesTable&,
			const dictionary& dict ) :
		    J0_(readScalar(dict.lookup("J0"))),
		    m_(readScalar(dict.lookup("m"))),
		    n_(readScalar(dict.lookup("n")))
			{ }

				////////////////////////////////////////////////
				//
				// calculate photolysis rate
				//
				// variable wrf_cos_theta is a global variable
				// from wrfFoam.  Since OF only allows rxn to
				// be sole functions of p and T, and photolysis
				// depends only on solar activity (i.e. theta),
				// using global variable is a very quicky way
				// around this limitation.
				//
				// I also don't want to calculate theta for all
				// the cells when I know it is uniform across
				// cells for the same time step.
				//
				// At some point I will need to figure out how
				// to incorporate cloudcover and albedo into
				// the photolysis rate, which are
				// cell dependent.
				//
				//////////////////////////////////////////////// 

inline Foam::scalar Foam::photolysisReactionRate::operator ( )  (
 			const scalar p,
			const scalar T,
			const scalarField& )  const  {

	scalar J0 = 0.0;

	if ( cosZenithAngle > VSMALL )  {
		J0 = J0_;
		if ( mag(m_) > vSmall )  J0 *= pow (cosZenithAngle,m_);
		if ( mag(n_) > vSmall )  J0 *= exp (-n_/cosZenithAngle);
	}

    return J0;
}

				////////////////////////////////////////////////
				//
				// reaction terms
				//
				//////////////////////////////////////////////// 

				// reaction rate

inline scalar photolysisReactionRate::ddT   (
					const scalar p,
					const scalar T,
					const scalarField& c ) const  {

	scalar J0 = 0.0;

	if ( cosZenithAngle > vSmall )  {
		J0 = J0_;
		if ( mag(m_) > vSmall )  J0 *= pow (cosZenithAngle,m_);
		if ( mag(n_) > vSmall )  J0 *= exp (-n_/cosZenithAngle);
	}

    return J0;

}

				// third body coefficients (1-alpha)

inline const List<Foam::Tuple2<label,scalar>>& photolysisReactionRate::beta() const  {
    return NullObjectRef<List<Tuple2<label, scalar>>>();
}

				// concentration derivative of pressure dependent term

inline scalar photolysisReactionRate::dcidc (
					const scalar p,
					const scalar T,
					const scalarField& c,
					scalarField& dcidc ) const  { return 0; }

				// temperature derivative of pressure dependent term

inline scalar photolysisReactionRate::dcidT (
					const scalar p,
				 	const scalar T,
					const scalarField& c ) const { return 0; }

				////////////////////////////////////////////////
				//
				// output functions
				//
				//////////////////////////////////////////////// 

inline void Foam::photolysisReactionRate::write ( Ostream& os ) const  {
    os.writeKeyword("J0") << J0_ << token::END_STATEMENT << nl;
    os.writeKeyword("m") << m_ << token::END_STATEMENT << nl;
    os.writeKeyword("n") << n_ << token::END_STATEMENT << nl;
}

inline Foam::Ostream& Foam::operator << (
			Ostream& os,
			const photolysisReactionRate& photolysis )  {

    photolysis.write(os);
    return os;

}

				////////////////////////////////////////////////
