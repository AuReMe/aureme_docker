/*
 *    Copyright (C) 1999-2002 Stijn van Dongen.
*/

#include <stdio.h>
#include <stdlib.h>

#include "ops.h"
#include "stack.h"
#include "glob.h"
#include "util.h"

#include "util/txt.h"
#include "util/hash.h"
#include "util/file.h"
#include "util/minmax.h"

#include "nonema/matrix.h"
#include "nonema/compose.h"
#include "nonema/io.h"
#include "mcl/interpret.h"
#include "mcl/compose.h"
#include "mcl/params.h"


#define     SORT_OPHOOKS 1

int      opMul                (  void  )  ;
int      opImac               (  void  )  ;
int      opAdd                (  void  )  ;
int      opMod                (  void  )  ;
int      opDiv                (  void  )  ;
int      opPow                (  void  )  ;
int      opMin                (  void  )  ;
int      opMax                (  void  )  ;
int      opAllOne             (  void  )  ;
int      opColDimension       (  void  )  ;
int      opDigits             (  void  )  ;
int      opDimension          (  void  )  ;
int      opStackList          (  void  )  ;
int      opDup                (  void  )  ;
int      opCopy               (  void  )  ;
int      opStackMDup          (  void  )  ;
int      opStackExch          (  void  )  ;
int      opStackPop           (  void  )  ;
int      opStackClear         (  void  )  ;
int      opStackRoll          (  void  )  ;
int      opDef                (  void  )  ;
int      opOpList             (  void  )  ;
int      opFree               (  void  )  ;
int      opHadamardPower      (  void  )  ;
int      opInflate            (  void  )  ;
int      opExpand             (  void  )  ;
int      opHadamard           (  void  )  ;
int      opHelp               (  void  )  ;
int      opTut                (  void  )  ;
int      opInfo               (  void  )  ;
int      opTell               (  void  )  ;
int      opSearch             (  void  )  ;
int      opIdentity           (  void  )  ;
int      opLoadMatrix         (  void  )  ;
int      opLoadFile           (  void  )  ;
int      opMakeCharacteristic (  void  )  ;
int      opMakeStochastic     (  void  )  ;
int      opTrim               (  void  )  ;
int      opQuit               (  void  )  ;
int      opDong               (  void  )  ;
int      opRowDimension       (  void  )  ;
int      opRepeat             (  void  )  ;
int      opIfelse             (  void  )  ;
int      opDo                 (  void  )  ;
int      opTypes              (  void  )  ;
int      opWhile              (  void  )  ;
int      opEq                 (  void  )  ;
int      opLt                 (  void  )  ;
int      opLq                 (  void  )  ;
int      opGt                 (  void  )  ;
int      opGq                 (  void  )  ;
int      opTranspose          (  void  )  ;
int      opUnlink             (  void  )  ;
int      opViewMatrix         (  void  )  ;
int      opWriteMatrix        (  void  )  ;
int      opVars               (  void  )  ;

typedef struct
{  int            (*op_op)(void)
;  const char*    op_token
;  const char*    op_meaning
;  const char*    stack_pre
;  const char*    stack_post
;
}  opHook         ;

int opHookCmp
(  const void* oh1
,  const void* oh2
)  ;

typedef struct
{  const char*    op_token
;  const char*    op_alias
;
}  opAlias        ;

int opAliasCmp
(  const void* oa1
,  const void* oa2
)  ;


static mcxHash *symtable_g    =  NULL;

const char* opHelpTxt
=
" Strings are  always entered with  a leading  slash, as in  /string, handles\n"
" (which are names for objects) are always  entered with a leading dot, as in\n"
" .hdl (no trailing dot though).  '/<op> help' prints <op> docstring. '/<str>\n"
" grep' searches for <str> in the docstrings. When stack is empty, 'help' and\n"
" 'grep' print all docstrings, otherwise '/* help' and '/* grep' do the same.\n"
" 'ops' lists  all operators. Some  operators, such as 'mul',  are overloaded\n"
" and can be applied to mixed  types. Some operators do in-place modification\n"
" of  the top  object (such  as 'st'  and 'ch').  In the  docstrings this  is\n"
" denoted as  <ob> -> <ob'>. Operators  such as 'lm' (load  matrix) and 'mul'\n"
" (applied to  matrices) push an  *anonymous* matrix. Anonymous  matrices are\n"
" freed when  popped. A matrix  can be transformed  into a *named*  matrix by\n"
" 'def'. '.<hdl> d' transforms an anonymous matrix into a named matrix, which\n"
" can be pushed *by reference* using  '.<hdl>'. So '.<hdl> .<hdl> mul' leaves\n"
" the square of .<hdl>  on the stack, and '.<hdl> .<hdl>  st mul' squares the\n"
" stochastic matrix  stored in .<hdl>.  Named matrices are not  freed, unless\n"
" specifically requested by '.<hdl> free' or '.<hdl> unlink'.                \n"

;

