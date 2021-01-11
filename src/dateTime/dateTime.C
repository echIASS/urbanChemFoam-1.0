///////////////////////////////////////////////////////////////
//
//  dateTime.C
//  ----------
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
//   Definition of dateTime class properties and members
//
///////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <sstream>
#include <tuple>

#include "dateTime.H"

				////////////////////////////////////////////////

dateTime::dateTime ( )  {
	setDate ( 1900, 1, 1, 0, 0, 0.0 );
}

				////////////////////////////////////////////////

dateTime::dateTime ( Foam::string dateStr )  {
	setDateString ( dateStr );
}

				////////////////////////////////////////////////

dateTime::dateTime ( label YY, label MM, label DD,
					 label hh, label mm, scalar ss )  {
	setDate ( YY, MM, DD, hh, mm, ss );
}

				////////////////////////////////////////////////

dateTime::dateTime ( scalar jd, scalar jt )  {
	setJD ( jd, jt );
}

dateTime::~dateTime ( )  { }

				////////////////////////////////////////////////

void dateTime::setDateString ( Foam::string dateStr )  {
	dateString = dateStr;
	dateString2Date ( );
	date2JD ( );
}

				////////////////////////////////////////////////

void dateTime::setDate ( label YY, label MM, label DD,
						 label hh, label mm, scalar ss )  {
	year = YY;  month = MM;  day = DD;
	hour = hh;  minute = mm; second = ss;
	date2DateString ( );
	date2JD ( );
}

				////////////////////////////////////////////////

void dateTime::setJD ( scalar jd, scalar jt )  {
	jday  = jd;
	jtime = jt;
	JD2Date ( );
	date2DateString ( );
}

				////////////////////////////////////////////////

void dateTime::dateString2Date ( )  {

	size_t i0=0, i1;
	Foam::string token = dateString;

	i1 = token.find (DATE_SEP);
	year = std::stoi ( token.substr ( i0, i1 ) );
	token = token.substr ( i1+1, string::npos );

	i1 = token.find (DATE_SEP);
	month = std::stoi ( token.substr ( i0, i1 ) );
	token = token.substr ( i1+1, string::npos );

	i1 = token.find (DATE_SEP);
	day = std::stoi ( token.substr ( i0, i1 ) );
	token = token.substr ( i1+1, string::npos );

	i1 = token.find (PART_SEP);
	token = token.substr ( i1+1, string::npos );

	i1 = token.find (TIME_SEP);
	hour = std::stoi ( token.substr ( i0, i1 ) );
	token = token.substr ( i1+1, string::npos );

	i1 = token.find (TIME_SEP);
	minute = std::stoi ( token.substr ( i0, i1 ) );
	token = token.substr ( i1+1, string::npos );

	i1 = token.find (TIME_SEP);
	second = static_cast<scalar>( std::stod ( token.substr ( i0, i1 ) )) ;
	token = token.substr ( i1+1, Foam::string::npos );

}

				////////////////////////////////////////////////

void dateTime::date2DateString ( )  {

	std::ostringstream dateStrStream;

	dateStrStream <<
		std::fixed<<std::setw(4)<<std::setfill('0') << year   << DATE_SEP <<
		std::fixed<<std::setw(2)<<std::setfill('0') << month  << DATE_SEP <<
		std::fixed<<std::setw(2)<<std::setfill('0') << day    << PART_SEP <<
		std::fixed<<std::setw(2)<<std::setfill('0') << hour   << TIME_SEP <<
		std::fixed<<std::setw(2)<<std::setfill('0') << minute << TIME_SEP <<
		std::fixed<<std::setw(9)<<std::setprecision(6)<< std::setfill('0') << second;

	dateString = dateStrStream.str();

}

				////////////////////////////////////////////////

