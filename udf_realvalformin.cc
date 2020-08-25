/*
  returns the realvalformin  of the regression function a data set

  input parameters:
  dependent data (real)
  independent data (real)

  output:
  realvalformin  of the regression function (real)

  registering the function:
  CREATE AGGREGATE FUNCTION realvalformin RETURNS REAL SONAME 'udf_realvalformin.so';

  getting rid of the function:
  DROP FUNCTION realvalformin;

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

  my_bool realvalformin_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  void realvalformin_deinit( UDF_INIT* initid );
  void realvalformin_clear(UDF_INIT *initid, char *is_null, char *is_error);
  void realvalformin_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  void realvalformin_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  double realvalformin( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );

}


struct min_data
{
  double min;
  double colval;
};


my_bool realvalformin_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 2)
    {
      strcpy(message,"wrong number of arguments: realvalformin() requires two arguments");
      return 1;
    }
  initid->decimals=2;  
  min_data   *buffer = new min_data;
  buffer->min = 0;
  buffer->colval=0;
  initid->ptr = (char*)buffer;
  return 0;
}


void realvalformin_deinit( UDF_INIT* initid )
{
  min_data *buffer = (min_data*)initid->ptr;
  delete initid->ptr;
}

void realvalformin_clear(UDF_INIT *initid, char *is_null, char *is_error)
{  
  min_data *buffer = (min_data*)initid->ptr;
  *is_null = 0;
  *is_error = 0;
  buffer->min=NULL;
  buffer->colval=NULL;
}

void realvalformin_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  realvalformin_clear(initid, is_null, is_error);
  realvalformin_add( initid, args, is_null, is_error );
}


void realvalformin_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  min_data *buffer = (min_data*)initid->ptr;
  if (args->args[0]!=NULL && args->args[1]!=NULL)
    {
      if(buffer->min==NULL){
        buffer->min = *(double*)args->args[0];
        buffer->colval = *(double *)args->args[1];
      }else{
        if((*(double*)args->args[0])<(buffer->min)){
          buffer->min = *(double *)args->args[0];
          buffer->colval = *(double *)args->args[1];
        }
      }
    }
}



double realvalformin( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  min_data* buffer = (min_data*)initid->ptr;
  return buffer->colval;
}



#endif


