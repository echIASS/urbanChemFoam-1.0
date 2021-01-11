///////////////////////////////////////////////////////////////
//
//   pEqn.H
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
//   Defines prognostic equestion for pressure correction
//	 with Courant number consistency
//
///////////////////////////////////////////////////////////////

rho = thermo.rho();

volScalarField rAU(1.0/UEqn.A());
volScalarField rAtU(1.0/(1.0/rAU - UEqn.H1()));
volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));

if (pimple.nCorrPiso() <= 1)
{
    tUEqn.clear();
}

if (pimple.transonic())
{
    surfaceScalarField phid
    (
        "phid",
        fvc::interpolate(psi)
       *(
            fvc::flux(HbyA)
        )
    );

    surfaceScalarField phic
    (
        "phic",
        fvc::interpolate(rho*(rAtU - rAU))*fvc::snGrad(p)*mesh.magSf()
    );

    HbyA -= (rAU - rAtU)*fvc::grad(p);

    volScalarField rhorAtU("rhorAtU", rho*rAtU);

    while (pimple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvm::ddt(psi, p)
          + fvm::div(phid, p)
          + fvc::div(phic)
          - fvm::laplacian(rhorAtU, p)
         ==
            fvOptions(psi, p, rho.name())
        );

        pEqn.solve();

        if (pimple.finalNonOrthogonalIter())
        {
            phi == phic + pEqn.flux();
        }
    }
}
else
{
    surfaceScalarField phiHbyA
    (
        "phiHbyA",
        (
            fvc::flux(rho*HbyA)
         )
    );

    phiHbyA += fvc::interpolate(rho*(rAtU - rAU))*fvc::snGrad(p)*mesh.magSf();
    HbyA -= (rAU - rAtU)*fvc::grad(p);

    volScalarField rhorAtU("rhorAtU", rho*rAtU);

    constrainPressure(p, rho, U, phiHbyA, rhorAtU);

    while (pimple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvm::ddt(psi, p)
          + fvc::div(phiHbyA)
          - fvm::laplacian(rhorAtU, p)
         ==
            fvOptions(psi, p, rho.name())
        );

        pEqn.solve();

        if (pimple.finalNonOrthogonalIter())
        {
            phi = phiHbyA + pEqn.flux();
        }
    }
}

#include "rhoEqn.H"
#include "compressibleContinuityErrs.H"

// Explicitly relax pressure for momentum corrector
p.relax();

U = HbyA - rAtU*fvc::grad(p);
U.correctBoundaryConditions();
fvOptions.correct(U);
K = 0.5*magSqr(U);

if (pressureControl.limit(p))
{
    p.correctBoundaryConditions();
    rho = thermo.rho();
}

if (thermo.dpdt())
{
    dpdt = fvc::ddt(p);
}