void dateTime::date2JD ( )  {

	label Y4800 =  year + 4800;
	label M2    =  month - 2;
	label M1412 = (month - 14) / 12;
	label D32K  =  day - 32075;

	label A = 1461*(Y4800+M1412)/4;
	label B = 367*(M2-(12*M1412))/12;
	label C = 3*(((Y4800+100)+M1412)/100)/4;
	label D = D32K;

	scalar HH = static_cast<scalar>(hour) / D2H; 
	scalar MM = static_cast<scalar>(minute) / D2M;
	scalar SS = static_cast<scalar>(second) / D2S;

	jday  = static_cast<scalar>(A+B-C+D) - 0.5;
	jtime = HH + MM + SS;

}

				////////////////////////////////////////////////

void dateTime::JD2Date ( )  {

	scalar jtimeRound = jtime + eps;
	scalar jdayRound  = jday;

			// make sure seconds get carried over correctly

	if ( jtimeRound > 1.0 )  {
		jdayRound += 1.0;
		jtimeRound -= 1.0;
	}
 
	scalar jtimeInHours = jtimeRound * D2H;
	hour = static_cast<label>(std::floor(jtimeInHours));

	scalar jtimeInMinutes = ( jtimeInHours -
			static_cast<scalar>(hour) ) * H2M;

	minute = static_cast<label>(std::floor(jtimeInMinutes));

	second = ( jtimeInMinutes -
			static_cast<scalar>(minute) ) * M2S;

	label Z = static_cast<label>( jdayRound + 0.5         );
	label W = static_cast<label>( (Z-1867216.25)/36524.25 );
	label X = static_cast<label>( W/4.0                   );
	label A = static_cast<label>( Z + 1.0 + W - X         );
	label B = static_cast<label>( A + 1524.0              );
	label C = static_cast<label>( (B-122.1)/365.25        );
	label D = static_cast<label>( 365.25*C                );
	label E = static_cast<label>( (B-D)/30.6001           );
	label F = static_cast<label>( 30.6001*E               );

	day   = B - D - F;
	month = E - 1 - (E>13)*12;
	year  = C - 4715 - (month > 2);

}

				////////////////////////////////////////////////

void dateTime::incrementSeconds ( scalar sec )  {

	scalar dtime   =  sec/D2S;				// convert to JD
	scalar dday    =  std::floor(dtime);	// get whole days

	jtime += dtime - dday;					// increment JD
	jday  += dday;

	if ( jtime > 1.0 )  {					// carry excess jtime
		jtime -= 1.0;
		jday  += 1.0;
	}

				// propagate updated time

	JD2Date ( );
	date2DateString ( );

}

				////////////////////////////////////////////////

Foam::string dateTime::getDateString ( )  { return dateString; }

				////////////////////////////////////////////////

std::tuple<label, label, label, label, label, scalar> dateTime::getDate ( )  {
	return std::make_tuple ( year, month, day, hour, minute, second );
}

				////////////////////////////////////////////////

std::tuple<scalar, scalar> dateTime::getJD ( )  {
	return std::make_tuple ( jday, jtime );
}

				////////////////////////////////////////////////

scalar dateTime::getJDT ( )  { return ( jday + jtime ); }

				////////////////////////////////////////////////


label dateTime::getWeekDay ( )  { return ( static_cast<label>(jday) % 7 ); }

				////////////////////////////////////////////////

bool dateTime::operator >  ( dateTime &x )  { return ( getJDT() > x.getJDT() ); }
bool dateTime::operator <  ( dateTime &x )  { return ( getJDT() < x.getJDT() ); }
bool dateTime::operator >= ( dateTime &x )  { return ( !( *this < x ) ); }
bool dateTime::operator <= ( dateTime &x )  { return ( !( *this > x ) ); }
bool dateTime::operator == ( dateTime &x )  { return ( !((*this>x)||(*this<x)) ); }

				////////////////////////////////////////////////

dateTime &dateTime::operator = ( dateTime &x )  {
	setDateString ( x.getDateString ( ) );
	return ( *this );
}

				////////////////////////////////////////////////
