/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: Megan Clinch 041043369, Cameron Ewing 041037946
* Professors: Paulo Sousa
************************************************************

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@                                                               @@@@
@@@																	@@@
@@																	 @@
@	  __   __   ______     __         __   __   ______     ______	  @
@    /\ \ / /  /\  ___\   /\ \       /\ \ / /  /\  ___\   /\__  _\    @
@	 \ \ \'/   \ \  __\   \ \ \____  \ \ \'/   \ \  __\   \/_/\ \/    @
@	  \ \__|    \ \_____\  \ \_____\  \ \__|    \ \_____\    \ \_\    @
@  	   \/_/      \/_____/   \/_____/   \/_/      \/_____/     \/_/    @
@																	  @
@                                                                     @
@           A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W           @
@@																     @@
@@@					M E G A N   &   C A M E R O N					@@@
@@@@															   @@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Sep 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/* #pragma warning(1:4001) */			/*to enforce C89 type comments  - to make //comments an warning */

/* #pragma warning(error:4001) */		/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: _ ) */
	CHN_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token ( '(' ) */
	RPR_T,		/*  4: Right parenthesis token ( ')' ) */
	LBR_T,		/*  5: Left brace token ( '{' ) */
	RBR_T,		/*  6: Right brace token ( '}' ) */
	KEY_T,		/*  7: Keyword token */
	EOS_T,		/*  8: End of statement ( ';' ) */
	ENID_T,		/*  9: Variable name identifier for entero & decimal (start: #) */
	CNID_T,		/* 10: Variable name identifier for chain (start: &) */
	DECI_T,		/* 22: Floating point token contains ( '.' ) */
	RTE_T,		/* 11: Run-time error token */
	ENL_T,		/* 12: Enteger literal token */
	SEOF_T,		/* 13: Source end-of-file token */
	ADD_T,		/* 14: Addition token ( '+' ) */
	SUB_T,		/* 15: Subtraction token ( '-' ) */
	MUL_T,		/* 16: Multiplication token ( '*' ) */
	DIV_T,		/* 17: Division token ( '/' ) */
	EQ_T,		/* 18: Equal token ( '=' ) */
	GT_T,		/* 19: Greater-than token ( '>' ) */
	LT_T,		/* 20: Less-than token ( '<' ) */
	COM_T,		/* 21: Comment token ( '/' ) */
	COMA_T		/* 25: Comma token ( ',' ) */

// Make single operator for ArithmeticOperators, RelationalOperators
// and use currentToken.attribute.arithmeticOperator to differentiate in tokenizer
};

