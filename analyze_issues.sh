#!/bin/bash

# Detailed analysis of minishell vs bash differences
RESULTS_DIR="test_results"
BASH_DIR="$RESULTS_DIR/bash"
MINISHELL_DIR="$RESULTS_DIR/minishell"
ANALYSIS_FILE="$RESULTS_DIR/detailed_analysis.txt"

> "$ANALYSIS_FILE"

echo "=== MINISHELL vs BASH COMPARISON ANALYSIS ===" | tee -a "$ANALYSIS_FILE"
echo "Generated: $(date)" | tee -a "$ANALYSIS_FILE"
echo "" | tee -a "$ANALYSIS_FILE"

# Initialize counters
total_tests=0
prompt_issues=0
parsing_errors=0
builtin_issues=0
redirection_issues=0
pipe_issues=0
variable_expansion_issues=0
quote_handling_issues=0
exit_code_issues=0

echo "Analyzing test results..." | tee -a "$ANALYSIS_FILE"

# Function to analyze a single test
analyze_test() {
    local line_num="$1"
    local cmd_line
    
    # Read the original command
    cmd_line=$(sed -n "${line_num}p" test.txt | sed 's/^[0-9]*|//')
    
    if [[ -z "$cmd_line" ]]; then
        return
    fi
    
    ((total_tests++))
    
    local bash_out="$BASH_DIR/line_${line_num}.out"
    local minishell_out="$MINISHELL_DIR/line_${line_num}.out"
    local bash_err="$BASH_DIR/line_${line_num}.err"
    local minishell_err="$MINISHELL_DIR/line_${line_num}.err"
    local bash_exit="$BASH_DIR/line_${line_num}.exit"
    local minishell_exit="$MINISHELL_DIR/line_${line_num}.exit"
    
    # Check if files exist
    if [[ ! -f "$bash_out" || ! -f "$minishell_out" ]]; then
        return
    fi
    
    local has_diff=false
    
    # Check for prompt issues (minishell shows prompt in output)
    if grep -q "minishell\\$" "$minishell_out" 2>/dev/null; then
        ((prompt_issues++))
        has_diff=true
    fi
    
    # Check for parsing errors
    if grep -q "command not found\\|syntax error\\|parse error" "$minishell_err" 2>/dev/null; then
        if ! grep -q "command not found\\|syntax error\\|parse error" "$bash_err" 2>/dev/null; then
            ((parsing_errors++))
            has_diff=true
        fi
    fi
    
    # Categorize by command type
    if [[ "$cmd_line" =~ ^(echo|pwd|cd|env|export|unset|exit) ]]; then
        if ! diff -q "$bash_out" "$minishell_out" >/dev/null 2>&1; then
            ((builtin_issues++))
            has_diff=true
        fi
    elif [[ "$cmd_line" =~ [\>\<] ]]; then
        if ! diff -q "$bash_out" "$minishell_out" >/dev/null 2>&1; then
            ((redirection_issues++))
            has_diff=true
        fi
    elif [[ "$cmd_line" =~ \| ]]; then
        if ! diff -q "$bash_out" "$minishell_out" >/dev/null 2>&1; then
            ((pipe_issues++))
            has_diff=true
        fi
    elif [[ "$cmd_line" =~ \$ ]]; then
        if ! diff -q "$bash_out" "$minishell_out" >/dev/null 2>&1; then
            ((variable_expansion_issues++))
            has_diff=true
        fi
    elif [[ "$cmd_line" =~ [\'\"] ]]; then
        if ! diff -q "$bash_out" "$minishell_out" >/dev/null 2>&1; then
            ((quote_handling_issues++))
            has_diff=true
        fi
    fi
    
    # Check exit codes
    if [[ -f "$bash_exit" && -f "$minishell_exit" ]]; then
        local bash_exit_code=$(cat "$bash_exit")
        local minishell_exit_code=$(cat "$minishell_exit")
        if [[ "$bash_exit_code" != "$minishell_exit_code" ]]; then
            ((exit_code_issues++))
            has_diff=true
        fi
    fi
    
    if $has_diff; then
        echo "ISSUE Line $line_num: $cmd_line" >> "$ANALYSIS_FILE"
    fi
}

# Analyze all tests
for line_num in {1..635}; do
    analyze_test "$line_num"
done

echo "" | tee -a "$ANALYSIS_FILE"
echo "=== SUMMARY ===" | tee -a "$ANALYSIS_FILE"
echo "Total tests analyzed: $total_tests" | tee -a "$ANALYSIS_FILE"
echo "Tests with prompt in output: $prompt_issues" | tee -a "$ANALYSIS_FILE"
echo "Parsing error differences: $parsing_errors" | tee -a "$ANALYSIS_FILE"
echo "Builtin command issues: $builtin_issues" | tee -a "$ANALYSIS_FILE"
echo "Redirection issues: $redirection_issues" | tee -a "$ANALYSIS_FILE"
echo "Pipe issues: $pipe_issues" | tee -a "$ANALYSIS_FILE"
echo "Variable expansion issues: $variable_expansion_issues" | tee -a "$ANALYSIS_FILE"
echo "Quote handling issues: $quote_handling_issues" | tee -a "$ANALYSIS_FILE"
echo "Exit code differences: $exit_code_issues" | tee -a "$ANALYSIS_FILE"

echo "" | tee -a "$ANALYSIS_FILE"
echo "Analysis complete. See $ANALYSIS_FILE for details."
