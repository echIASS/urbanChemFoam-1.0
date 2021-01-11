///////////////////////////////////////////////////////////////
//
//   urbanChemFoam.C
//   ---------------
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
//	 Solver based on rhoReactingFoam with adaptations for urban
//	 emission dispersion and chemical transformation.
//
////////////////////////////////////////////////////////////////

#include "fvCFD.H"
#include "uniformDimensionedFields.H"
#include "turbulentFluidThermoModel.H"
#include "rhoReactionThermo.H"
#include "ReactionModel.H"
#include "multivariateScheme.H"
#include "pimpleControl.H"
#include "pressureControl.H"
#include "fvOptions.H"
#include "localEulerDdtScheme.H"
#include "fvcSmooth.H"

#include "commonGlobals.H"		// globals used by all modules
#include "commonConstants.H"
#include "commonPatchEmissions.H"
#include "globals.H"			// globals used only in urbanChemFoam
#include "dateTime.H"
#include "solarUtils.H"

				////////////////////////////////////////////////
				////////////////////////////////////////////////

//Foam::DynamicList<Foam::word> patchEmissionSpecies(0);

int main ( int argc, char *argv[] )  {

	#include "postProcess.H"

	#include "setRootCaseLists.H"
	#include "createTime.H"
	#include "createMesh.H"
	#include "createControl.H"
	#include "createTimeControls.H"
	#include "initContinuityErrs.H"
	#include "createFields.H"
	#include "createFieldRefs.H"

	#include "urbanChemFoamDictOptions.H"
	#include "planetaryPropertiesOptions.H"
	#include "derivedOptions.H"
	
	turbulence->validate();

	if (!LTS)  {
		#include "compressibleCourantNo.H"
		#include "setInitialDeltaT.H"
	}

				// set up dateTime to curent run time

	dateTime runDateTime ( referenceTimestamp );
	runDateTime.incrementSeconds ( runTime.value() );

				// if solar state is fixed set them here
				// and leave it alone

	if ( fixedSolarState )  {
		zenithAngle    = fixedZenithAngle;
		cosZenithAngle = Foam::cos ( D2R*zenithAngle );
	}

				// start solving

	Info<< "\nStarting time loop\n" << endl;

	while (runTime.run())  {

		#include "readTimeControls.H"

		if (LTS)  {
			#include "setRDeltaT.H"
		}
		else  {
			#include "compressibleCourantNo.H"
			#include "setDeltaT.H"
		}

				// update runtime and timestamp

		runTime++;
		runDateTime.incrementSeconds ( runTime.deltaTValue() );

				// update dynamic solar state (if not fixed)

		if ( !fixedSolarState )  {
			zenithAngle    = solarGetZenithAngle ( runDateTime.getJDT(),
											originLatitude, originLongitude );
			cosZenithAngle = Foam::cos ( D2R*zenithAngle );
		}

				// print out time

		Info << "Timestamp    : " << runDateTime.getDateString() << "   "
			 << runTime.timeName() << " from start" << endl;

		if ( !fixedSolarState )  {
			Info << "Zenith angle [deg] : " << zenithAngle << endl;
		}

		firstSolverIteration = true;
		externalPatchCallOverride = false;

		#include "rhoEqn.H"

		volVectorField rhoG("rhoG", rho*g );

		while (pimple.loop())  {


			#include "UEqn.H"
			#include "YEqn.H"
			#include "EEqn.H"

						// --- Pressure corrector loop

			while ( pimple.correct() )  {

				if ( pimple.consistent() )  {
					#include "pcEqn.H"
				}
				else  {
					#include "pEqn.H"
				}

			}

			if ( firstSolverIteration )  firstSolverIteration = false;
			if ( pimple.turbCorr() )  { turbulence->correct(); }

		}

		rho = thermo.rho();

				// calculate diagnostic variables

		#include "thetaEqn.H"

		runTime.write();

		Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
			 << "  ClockTime = " << runTime.elapsedClockTime() << " s"
			 << nl << endl;
	}

	commonPatchEmissions::cleanup ( );

	Info<< "End\n" << endl;

	return 0;
}

				////////////////////////////////////////////////
