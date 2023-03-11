# ************************************************************
# * COMPILERS COURSE - Algonquin College
# * Code version: Winter, 2023
# * Author: Megan Clinch 041043369, Cameron Ewing 041037946
# * Professors: Paulo Sousa
# ************************************************************
# 
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# @@@@                                                               @@@@
# @@@																  @@@
# @@																   @@
# @	  __   __   ______     __         __   __   ______     ______	    @
# @  /\ \ / /  /\  ___\   /\ \       /\ \ / /  /\  ___\   /\__  _\      @
# @	 \ \ \'/   \ \  __\   \ \ \____  \ \ \'/   \ \  __\   \/_/\ \/      @
# @	  \ \__|    \ \_____\  \ \_____\  \ \__|    \ \_____\    \ \_\      @
# @	   \/_/      \/_____/   \/_____/   \/_/      \/_____/     \/_/      @
# @																	    @
# @                                                                     @
# @           A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W           @
# @@																   @@
# @@@					M E G A N   &   C A M E R O N			      @@@
# @@@@															     @@@@
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# 

cmake_minimum_required (VERSION 3.8)

project ("VelvetCompiler")

# Add source to this project's executable: check name and dependences
add_executable (Velvet
	"Compilers.h"	"Compilers.c"
	"Reader.h"		"Reader.c"		"MainReader.c"
	"Scanner.h"		"Scanner.c"		"MainScanner.c"
#	"Parser.h"		"Parser.c"		"MainParser.c"
)

# Add tests and install targets if needed.
