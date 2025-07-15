#!/bin/bash

# Create test files
echo "hello" > file1.txt
echo "world" >> file1.txt
echo "test" >> file1.txt
echo "hello" > file2.txt
echo "grep" >> file2.txt
echo "world" >> file2.txt
echo "hello world" > file3.txt

# Test cases
test_cases=(
    "hello file1.txt"
    "hello file1.txt file2.txt"
    "-i HELLO file1.txt"
    "-v hello file1.txt"
    "-c hello file1.txt"
    "-l hello file1.txt file2.txt"
    "-n hello file1.txt"
    "-e hello file1.txt"
    "-i -v hello file1.txt"
    "-iv hello file1.txt"
    "-n -i HELLO file1.txt"
    "-ni HELLO file1.txt"
    "-v [a-z] file1.txt"
    "-c hello file1.txt file2.txt"
    "-l hello file1.txt file2.txt"
    "world file2.txt"
    "-i WORLD file2.txt"
    "-v grep file2.txt"
    "-c grep file2.txt"
    "-l grep file1.txt file2.txt"
    "-n grep file2.txt"
    "-e grep file2.txt"
    "'hello world' file3.txt"
    "-i 'HELLO WORLD' file3.txt"
    "-v 'test' file3.txt"
    "-c 'hello world' file3.txt"
    "-l 'hello world' file1.txt file3.txt"
    "-n 'hello world' file3.txt"
)

errors=(
    "pattern non_existent_file.txt"
    "-e pattern non_existent_file.txt"
    "hello file1.txt no_file.txt"
)

# Run tests
passed=0
total=0

for test_case in "${test_cases[@]}"; do
    echo "Test: grep $test_case"
    ./s21_grep $test_case > my_output.txt 2> my_error.txt
    grep $test_case > real_output.txt 2> real_error.txt
    sed -i 's/s21_grep/grep/g' my_error.txt
    diff my_output.txt real_output.txt
    if [ $? -eq 0 ]; then
        diff my_error.txt real_error.txt
        if [ $? -eq 0 ]; then
            echo "Passed"
            ((passed++))
        else
            echo "Errors differ"
        fi
    else
        echo "Outputs differ"
    fi
    ((total++))
    echo
done

# Error tests
for test_case in "${errors[@]}"; do
    echo "Test: grep $test_case"
    ./s21_grep $test_case > my_output.txt 2> my_error.txt
    grep $test_case > real_output.txt 2> real_error.txt
    sed -i 's/s21_grep/grep/g' my_error.txt
    diff my_output.txt real_output.txt
    if [ $? -eq 0 ]; then
        diff my_error.txt real_error.txt
        if [ $? -eq 0 ]; then
            echo "Passed"
            ((passed++))
        else
            echo "Errors differ"
        fi
    else
        echo "Outputs differ"
    fi
    ((total++))
    echo
done

# Cleanup
rm file1.txt file2.txt file3.txt my_output.txt my_error.txt real_output.txt real_error.txt

echo "Passed $passed of $total tests"