opHook opHookDir[] =  
{
   {  opAdd
   ,  TOKEN_ADD
   ,  "add objects"
   ,  "<o1> <o2>"
   ,  "<o3>"
   }
,  {  opPow
   ,  TOKEN_POW
   ,  "take power of object"
   ,  "<o1> <i>"
   ,  "<o2>"
   }
,  {  opMod
   ,  TOKEN_MOD
   ,  "modulus, i1 % i2"
   ,  "<i1> <i2>"
   ,  "<i3>"
   }
,  {  opMin
   ,  TOKEN_MIN
   ,  "minimum"
   ,  "<n1> <n2>"
   ,  "<n3>"
   }
,  {  opMax
   ,  TOKEN_MAX
   ,  "maximum"
   ,  "<n1> <n2>"
   ,  "<n3>"
   }
,  {  opDiv
   ,  TOKEN_DIV
   ,  "division, n1 / n2"
   ,  "<n1> <n2>"
   ,  "<n3>"
   }
,  {  opStackExch
   ,  TOKEN_EXCH
   ,  "swap objects"
   ,  "<o1> <o2>"
   ,  "<o2> <o1>"
   }
,  {  opDong
   ,  TOKEN_DONG
   ,  "set verbosity level"
   ,  "[<i>]"
   ,  "*"
   }
,  {  opColDimension
   ,  TOKEN_COLDIMENSION
   ,  "push col dimension"
   ,  "<m>"
   ,  "<m> <i>"
   }
,  {  opMakeCharacteristic
   ,  TOKEN_MAKECHARACTERISTIC
   ,  "make characteristic"
   ,  "<m>"
   ,  "<m'>"
   }
,  {  opStackClear
   ,  TOKEN_CLEAR
   ,  "clear"
   ,  ".."
   ,  "nil"
   }
,  {  opStackMDup
   ,  TOKEN_MDUP
   ,  "mdup, dup <int> obs"
   ,  "<int>"
   ,  ".."
   }
,  {  opCopy
   ,  TOKEN_COPY
   ,  "copy, (deep) copy ob"
   ,  "<o1>"
   ,  "<o1> <o2>"
   }
,  {  opDup
   ,  TOKEN_DUP
   ,  "dup, (value) copy ob"
   ,  "<o1>"
   ,  "<o1> <o2>"
   }
,  {  opDimension
   ,  TOKEN_DIMENSION
   ,  "push dimension"
   ,  "<m>"
   ,  "<m> <i>"
   }
,  {  opDigits
   ,  TOKEN_DIGITS
   ,  "set precision"
   ,  "<i>"
   ,  "*"
   }
,  {  opFree
   ,  TOKEN_FREE
   ,  "free object in handle"
   ,  "<h>"
   ,  "*"
   }
,  {  opHelp
   ,  TOKEN_HELP
   ,  "help on operator"
   ,  "<s>"
   ,  "*"
   }
,  {  opExpand
   ,  TOKEN_EXPAND
   ,  "mcl flow expansion"
   ,  "<m1>"
   ,  "<m2>"
   }
,  {  opInflate
   ,  TOKEN_INFLATE
   ,  "mcl matrix inflation"
   ,  "<m> <d>"
   ,  "<m'>"
   }
,  {  opHadamard
   ,  TOKEN_HADAMARD
   ,  "hadamard product of matrices"
   ,  "<m1> <m2>"
   ,  "<m3>"
   }
,  {  opHadamardPower
   ,  TOKEN_HADAMARDPOWER
   ,  "take hadamard power of matrix"
   ,  "<m> <d>"
   ,  "<m'>"
   }
,  {  opIdentity
   ,  TOKEN_IDENTITY
   ,  "push identity matrix"
   ,  "<i>"
   ,  "<m>"
   }
,  {  opTut
   ,  TOKEN_TUT
   ,  "mini crash course"
   ,  "*"
   ,  "*"
   }
,  {  opAllOne
   ,  TOKEN_ALLONE
   ,  "push all one matrix"
   ,  "<i1> <i2>"
   ,  "<m>"
   }
,  {  opLoadFile
   ,  TOKEN_LOADFILE
   ,  "load file"
   ,  "<s>"
   ,  ".."
   }
,  {  opLoadMatrix
   ,  TOKEN_LOADMATRIX
   ,  "load matrix from file"
   ,  "<s>"
   ,  "<m>"
   }
,  {  opVars
   ,  TOKEN_VARS
   ,  "list all deffed handles"
   ,  "*"
   ,  "*"
   }
,  {  opOpList
   ,  TOKEN_OPLIST
   ,  "compact listing of operators"
   ,  "*"
   ,  "*"
   }
,  {  opMul
   ,  TOKEN_MUL
   ,  "multiply objects"
   ,  "<o1> <o2>"
   ,  "<o3>"
   }
,  {  opImac
   ,  TOKEN_IMAC
   ,  "interpret matrix as clustering"
   ,  "<m1>"
   ,  "<m1> <m2>"
   }
,  {  opDef
   ,  TOKEN_DEF
   ,  "save object in handle"
   ,  "<o> <h>"
   ,  "*"
   }
,  {  opStackRoll
   ,  TOKEN_ROLL
   ,  "shift i1 objects i2 times"
   ,  "<i1> <i2>"
   ,  ".."
   }
,  {  opStackPop
   ,  TOKEN_POP
   ,  "pop top"
   ,  "<o>"
   ,  "*"
   }
,  {  opTrim 
   ,  TOKEN_TRIM
   ,  "-P i1 -S i2 -R i3 -pct i4, (for xpn)"
   ,  "<i1> <i2> <i3> <i4>"
   ,  "*"
   }
,  {  opQuit
   ,  TOKEN_QUIT
   ,  "quit"
   ,  "*"
   ,  "graceful exit"
   }
,  {  opRowDimension
   ,  TOKEN_ROWDIMENSION
   ,  "push row dimension"
   ,  "<m>"
   ,  "<m> <i>"
   }
,  {  opGq
   ,  TOKEN_GQ
   ,  "<o1> >= <o2> ? 1 : 0"
   ,  "<o1> <o2>"
   ,  "<i>"
   }
,  {  opGt
   ,  TOKEN_GT
   ,  "<o1> >  <o2> ? 1 : 0"
   ,  "<o1> <o2>"
   ,  "<i>"
   }
,  {  opLq
   ,  TOKEN_LQ
   ,  "<o1> <= <o2> ? 1 : 0"
   ,  "<o1> <o2>"
   ,  "<i>"
   }
,  {  opLt
   ,  TOKEN_LT
   ,  "<o1> <  <o2> ? 1 : 0"
   ,  "<o1> <o2>"
   ,  "<i>"
   }
,  {  opEq
   ,  TOKEN_EQ
   ,  "<o1> == <o2> ? 1 : 0"
   ,  "<o1> <o2>"
   ,  "<i>"
   }
,  {  opIfelse
   ,  TOKEN_IFELSE
   ,  "if <i> <b1> else <b2>"
   ,  "<i> <b1> <b2>"
   ,  ".."
   }
,  {  opWhile
   ,  TOKEN_WHILE
   ,  "while <b1> pushes <i> != 0, do <b2>"
   ,  "<b1> <b2>"
   ,  ".."
   }
,  {  opDo
   ,  TOKEN_DO
   ,  "apply <b>"
   ,  "<b>"
   ,  ".."
   }
,  {  opRepeat
   ,  TOKEN_REPEAT
   ,  "<i> times apply <b>"
   ,  "<i> <b>"
   ,  ".."
   }
,  {  opSearch
   ,  TOKEN_SEARCH
   ,  "search for (sub)string in docstrings"
   ,  "<s>"
   ,  "*"
   }
,  {  opMakeStochastic
   ,  TOKEN_MAKESTOCHASTIC
   ,  "make stochastic (diagonal scaling)"
   ,  "<m>"
   ,  "<m'>"
   }
,  {  opStackList
   ,  TOKEN_LIST
   ,  "list (stack)"
   ,  "*"
   ,  "*"
   }
,  {  opTranspose
   ,  TOKEN_TRANSPOSE
   ,  "push transpose"
   ,  "<m1>"
   ,  "<m1> <m2>"
   }
,  {  opUnlink
   ,  TOKEN_UNLINK
   ,  "unlink object from handle and push it"
   ,  "<h>"
   ,  "<o>"
   }
,  {  opTell
   ,  TOKEN_TELL
   ,  "print info for top <i> objects"
   ,  "<i>"
   ,  "*"
   }
,  {  opInfo
   ,  TOKEN_INFO
   ,  "print top object info"
   ,  "*"
   ,  "*"
   }
,  {  opViewMatrix
   ,  TOKEN_VIEWMATRIX
   ,  "print (small) matrix"
   ,  "<m>"
   ,  "<m>"
   }
,  {  opWriteMatrix
   ,  TOKEN_WRITEMATRIX
   ,  "write matrix to file"
   ,  "<m> <s>"
   ,  "<m>"
   }
,  {  NULL
   ,  ""
   ,  ""
   ,  ""
   ,  ""
   }
}  ;

