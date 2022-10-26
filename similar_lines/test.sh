#!/bin/bash

# get user input into variables
prog=${1}
dir=${2}


# some colours for later echos
L_RED='\033[1;31m'
L_GREEN='\033[1;32m'
BROWN='\033[0;33m'
NO_COLOUR='\033[0m'

# loop over files in given directory
for FILE in $dir/*	
do
	# if file has .in extension
	if [ ${FILE: -3} == ".in" ]
	then
		# define temporary files
		tmp_out='test.out'
		tmp_err='test.err'
		tmp_valgrind='test.mem'

		# create temporary files
		touch "$tmp_out"
		touch "$tmp_err"
		touch "$tmp_valgrind"
		
		# echo name of test to be run
		echo -e "\n${NO_COLOUR}${FILE##*/}... "
		
		# echo lines to be filled during testing
		echo -e "${BROWN}out:\nerr:\nvalgrind:${NO_COLOUR}"

		# run test using given prog
		"$prog" <"$FILE" >"$tmp_out" 2> "$tmp_err"
		
		# variable to store information whether a test
		# was passed ("1") or failed ("0")
		test_passed="1"
		
		#line = first line where difference occurs
		#between FILE.out and test.out
		#if there is, echo it's location in file
		line=$(cmp "${FILE%in}out" "$tmp_out")
		if [ ! -z "$line" ]
		then
			echo -e "\e[3A\r\e[0K${BROWN}out:\t  ${L_RED}WRONG ANSWER!  ${BROWN}1st error:${NO_COLOUR}${line/*:}\n\n"
			test_passed="0"
		else
			echo -e "\e[3A\r\e[0K${BROWN}out:\t  ${L_GREEN}OK!\n\n"
		fi
		
		echo -e -n "${NO_COLOUR}"
		
		#line = first line where difference occurs
		#between FILE.err and test.err
		#if there is, echo it's location in file
		line=$(cmp "${FILE%in}err" "$tmp_err")
		if [ ! -z "$line" ]
		then
			echo -e "\e[2A\r\e[0K${BROWN}err:\t  ${L_RED}WRONG ANSWER!  ${BROWN}1st error:${NO_COLOUR}${line/*:}\n"
			test_passed="0"
		else
			echo -e "\e[2A\r\e[0K${BROWN}err:\t  ${L_GREEN}OK!\n"
		fi
		
		echo -e -n "${NO_COLOUR}"
		
		# run test using valgrind
		valgrind --error-exitcode=123 --leak-check=full --errors-for-leak-kinds=all ./bin/similar_lines <"$FILE" >"$tmp_valgrind" 2>&1
		
		# operations to get number of
		# errors from full valgrind log
		tmp=$( tail -n 1 "$tmp_valgrind")
		valgrind_end=${tmp#*SUMMARY: }
		tmp=${valgrind_end% contexts*}
		valgrind_end=${tmp% from*}
		
		#if there are any errors from valgrind,
		#give information about number of errors
		if [[ $valgrind_end == 0* ]]
		then
			echo -e "\e[1A\r\e[0K${BROWN}valgrind: ${L_GREEN}OK!"
		else
			echo -e "\e[1A\r\e[0K${BROWN}valgrind: ${L_RED}${valgrind_end}!"
			test_passed="0"
		fi
		
		# check if all files were matching
		if [[ $test_passed == "0" ]]
		then
			echo -e "${L_RED}\nTEST FAILED!${NO_COLOUR}\n"
			rm "$tmp_out" "$tmp_err" "$tmp_valgrind"
			exit;
		fi
		
		echo -e -n "${NO_COLOUR}"
		
		#remove temporary files
		rm "$tmp_out"
		rm "$tmp_err"
		rm "$tmp_valgrind"
	fi
	
done

echo -e "\n${L_GREEN}ALL TESTS PASSED!${NO_COLOUR}\n"
