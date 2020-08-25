/*
  returns the realvalfordatemin  of the regression function a data set
  realvalfordatemin(year,month,day,str)
  input parameters:
  dependent data (real)
  independent data (real)

  output:
  realvalfordatemin  of the regression function (real)

  registering the function:
  CREATE AGGREGATE FUNCTION realvalfordatemin RETURNS REAL SONAME 'udf_realvalfordatemin.so';

  getting rid of the function:
  DROP FUNCTION realvalfordatemin;

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

  my_bool realvalfordatemin_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  void realvalfordatemin_deinit( UDF_INIT* initid );
  void realvalfordatemin_clear(UDF_INIT *initid, char *is_null, char *is_error);
  void realvalfordatemin_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  void realvalfordatemin_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  double realvalfordatemin( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );

}

struct date{
  int year;
  int month;
  int day;
};

struct min_data
{
  struct date mindate;
  double colval;
};

/**
   
 */
my_bool realvalfordatemin_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 4)
    {
      strcpy(message,"wrong number of arguments: realvalfordatemin() requires two arguments");
      return 1;
    }
  if (args->arg_type[0]!=INT_RESULT || args->arg_type[1]!=INT_RESULT || args->arg_type[2]!=INT_RESULT || args->arg_type[3]!=REAL_RESULT)
    {
      strcpy(message,"correlation() requires a string as parameter 2");
      return 1;
    }

  initid->decimals=2;  
  min_data   *buffer = new min_data;
  buffer->mindate.year = 0;
  buffer->mindate.month = 0;
  buffer->mindate.day = 0;

  buffer->colval=0;
  initid->ptr = (char*)buffer;
  return 0;
}


void realvalfordatemin_deinit( UDF_INIT* initid )
{
  min_data *buffer = (min_data*)initid->ptr;
  delete initid->ptr;
}

void realvalfordatemin_clear(UDF_INIT *initid, char *is_null, char *is_error)
{  
  min_data *buffer = (min_data*)initid->ptr;
  *is_null = 0;
  *is_error = 0;
  buffer->mindate.year=0;
  buffer->mindate.month=0;  
  buffer->mindate.day=0;
  buffer->colval=0;
}

void realvalfordatemin_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  realvalfordatemin_clear(initid, is_null, is_error);
  realvalfordatemin_add( initid, args, is_null, is_error );
}


void realvalfordatemin_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  min_data *buffer = (min_data*)initid->ptr;
  if (args->args[0]!=NULL && args->args[1]!=NULL && args->args[2]!=NULL && args->args[3]!=NULL)
    {
      if(buffer->mindate.year==0){
        buffer->mindate.year = *(int*)args->args[0];
        buffer->mindate.month = *(int*)args->args[1];
        buffer->mindate.day= *(int*)args->args[2];
        buffer->colval = *(double *)args->args[3];
      }else{
        /**
           either year is smaller
           either year are equal and month is smaller
           either year month are equal and day is smaller
        */
        if((*(int*)args->args[0])<(buffer->mindate.year) || ((*(int*)args->args[0])==(buffer->mindate.year) && (*(int*)args->args[1])<(buffer->mindate.month)) || ((*(int*)args->args[0])==(buffer->mindate.year) && (*(int*)args->args[1])==(buffer->mindate.month ) && (*(int*)args->args[2])<(buffer->mindate.day))){
          buffer->mindate.year = *(int *)args->args[0];
          buffer->mindate.month = *(int *)args->args[1];
          buffer->mindate.day = *(int *)args->args[2];
          buffer->colval = *(double *)args->args[3];
        }
      }
    }
}



double realvalfordatemin( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  min_data* buffer = (min_data*)initid->ptr;
  return buffer->colval;
}



#endif