opAlias opAliasDir[] =
{
   {  TOKEN_DEF
   ,  "d"
   }
,  {  TOKEN_EXCH
   ,  "x"
   }
,  {  TOKEN_INFO
   ,  "i"
   }
,  {  TOKEN_POP
   ,  "p"
   }
,  {  TOKEN_FREE
   ,  "f"
   }
,  {  TOKEN_UNLINK
   ,  "u"
   }
,  {  TOKEN_LIST
   ,  "l"
   }
,  {  TOKEN_HELP
   ,  "h"
   }
,  {  TOKEN_QUIT
   ,  "bye"
   }
,  {  TOKEN_SEARCH
   ,  "g"
   }
,  {  NULL
   ,  NULL
   }
}  ;


void opHookHelp
(  opHook* ophook
)
   {  fprintf
      (  stdout
      ,  "%-7s%12s  ->  %-15s%s\n"
      ,  ophook->op_token
      ,  ophook->stack_pre
      ,  ophook->stack_post
      ,  ophook->op_meaning
      )
;  }


void opInitialize
(  void
)
   {  opHook         *ophook        =  opHookDir+0
   ;  opAlias        *opalias       =  opAliasDir+0
   ;  int            n_ophooks      =  sizeof(opHookDir) / sizeof(opHook) - 1
   ;  int            n_opalias      =  sizeof(opAliasDir) / sizeof(opAlias) - 1

   ;  symtable_g = mcxHashNew(100, mcxTingHash, mcxTingCmp)

   ;  if (SORT_OPHOOKS)
      {  qsort(opHookDir, n_ophooks, sizeof(opHook), opHookCmp)
      ;  qsort(opAliasDir, n_opalias, sizeof(opAlias), opAliasCmp)
   ;  }

   ;  while (ophook->op_op)
      {  mcxTing*    ting  =  mcxTingNew(ophook->op_token)
      ;  mcxKV*      kv    =  mcxHashSearch(ting, symtable_g, MCX_DATUM_INSERT)

      ;  if (!kv)
            fprintf(stderr, "mission impossible I\n")
         ,  exit(1)

      ;  kv->val = ophook
      ;  ophook++
   ;  }

   ;  while (opalias->op_token)
      {
         mcxTing  *opting  =  mcxTingNew(opalias->op_token)
      ;  mcxTing  *alting  =  mcxTingNew(opalias->op_alias)
      ;  mcxKV*      kv    =  mcxHashSearch(opting, symtable_g, MCX_DATUM_FIND)

      ;  if (!kv)
         {  fprintf
            (stderr, "ignoring (internal) alias error: no such token\n")
      ;  }
         else
         {  opHook *hook=  (opHook*) kv->val
         ;  mcxKV* kv   =  mcxHashSearch(alting, symtable_g, MCX_DATUM_INSERT)
         ;  if (!kv)
               fprintf(stderr, "mission impossible II\n")
            ,  exit(1)
         ;  kv->val     =  hook
      ;  }
      ;  mcxTingFree(&opting)
      ;  opalias++
   ;  }
;  }


