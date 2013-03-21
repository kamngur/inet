/*
 * build_time.h
 *
 *  Created on: 15-03-2013
 *      Author: Kangur
 */

#ifndef _BUILD_TIME_H_
#define _BUILD_TIME_H_

char compile_time[]= __TIME__;
char compile_date[]= __DATE__;

int hour=0;
int minute=0;
int second =0;



//void init_data()
//{
//
//	int hour= atoi(compile_time);
//	int minute=atoi(compile_time+3);
//	int second=atoi (compile_time+6);
//}

int getCompileHour()
{
	return atoi(compile_time);
}

int getCompileMinutes()
{
	return atoi(compile_time+3);
}

int getCompileSeconds()
{
	return atoi(compile_time+6);
}

int getCompileYear()
{
	return atoi(compile_date+7);
}
int getCompileDay()
{
	return atoi(compile_date+4);
}
#endif /* _BUILD_TIME_H_ */
