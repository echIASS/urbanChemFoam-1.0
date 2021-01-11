///////////////////////////////////////////////////////////////
//
//  solarUtils.C
//  ------------
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
//	 Functions for calculating solar state
//
///////////////////////////////////////////////////////////////

#include "commonConstants.H"
#include "solarUtils.H"

scalar solarGetZenithAngle ( scalar jd, scalar phi, scalar lam )  {

	scalar theta = 90.0;	// solar zenith angle [deg]

	scalar jc;				// julian century since Jan 1, 2000
	scalar lam0m;			// geo. mean of solar longitude [deg]
	scalar M;				// geo. mean of solar anomaly [deg]
	scalar e;				// eccentricity of earth's orbit [ ]
	scalar C;				// eqn center for the sun [deg]
	scalar lam0t;			// true solar longitude [deg]
	scalar lam0a;			// apparent solar longitude [deg]
	scalar eps0;			// mean obliquity [deg]
	scalar eps;				// corrected obliquity [deg]
	scalar delta;			// solar declination [deg]
	scalar te22;			// tan^2(eps/2) [ ] (used only for dt)
	scalar dt;				// equation of time [min]
	scalar tau;				// true solar time [min]
	scalar H;				// hour angle [min]

	jc     = ( jd - 2451545.0 ) / 36525.0;
	lam0m = fmod ( 280.46646 + jc*(36000.76983+0.0003032*jc), 360.0 );
	M      = 357.52911 + jc*(35999.05029-0.0001537*jc);
	e      = 0.016708634 - jc*(0.000042037+0.0000001267*jc);
	C      = Foam::sin(D2R*M) * ( 1.914602 - jc*(0.004817+0.000014*jc) ) +
				Foam::sin(2.0*D2R*M) * ( 0.019993 - 0.000101*jc ) +
				Foam::sin(3.0*D2R*M) * 0.000289;
	lam0t = lam0m + C;
	lam0a = lam0t - 0.00569 - 0.00478*Foam::sin(D2R*(125.04-1934.136*jc));
	eps0   = 23.0 + (26.0 + (
					(21.448-jc*(46.815+jc*(0.00059-0.001813*jc)))
				)/60.0)/60.0;
	eps    = eps0 + 0.00256*Foam::cos(D2R*(125.04-1934.136*jc));
	delta  = R2D * Foam::asin ( Foam::sin(D2R*eps) * Foam::sin(D2R*lam0a) );
	te22   = Foam::tan (0.5*D2R*eps) * Foam::tan (0.5*D2R*eps);
	dt     = (4.0*R2D)*( te22*Foam::sin(2.0*D2R*lam0m)-2.0*e*Foam::sin(D2R*M) +
				4.0*e*te22*Foam::sin(D2R*M)*Foam::cos(2.0*D2R*lam0m) -
				0.5*(te22*te22)*Foam::sin(4.0*D2R*lam0m) -
				1.25*(e*e)*Foam::sin(2.0*D2R*M) );
	tau    = fmod (
				1440.0*((jd+0.5)-floor(jd+0.5)) +
				dt + ( 4.0*lam ), 1440.0 );
	H      = 0.25*tau + 180.0*static_cast<scalar>((tau<0.0)-(tau>0.0));

	theta  = R2D * Foam::acos( Foam::sin(D2R*phi)*Foam::sin(D2R*delta) +
				Foam::cos(D2R*phi)*Foam::cos(D2R*delta)*Foam::cos(D2R*H) );

	return (theta);

}
