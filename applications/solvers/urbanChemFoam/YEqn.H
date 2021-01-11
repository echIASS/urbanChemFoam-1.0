///////////////////////////////////////////////////////////////
//
//   YEqn.H
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
//   Defines prognostic equestion for species mass fraction
//
//	 Notes:
//   1) This incorporates simpleEmission boundary
//   2) Advective terms and diffusion for radicals
//	    (O, hardcoded) are removed to speed up calculation
//   3) Inert species (usually N2) is calculted by
//      complement (ie, 1 - Y of all other species)
//
///////////////////////////////////////////////////////////////

tmp<fv::convectionScheme<scalar>> mvConvection
(
    fv::convectionScheme<scalar>::New
    (
        mesh,
        fields,
        phi,
        mesh.divScheme("div(phi,Yi_h)")
    )
);

{
    reaction->correct();
    volScalarField Yt(0.0*Y[0]);	// mass fractions sum sans inert

    forAll(Y, i)
    {
        if (i != inertIndex && composition.active(i))  {

            volScalarField& Yi    = Y[i];

				// source term and initiate them to zero

			word rhoSpName ( "emissionSourceTerm" );
			volScalarField  rhoSp	(	IOobject (
											rhoSpName,
											runTime.timeName(),
											mesh,
											IOobject::NO_READ,
											IOobject::NO_WRITE ),
										mesh,
										dimensionedScalar (
											rhoSpName,
											dimensionSet(1,-3,-1,0,0,0,0),
											Zero ) );

				// if species is attached to a simpleEmission boundary condition
				// transfer data from global surface emission

			label iEmissSp = commonPatchEmissions::getSpeciesIndex (
								commonPatchEmissions::speciesNamesList,
								Yi.name() );
			
			if ( ! ( iEmissSp < 0 ) )  {

				const Field<scalar> & densitySource =
						commonPatchEmissions::densitySourceTerms[iEmissSp];

				forAll ( rhoSp, iCell )  rhoSp.ref()[iCell] = densitySource[iCell];

			}

			if ( Yi.name() == "O" )  {

				fvScalarMatrix YiEqn  (
					  fvm::ddt(rho, Yi)
					==
					  reaction->R(Yi)
					+ fvOptions(rho, Yi)
					+ rhoSp		// emission source term
				);

				YiEqn.relax();
				fvOptions.constrain(YiEqn);
				YiEqn.solve("Yi");
			}

			else  {

				fvScalarMatrix YiEqn  (
					  fvm::ddt(rho, Yi)
					+ mvConvection->fvmDiv(phi, Yi)
					- fvm::laplacian(turbulence->muEff(), Yi)
					==
					  reaction->R(Yi)
					+ fvOptions(rho, Yi)
					+ rhoSp		// emission source term
				);

				YiEqn.relax();
				fvOptions.constrain(YiEqn);
				YiEqn.solve("Yi");
			}

			fvOptions.correct(Yi);
			Yi.max(0.0);
			Yt += Yi;

        }
    }


    Y[inertIndex] = scalar(1) - Yt;
    Y[inertIndex].max(0.0);
}

				///////////////////////////////////////////////
