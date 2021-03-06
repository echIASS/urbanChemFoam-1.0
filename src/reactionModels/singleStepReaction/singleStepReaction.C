/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "singleStepReaction.H"
#include "fvmSup.H"

namespace Foam
{
namespace reactionModels
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
singleStepReaction<ReactionThermo, ThermoType>::singleStepReaction
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressibleTurbulenceModel& turb,
    const word& reactionProperties
)
:
    ThermoReaction<ReactionThermo>(modelType, thermo, turb),
    singleMixturePtr_(nullptr),
    wFuel_
    (
        IOobject
        (
            this->thermo().phasePropertyName("wFuel"),
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        this->mesh(),
        dimensionedScalar(dimMass/dimVolume/dimTime, 0)
    ),
    semiImplicit_(readBool(this->coeffs_.lookup("semiImplicit")))
{
    if (isA<singleStepReactingMixture<ThermoType>>(this->thermo()))
    {
        singleMixturePtr_ =
            &dynamic_cast<singleStepReactingMixture<ThermoType>&>
            (
                this->thermo()
            );
    }
    else
    {
        FatalErrorInFunction
            << "Inconsistent thermo package for " << this->type() << " model:\n"
            << "    " << this->thermo().type() << nl << nl
            << "Please select a thermo package based on "
            << "singleStepReactingMixture" << exit(FatalError);
    }

    if (semiImplicit_)
    {
        Info<< "Reaction mode: semi-implicit" << endl;
    }
    else
    {
        Info<< "Reaction mode: explicit" << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
singleStepReaction<ReactionThermo, ThermoType>::~singleStepReaction()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
tmp<fvScalarMatrix> singleStepReaction<ReactionThermo, ThermoType>::R
(
    volScalarField& Y
) const
{
    const label specieI =
        this->thermo().composition().species()[Y.member()];

    volScalarField wSpecie
    (
        wFuel_*singleMixturePtr_->specieStoichCoeffs()[specieI]
    );

    if (semiImplicit_)
    {
        const label fNorm = singleMixturePtr_->specieProd()[specieI];
        const volScalarField fres(singleMixturePtr_->fres(specieI));
        wSpecie /= max(fNorm*(Y - fres), scalar(1e-2));

        return -fNorm*wSpecie*fres + fNorm*fvm::Sp(wSpecie, Y);
    }
    else
    {
        return wSpecie + fvm::Sp(0.0*wSpecie, Y);
    }
}


template<class ReactionThermo, class ThermoType>
tmp<volScalarField>
singleStepReaction<ReactionThermo, ThermoType>::Qdot() const
{
    const label fuelI = singleMixturePtr_->fuelIndex();
    volScalarField& YFuel =
        const_cast<volScalarField&>(this->thermo().composition().Y(fuelI));

    return -singleMixturePtr_->qFuel()*(R(YFuel) & YFuel);
}


template<class ReactionThermo, class ThermoType>
bool singleStepReaction<ReactionThermo, ThermoType>::read()
{
    if (ThermoReaction<ReactionThermo>::read())
    {
        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace reactionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
