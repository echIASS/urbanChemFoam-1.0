///////////////////////////////////////////////////////////////
//
//   EEqn.H
//   ------
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
//   Defines prognostic equestion for total energy
//
///////////////////////////////////////////////////////////////

{

    volScalarField& he = thermo.he();

    fvScalarMatrix EEqn  (
		fvm::ddt(rho, he) + mvConvection->fvmDiv(phi, he) +
		fvc::ddt(rho, K) + fvc::div(phi, K) + (
			he.name() == "e" ?
				fvc::div (
					fvc::absolute(phi/fvc::interpolate(rho), U),
					p,
					"div(phiv,p)" ) :
				-dpdt ) -
		fvm::laplacian(turbulence->alphaEff(), he)
		==
		fvOptions(rho, he)
    );

	EEqn.relax();
	fvOptions.constrain(EEqn);
	EEqn.solve();
	fvOptions.correct(he);
	thermo.correct();

	Info << "min/max(T) = "
		 << min(T).value() << ", " << max(T).value() << endl;

}

				///////////////////////////////////////////////
