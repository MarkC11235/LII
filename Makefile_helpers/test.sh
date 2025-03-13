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
    # Get the base name without extension
    base_name="${i%.*}"
    
    # Step 1: Compile .cl to .cl_exe
    ./liic $i $3 $4
    
    # Check if compilation succeeded
    if [ ! -f "${base_name}.cl_exe" ]; then
        echo -e "\033[0;31mCompilation failed for $i\033[0m"
        fail_count=$((fail_count + 1))
        continue
    fi
    
    # Step 2: Run the compiled file through VM
    ./liivm "${base_name}.cl_exe" > "${i}.temp"
    
    # Compare output with expected
    if diff -b -w "${i}.temp" "${i}.out" > /dev/null; then
        success_count=$((success_count + 1))
    else
        echo -e "\033[0;31mTest $i Failed\033[0m"
        diff -b -w "${i}.temp" "${i}.out"
        echo "-----------------------------------"
        fail_count=$((fail_count + 1))
    fi
    
    # Cleanup the temporary executable
    rm -f "${base_name}.cl_exe"
    
    current_test=$((current_test + 1))
    show_progress
done

echo -e "\033[0;32mTotal Successes: $success_count\033[0m"
echo -e "\033[0;31mTotal Failures: $fail_count\033[0m"