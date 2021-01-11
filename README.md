# urbanChemFoam

urbanChemFoam is a weakly compressible, reactive solver developed based on the open source finite volume computational continuum mechanics framework OpenFOAM (Weller et al., 1998).  Additional components have been introduced in conjunction to provide characteristics of a chemical transport model for an infinite street canyon.  These are:

**dateTime** :	Associating simulation time with physical time based on Julian Date

**solarUtils** :	Calculating solar state based on geographical information and physical time 

**irreversiblephotolysisReaction** :	Implementation of photolysis reactions for the OpenFOAM core thermophysics module

**simpleEmission** :	Enables emission boundary conditions to be prescribed in units of mass instead of mass mixing ratio

**cyclicFixedValue** :	Allows scalars to be prescribed as a Dirchlet condition on a cyclic boundary

**cyclicZeroGradient** :	Allows scalars to be prescribed as a Neumann condition on a cyclic boundary

**initCanyon** :	Utility to initialize velocity field in an infinite street canyon domain with perturbation

urbanChemFoam and all related components, described in this document or otherwise, are licensed under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or, at your option, any later version.  A copy of the GNU General Public License can be found along with the distribution of urbanChemFoam.  If not, please refer to the URL http://www.gnu.org/licenses/.

## Compilation Instructions for urbanChemFoam and related components

These modules can be built as user applications and libraries upon a existing functional installation of OpenFOAM version 7, maintained by CFD Direct Ltd..  Instructions to install OpenFOAM version 7, urbanChemFoam, and all related components are presented below.  It is assumed that the user will compile said source locally on a Linux system using GCC version 4.8 and above.  In addition to the GCC version requirement, a version of MPI library compiled with the same version GCC tools for OpenFOAM is also requisite.  In the case of this study, it is version 4.0.1 of OpenMPI (default MPI library for OpenFOAM) compiled with GCC 9.2.0:

1.	Download the source packages for OpenFOAM version 7 and accompanying third-party tools from the OpenFOAM Foundation website

    1.	The URL at the time of writing is https://openfoam.org/download/7-source/

    2.	Some urbanChemFoam components have been known to work only with OpenFOAM version 7.  The reaction class interface has been modified in this version and is incompatible with previous versions of OpenFOAM.  In addition, there are irreconcilable differences in basic data structures implementation between OpenFOAM from CFD Direct (the “.org” URL) and that from ESI (the “.com” URL) that urbanChemFoam will fail to compile in the ESI release
 
2.	Follow the instructions for installing the OpenFOAM 7 framework, first by installing the third-party tools, followed by the OpenFOAM core, with the following notes

    1.	Compilation of third-party library CGAL and its dependencies is not required

    2.	Compilation of third-party application Paraview and its dependencies is not necessary as Paraview 5.4.0 can also be installed independently, for instance, from a binary package

3.	Upon successful installation of OpenFOAM (this will take a while), inspect the paths reported by following environment variables to ensure they are correct

    1.	WM_PROJECT_INST (location of the OpenFOAM framework)
    2.	WM_PROJECT_DIR (location of the OpenFOAM core)
    3.	WM_PROJECT_USER (location of the OpenFOAM user-defined modules)
    4.	WM_THIRD_PARTY_DIR (location of the OpenFOAM third-party components)

4.	Source the script ${WM_PROJECT_DIR}/etc/bashrc before using any OpenFOAM solvers or components

5.	Unpack the tarball urbanChemFoam-1.0.tar.gz and component source codes into the location indicated by ${WM_PROJECT_USER}.  Create the directory if it does not exist.  Adjust any contents in the directory as necessary should the path in question be not empty

6.	Link or copy the following files or directory from directory ${WM_PROJECT_USER}/src/ to corresponding locations in directory ${WM_PROJECT_DIR}/src/

    1.	makePhotolysisReactions.C in thermophysicalModels/specie/reaction/reactions
    2.	photolysisReactionRate in thermophysicalModels/specie/reaction/reactionRate