/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	entero codeType;					/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	entero entValue;					/* integer literal attribute (value) - PREVIOUSLY intValue */
	entero keywordIndex;				/* keyword index in the keyword table */
	entero chainContent;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) - PREVIOUSLY contentString */
	decimal decimalValue;				/* floating-point literal attribute (value) - PREVIOUSLY floatValue */
	char idLexeme[VID_LEN + 1];			/* variable identifier token attribute */
	char errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	byte flags;							/* Flags information */
	union {
		entero entValue;				/* Integer value - PREVIOUSLY intValue */
		decimal decimalValue;			/* Float value - PREVIOUSLY floatValue */
		char* chainContent;				/* String value - PREVIOUSLY stringContent */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	entero code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .&., .|. , .!. , SEOF.
 */

/* Define lexeme FIXED classes */
/* BAR(0), HAS(1), AMP(2), UND(3), -WHT(4), -NEW(5), -ABC(6), -NUM(7), POI(8), QUO(9), -OTH(10) */
/* These constants will be used on nextClass */
#define CHRCOL1 '#'
#define CHRCOL2 '&'
#define CHRCOL3 '_'
#define CHRCOL4 ' '
#define CHRCOL8 '.'	
#define CHRCOL9 '\"'

/* These constants will be used on VID / MID function */
#define MNIDPREFIX '_'
#define ENIDPREFIX '#'
#define CNIDPREFIX '&'

/* Error states and illegal state */
#define FS		100		/* Illegal state */
#define ESNR	6		/* Error state with no retract */
#define ESWR	7		/* Error state with retract */

 /* State transition table definition */
#define TABLE_COLUMNS 11

/* Transition table - type of states defined in separate table */

static entero transitionTable[][TABLE_COLUMNS] = {
	/* SEOF,      #,	  &,      _,    " ",     \n,  [A-z],  [0-9],      .,      ",   other,
	 EOF(0), HAS(1), AMP(2), UND(3), WHT(4), NEW(5), ABC(6), NUM(7), POI(8), QUO(9), OTH(10),  */
	{  ESWR,      1,      3,      5,   ESNR,   ESNR,     13,      7,   ESNR,     11,   ESNR}, // S0  - NOFS
	{  ESWR,      2,      2,      2,      2,      2,      1,      1,      2,      2,      2}, // S1  - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S2  - FSWR
	{  ESWR,      4,      4,      4,      4,      4,      3,      3,      4,      4,      4}, // S3  - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S4  - FSWR
	{  ESWR,      6,      6,      6,      6,      6,      5,      5,      6,      6,      6}, // S5  - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S6  - FSWR
	{  ESWR,      8,      8,      8,      8,      8,      8,      7,      9,      8,      8}, // S7  - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S8  - FSWR
	{  ESWR,     10,     10,     10,     10,     10,     10,      9,     10,     10,     10}, // S9  - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S10 - FSWR
	{  ESWR,   ESNR,   ESNR,   ESNR,     11,   ESNR,     11,     11,   ESNR,     12,   ESNR}, // S11 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S12 - FSNR
	{  ESWR,     14,     14,     14,     14,     14,     13,     14,     14,     14,     14}, // S13 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S14 - FSWR
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S15 - FSNR
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}  // S16 - FSWR
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* Define list of acceptable states */
static entero stateType[] = {
	NOFS, // S0
	NOFS, // S1
	FSWR, // S2
	NOFS, // S3
	FSWR, // S4
	NOFS, // S5
	FSWR, // S6
	NOFS, // S7
	FSWR, // S8
	NOFS, // S9
	FSWR, // S10
	NOFS, // S11
	FSNR, // S12
	NOFS, // S13
	FSWR, // S14
	FSNR, // S15
	FSWR  // S16
};

/*
-------------------------------------------------
Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
entero startScanner(ReaderPointer psc_buf);
Token tokenizer(void);
static entero nextClass(char c);				/* character class function */
static entero nextState(entero, char);			/* state machine function */
void printToken(Token t);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(char* lexeme);

/* Declare accepting states functions */
Token funcSL	(char lexeme[]);
Token funcID	(char lexeme[]);
Token funcKEY	(char lexeme[]);
Token funcErr	(char lexeme[]);
Token funcIL	(char lexeme[]);
Token funcDL	(char lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -     [00] */
	NULL,		/* -     [01] */
	funcID,		/* ENID	 [02] - Numerical Variable ID */
	NULL,		/* -	 [03] */
	funcID,		/* CNID  [04] - Chain Variable ID*/
	NULL,		/* -     [05]  */
	funcID,		/* MNID  [06] - Method Name ID */
	NULL,		/* -     [07] */
	funcIL,		/* ENL_T [08] - Integer Literal */
	NULL,		/* -	 [09] */
	funcDL,		/* DECI_T[10] - Decimal Literal */
	NULL,		/* -     [11] */
	funcSL,		/* CHN_T [12] - String Literal */
	NULL,		/* -     [13]  */
	funcKEY,	/* KEY_T [14] - Keywords */
	funcErr,	/* ERR1	 [15] - Error No Return */
	funcErr		/* ERR2	 [16] - Error With Return */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of Keywords from the language */
#define KWT_SIZE 15

/* Define the list of keywords */
static char* keywordTable[KWT_SIZE] = {
	"ent",
	"decimal",
	"chain",
	"if",
	"elseif",
	"else",
	"for",
	"true",
	"false",
	"send",
	"print",
	"input",
	"when",
	"AND",
	"OR"
};

/* Number of errors */
entero numScannerErrors;

#endif
