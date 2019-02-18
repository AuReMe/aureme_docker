/*
 *    Copyright (C) 1999-2002 Stijn van Dongen.
*/

#ifndef mcx_glob_h__
#define mcx_glob_h__

#include "nonema/matrix.h"


void globInitialize
(  void
)  ;

extern int UTYPE_NONE;
extern int UTYPE_INT;
extern int UTYPE_DBL;
extern int UTYPE_MX ;
extern int UTYPE_STR;
extern int UTYPE_SEQ;
extern int UTYPE_NEWHDL;

#define N_UTYPE 7
extern int UCLASS_NUM[N_UTYPE];

typedef struct zgglob_t* zgglob_p;

/*
 * The 'type' in the four routines below is the user interface type,
 * not the internal storage type.
*/
zgglob_p zgNew
(  int   type
,  void* object
)  ;
int zgPush
(  int         type
,  void*       object
)  ;
void* zgGetOb
(  zgglob_p glob,  int type
)  ;
int zgGetType
(  zgglob_p glob
)  ;
const char* zgGetTypeName
(  int utype
)  ;


int zgUser
(  const char* token
)  ;
void zgFree
(  zgglob_p* globpp
)  ;
zgglob_p zgRip
(  zgglob_p glob
)  ;
zgglob_p zgCopyObject  /* copies objects from names */
(  zgglob_p glob
)  ;
zgglob_p zgDupObject   /* copies names */
(  zgglob_p glob
)  ;
int zgMDup
(  int n
)  ;

zgglob_p zgAdd
(  zgglob_p o1
,  zgglob_p o2
)  ;
zgglob_p zgMul
(  zgglob_p o1
,  zgglob_p o2
)  ;
zgglob_p zgPow
(  zgglob_p o1
,  zgglob_p o2
)  ;
zgglob_p zgEq
(  zgglob_p o1
,  zgglob_p o2
)  ;
zgglob_p zgLq
(  zgglob_p o1
,  zgglob_p o2
)  ;
zgglob_p zgLt
(  zgglob_p o1
,  zgglob_p o2
)  ;

void zgAccessError
(  const char* what
,  const zgglob_p glob
)  ;
void zgSupportError
(  const char* what
,  const zgglob_p glob
)  ;

void zgInfo
(  zgglob_p glob
)  ;
int zgVars
(  void
)  ;
int zgUnlink
(  void
)  ;
int zgDef
(  void
)  ;

#endif
