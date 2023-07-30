#!/bin/bash

end_scripttime=$(date)

calculator() { #function does math depending on what the user chooses, it goes into the function which holds multiple cases for each operator then returns the calculations requested by the user 
  local operand1=$1 #variable holds the value of the first operand
  local operator=$2 #variable holds the value of the operator
  local operand2=$3 #variable holds the value of the operand

  case $operator in
    +)
      echo "$((operand1 + operand2))" #does math if user enters +
      ;;
    -)
      echo "$((operand1 - operand2))" #does subtraction if user enters -
      ;;
    \*)
      echo "$((operand1 * operand2))" #does multiplication if user enters *
      ;;
    /)
	    if [[ $operand2 -eq 0 ]]; then #checks for division by 0 which is undefined
        echo "Warning: Division by zero is not possible. Try again."
        exit 1
      else
        echo "$((operand1 / operand2)) with a remainder of $((operand1 % operand2))" #does division if user enters / also returns the remainder
      fi
      ;;
    *)
      echo "Invalid operator used. The following operators are only valid: +, -, *, /"
      exit 1
      ;;
      esac
}

if [[ $# -eq 0 ]]; then #asks if user wants to do math interactively 
  echo "Do you wanna do math? (yes/no):"
  read response
  if [[ $response == "no" ]]; then #if yes program enters a loop until they don't want to keep calculating
    echo "Thanks for using the calculator. Try again anytime. This script ended at: $end_scripttime" #if first answer to calculation is 'no' then it sends the time program was last accessed 
  fi
  if [[ $response == "yes" ]]; then
    calculator_loop="yes"

    while [[ $calculator_loop == "yes" ]]; do #reads the operand1, operator, and operand2 one by one
      echo "Enter the first operand:"
      read operand1

      echo "Enter an operator (+, -, *, /):"
      read operator

      echo "Enter the second operand:"
      read operand2

      if [[ "$operator" != "+" && "$operator" != "-" && "$operator" != "*" && "$operator" != "/" ]]; then
        echo "Invalid operator used. The following operators are only valid: +, -, *, /"
        exit 1 #exits and prompts to try again if no valid operand is entered 
      fi

      result=$(calculator "$operand1" "$operator" "$operand2") #enters given operands and operators into the function and returns desired calculation 
      echo "$operand1 $operator $operand2 = $result"

      echo "Would you like to continue calculating? (yes/no):"
      read calculator_loop #checks if user wants to keep calculating 

          if [[ $calculator_loop == "no" ]]; then
            echo "Thanks for using the calculator. Try again anytime. This script ended at: $end_scripttime" #if user decides to end the loop it gives the last time the program was accessed. 
          fi
    done
  fi
else
  if [[ $# -ne 3 ]]; then
    echo "Invalid operator used. The following operators are only valid: +, -, *, /"
    exit 1 #checks for invalid operator
  fi

  operand1="$1"
  operator="$2"
  operand2="$3"

  if [[ "$operator" != "+" && "$operator" != "-" && "$operator" != "*" && "$operator" != "/" ]]; then
    echo "Invalid operator used. The following operators are only valid: +, -, *, /"
    exit 1
  fi

  result=$(calculator "$operand1" "$operator" "$operand2") #if math is done in the command line goes straight into the function and return with the 'echo' command below
  echo "$operand1 $operator $operand2 = $result" 

fi

