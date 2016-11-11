/*
  returns the realvalformax  of the regression function a data set

  input parameters:
  dependent data (real)
  independent data (real)

  output:
  realvalformax  of the regression function (real)

  registering the function:
  CREATE AGGREGATE FUNCTION realvalformax RETURNS REAL SONAME 'udf_realvalformax.so';

  getting rid of the function:
  DROP FUNCTION realvalformax;

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

my_bool realvalformax_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
void realvalformax_deinit( UDF_INIT* initid );
void realvalformax_clear(UDF_INIT *initid, char *is_null, char *is_error);
void realvalformax_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
void realvalformax_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
double realvalformax( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );

}


struct max_data
{
  double max;
  double colval;
};


my_bool realvalformax_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 2)
  {
    strcpy(message,"wrong number of arguments: realvalformax() requires two arguments");
    return 1;
  }
  initid->decimals=2;  
  max_data   *buffer = new max_data;
  buffer->max = 0;
  buffer->colval=0;
  initid->ptr = (char*)buffer;
  return 0;
}


void realvalformax_deinit( UDF_INIT* initid )
{
  max_data *buffer = (max_data*)initid->ptr;
  delete initid->ptr;
}

void realvalformax_clear(UDF_INIT *initid, char *is_null, char *is_error)
{  
  max_data *buffer = (max_data*)initid->ptr;
  *is_null = 0;
  *is_error = 0;
  buffer->max=NULL;
  buffer->colval=NULL;
}

void realvalformax_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  realvalformax_clear(initid, is_null, is_error);
  realvalformax_add( initid, args, is_null, is_error );
}


void realvalformax_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  max_data *buffer = (max_data*)initid->ptr;
  if (args->args[0]!=NULL && args->args[1]!=NULL)
  {
    if(buffer->max==NULL){
      buffer->max = *(double*)args->args[0];
      buffer->colval = *(double *)args->args[1];
    }else{
      if((*(double*)args->args[0])>(buffer->max)){
	buffer->max = *(double *)args->args[0];
	buffer->colval = *(double *)args->args[1];
      }
    }
  }
}



double realvalformax( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  max_data* buffer = (max_data*)initid->ptr;
  return buffer->colval;
}



#endif


