/*
 * build_time.h
 *
 *  Created on: 15-03-2013
 *      Author: Kangur
 *
 *	Description: Functions to generate compilation-time related info
 */

#ifndef _BUILD_TIME_H_
#define _BUILD_TIME_H_




#define BUILD_YEAR ((__DATE__[7] - '0') * 1000 +  (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + __DATE__[10] - '0')

#define BUILD_DATE ((__DATE__[4] - '0') * 10 + __DATE__[5] - '0')

int getCompileHour();
int getCompileMinutes();
int getCompileSeconds();
int getCompileYear();
int getCompileDay();



extern const char compile_time[];
extern const char compile_date[];


#endif /* _BUILD_TIME_H_ */
