# for i in $(find $2 -type f -name '*.cl'); do 
# 	echo "Running test $i"; 
# 	$1 $i > ${i}.temp; 
# 	diff -b -w ${i}.temp ${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; 
# 	echo "-----------------------------------"; 
# done

########################################################

# success_count=0
# fail_count=0

# for i in $(find $2 -type f -name '*.cl'); do 
#     $1 $i $3 > ${i}.temp; 
#     if diff -b -w ${i}.temp ${i}.out > /dev/null; then
#         success_count=$((success_count + 1))
#     else
#         echo "\033[0;31mTest $i Failed\033[0m"
#         diff -b -w ${i}.temp ${i}.out
#         echo "-----------------------------------"; 
#         fail_count=$((fail_count + 1))
#     fi
# done

# echo "\033[0;32mTotal Successes: $success_count\033[0m"
# echo "\033[0;31mTotal Failures: $fail_count\033[0m"

########################################################

success_count=0
fail_count=0

# Find all .cl files and count them
test_files=$(find $2 -type f -name '*.cl')
total_tests=$(echo "$test_files" | wc -l)
current_test=0

# Function to display progress bar
show_progress() {
    progress=$((current_test * 100 / total_tests))
    echo -n "Progress: $progress% \r"
    if [ $current_test -eq $total_tests ]; then
        echo
    fi
}

for i in $test_files; do 
    $1 $i $3 $4 > ${i}.temp; 
    if diff -b -w ${i}.temp ${i}.out > /dev/null; then
        success_count=$((success_count + 1))
    else
        echo -e "\033[0;31mTest $i Failed\033[0m"
        diff -b -w ${i}.temp ${i}.out
        echo "-----------------------------------"; 
        fail_count=$((fail_count + 1))
    fi
    current_test=$((current_test + 1))
    show_progress
done

echo "\033[0;32mTotal Successes: $success_count\033[0m"
echo "\033[0;31mTotal Failures: $fail_count\033[0m"