int opHookCmp
(  const void* oh1
,  const void* oh2
)
   {  return strcmp(((opHook*) oh1)->op_token,((opHook*) oh2)->op_token)
;  }


int opAliasCmp
(  const void* oa1
,  const void* oa2
)
   {  return strcmp(((opAlias*) oa1)->op_alias,((opAlias*) oa2)->op_alias)
;  }


int opTranspose
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  mclMatrix* tp = mx ? mclMatrixTranspose(mx) : NULL
   ;  if (!mx) return 0
   ;  return zgPush(UTYPE_MX, tp)
;  }


int opImac
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  mclMatrix* cl
   ;  if (!mx) return 0
   ;  cl = mclInterpret(mx, NULL)
   ;  return zgPush(UTYPE_MX, cl)
;  }


int opMakeStochastic
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  if (!mx) return 0
   ;  mclMatrixMakeStochastic(mx)
   ;  return 1
;  }


int opWriteMatrix
(  void
)
   {  mclMatrix* mx = zsGetOb(1, UTYPE_MX)
   ;  mcxTing* ting = zsGetOb(0, UTYPE_STR)
   ;  mcxIOstream* xf

   ;  if (!mx || !ting) return 0
   ;  xf = mcxIOstreamNew(ting->str, "w")

   ;  mclMatrixWriteAscii(mx, xf, digits_g, EXIT_ON_FAIL)
   ;  fflush(xf->fp)
   ;  mcxIOstreamFree(&xf)

   ;  return zsPop()
;  }


int opDigits
(  void
)
   {  int *ip = zsGetOb(0, UTYPE_INT)
   ;  if (!ip) return 0

   ;  digits_g = *ip
   ;  return zsPop()
;  }

int opMakeCharacteristic
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  if (!mx) return 0
   ;  mclMatrixMakeCharacteristic(mx)
   ;  return 1
;  }

int opLoadFile
(  void
)
   {  mcxIOstream *xf      =  NULL
   ;  mcxTing*    fname    =  zsGetOb(0, UTYPE_STR)
   ;  mcxTing*    seqting  =  NULL
   ;  int         ok       =  1

   ;  if (!fname)
      return 0

   ;  xf = mcxIOstreamNew(fname->str, "r")

   ;  if (mcxIOstreamOpen(xf, RETURN_ON_FAIL) != STATUS_OK)
      {  zmTell('e', "file [%s] could not be opened", fname->str)
      ;  ok = 0
   ;  }
      else
      {  zsPop()
      ;  seqting = mcxIOstreamReadFile(xf, NULL)
      ;  ok = zsDoSequence(seqting->str)
   ;  }

   ;  mcxTingFree(&seqting)
   ;  mcxIOstreamFree(&xf)
   ;  return ok
;  }

int opLoadMatrix
(  void
)
   {  mcxIOstream *xf
   ;  mclMatrix* mx

   ;  mcxTing* fname = zsGetOb(0, UTYPE_STR)
   ;  if (!fname)
      return 0

   ;  xf = mcxIOstreamNew(fname->str, "r")
   ;  mx = mclMatrixRead(xf, RETURN_ON_FAIL)
   ;  mcxIOstreamFree(&xf)

   ;  if (mx)
      {  zsPop()
      ;  return zgPush(UTYPE_MX, mx)
   ;  }
      else
      {  zmTell('e', "some error occurred while reading matrix")
      ;  return 0
   ;  }
   ;  return 1
;  }

