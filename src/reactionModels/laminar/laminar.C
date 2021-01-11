/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2018 OpenFOAM Foundation
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

#include "laminar.H"
#include "fvmSup.H"
#include "localEulerDdtScheme.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
Foam::reactionModels::laminar<ReactionThermo>::laminar
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressibleTurbulenceModel& turb,
    const word& reactionProperties
)
:
    ChemistryReaction<ReactionThermo>
    (
        modelType,
        thermo,
        turb,
        reactionProperties
    ),
    integrateReactionRate_
    (
        this->coeffs().lookupOrDefault("integrateReactionRate", true)
    )
{
    if (integrateReactionRate_)
    {
        Info<< "    using integrated reaction rate" << endl;
    }
    else
    {
        Info<< "    using instantaneous reaction rate" << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo>
Foam::reactionModels::laminar<ReactionThermo>::~laminar()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
Foam::tmp<Foam::volScalarField>
Foam::reactionModels::laminar<ReactionThermo>::tc() const
{
    return this->chemistryPtr_->tc();
}


template<class ReactionThermo>
void Foam::reactionModels::laminar<ReactionThermo>::correct()
{
    if (integrateReactionRate_)
    {
        if (fv::localEulerDdt::enabled(this->mesh()))
        {
            const scalarField& rDeltaT =
                fv::localEulerDdt::localRDeltaT(this->mesh());

            if (this->coeffs().found("maxIntegrationTime"))
            {
                const scalar maxIntegrationTime
                (
                    readScalar(this->coeffs().lookup("maxIntegrationTime"))
                );

                this->chemistryPtr_->solve
                (
                    min(1.0/rDeltaT, maxIntegrationTime)()
                );
            }
            else
            {
                this->chemistryPtr_->solve((1.0/rDeltaT)());
            }
        }
        else
        {
            this->chemistryPtr_->solve(this->mesh().time().deltaTValue());
        }
    }
}


template<class ReactionThermo>
Foam::tmp<Foam::fvScalarMatrix>
Foam::reactionModels::laminar<ReactionThermo>::R(volScalarField& Y) const
{
    tmp<fvScalarMatrix> tSu(new fvScalarMatrix(Y, dimMass/dimTime));
    fvScalarMatrix& Su = tSu.ref();

    const label specieI = this->thermo().composition().species()[Y.member()];
    Su += this->chemistryPtr_->RR(specieI);

    return tSu;
}


template<class ReactionThermo>
Foam::tmp<Foam::volScalarField>
Foam::reactionModels::laminar<ReactionThermo>::Qdot() const
{
    return this->chemistryPtr_->Qdot();
}


template<class ReactionThermo>
bool Foam::reactionModels::laminar<ReactionThermo>::read()
{
    if (ChemistryReaction<ReactionThermo>::read())
    {
        integrateReactionRate_ =
            this->coeffs().lookupOrDefault("integrateReactionRate", true);
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
