#!/bin/bash

DIR=""

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

read -p "Enter input file: " DIR
DIR="$SCRIPT_DIR/$DIR"
INPUT_kiss="$DIR/input.kiss"
INPUT_dot="$DIR/input.dot"
OUTPUT_kiss="$DIR/output.kiss"
OUTPUT_dot="$DIR/output.dot"

#echo $INPUT_kiss
#echo $INPUT_dot

touch $INPUT_dot
touch $OUTPUT_dot

./project03 $INPUT_kiss $INPUT_dot
./project03 $OUTPUT_kiss $OUTPUT_dot

dot -T png $INPUT_dot > "$DIR/input.png"
dot -T png $OUTPUT_dot > "$DIR/output.png"