int opViewMatrix
(  void
)
   {  mclMatrix*  mx
   ;  float       maxval

   ;  mx       =  zsGetOb(0, UTYPE_MX)
   ;  if (!mx) return 0

   ;  maxval   =  mclMatrixMaxValue(mx)

   ;  if (mx->N_cols <= 15)
      {  if (maxval < 10.0)
         {  mcxPrettyPrint(mx, stdout, 4, 2, NULL)
      ;  }
         else
         {  double lg = log10(maxval)
         ;  int width = (int) (lg+1.0)
         ;  mcxPrettyPrint(mx, stdout, width, 0, NULL)
      ;  }
   ;  }
      else if (mx->N_cols <= 35)
      mclMatrixBoolPrint(mx, 0)
   ;  else if (mx->N_cols <= 75)
      mclMatrixBoolPrint(mx, 1)
   ;  else
      {  int n_entries = mclMatrixNrofEntries(mx)
      ;  zmTell
         ('m', "mx  [%dx%d] %d entries", mx->N_rows, mx->N_cols, n_entries)
   ;  }
   ;  fprintf(stdout, "\n")
   ;  return 1
;  }


int opCopy
(  void
)
   {  zgglob_p glob = zsGetGlob(0)
   ;  zgglob_p new

   ;  if (!glob)
      return 0

   ;  if ((new = zgCopyObject(glob)))
      zsPush(new)
   ;  else
      return 0

   ;  return 1
;  }


int opDup
(  void
)
   {  zgglob_p glob = zsGetGlob(0)
   ;  zgglob_p new

   ;  if (!glob) return 0

   ;  if ((new = zgDupObject(glob)))
      zsPush(new)
   ;  else
      return 0

   ;  return 1
;  }


int opDong
(  void
)
   {  const int   *ip
   ;  int   vb

   ;  if (zsEmpty())
      vb = 0
   ;  else if ((ip = zsGetOb(0, UTYPE_INT)))
      {  vb = *ip
      ;  zsPop()
   ;  }
      else
      return 0

   ;  vb = MAX(vb, 0)

   ;  if (!vb) /* used as toggle */
      {  v_g = v_g ? 0 : 1
      ;  fprintf(stdout, v_g ? "talkative\n" : "silent\n")
   ;  }
      else
      {  v_g = 1
      ;  while (--vb)
         v_g = (v_g << 1) | 1
   ;  }
   ;  return 1
;  }


int opSearch
(  void
)
   {  mcxTing* ting  =  zsEmpty() ? NULL : zsGetOb(0, UTYPE_STR)
   ;  opHook* ophook =  opHookDir+0
   ;  mcxbool  all   =  (!ting || !strcmp(ting->str, "*"))

   ;  while (ophook->op_op)
      {  if
         (  all
         || strstr(ophook->op_token, ting->str)
         || strstr(ophook->op_meaning, ting->str)
         )
         opHookHelp(ophook)
      ;  ophook++
   ;  }

   ;  return ting ? zsPop() : 0
;  }


int opHelp
(  void
)
   {  mcxTing* ting  =  zsEmpty() ? NULL : zsGetOb(0, UTYPE_STR)
   ;  opHook* ophook =  opHookDir+0
   ;  mcxbool  all   =  (!ting || !strcmp("*", ting->str))

   ;  while (ophook->op_op)
      {  if (all || !strcmp(ophook->op_token, ting->str))
         opHookHelp(ophook)
      ;  ophook++
   ;  }
      if (all)
      fprintf
      (  stdout
      ,  "<d> double <i> int <n> number"
         " <h> handle <s> string <b> block <o> object\n"
         "'tut' prints some guidelines\n"
      )

   ;  return ting ? zsPop() : 0
;  }


int opOpList
(  void
)
   {  opHook* hook   =  opHookDir+0
   ;  opAlias* alias =  opAliasDir+0
   ;  int printed_length = 0

   ;  while (hook->op_op)
      {
         int length = strlen(hook->op_token) + 1
      ;  if (length + printed_length > 75)
         {  fprintf(stdout, "\n")
         ;  printed_length = 0
      ;  }
         fprintf(stdout, " %s",  hook->op_token)
      ;  printed_length += length
      ;  hook++
   ;  }

   ;  while (alias->op_token)
      {
         int length = strlen(alias->op_token) + strlen(alias->op_alias) + 2
      ;  if (length + printed_length > 75)
         {  fprintf(stdout, "\n")
         ;  printed_length = 0
      ;  }
         fprintf(stdout, " %s=%s",  alias->op_alias, alias->op_token)
      ;  printed_length += length
      ;  alias++
   ;  }

      if (printed_length)
      fprintf(stdout, "\n")
   ;  return 1
;  }


int opStackRoll
(  void
)
   {  const int   *np   =  zsGetOb(1, UTYPE_INT)
   ;  const int   *jp   =  zsGetOb(0, UTYPE_INT)
   ;  int   n           =  np ? *np - 1 : 0
   ;  int   j           =  jp ? *jp : 0

   ;  if (!np || !jp) return 0

   ;  zsPop()
   ;  zsPop()
   ;  return zsRoll(n, j)
;  }


int opFree
(  void
)
   {  if (opUnlink())
      {  zmTell('d', "[%s] removing object now", TOKEN_FREE)
      ;  zsPop()
   ;  }
      else
      return 0

   ;  return 1
;  }


