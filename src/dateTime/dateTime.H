///////////////////////////////////////////////////////////////
//
//  dateTime.H
//  ----------
//
//   ECC / IASS
//
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
//   Declaration of the dateTime class
//
//	 The dateTime class links the simulation time (seconds
//	 from a reference time) to physical time by the attaching
//	 the reference time (t = 0) to a physical time.
//	 The physical time can be represented in YMDHMS
//	 timestamp (in WRF format), or as Julian Date (JD),
//   which simplifies day-of-week and leap year calculation
//	 considerably.  The time is updated at every time step
//	 in JD and is updated to the other formats.
//
///////////////////////////////////////////////////////////////

#ifndef __WRFDATE_H__
#define __WRFDATE_H__

#include "fvCFD.H"

class dateTime  {

	private :

		const char DATE_SEP = '-';
		const char TIME_SEP = ':';
		const char PART_SEP = '_';
		const char DECIMAL  = '.';

		const scalar D2H =     24.0;
		const scalar D2M =   1440.0;
		const scalar D2S =  86400.0;
		const scalar H2M =     60.0;
		const scalar M2S =     60.0;

		const scalar eps = 5.0e-13;		// about 50 ns

		Foam::string dateString;
		label year, month, day, hour, minute;
		scalar second;
		scalar jday, jtime;

	protected :

		void dateString2Date ( );
		void date2DateString ( );
		void date2JD ( );
		void JD2Date ( );

	public :

		dateTime  ( );
		dateTime  ( Foam::string dateStr );
		dateTime  ( label YY, label MM, label DD,
				    label hh, label mm, scalar ss );
		dateTime  ( scalar jd, scalar jt );
		~dateTime ( );

		void setDateString  (  Foam::string dateStr );
		void setDate        ( label YY, label MM, label DD,
							  label hh, label mm, scalar ss );
		void setJD          ( scalar jd, scalar jt );

		void incrementSeconds  ( scalar sec );

		Foam::string getDateString ( );
		std::tuple<label, label, label,
				   label, label, scalar> getDate ( );
		std::tuple<scalar, scalar> getJD ( );
		scalar getJDT   ( );
		scalar getJDay  ( );
		scalar getJTime ( );

		label getWeekDay ( );

		bool operator >  ( dateTime &x );
		bool operator <  ( dateTime &x );
		bool operator == ( dateTime &x );
		bool operator >= ( dateTime &x );
		bool operator <= ( dateTime &x );

		dateTime & operator = ( dateTime &x );

};

#endif

				////////////////////////////////////////////////
