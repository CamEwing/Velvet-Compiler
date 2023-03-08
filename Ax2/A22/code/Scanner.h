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

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: _ ) */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token ( '(' ) */
	RPR_T,		/*  4: Right parenthesis token ( ')' ) */
	LBR_T,		/*  5: Left brace token ( '{' ) */
	RBR_T,		/*  6: Right brace token ( '}' ) */
	KEY_T,		/*  7: Keyword token */
	EOS_T,		/*  8: End of statement ( ';' ) */
	ENID_T,		/*  9: Variable name identifier for entero & decimal (start: #) */
	CNID_T,		/* 10: Variable name identifier for chain (start: &) */
	RTE_T,		/* 11: Run-time error token */
	INL_T,		/* 12: Integer literal token */
	SEOF_T,		/* 13: Source end-of-file token */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
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

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	byte flags;			/* Flags information */
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

/* TO_DO: Define lexeme FIXED classes */
/* BAR(0), HAS(1), AMP(2), UND(3), -WHT(4), -NEW(5), -ABC(6), -NUM(7), POI(8), QUO(9), -OTH(10) */
/* These constants will be used on nextClass */
//#define CHRCOL2 '_'
//#define CHRCOL3 '&'
//#define CHRCOL4 '\'

#define CHRCOL2 '/'
#define CHRCOL3 '#'
#define CHRCOL4 '&'
#define CHRCOL5 '_'
#define CHRCOL6 '.'		//change to col 8 or change TT
#define CHRCOL7 '\"'	//change to col 9 or change TT

/* These constants will be used on VID / MID function */
#define MNIDPREFIX '_'
#define ENIDPREFIX '#'
#define CNIDPREFIX '&'

/* TO_DO: Error states and illegal state */
#define FS		100		/* Illegal state */
#define ESNR	6		/* Error state with no retract */
#define ESWR	7		/* Error state with retract */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 11

/* TO_DO: Transition table - type of states defined in separate table */
//Added
#define ES 5 //temp empty state

static entero transitionTable[][TABLE_COLUMNS] = {
	/*	  /,      #,	  &,      _,    " ",     \n,  [A-z],  [0-9],      .,      ",   other,
	 BAR(0), HAS(1), AMP(2), UND(3), WHT(4), NEW(5), ABC(6), NUM(7), POI(8), QUO(9), OTH(10),  */
	{     1,      4,      6,      8,     ES,     ES,     16,     10,     ES,     14,     ES}, // S0 - NOFS
	{     1,     ES,     ES,     ES,     ES,     ES,     ES,     ES,     ES,     ES,     ES}, // S1 - NOFS
	{     2,      2,      2,      2,      2,      3,      2,      2,      2,      2,      2}, // S2	- NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S3 - FSNR
	{     5,      5,      5,      5,      5,      5,      4,      4,      5,      5,      5}, // S4 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S5 - FSWR
	{     7,      7,      7,      7,      7,      7,      6,      6,      7,      7,      7}, // S6 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S7 - FSWR
	{     9,      9,      9,      9,      9,      9,      8,      8,      9,      9,      9}, // S8 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S9 - FSWR
	{    11,     11,     11,     11,     11,     11,     11,     10,     12,     11,     11}, // S10 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S11 - FSWR
	{    13,     13,     13,     13,     13,     13,     13,     12,     13,     13,     13}, // S12 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S13 - FSWR
	{    ES,     ES,     ES,     ES,     14,     ES,     14,     14,     ES,     15,     ES}, // S14 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}, // S15 - FSNR
	{    17,     17,     17,     17,     17,     17,     16,     17,     17,     17,     17}, // S16 - NOFS
	{    FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS,     FS}  // S17 - FSWR
};

//static entero transitionTable[][TABLE_COLUMNS] = {
//	/*[A-z], [0-9],    _,    &,    ', SEOF, other
//	   L(0),  D(1), U(2), M(3), Q(4), E(5),  O(6) */
//	{     1,  ESNR, ESNR, ESNR,    4, ESWR, ESNR}, // S0: NOAS
//	{     1,     1,    1,    2, ESWR, ESWR,    3}, // S1: NOAS
//	{    FS,    FS,   FS,   FS,   FS,   FS,   FS}, // S2: ASNR (MVID)
//	{    FS,    FS,   FS,   FS,   FS,   FS,   FS}, // S3: ASWR (KEY)
//	{     4,     4,    4,    4,    5, ESWR,    4}, // S4: NOAS
//	{    FS,    FS,   FS,   FS,   FS,   FS,   FS}, // S5: ASNR (SL)
//	{    FS,    FS,   FS,   FS,   FS,   FS,   FS}, // S6: ASNR (ES)
//	{    FS,    FS,   FS,   FS,   FS,   FS,   FS}  // S7: ASWR (ER)
//};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static entero stateType[] = {

	NOFS, // S0
	NOFS, // S1
	NOFS, // S2
	FSNR, // S3
	NOFS, // S4
	FSWR, // S5
	NOFS, // S6
	FSWR, // S7
	NOFS, // S8
	FSWR, // S9
	NOFS, // S10
	FSWR, // S11
	NOFS, // S12
	FSWR, // S13
	NOFS, // S14
	FSNR, // S15
	NOFS, // S16
	FSWR  // S17

	//NOFS, /* 00 */
	//NOFS, /* 01 */
	//FSNR, /* 02 (MID) - Methods */
	//FSWR, /* 03 (KEY) */
	//NOFS, /* 04 */
	//FSNR, /* 05 (SL) */
	//FSNR, /* 06 (Err1 - no retract) */
	//FSWR  /* 07 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
entero startScanner(ReaderPointer psc_buf);
Token tokenizer(void);
static entero nextClass(char c);				/* character class function */
static entero nextState(entero, char);		/* state machine function */
void printToken(Token t);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(char* lexeme);

/* Declare accepting states functions */
Token funcSL	(char lexeme[]);
Token funcID	(char lexeme[]);
Token funcKEY	(char lexeme[]);
Token funcErr	(char lexeme[]);
Token funcIL	(char lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] - Methods */
	funcKEY,	/* KEY  [03] - Keywords */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] - String Literal */
	funcErr,	/* ERR1 [06] - No retract */
	funcErr		/* ERR2 [07] - Retract */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 13

/* TO_DO: Define the list of keywords */
static char* keywordTable[KWT_SIZE] = {
	"method",
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
	"input"
};



/* Number of errors */
entero numScannerErrors;

#endif