int opMul
(  void
)
   {  int ok = zsHaveNargs(2) ? 1 : 0
   ;  zgglob_p o1, o2, o3
   
   ;  if (!ok)
      return 0
   
   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)
                          /* below can be either error or mx scaling */
   ;  if (!(o3 = zgMul(o1,o2)))
      {
         int typex = zgGetType(o1)
      ;  int typey = zgGetType(o2)

      ;  if (typex == UTYPE_MX && UCLASS_NUM[typey])
         {  zsPop()
         ;  return 1
      ;  }
         else
         return 0
   ;  }

   ;  zsPop()
   ;  zsPop()

   ;  zsPush(o3)
   ;  return 1
;  }


int opAdd
(  void
)
   {  int ok = zsHaveNargs(2) ? 1 : 0
   ;  zgglob_p o1, o2, o3
   
   ;  if (!ok)
      return 0
   
   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)

   ;  if (!(o3 = zgAdd(o1,o2)))
      return 0

   ;  zsPop()
   ;  zsPop()

   ;  zsPush(o3)
   ;  return 1
;  }


int opMax
(  void
)
   {  zgglob_p o1, o2, o3
   ;  if (!zsHaveNargs(2)) 
      return 0

   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)

   ;  if (!(o3 = zgLt(o1,o2)))
      return 0

   ;  if (zgGetOb(o3, UTYPE_INT)) /* It is true that o1 <= o2 */
      {  zsExch()
      ;  zsPop()
   ;  }
      else
      zsPop()

   ;  zgFree(&o3)
   ;  return 1
;  }


int opMin
(  void
)
   {  zgglob_p o1, o2, o3
   ;  if (!zsHaveNargs(2)) 
      return 0

   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)

   ;  if (!(o3 = zgLq(o1,o2)))
      return 0

   ;  if (zgGetOb(o3, UTYPE_INT)) /* It is true that o1 < o2 */
      zsPop()

   ;  else
      {  zsExch()
      ;  zsPop()
   ;  }
   ;  zgFree(&o3)
   ;  return 1
;  }


int opDiv
(  void
)
   {  int   typex =  zsGetType(1)
   ;  int   typey =  zsGetType(0)

   ;  if (UCLASS_NUM[typex] && UCLASS_NUM[typey])
      {  if (typex == UTYPE_INT && typey == UTYPE_INT)
         {  const int* ip1 = zsGetOb(1, UTYPE_INT)
         ;  const int* ip2 = zsGetOb(0, UTYPE_INT)
         ;  int i1 = ip1 ? *ip1 : 0
         ;  int i2 = ip2 ? *ip2 : 0
         ;  int i3 = i1 / (i2 ? i2 : 1)
         ;  if (!ip1 || !ip2) return 0
         ;  zsPop()
         ;  zsPop()
         ;  return zgPush(UTYPE_INT, &i3)
      ;  }
         else
         {  const double* dp1 = zsGetOb(1, UTYPE_DBL)
         ;  const double* dp2 = zsGetOb(0, UTYPE_DBL)
         ;  double d1 = dp1 ? *dp1 : 0
         ;  double d2 = dp2 ? *dp2 : 0
         ;  double d3 = d1 / (d2 ? d2 : 1)
         ;  if (!dp1 || !dp2) return 0
         ;  zsPop()
         ;  zsPop()
         ;  return zgPush(UTYPE_DBL, &d3)
      ;  }
   ;  }
      else
      {  zmNotSupported2(TOKEN_DIV, typex, typey)
      ;  return 0
   ;  }
      return 0 
;  }


int opMod
(  void
)
   {  const int* ip1 = zsGetOb(1, UTYPE_INT)
   ;  const int* ip2 = zsGetOb(0, UTYPE_INT)
   ;  int i1 = ip1 ? *ip1 : 0
   ;  int i2 = ip2 ? *ip2 : 0
   ;  int i3 = i1 % i2

   ;  if (!ip1 || !ip2)
      return 0
   
   ;  zsPop()
   ;  zsPop()
   ;  return zgPush(UTYPE_INT, &i3)
;  }


int opPow
(  void
)
   {  int ok = zsHaveNargs(2) ? 1 : 0
   ;  zgglob_p o1, o2, o3
   
   ;  if (!ok)
      return 0
   
   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)
                          /* below can be either error or mx scaling */
   ;  if (!(o3 = zgPow(o1,o2)))
      return 0

   ;  zsPop()
   ;  zsPop()

   ;  zsPush(o3)
   ;  return 1
;  }


int opExpand
(  void
)
   {  mclMatrix *mx = zsGetOb(0, UTYPE_MX)
   ;  mclComposeStats* stats = mx ? mclComposeStatsNew(10, 100) : NULL
   ;  mclMatrix *sq

   ;  if (!mx) return 0
   ;  sq = mclFlowExpand(mx, stats)
   ;  mclComposeStatsFree(&stats)

   ;  zsPop()
   ;  return zgPush(UTYPE_MX, sq)
;  }


int opInflate
(  void
)
   {  mclMatrix *mx     =  zsGetOb(1, UTYPE_MX)
   ;  const double *dp  =  zsGetOb(0, UTYPE_DBL)

   ;  if (!mx || !dp) return 0

   ;  mclMatrixInflate(mx, *dp)
   ;  zsPop()
   ;  return 1
;  }


