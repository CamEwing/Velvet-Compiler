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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
 /* TO_DO: This is the function to start the parser - check your program definition */

void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 /* TO_DO: This is the main code for match - check your definition */
void matchToken(entero tokenCode, entero tokenAttribute) {
	entero matchFlag = 1;
	switch (lookahead.code) {
	case KEY_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
 /* TO_DO: This is the function to handler error - adjust basically datatypes */
void syncErrorHandler(entero syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
 /* TO_DO: This is the function to error printing - adjust basically datatypes */
void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case RTE_T:
		//numScannerErrors++;
		printf("RTE_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case ERR_T:
		numScannerErrors++;
		printf("*ERROR*\t\t%s\n", t.attribute.errLexeme);
		/* Update numScannerErrors */
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case CHN_T:
		printf("CHN_T\t\t%d\t ", (entero)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (entero)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KEY_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case COMA_T:
		printf("COMA_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	case ENID_T:
		printf("ENID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case CNID_T:
		printf("CNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case ENL_T:
		printf("ENL_T\t\t%d\t\n", (entero)t.attribute.codeType);
		break;
	case DECI_T:
		printf("DECI_T\t\t%f\t\n", (decimal)t.attribute.decimalValue);
		break;

	case ART_OP_T:
		switch (t.attribute.arithmeticOperator) {
		case 0:
			printf("OP_ADD\n");
			break;
		case 1:
			printf("OP_SUB\n");
			break;
		case 2:
			printf("OP_MUL\n");
			break;
		case 3:
			printf("OP_DIV\n");
			break;
		}
		break;

	case EQ_T:
		printf("EQ_T\n");
		break;

	case REL_OP_T:
		switch (t.attribute.relationalOperator) {
		case 0:
			printf("OP_EQ\n");
			break;
		case 1:
			printf("OP_GT\n");
			break;
		case 2:
			printf("OP_LT\n");
			break;
		}
		break;
	//case ERR_T:
	//	printf("*ERROR*: %s\n", t.attribute.errLexeme);
	//	break;
	//case SEOF_T:
	//	printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
	//	break;
	//case MNID_T:
	//	printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
	//	break;
	//case CHN_T:
	//	printf("CHN_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
	//	break;
	//case KEY_T:
	//	printf("KEY_T: %s\n", keywordTable[t.attribute.codeType]);
	//	break;
	//case LPR_T:
	//	printf("LPR_T\n");
	//	break;
	//case RPR_T:
	//	printf("RPR_T\n");
	//	break;
	//case LBR_T:
	//	printf("LBR_T\n");
	//	break;
	//case RBR_T:
	//	printf("RBR_T\n");
	//	break;
	//case EOS_T:
	//	printf("NA\n");
	//	break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {MNID_T (main&)}.
 ***********************************************************
 */
void program() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
			matchToken(LPR_T, NO_ATTR);
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);

			while(lookahead.code != RBR_T) {
				opt_data_declarations();
				optionalStatements();
			}

			matchToken(RBR_T, NO_ATTR);
			//matchToken(MNID_T, NO_ATTR);
			//matchToken(LBR_T, NO_ATTR);
			//dataSession();
			//codeSession();
			//matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KEY_T (KW_data)}.
 ***********************************************************
 */
//void dataSession() {
//	matchToken(KEY_T, KW_data);
//	matchToken(LBR_T, NO_ATTR);
//	optVarListDeclarations();
//	matchToken(RBR_T, NO_ATTR);
//	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
//}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KEY_T (KW_int), KEY_T (KW_real), KEY_T (KW_string)}.
 ***********************************************************
 */
void opt_data_declarations() {
	switch (lookahead.code) {
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KEY_T (KW_code)}.
 ***********************************************************
 */
//void codeSession() {
//	matchToken(KEY_T, KW_code);
//	matchToken(LBR_T, NO_ATTR);
//	optionalStatements();
//	matchToken(RBR_T, NO_ATTR);
//	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
//}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KEY_T(KW_if),
 *				KEY_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
void optionalStatements() {
	switch (lookahead.code) {
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			statements();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KEY_T(KW_if),
 *		KEY_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KEY_T(KW_if), KEY_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KEY_T(KW_if), KEY_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statement() {
	switch (lookahead.code) {
	case KEY_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
void outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
void outputVariableList() {
	switch (lookahead.code) {
	case CHN_T:
		matchToken(CHN_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}