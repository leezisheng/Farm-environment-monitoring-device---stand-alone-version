#ifndef _NUMTYPE_H_ 
#define _NUMTYPE_H_ 

#ifdef  __cplusplus
Extern  "C"  {
#endif

#include <stdint.h>

/* Signed */
typedef signed char		        int8_t;
typedef short int		        int16_t;
typedef int			            int32_t;

/* Unsigned */
typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;

/* Signed pointer */
typedef signed char	*	        p_int8_t;
typedef short int*	            p_int16_t;
typedef int*			        p_int32_t;

/* Unsigned pointer */
typedef unsigned char*		    p_uint8_t;
typedef unsigned short int* 	p_uint16_t;
typedef unsigned int*		    p_uint32_t;

/* Void pointer */
typedef void*                   p_void;

/* Common macro */
#define TASK_SUCCESS            1
#define TASK_FALSE              0
#define TASK_ERROR              255

#define OPERATION_SUCCESS       1
#define OPERATION_FALSE         0
#define OPERATION_ERROR         255

/* Data Stuct */
typedef struct Data_Struct
{
    int  iValue; 
    int iMeaning;
}xData;


#ifdef __cplusplus
}
#endif

#endif