int opHadamard
(  void
)
   {  mclMatrix *x = zsGetOb(1, UTYPE_MX)
   ;  mclMatrix *y = zsGetOb(0, UTYPE_MX)
   ;  mclMatrix *z

   ;  if (!x || !y) return 0
   ;  z = mclMatrixHadamard(x, y)

   ;  zsPop()
   ;  zsPop()
   ;  return zgPush(UTYPE_MX, z)
;  }


int opWhile
(  void
)
   {  mcxTing *cting    =  zsGetOb(1, UTYPE_SEQ)
   ;  mcxTing *lting    =  zsGetOb(0, UTYPE_SEQ)
   ;  mcxTing *loopting =  lting ? mcxTingNew(lting->str) : NULL
   ;  mcxTing *condting =  cting ? mcxTingNew(cting->str) : NULL
   ;  int ok            =  1

   ;  if (!loopting || !condting)
      {  ok = 0
      ;  goto done
   ;  }

   ;  zsPop()
   ;  zsPop()

   ;  while (1)
      {
         int *cntn

      ;  if (!zsDoSequence(condting->str))
         {  ok = 0
         ;  goto done
      ;  }

      ;  if (!(cntn = zsGetOb(0, UTYPE_INT)))
         {  zmTell('e', "[%s] condition did not leave integer", TOKEN_WHILE)
         ;  ok = 0
         ;  goto done
      ;  }

         if (!(*cntn))
         {  zsPop()
         ;  break
      ;  }

      ;  zsPop()

      ;  if (!zsDoSequence(loopting->str))
         {  ok = 0
         ;  goto done
      ;  }
   ;  }

      done
   :
      mcxTingFree(&condting)
   ;  mcxTingFree(&loopting)
   ;  return ok
;  }


int opIfelse
(  void
)
   {  int*  cntn        =  zsGetOb(2, UTYPE_INT)
   ;  mcxTing *ting0    =  zsGetOb(0, UTYPE_SEQ)
   ;  mcxTing *ting1    =  zsGetOb(1, UTYPE_SEQ)
   ;  mcxTing *ifting   =  ting1 ? mcxTingNew(ting1->str) : NULL
   ;  mcxTing *elseting =  ting0 ? mcxTingNew(ting0->str) : NULL
   ;  int ok            =  1

   ;  if (!ifting || !elseting || !cntn)
      {  ok = 0
      ;  goto done
   ;  }

   ;  zsPop()
   ;  zsPop()
   ;  zsPop()

   ;  if (*cntn)
      zsDoSequence(ifting->str)
   ;  else
      zsDoSequence(elseting->str)

   ;  done
   :  
      mcxTingFree(&ifting)
   ;  mcxTingFree(&elseting)
   ;  return ok
;  }


int opDo
(  void
)
   {  mcxTing *ting     =  zsGetOb(0, UTYPE_SEQ)
   ;  mcxTing *seqting  =  ting ? mcxTingNew(ting->str) : NULL
   ;  int ok            =  1
   ;
      if (!seqting)    /* seqting new because of pop */
      {  ok = 0
      ;  goto done
   ;  }

      zsPop()
   ;
      if (!zsDoSequence(seqting->str))
      {  ok = 0
      ;  goto done
   ;  }

      done
   :
      mcxTingFree(&seqting)
   ;  return ok
;  }


int opRepeat
(  void
)
   {  int *ip           =  zsGetOb(1, UTYPE_INT)
   ;  mcxTing *ting     =  zsGetOb(0, UTYPE_SEQ)
   ;  mcxTing *seqting  =  ting ? mcxTingNew(ting->str) : NULL
   ;  int n             =  ip ? *ip : 0
   ;  int ok            =  1
   ;
      if (!ip || !seqting)    /* seqting new because of pop */
      {  ok = 0
      ;  goto done
   ;  }
    
      zsPop()
   ;  zsPop()
   ;
      while (n--)
      {  if (!zsDoSequence(seqting->str))
         {  ok = 0
         ;  goto done
      ;  }
      }

      done
   :
      mcxTingFree(&seqting)
   ;  return ok
;  }


int opEq
(  void
)
   {  int ok = zsHaveNargs(2) ? 1 : 0
   ;  zgglob_p o1, o2, o3
   
   ;  if (!ok)
      return 0
   
   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)

   ;  if (!(o3 = zgEq(o1,o2)))
      return 0

   ;  zsPop()
   ;  zsPop()

   ;  zsPush(o3)
   ;  return 1
;  }


int opLq
(  void
)
   {  int ok = zsHaveNargs(2) ? 1 : 0
   ;  zgglob_p o1, o2, o3
   
   ;  if (!ok)
      return 0
   
   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)

   ;  if (!(o3 = zgLq(o1,o2)))
      return 0

   ;  zsPop()
   ;  zsPop()

   ;  zsPush(o3)
   ;  return 1
;  }


