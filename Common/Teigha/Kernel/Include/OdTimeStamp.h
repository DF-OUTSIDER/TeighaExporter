/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////




#ifndef _ODTIME_INCLUDED_
#define _ODTIME_INCLUDED_

#include "TD_PackPush.h"
class OdString;

/** \details
    This class represents TimeStamp objects in an OdDbDatabase instance.

    \sa
    TD_Db

    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdTimeStamp
{
public:
  enum InitialValue
  {
    kInitZero          = 1,  // Midnight, January 1, 1970, UTC.
    kInitLocalTime     = 2,  // Workstation date in local time.
    kInitUniversalTime = 3   // Workstation date in Universal (Greenwich Mean) Time.
  };

  /** \param init [in]  Initial value.
    
    \remarks
    The default constructor initializes the Julian date and time to midnight, January 1, 1970,  UTC.
    
    init must be one of the following:
    
    <table>
    Name                 Value    Description
    kInitZero            1        Midnight, January 1, 1970, UTC.
    kInitLocalTime       2        Workstation date in local time.
    kInitUniversalTime   3        Workstation date in Universal (Greenwich Mean) Time.
    </table>
  */
   OdTimeStamp();
  OdTimeStamp(
    InitialValue init);

	/** \details
	  Returns the date of this TimeStamp object.
	  \param month [out]  Receives the month.
	  \param day [out]  Receives the day.
	  \param year [out]  Receives the year.
	*/
  void getDate(
    short& month, 
    short& day, 
    short& year) const;

	/** \details
	  Sets the date of this TimeStamp object.
	  \param month [in]  Month.
	  \param day [in]  Day.
	  \param year [in]  Year.
	*/
  void setDate(
    short month, 
    short day, 
    short year);

	/** \details
	  Returns the month of this TimeStamp object.
	*/
  short month() const;

	/** \details
	  Sets the month of this TimeStamp object.
	  \param month [in]  Month.
	*/
  void setMonth(
    short month);

	/** \details
	  Returns the day of this TimeStamp object.
	*/
  short day() const;

	/** \details
	  Sets the day of this TimeStamp object.
	  \param day [in]  Day.
	*/
  void setDay(
    short day);

	/** \details
	  Returns the year of this TimeStamp object.
	*/
  short year() const;

	/** \details
	  Sets the year of this TimeStamp object.
	  \param year [in]  Year.
	*/
  void setYear(
    short year);

	/** \details
	  Returns the time of this TimeStamp object.
	  \param hour [out]  Receives the hour.
	  \param minute [out]  Receives the minute.
	  \param second [out]  Receives the second.
	  \param millisecond [out]  Receives the millisecond.
	*/
  void getTime(
    short& hour, 
    short& minute, 
    short& second, 
    short& millisecond) const;

	/** \details
	  Sets the time of this TimeStamp object.
	  \param hour [in]  Hour.
	  \param minute [in]  Minute.
	  \param second [in]  Second.
	  \param millisecond [in]  Millisecond.
	*/
  void setTime(
    short hour, 
    short minute, 
    short second, 
    short millisecond);

	/** \details
	  Returns the hour of this TimeStamp object.
	*/
  short hour() const;

	/** \details
	  Sets the hour of this TimeStamp object.
	  \param hour [in]  Hour.
	*/
  void setHour(
    short hour);

	/** \details
	  Returns the minute of this TimeStamp object.
	*/
  short minute() const;

	/** \details
	  Sets the minute of this TimeStamp object.
	  \param minute [in]  Minute.
	*/
  void setMinute(
    short minute);

	/** \details
	  Returns the second of this TimeStamp object.
	*/
  short second() const;

	/** \details
	  Sets the second of this TimeStamp object.
	  \param second [in]  Second.
	*/
  void setSecond(short second);

	/** \details
	  Returns the millisecond of this TimeStamp object.
	*/
  short millisecond() const;

	/** \details
	  Sets the millisecond of this TimeStamp object.
	  \param millisecond [in]  Millisecond.
	*/
  void setMillisecond(
    short millisecond);

	/** \details
	  Sets this TimeStamp object to Midnight, January 1, 1970, UTC.
	*/
  void setToZero();

  /** \details
	  Sets this TimeStamp object to the workstation date in local time.
  */
  void getLocalTime();

  /** \details
	  Sets this TimeStamp object to the workstation date in Universal (Greenwich Mean) Time.
  */
  void getUniversalTime();

	/** \details
	  Converts this TimeStamp object from local time to Universal (Greenwich Mean) Time.
	*/
  void localToUniversal();

	/** \details
	  Converts this TimeStamp object from Universal (Greenwich Mean) Time to local time.
	*/
  void universalToLocal();

	/** \details
	  Returns the Julian day of this TimeStamp object.
	*/
  OdUInt32 julianDay() const;

	/** \details
	  Sets the Julian day of this TimeStamp object.
	  
	  \param julianDay [in]  Julian day.
	*/
  void setJulianDay(
    OdUInt32 julianDay);

	/** \details
	  Returns the milliseconds past midnight for this TimeStamp object.
	*/
  OdUInt32 msecsPastMidnight() const;

	/** \details
	  Sets the milliseconds past midnight for this TimeStamp object.
	  
	  \param msecsPastMidnight [in]  Milliseconds past midnight.
	*/
  void setMsecsPastMidnight(
    OdUInt32 msecsPastMidnight);

	/** \details
	  Sets the Julian Date for this TimeStamp object.

	  \param julianDay [in]  Julian day.
	  \param msecsPastMidnight [in]  Milliseconds past midnight.
	*/
  void setJulianDate(
    OdUInt32 julianDay, 
    OdUInt32 msecsPastMidnight);

	/** \details
	  Returns the fraction of the Julian date of this TimeStamp object.

	  \remarks
	  The Julian fraction is the fraction of the day since midnight.
	*/
  double julianFraction() const;

	/** \details
	  Sets the fraction of the Julian date of this TimeStamp object.
	  \param julianFraction [in]  Julian fraction.
	  \remarks
	  The Julian fraction is the fraction of the day since midnight.
	*/
  void setJulianFraction(
    double julianFraction);

  bool operator==(
    const OdTimeStamp& tStamp) const;

  bool operator!=(
    const OdTimeStamp& tStamp) const
  {
    return !(operator==(tStamp));
  }

  bool operator>(
    const OdTimeStamp& tStamp) const;

  bool operator>=(
    const OdTimeStamp& tStamp) const
  {
    return ! operator<(tStamp);
  }

  bool operator<(
    const OdTimeStamp& tStamp) const;

  bool operator<=(
    const OdTimeStamp& tStamp) const
  {
    return ! operator>(tStamp);
  }

  const OdTimeStamp operator+(
    const OdTimeStamp &tStamp) const
  {
    return OdTimeStamp(*this) += tStamp;
  }

  const OdTimeStamp operator-(
    const OdTimeStamp &tStamp) const
  {
    return OdTimeStamp(*this) -= tStamp;
  }

  const OdTimeStamp& operator+=(
    const OdTimeStamp &tStamp);

  const OdTimeStamp& operator-=(
    const OdTimeStamp &tStamp);

  /** \details
      Adds the specified TimeStamp object to this TimeStamp object, and returns the sum.
      
      \param tStamp [in]  TimeStamp.
  */
  const OdTimeStamp& add(
    const OdTimeStamp &tStamp)
  {
    return operator+=(tStamp);
  }

  /** \details
      Subtracts the specified TimeStamp object from this TimeStamp object, and returns the difference.
      
      \param tStamp [in]  TimeStamp.
  */
  const OdTimeStamp& subtract(
    const OdTimeStamp &tStamp)
  {
    return operator-=(tStamp);
  }

  /** \details
    Returns this TimeStamp object as standard formatted string.
    
    \param timeString [out]  Receives the formatted time string.
    
    \remarks
    The returned string is always 24 characters in length, and is in the form
    
                  Tue Oct 11 08:06:22 2005
  */
  void ctime(
    class OdString& timeString) const;

  /** \details
    Returns this TimeStamp object as a user-formatted string.
    
    \param timeString [out]  Receives the formatted time string.
    \param format [in]  Format string.
    
    \remarks
    The formatting codes for the format string are as follows; the # suppresses leading zeros:

    <table>
    Code          Description
    %a            Short weekday 
    %A            Full weekday 
    %b            Short month 
    %B            Full month 
    %c            Short date and time for current locale
    %#c           Long date and time for current locale 
    %d            Day of month (01 - 31) 
    %#d           Day of month (1 - 31) 
    %H            Hour in 24 hour format (00 - 23) 
    %#H           Hour in 24 hour format (0 - 23) 
    %I            Hour in 12 hour format (01 - 12) 
    %#I           Hour in 12 hour format (1 - 12) 
    %j            Day of year (001 - 366) 
    %#j           Day of year (1 - 366) 
    %m            Month (01 - 12) 
    %#m           Month (1 - 12) 
    %M            Minute (00 - 59) 
    %#M           Minute (0 - 59) 
    %p            AM/PM indicator for current local
    %S            Second (00 - 59) 
    %#S           Second (0 - 59) 
    %U            Week of the year, first day of week is Sunday (00 - 53) 
    %#U           Week of the year, first day of week is Sunday (0 - 53) 
    %w            Weekday, Sunday is 0, (0 - 6) 
    %#w           Weekday, Sunday is 0, (0 - 6) 
    %W            Week of the year, first day of week is Monday (00 - 53) 
    %#W           Week of the year, first day of week is Monday (0 - 53) 
    %x            Date for the current locale 
    %X            Time for the current locale 
    %y            Year without century (00 - 99) 
    %#y           Year without century (0 - 99) 
    %Y            Year with century  
    %Y            Year with century, no leading zeros 
    %z            Time-zone name  
    %Z            Time zone abbreviation
    </table>
  */
  void strftime(
    const OdString& format, 
    OdString &timeString) const;

  /** \details
    Returns this TimeStamp object as long.
    
    \remarks
    packedValue returns the number of seconds elapsed since midnight, January 1, 1970, Universal (Greenwich Mean) Time.
    
    If this TimeStamp object contains a date before midnight, January 1, 1970,  UTC, 
    or after 3:14:07 on January 19, 2038, UT, packedValue() returns -1.
  */
  OdInt64 packedValue() const;
  void setPackedValue(OdInt64 t);
  short weekday() const;
private:
  OdUInt32 m_julianDay;
  OdUInt32 m_msec;
};
#include "TD_PackPop.h"

#endif // _ODTIME_INCLUDED_

