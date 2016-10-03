#!/bin/bash

# Check if theres a  variable
if [ -z "$1" ]
then
	echo "[ERROR] NOT SPECIFIED EXECUTABLE FILES!"
	exit 1
fi

if [ -z "$2" ]
then
    echo "[ERROR] NOT SPECIFIED FILE FOR TEST!"
    exit 1
fi

# Check if file exists and can be executed
if [ -e "$1" ] && [ -s "$1" ] && [ -x "$1" ]
then
	# Execute compiled code and save the result in a test file
	"./$1" "$2" > /dev/null 2>&1
else
	echo "[ERROR] FILE NOT EXIST, IS EMPTY OR CAN'T BE EXECUTABLE!"
	exit 1
fi

OUPUT_FILE=$2.out

# Check if the other file exist
# Check if file exists and can be executed
if [ -e "$OUPUT_FILE" ]
then
	# File EXIST
	echo ">> TESTING $2 WITH BINARY $1" 
else
	echo "[ERROR] OUTPUT FILE NOT EXIST!"
	exit 1
fi

# Compara os resultados
DIFF=$(diff -q "$2.out" "$2.exp")

if [ "$DIFF" != "" ]
then
	if [ "$(command -v diffchecker)" ]
	then
		echo "Opening diff between the expected and output"
		echo "If it's slow please open test.sh and check the comment bellow"
		# Use the following command to make it more faster
		# It will disable open browser after create diff
		# cat "$(whereis diffchecker | cut -d ":" -f 2 | cut -d " " -f 2- | rev | cut -d "/" -f 2- | rev)/$(readlink -- $(whereis diffchecker) | rev | cut -d "/" -f 2- | rev)/transmit.js" | tr '\n' '\f' | sed -e 's/(0, _opener2.default)(url, function () {\f        process.exit();\f      });/\/\/(0, _opener2.default)(url, function () {\f        \/\/process.exit();\f      \/\/});/' | tr '\f' '\n' > transmit.js && sudo mv transmit.js "$(whereis diffchecker | cut -d ":" -f 2 | cut -d " " -f 2- | rev | cut -d "/" -f 2- | rev)/$(readlink -- $(whereis diffchecker) | rev | cut -d "/" -f 2- | rev)/transmit.js"
		diffchecker --expires day "$2.out" "$2.exp"
		#vimdiff "$2.out" "$2.exp"
	else
		#echo "vimdiff isn't installed"
		#echo "Install vimdiff to check what's wrong"
		echo "Install diffchecker with 'sudo npm install -g diffchecker' to check what's wrong"
	fi
    echo "[ERROR] RESULT IS DIFFERENT FROM WHAT IS EXPECTED"
    exit 1
else
    echo ">> COMPLETED! EVERYTHING IS CORRECT WITH $2 :)"	
    exit 0
fi