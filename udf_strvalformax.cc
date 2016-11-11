/*
  returns the strvalformax  of the regression function a data set

  input parameters:
  dependent data (real)
  independent data (string)

  output:
  strvalformax  of the regression function (real)

  registering the function:
  CREATE AGGREGATE FUNCTION strvalformax RETURNS REAL SONAME 'udf_strvalformax.so';

  getting rid of the function:
  DROP FUNCTION strvalformax;

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

  my_bool strvalformax_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  void strvalformax_deinit( UDF_INIT* initid );
  void strvalformax_clear(UDF_INIT *initid, char *is_null, char *is_error);
  void strvalformax_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  void strvalformax_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  char* strvalformax( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  
}


struct max_data
{
  double max;
  char* colval;
};


my_bool strvalformax_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 2)
  {
    strcpy(message,"wrong number of arguments: strvalformax() requires two arguments");
    return 1;
  }
  if (args->arg_type[1]!=STRING_RESULT)
  {
    strcpy(message,"correlation() requires a string as parameter 2");
    return 1;
  }

  max_data   *buffer = new max_data;
  buffer->max = NULL;
  buffer->colval= NULL;
  initid->ptr = (char*)buffer;
  return 0;
}


void strvalformax_deinit( UDF_INIT* initid )
{
  max_data *buffer = (max_data*)initid->ptr;
  if(buffer->colval!=NULL){
    free(buffer->colval);
    buffer->colval=NULL;
  }
  delete initid->ptr;
}

void strvalformax_clear(UDF_INIT *initid, char *is_null, char *is_error)
{  
  max_data *buffer = (max_data*)initid->ptr;
  *is_null = 0;
  *is_error = 0;
  buffer->max=NULL;
  buffer->colval=NULL;
}

void strvalformax_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  strvalformax_clear(initid, is_null, is_error);
  strvalformax_add( initid, args, is_null, is_error );
}


void strvalformax_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  max_data *buffer = (max_data*)initid->ptr;
  if (args->args[0]!=NULL && args->args[1]!=NULL)
  {
    if(buffer->max==NULL){
      buffer->max = *(double*)args->args[0];
      if(buffer->colval!=NULL){
	free(buffer->colval);
	buffer->colval=NULL;
      }
      buffer->colval = (char *)malloc(args->attribute_lengths[1]+1);
      strcpy(buffer->colval,args->args[1],attribute_lengths[1]);
    }else{
      if((*(double*)args->args[0])>(buffer->max)){
	buffer->max = *(double *)args->args[0];
	if(buffer->colval!=NULL){
	  free(buffer->colval);
	  buffer->colval=NULL;
	}
	buffer->colval = (char *)malloc(args->attribute_lengths[1]+1);
	strcpy(buffer->colval,args->args[1]);
      }
    }
  }
}


char *strvalformax ( UDF_INIT* initid, UDF_ARGS* args,char* result,unsigned long* res_length, char* is_null, char* is_error )
{
  max_data* buffer = (max_data*)initid->ptr;
  result = buffer->colval;
  *res_length = strlen(buffer->colval);
  return result;
}



#endif


