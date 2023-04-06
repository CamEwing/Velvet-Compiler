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
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
extern BufferReader* stringLiteralTable;
extern entero line;
extern Token tokenizer();
extern char* keywordTable[];
entero syntaxErrorNumber = 0;

#define STR_LANGNAME	"Velvet"
#define LANG_WRTE		"_print"
#define LANG_READ		"_input"
#define LANG_MAIN		"_main"

/* Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_ent,
	KW_decimal,
	KW_chain,
	KW_if,
	KW_elseif,
	KW_else,
	KW_for,
	KW_true,
	KW_false,
	KW_send,
	KW_print,
	KW_input,
	KW_when,
	KW_AND,
	KW_OR
};

/* Function definitions */
void startParser();
void matchToken(entero, entero);
void syncErrorHandler(entero);
void printError();

/* Place ALL non-terminal function declarations */
void program();
void variable_declaration();
void decision_maker();
void opt_code_statements();
void statements();
void statementsPrime();
void statement();
void selection_statement();
void iteration_statement();
void output_statement();
void opt_variable_list();
void arithmetic_expression();
void arithmetic_expressions();
void unary_arithmetic_expression();
void primary_arithmetic_expression();
void conditional_expression();
void logical_AND_expression();
void logical_OR_expression();
void relational_expression();
void relational_a_expression();
void relational_s_expression();
void primary_a_relational_expressions();
void primary_s_relational_expressions();

#endif