7.	Link the files below in ${WM_PROJECT_DIR}/src/thermopyhsicalModels/specie/lnInclude

    1.	${WM_PROJECT_USER}/src/common/commonGlobals.H
    2.	${WM_PROJECT_DIR}/src/ … /photolysisReactionRate/photolysisReactionRate.H
    3.	${WM_PROJECT_DIR}/src/ … /photolysisReactionRate/photolysisReactionRateI.H

8.	Execute the script Allwmake in directory ${WM_PROJECT_DIR}/src/thermophysicalModels to update the OpenFOAM core thermophysics module with photolysis reactions

9.	Execute the script Allwmake in directory ${WM_PROJECT_USER} to build the remaining components and utilities for urbanChemFoam

10.	Verify that a platform directory has been created in ${WM_PROJECT_USER} after the build, and urbanChemFoam, initCanyon, as well as all other components are located in the sub-directories (e.g., platform/linux64GccDPInt32Opt/bin and platform/linux64GccDPInt32Opt/lib)

## Workflow for executing accompanied cases

The following accompanied cases for a core mesh size of 0.5 m (fine mesh) are provided in the tar archive `testCases-1.0.tar.gz`:

  1.	`stationary-57600` : Stationary model run for 16:00 UTC (57600s)

  2.	`transient` : Transient model run for a 24-hour period

Both cases begin with a six-hour (21600 s) spin-up period; they are contained in the spinup directory for the stationary case, and the `day00` directory in the transient case.  In the stationary case, the model rub for the sampling period is contained in the sample directory, while, in the transient case, this is in the `day01` directory.  The transient run has an additional `day02` directory, which performs an additional hour of simulation (from 00:00 to 01:00 UTC) to cover a full 24-hour period of central hourly average with a 3600 s window.

Grid generation takes place prior to the spin-up run, and the same grid is used throughout the remaining stages of each model run.  On the other hand, domain decomposition (almost certainly necessary given the size of the problem) is required for each stage of the run, as field data are also decomposed along with mesh data.  Therefore, field data obtained at the end of each stage are recombined from the parallel domains and served as initial conditions for the proceeding stage.  Each parallel run could be executed from the console or submitted to a queue manager.

Each directory represents a separate run following the same general workflow:

1.	Go to the spin-up directory (`spinup` or `day00`)

2.	Generate mesh by executing the OpenFOAM application blockMesh

3.	Initialize perturbed flow field with the application initCanyon

4.	Perform domain decomposition using the OpenFOAM utility decomposePar

5.	Execute model run with the command mpirun –np [NP] urbanChemFoam –parallel

    1.	The number of processors, NP, must match the number of decomposed domains specified in OpenFOAM dictionary file system/decomposeParDict 

6.	Prepare field data for the proceeding run phase by using the script prepNextDay.sh

    1.	A numerical directory will be created for the end-of-spin-up field data (21600 for the stationary run, and 86400 for the transient run).  Due to double precision storage of time the field data directory might not be written exactly as shown; simply rename the directory to remove the fractional part. 

7.	Switch to the sampling run directory (sample or day01)

8.	Check that the 0 directory (initial conditions) is linked correctly to the previous end-of-run field data (../sample/21600 or ../day00/86400)

    1.	Note step 6a, if the previous end-of-run field data is not exactly as shown

9.	Repeat Steps (4) and (5) for the sampling run.

The stationary model run should be complete at the conclusion of this step.  The following steps apply to the transient model run.

10.	Repeat Step (6) to generate end-of-run field data 

11.	Switch to trailing run directory day02

12.	Repeat Steps (8) and (9) to complete the trailing data

Field data for all output time steps in the sampling run can be reconstructed using the OpenFOAM utility `reconstructPar`, which must be converted into VTK format using the OpenFOAM utility `foamToVTK` in order to be accessed in Paraview.  Line sample data (for instance, for the vertical and spanwise horizontal stations defined in Section 4) can be found stored in text format in the `postProcessing` directory.

