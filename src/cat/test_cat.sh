#!/bin/bash

# Создаем тестовые файлы
echo -e "Line 1\n\nLine 3" > file1.txt
echo -e "Line 1\tLine 2" > file2.txt
echo -e "Line 1\n\n\nLine 4" > file3.txt
echo -e "Test\0char" > file4.bin

# Тестовые случаи
test_cases=(
    ""  # Без флагов
    "-b"
    "-e"
    "-E"
    "-n"
    "-s"
    "-t"
    "-T"
    "-v"
    "-be"
    "-bn"
    "-bs"
    "-bt"
    "-bv"
    "-en"
    "-es"
    "-et"
    "-ev"
    "-tn"
    "-ts"
    "-tv"
    "--number-nonblank"
    "--number"
    "--squeeze-blank"
)

# Запускаем тесты
passed=0
total=0

for test_case in "${test_cases[@]}"; do
    echo "Test: cat $test_case"
    
    # Тестируем на текстовых файлах
    for file in file1.txt file2.txt file3.txt; do
        ./s21_cat $test_case $file > my_output.txt
        cat $test_case $file > real_output.txt
        diff my_output.txt real_output.txt
        if [ $? -eq 0 ]; then
            echo "$file: Passed"
            ((passed++))
        else
            echo "$file: Failed"
        fi
        ((total++))
    done
    
    # Тестируем на бинарном файле
    ./s21_cat $test_case file4.bin > my_output.bin
    cat $test_case file4.bin > real_output.bin
    diff my_output.bin real_output.bin
    if [ $? -eq 0 ]; then
        echo "file4.bin: Passed"
        ((passed++))
    else
        echo "file4.bin: Failed"
    fi
    ((total++))
    
    echo
done

# Тест на несуществующий файл
echo "Test: non-existent file"
./s21_cat no_file.txt 2> my_error.txt
cat no_file.txt 2> real_error.txt
sed -i 's/s21_cat/cat/g' my_error.txt
diff my_error.txt real_error.txt
if [ $? -eq 0 ]; then
    echo "Error handling: Passed"
    ((passed++))
else
    echo "Error handling: Failed"
fi
((total++))

# Удаляем временные файлы
rm file1.txt file2.txt file3.txt file4.bin my_output.txt real_output.txt my_output.bin real_output.bin my_error.txt real_error.txt

echo "Passed $passed of $total tests"