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

Class
    Foam::ReactionModel

Description
    Reaction models for templated thermodynamics

SourceFiles
    ReactionModelI.H
    ReactionModel.C
    ReactionModelNew.C

\*---------------------------------------------------------------------------*/

#ifndef ReactionModel_H
#define ReactionModel_H

#include "reactionModel.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     class ReactionModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class ReactionModel
:
    public reactionModel
{
public:

    //- Thermo type
    typedef ReactionThermo reactionThermo;


    //- Runtime type information
    TypeName("ReactionModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        ReactionModel,
        dictionary,
        (
            const word& modelType,
            ReactionThermo& thermo,
            const compressibleTurbulenceModel& turb,
            const word& reactionProperties
        ),
        (modelType, thermo, turb, reactionProperties)
    );


    // Constructors

        //- Construct from components
        ReactionModel
        (
            const word& modelType,
            ReactionThermo& thermo,
            const compressibleTurbulenceModel& turb,
            const word& reactionProperties
        );


        //- Selector
        static autoPtr<ReactionModel> New
        (
            ReactionThermo& thermo,
            const compressibleTurbulenceModel& turb,
            const word& reactionProperties=reactionPropertiesName
        );


    //- Destructor
    virtual ~ReactionModel();


    // Member Functions

        //- Return access to the thermo package
        virtual ReactionThermo& thermo() = 0;

        //- Return const access to the thermo package
        virtual const ReactionThermo& thermo() const = 0;


    // IO

        //- Update properties from given dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "ReactionModel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