int opLt
(  void
)
   {  int ok = zsHaveNargs(2) ? 1 : 0
   ;  zgglob_p o1, o2, o3
   
   ;  if (!ok)
      return 0
   
   ;  o1 = zsGetGlob(1)
   ;  o2 = zsGetGlob(0)

   ;  if (!(o3 = zgLt(o1,o2)))
      return 0

   ;  zsPop()
   ;  zsPop()

   ;  zsPush(o3)
   ;  return 1
;  }


int opGq
(  void
)
   {  if (zsExch())
      return opLq()
   ;  else
      return 0
;  }


int opGt
(  void
)
   {  if (zsExch())
      return opLt()
   ;  else
      return 0
;  }


int opRowDimension
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  if (!mx) return 0
   ;  return zgPush(UTYPE_INT, &(mx->N_rows))
;  }


int opColDimension
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  if (!mx) return 0
   ;  return zgPush(UTYPE_INT, &(mx->N_cols))
;  }


int opTrim
(  void
)
   {  int ok   = zsHaveNargs(4) ?  1 : 0
   ;  int *pp  = ok ? zsGetOb(3, UTYPE_INT) : NULL
   ;  int *sp  = ok ? zsGetOb(2, UTYPE_INT) : NULL
   ;  int *rp  = ok ? zsGetOb(1, UTYPE_INT) : NULL
   ;  int *pctp= ok ? zsGetOb(0, UTYPE_INT) : NULL

   ;  if (!ok) return 0

   ;  if (*pp <= 0 || *sp <=  0  || *rp <= 0 || *pctp <= 0 || *pctp > 100)
      {  zmTell('e', "[%s] integer params not ok", TOKEN_TRIM)
      ;  return 0
   ;  }

   ;  mclPrecision   =  1 / ((float) *pp)
   ;  mclRecoverNumber= *rp
   ;  mclSelectNumber=  *sp
   ;  mclPct         =  *pctp

   ;  zsPop()
   ;  zsPop()
   ;  zsPop()
   ;  zsPop()
   ;  return 1
;  }


int opHadamardPower
(  void
)
   {  mclMatrix* mx = zsGetOb(1, UTYPE_MX)
   ;  double* dp = zsGetOb(0, UTYPE_DBL)
   ;  if (!mx || !dp) return 0
   ;  mclMatrixHdp(mx, *dp)
   ;  return zsPop()
;  }


int opDimension
(  void
)
   {  mclMatrix* mx = zsGetOb(0, UTYPE_MX)
   ;  int r, c

   ;  if (!mx) return 0
   ;  r = mx->N_rows
   ;  c = mx->N_cols

   ;  if (r != c)
      {  zmTell('e', "dimensions [%dx%d] differ", r, c)
      ;  return 0
   ;  }

   ;  return zgPush(UTYPE_INT, &r)
;  }


int opAllOne
(  void
)
   {  const int  *kp    =  zsGetOb(1, UTYPE_INT)
   ;  const int  *lp    =  zsGetOb(0, UTYPE_INT)

   ;  if (!kp || lp)
      return 0

   ;  zsPop()
   ;  zsPop()
   ;  return zgPush(UTYPE_MX, mclMatrixComplete(*kp,*lp,1.0))
;  }


int opIdentity
(  void
)
   {  const int* ip  =  zsGetOb(0, UTYPE_INT)
   ;  mclMatrix* id

   ;  if (!ip)
      return 0

   ;  if (*ip<=0)
      {  zmTell
         (  'e'
         ,  "[%s] positive dimension please (got %d)\n"
         ,  TOKEN_IDENTITY
         ,  *ip
         )
      ;  return 0
   ;  }

   ;  id  =  mclMatrixIdentity(*ip)
   ;  zsPop()
   ;  return zgPush(UTYPE_MX, id)
;  }


opFunc opGetOpByToken
(  mcxTing* token
)
   {  mcxKV* kv = mcxHashSearch(token, symtable_g, MCX_DATUM_FIND)

   ;  if (kv)
      {  opHook   *hook =  (opHook*) kv->val
      ;  return hook->op_op
   ;  }
      return NULL
;  }


int opTell
(  void
)
   {  const int* ip =   zsGetOb(0, UTYPE_INT)
   ;  int   i       =   ip ? *ip : 0
   ;  if (!ip) return 0

   ;  zsPop()
   ;  return zsList(i)
;  }


int opInfo
(  void
)
   {  return zsList(1)
;  }


int opStackMDup
(  void
)
   {  int *ip = zsGetOb(0, UTYPE_INT)
   ;  if (!ip) return 0

   ;  zsPop()
   ;  return zgMDup(*ip)
;  }


int opStackClear
(  void
)
   {  return zsClear()
;  }


int opStackExch
(  void
)
   {  return zsExch()
;  }


int opStackList
(  void
)
   {  return zsList(0)
;  }


int opQuit
(  void
)
   {  /* hierverder? walk stack and free matrices etc */
      exit(0)
;  }


int opTut
(  void
)
   {  fprintf(stdout, opHelpTxt)
   ;  return 1
;  }


int opVars
(  void
)
   {  return zgVars()
;  }


int opStackPop
(  void
)
   {  return zsPop()
;  }


int opUnlink
(  void
)
   {  return zgUnlink()
;  }


int opDef
(  void
)
   {  return zgDef()
;  }

