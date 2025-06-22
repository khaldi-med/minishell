# NORME COMPLIANCE PLAN FOR MINISHELL

## Status Overview
Based on norme.en.txt requirements, here are the critical violations and fixes:

### ✅ COMPLETED FIXES:
1. **Directory Structure Fixed:**
   - `src/parcing/` → `src/parsing/` ✅
   - `builins.c` → `builtins.c` ✅
   - `parce.c` → `parse.c` ✅

2. **Function Count Violations Fixed:**
   - `builtins.c`: Split from 11 functions → 2 functions ✅
   - Created: `builtins_basic.c` (5 functions) ✅
   - Created: `builtins_export.c` (3 functions) ✅
   - Created: `builtins_exit.c` (2 functions) ✅

### 🚧 IN PROGRESS:
3. **Files Still Need Splitting (>5 functions):**
   - `executor.c`: 19 functions → Need to split
   - `env.c`: 8 functions → Need to split
   - `lexer.c`: 9 functions → Need to split
   - `parse.c`: 7 functions → Need to split
   - `path.c`: 7 functions → Need to split

### 📋 TODO:
4. **Function Length Issues (>25 lines):**
   - Check all functions for line count compliance
   - Split overly long functions

5. **Formatting Issues:**
   - Fix lines exceeding 80 characters

6. **Code Structure:**
   - Remove comments inside function bodies
   - Ensure proper variable declarations
   - Fix multiple assignments

## Implementation Priority:
1. Fix file function counts (CRITICAL)
2. Fix function line lengths
3. Fix formatting and headers
4. Final norme check with norminette

## Files Modified:
- ✅ src/exec/builtins.c (2 functions)
- ✅ src/exec/builtins_basic.c (5 functions)
- ✅ src/exec/builtins_export.c (3 functions)
- ✅ src/exec/builtins_exit.c (2 functions)
- ✅ Makefile (updated sources)
- 🚧 src/exec/redirection.c (created, 5 functions)

## Next Steps:
1. Continue splitting executor.c
2. Split parsing files (env.c, lexer.c, parse.c, path.c)
3. Apply formatting fixes
4. Add 42 headers
5. Run norminette validation
