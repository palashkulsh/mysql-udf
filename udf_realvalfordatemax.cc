/*
  returns the realvalfordatemax  of the regression function a data set
  realvalfordatemax(year,month,day,str)
  input parameters:
  dependent data (real)
  independent data (real)

  output:
  realvalfordatemax  of the regression function (real)

  registering the function:
  CREATE AGGREGATE FUNCTION realvalfordatemax RETURNS REAL SONAME 'udf_realvalfordatemax.so';

  getting rid of the function:
  DROP FUNCTION realvalfordatemax;

*/



#ifdef STANDARD
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;	
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#endif
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>		

#ifdef HAVE_DLOPEN


extern "C" {

my_bool realvalfordatemax_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
void realvalfordatemax_deinit( UDF_INIT* initid );
void realvalfordatemax_clear(UDF_INIT *initid, char *is_null, char *is_error);
void realvalfordatemax_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
void realvalfordatemax_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
double realvalfordatemax( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );

}

struct date{
  int year;
  int month;
  int day;
};

struct max_data
{
  struct date maxdate;
  double colval;
};

/**
   
*/
my_bool realvalfordatemax_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 4)
  {
    strcpy(message,"wrong number of arguments: realvalfordatemax() requires two arguments");
    return 1;
  }
  if (args->arg_type[0]!=INT_RESULT || args->arg_type[1]!=INT_RESULT || args->arg_type[2]!=INT_RESULT || args->arg_type[3]!=REAL_RESULT)
  {
    strcpy(message,"correlation() requires a string as parameter 2");
    return 1;
  }

  initid->decimals=2;  
  max_data   *buffer = new max_data;
  buffer->maxdate.year = 0;
  buffer->maxdate.month = 0;
  buffer->maxdate.day = 0;

  buffer->colval=0;
  initid->ptr = (char*)buffer;
  return 0;
}


void realvalfordatemax_deinit( UDF_INIT* initid )
{
  max_data *buffer = (max_data*)initid->ptr;
  delete initid->ptr;
}

void realvalfordatemax_clear(UDF_INIT *initid, char *is_null, char *is_error)
{  
  max_data *buffer = (max_data*)initid->ptr;
  *is_null = 0;
  *is_error = 0;
  buffer->maxdate.year=0;
  buffer->maxdate.month=0;  
  buffer->maxdate.day=0;
  buffer->colval=0;
}

void realvalfordatemax_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  realvalfordatemax_clear(initid, is_null, is_error);
  realvalfordatemax_add( initid, args, is_null, is_error );
}


void realvalfordatemax_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  max_data *buffer = (max_data*)initid->ptr;
  if (args->args[0]!=NULL && args->args[1]!=NULL && args->args[2]!=NULL && args->args[3]!=NULL)
  {
    if(buffer->maxdate.year==0){
      buffer->maxdate.year = *(int*)args->args[0];
      buffer->maxdate.month = *(int*)args->args[1];
      buffer->maxdate.day= *(int*)args->args[2];
      buffer->colval = *(double *)args->args[3];
    }else{
      if((*(int*)args->args[0])>(buffer->maxdate.year) || ((*(int*)args->args[0])==(buffer->maxdate.year) && (*(int*)args->args[1])>(buffer->maxdate.month)) || ((*(int*)args->args[0])==(buffer->maxdate.year) && (*(int*)args->args[1])==(buffer->maxdate.month ) && (*(int*)args->args[2])>(buffer->maxdate.day))){
	buffer->maxdate.year = *(int *)args->args[0];
	buffer->maxdate.month = *(int *)args->args[1];
	buffer->maxdate.day = *(int *)args->args[2];
	buffer->colval = *(double *)args->args[3];
      }
    }
  }
}



double realvalfordatemax( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  max_data* buffer = (max_data*)initid->ptr;
  return buffer->colval;
}



#endif


