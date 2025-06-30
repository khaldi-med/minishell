#!/bin/bash

# Comprehensive test script for minishell vs bash comparison
MINISHELL="./minishell"
TEST_FILE="test.txt"
RESULTS_DIR="test_results"
BASH_DIR="$RESULTS_DIR/bash"
MINISHELL_DIR="$RESULTS_DIR/minishell"
COMPARISON_LOG="$RESULTS_DIR/comparison_analysis.log"

# Create directories
mkdir -p "$BASH_DIR" "$MINISHELL_DIR"

# Clear previous results
> "$COMPARISON_LOG"

echo "Starting comprehensive test comparison..."
echo "Results will be saved in: $RESULTS_DIR"

# Function to clean command (remove line numbers)
clean_command() {
    echo "$1" | sed 's/^[0-9]*|//'
}

# Function to check if command should be skipped
should_skip() {
    local cmd="$1"
    # Skip interactive commands, sleep with manual intervention, etc.
    if [[ "$cmd" =~ (Ctlr-|sleep.*faire|cat.*faire|history|\[touche|chmod.*minishell|env.*minishell|\.\/minishell) ]]; then
        return 0
    fi
    return 1
}

# Process each line from test.txt
line_num=0
while IFS= read -r line; do
    ((line_num++))
    
    if [[ -z "$line" ]]; then
        continue
    fi
    
    clean_cmd=$(clean_command "$line")
    
    if should_skip "$clean_cmd"; then
        echo "Line $line_num: SKIPPED (interactive/special command)" | tee -a "$COMPARISON_LOG"
        continue
    fi
    
    echo "Testing line $line_num: $clean_cmd"
    
    # Test with bash
    bash_output_file="$BASH_DIR/line_${line_num}.out"
    bash_error_file="$BASH_DIR/line_${line_num}.err"
    
    timeout 3s bash -c "$clean_cmd" > "$bash_output_file" 2> "$bash_error_file"
    bash_exit_code=$?
    
    # Test with minishell
    minishell_output_file="$MINISHELL_DIR/line_${line_num}.out"
    minishell_error_file="$MINISHELL_DIR/line_${line_num}.err"
    
    echo "$clean_cmd" | timeout 3s "$MINISHELL" > "$minishell_output_file" 2> "$minishell_error_file"
    minishell_exit_code=$?
    
    # Quick comparison
    if ! diff -q "$bash_output_file" "$minishell_output_file" >/dev/null 2>&1; then
        echo "DIFF: Line $line_num: $clean_cmd" >> "$COMPARISON_LOG"
    fi
    
    # Store exit codes for comparison
    echo "$bash_exit_code" > "$BASH_DIR/line_${line_num}.exit"
    echo "$minishell_exit_code" > "$MINISHELL_DIR/line_${line_num}.exit"
    
done < "$TEST_FILE"

echo "Test execution complete!"
echo "Results saved in $RESULTS_DIR/"
echo "Check $COMPARISON_LOG for differences summary."
