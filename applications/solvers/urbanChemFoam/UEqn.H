///////////////////////////////////////////////////////////////
//
//   UEqn.H
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
//   Defines prognostic equestion for momentum
//
///////////////////////////////////////////////////////////////

tmp<fvVectorMatrix> tUEqn  (
    fvm::ddt(rho, U) +
	fvm::div(phi, U) +
	turbulence->divDevRhoReff(U)
 ==
	( (2.0*Omega)^(rho*U) ) +
	fvOptions(rho,U) 
);

fvVectorMatrix& UEqn = tUEqn.ref();

UEqn.relax();
fvOptions.constrain(UEqn);

if (pimple.momentumPredictor())  {
	solve(UEqn == -fvc::grad(p));
	fvOptions.correct(U);
	K = 0.5*magSqr(U);
}

				///////////////////////////////////////////////
