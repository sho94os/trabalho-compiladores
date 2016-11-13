#include "codegen_functions.h"

/* ------------------------------------------------------------- */
/*                         System Templates                      */
/* ------------------------------------------------------------- */

#define SYSCALL_PRINT_INT           "1"
#define SYSCALL_PRINT_STRING        "4"
#define SYSCALL_EXIT_PROG           "10"

/* Generated code Header */
const char mips_header[] = 
    "# ### START OF GENERATED CODE ### #\n"
    "\n"
    "# Code Generated By Jeferson Lima and Jeferson Rene Compiler\n"
    "# LUA MIPS Compiler Version 0.6 \n"
    "\n"
    "# Variable Declarations\n"
    ".data\n"
    "\n"
    "# System default variables \n"
    GLOBAL_SYSTEM_VARIABLE_PREFIX "newline: .asciiz \"\\n\"\n"
    GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_str: .asciiz \"nil\"\n"
    GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_val: .word   " NIL_TYPE_VALUE "\n"
    "\n"
    "# User Global Variables\n";

/* -- GLOBAL VARS -- */

/* System Functions */        
const char mips_main[] =
    "\n# Start of mips code\n"
    ".globl main\n"
    ".text\n"
    "\n"
    "# !!! System main is created by compiler and should not be edited !!! #\n"
    "\n"
    "# -- System Main Definition -- #\n"
    "main:\n"
        "\n"
        "\tjal main_user      # Jump to main user function\n"
        "\n"
    "# Close Main Declaration\n"
    "end_main:\n"
        "\tli $v0, " SYSCALL_EXIT_PROG "\n"
        "\tsyscall\n"
    "\n"
    "# -- Begin of Main User -- \n"
    "main_user:\n"
        "\tmove $fp, $sp\n"
        "\taddiu $sp, $sp, -8\n"
        "\tsw $ra, 4($sp)\n"
        "\tsw $fp, 8($sp)\n"
        "\n";
    
/* -- ALL INSTRUCTIONS -- "%s" */

/* Close main */
const char mips_footer[] =
        "\n"
    "# Close main user function\n"
    "end_main_function:\n"
        "\n"
        "\tlw $ra, 4($sp)\n"
        "\tlw $fp, 8($sp)\n"
        "\taddiu $sp, $sp, 8\n"
        "\tjr $ra\n"
        "\n"
    "# -- End of Main User -- #\n"
    "\n"
    "# -- End of Main -- #\n"
    "\n"
    "# System Defined Functions\n"
    "\n"
    "# -- Check if variable is nil -- #\n"
    "function_check_nil:\n"
        "\t# Load Function Frame Pointer and Return Adress\n"
        "\tmove $fp, $sp\n"
        "\taddiu $sp, $sp, -8\n"
        "\tsw $ra, 4($sp)\n"
        "\tsw $fp, 8($sp)\n"
        "\n"
        "\t# Load First Parameter\n"
        "\tlw $a0, 4($fp)\n"
        "\n"
        "\t# Check if it's a nil number\n"
        "\tli $t1, " NIL_TYPE_MASK "\n"
        "\txor $t0, $a0, $t1\n"
        "\tnot $t0, $t0\n"
        "\n"
        "\t# If this variable is not a nil, go to end\n"
        "\tbne $t0, $0, end_check_nil\n"
        "\n"
        "\t# It's a nil variable, so change this to 0\n"
        "\tli $a0, 0\n"
        "\n"
    "end_check_nil:\n"
        "\tlw $ra, 4($sp)\n"
        "\tlw $fp, 8($sp)\n"
        "\taddiu $sp, $sp, 12\n"
        "\tjr $ra\n"
    "# -- End Check Nil -- #\n"
    "\n"
    "# -- Print Function -- #\n"
    "function_print:\n"
        "\t# Load Function Frame Pointer and Return Adress\n"
        "\tmove $fp, $sp\n"
        "\taddiu $sp, $sp, -8\n"
        "\tsw $ra, 4($sp)\n"
        "\tsw $fp, 8($sp)\n"
        "\n"
        "\t# Load First Parameter\n"
        "\tlw $a0, 4($fp)\n"
        "\n"
        "\t# Check if it's a nil number\n"
        "\tli $t1, " NIL_TYPE_MASK "\n"
        "\txor $t0, $a0, $t1\n"
        "\tnot $t0, $t0\n"
        "\tbeq $t0, $0, print_nil_value\n"
        "\n"
        "\t# Print number if it's not nil\n"
        "\tli $v0, " SYSCALL_PRINT_INT "\n"
        "\tsyscall\n"
        "\tj end_print\n"
        "\n"
    "print_nil_value:\n"
        "\t# Print Value nil\n"
        "\tli $v0, " SYSCALL_PRINT_STRING "\n"
        "\tla $a0, " GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_str\n"
        "\tsyscall\n"
        "\n"
    "end_print:\n"
        "\t# Print linefeed\n"
        "\tli $v0, " SYSCALL_PRINT_STRING "\n"
        "\tla $a0, _newline\n"
        "\tsyscall\n"
        "\n"
        "\t# Close Print Function \n"
        "\tlw $ra, 4($sp)\n"
        "\tlw $fp, 8($sp)\n"
        "\taddiu $sp, $sp, 12\n"
        "\tlw $a0, " GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_val # Load nil as return\n"
        "\tjr $ra\n"
    "# -- End Print Function -- #\n"
    "\n"
    "# ### END OF GENERATED CODE ### #\n";

/* ------------------------------------------------------------- */
/*                   Basic Instructions Template                 */
/* ------------------------------------------------------------- */

/* Define a global variable */    
const char mips_global_define[] =
    "# ----------- Define a global variable ---------- #\n"
    "" GLOBAL_VARIABLE_PREFIX "%s: .word " NIL_TYPE_VALUE "\n"
    "# ----------------------------------------------- #\n";

/* Define a local variable */    
const char mips_local_define[] =
    "\t# ------------ Define a local variable ---------- #\n"
    "\taddiu $sp, $sp, -" TO_STRING(BYTE_VARIABLE_SIZE) "\n"
    "\t# ----------------------------------------------- #\n";

/* Store a global variable */    
const char mips_global_store[] =
    "\t# --------- Store $a0 in global variable -------- #\n"
    "\tsw $a0, " GLOBAL_VARIABLE_PREFIX "%s\n"
    "\t# ----------------------------------------------- #\n";

/* Load a global variable into $a0 */
const char mips_global_load[] =
    "\t# --------- Load global variable in $a0 --------- #\n"
    "\tlw $a0, " GLOBAL_VARIABLE_PREFIX "%s\n"
    "\t# ----------------------------------------------- #\n";

/* Store a local variable */    
const char mips_local_store[] =
    "\t# --------- Store $a0 in local variable --------- #\n"
    "\tsw $a0, %d($%cp)\n"
    "\t# ----------------------------------------------- #\n";

/* Load a local variable into $a0 */
const char mips_local_load[] =
    "\t# --------- Load local variable in $a0 ---------- #\n"
    "\tlw $a0, %d($%cp)\n"
    "\t# ----------------------------------------------- #\n";

/* Push temporary return of a expression */
const char mips_push_a0[] =
    "\t# -------------- Push $a0 to stack -------------- #\n"
    "\taddiu $sp, $sp, -" TO_STRING(BYTE_VARIABLE_SIZE) "\n"
    "\tsw $a0, 4($sp)\n"
    "\t# ----------------------------------------------- #\n";
    
/* Pop stack value */
const char mips_pop[] =
    "\t# ------------------- Pop stack ----------------- #\n"
    "\taddiu $sp, $sp, " TO_STRING(BYTE_VARIABLE_SIZE) "\n"
    "\t# ----------------------------------------------- #\n";

/* Pop stack params */
const char mips_pop_params[] =
    "\t# --------------- Pop Params Stack -------------- #\n"
    "\taddiu $sp, $sp, %d\n"
    "\t# ----------------------------------------------- #\n";
    
/* Pop local variables on stack */
const char mips_pop_local[] =
    "\t# -------------- Pop Local Variables ------------ #\n"
    "\taddiu $sp, $sp, %d\n"
    "\t# ----------------------------------------------- #\n";

/* Load top value to $t1 */
const char mips_top_t1[] =
    "\t# ------------- Top of stack to $t1 ------------- #\n"
    "\tlw $t1, " TO_STRING(BYTE_VARIABLE_SIZE) "($sp)\n"
    "\t# ----------------------------------------------- #\n";
    
/* Load top value to $t1 */
const char mips_top_a0[] =
    "\t# ------------- Top of stack to $a0 ------------- #\n"
    "\tlw $a0, " TO_STRING(BYTE_VARIABLE_SIZE) "($sp)\n"
    "\t# ----------------------------------------------- #\n";

/* Load a static number into $a0 */
const char mips_static_number_load[] =
    "\t# --------- Load static number into $a0 --------- #\n"
    "\tli $a0, %s\n"
    "\t# ----------------------------------------------- #\n";

/* Move from $a0 to $t1 */
const char mips_move_a0_t1[] =
    "\t# --------------- Move $t1 = $a0 ---------------- #\n"
    "\tmove $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* ------------------------------------------------------------- */
/*                  Unary Operations Template                    */
/* ------------------------------------------------------------- */

/**
 * Model for unary operations
 * 
 *  Default Model:
 *      CGEN(operand exp) ->
 *          CGEN(exp)
 *          mips_check_a0_nil()
 *          CGEN(operand)
 */

/* Not a number in $a0 */
const char mips_not_a0[] =
    "\t# ---------------- Not $a0 number --------------- #\n"
    "\tsltiu $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";
 
/* Convert $a0 to a negative number */
const char mips_neg_a0[] =
    "\t# ---------------- Neg $a0 number --------------- #\n"
    "\tnegu $a0, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* ------------------------------------------------------------- */
/*                  Binary Operations Template                   */
/* ------------------------------------------------------------- */

/**
 * Model for binary operations
 * 
 *  Default Model:
 *      CGEN(exp1 operand exp2) ->
 *          CGEN(exp1)
 *          mips_check_a0_nil()
 *          push_a0
 *          CGEN(exp2)
 *          mips_check_a0_nil()
 *          top_t1
 *          CGEN(operand)
 *          pop
 */

/* Add value of $t1 with $a0 and store in $a0 */
const char mips_add_a0_t1_a0[] =
    "\t# ------------- Add $a0 = $t1 + $a0 ------------- #\n"
    "\tadd $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* Sub value of $t1 with $a0 and store in $a0 */
const char mips_sub_a0_t1_a0[] =
    "\t# ------------- Sub $a0 = $t1 - $a0 ------------- #\n"
    "\tsub $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* Div value of $t1 with $a0 and store in $a0 */
const char mips_div_a0_t1_a0[] =
    "\t# ------------- Div $a0 = $t1 / $a0 ------------- #\n"
    "\tdiv $a0, $t1, $a0 \n"
    "\t# ----------------------------------------------- #\n";

/* Mul value of $t1 with $a0 and store in $a0 */
const char mips_mul_a0_t1_a0[] =
    "\t# ------------- Mul $a0 = $t1 * $a0 ------------- #\n"
    "\tmul $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* And the value of $t1 with $a0 and store in $a0 */
const char mips_and_a0_t1_a0[] =
    "\t# ------------- And $a0 = $t1 && $a0 ------------ #\n"
    "\tand $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* Or the value of $t1 with $a0 and store in $a0 */
const char mips_or_a0_t1_a0[] =
    "\t# ------------- Or $a0 = $t1 || $a0 ------------- #\n"
    "\tor $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/** 
 * History of this command:
 *  > 1.0 : slt $a0, $a0, $t1
 *          xori $a0, $a0, 1
 */

/* Check if is greater than, the value of $t1 with $a0 and store in $a0 */
const char mips_gt_a0_t1_a0[] =
    "\t# ------------- Gt $a0 = $t1 > $a0 -------------- #\n"
    "\tsgt $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if is less than, the value of $t1 with $a0 and store in $a0 */
const char mips_lt_a0_t1_a0[] =
    "\t# ------------- Lt $a0 = $t1 < $a0 -------------- #\n"
    "\tslt $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is equal $t1 */
const char mips_eq_a0_t1_a0[] =
    "\t# ------------- Eq $a0 = $t1 == $a0 ------------- #\n"
    "\tslt $t0, $t1, $a0\n"
    "\tslt $a0, $a0, $t1\n"
    "\txor $a0, $a0, $t0\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is not equal $t1 */
const char mips_neq_a0_t1_a0[] =
    "\t# ------------- Neq $a0 = $t1 ~= $a0 ------------ #\n"
    "\tslt $t0, $t1, $a0\n"
    "\tslt $a0, $a0, $t1\n"
    "\txor $a0, $a0, $t0\n"
    "\t# ----------------------------------------------- #\n";

/** 
 * History of this command:
 *  > 1.0 : slt $a0, $t1, $a0
 *          xori $a0, $a0, 1
 *  > 2.0 : slt $t0, $a0, $t1
            slt $t2, $t1, $a0
            sub $a0, $t0, $t2
            sltiu $a0, $a0, -1
 */

/* Check if $a0 is greater or equal $t1 */
const char mips_gte_a0_t1_a0[] =
    "\t# ------------- Gte $a0 = $t1 >= $a0 ------------ #\n"
    "\tsge $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/** 
 * History of this command:
 *  > 1.0 : slt $a0, $t1, $a0
 *          xori $a0, $a0, 1
 */
    
/* Check if $a0 is less or equal $t1 */
const char mips_lte_a0_t1_a0[] =
    "\t# ------------- Lte $a0 = $t1 <= $a0 ------------ #\n"
    "\tsle $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";
    
/* ------------------------------------------------------------- */
/*               Conditional Operations Template                 */
/* ------------------------------------------------------------- */

/**
 * Model for conditional operations
 * 
 *  Default Model:
 *      CGEN(if exp_list1 then block1 {elseif exp_list2 then bloco_i} [else block2] end) ->
 *          CGEN(exp_list1)          
 *          beq $a0, $0, end_if_n_exp_i
 *          CGEN(block1)
 *          j end_if_n
 *          end_if_n_exp_i:
 *          {
 *              CGEN(exp_list2)
 *              beq $a0, $0, end_if_n_exp_(i + 1)
 *              CGEN(bloco_i)
 *              j end_if_n
 *              end_if_n_exp_(i + 1):
 *          }+
 *          [
 *              CGEN(block2)
 *          ]*
 *          end_if_n:
 */
    
/* Conditional type If-elseif-else */
const char mips_start_if[] =
    "\t# v------------------- If ----------------------v #\n";

/* Check if condition */
const char mips_check_if[] =
    "\tbeq $a0, $0, end_if_%d_exp_%d\n";

/* Check the next if condition */
const char mips_next_if[] =
    "\tj end_if_%d\n"
    "end_if_%d_exp_%d:\n";
    
/* Header for else if */
const char mips_elseif_start[] =
    "\t### if - %d -- elseif - %d ###\n";
    
/* Header for else */
const char mips_else_start[] =
    "\t### if - %d -- else ###\n";

/* If end condition */
const char mips_end_if[] =
    "end_if_%d:\n"
    "\t# ^--------------- End of If -------------------^ #\n";
 
/* ------------------------------------------------------------- */
/*                      While Loop Template                      */
/* ------------------------------------------------------------- */

/**
 * Model for while loop operations.
 * 
 *     Default Model While:
 *        CGEN(while(exp) do bloco end) ->
 *          start_while_n:
 *          CGEN(exp)
 *          beq $a0, $0, end_while_n
 *          CGEN(bloco)
 *          b start_while_n
 *          end_while_n:
 *
 */

/* Loop type while */
const char mips_start_while[] =
    "\t# v------------------- While -------------------v #\n"
    "start_while_%d:\n";

/* While loop check while condition */
const char mips_check_while[] =
    "\tbeq $a0, $0, end_while_%d\n";

/* While end loop */
const char mips_end_while[] =
    "\tb start_while_%d\n"
    "end_while_%d:\n"
    "\t# ^--------------- End of While ----------------^ #\n";

/* ------------------------------------------------------------- */
/*                       For Loop Template                       */
/* ------------------------------------------------------------- */

/**
 * Model for 'for' loop operations.
 *
 *  Example with c-like syntax
 *  for(x = exp; x <= exp; x += [exp] || 1)
 * 
 *  | > initialization, condition, increment
 * 
 *     Default Model 'for':
 *          CGEN(for name = exp_ini, exp_cond [, exp_inc] do block end)
 *              CGEN(ASSIGN(local name = exp_ini))
 *              start_for_n:
 *              CGEN(load name)
 *              move $t1, $a0
 *              CGEN(exp_cond)
 *              sle $a0, $t1, $a0
 *              beq $a0, $0, end_for_n
 *              CGEN(block)
 *              CGEN(exp_inc) | li $a0, 1
 *              move $t1, $a0
 *              CGEN(name)        -- esse cara carrega o valor do name que inicializamos no começo
 *              add $a0, $a0, $t1 -- executa a expressao normal jogar em a0 se for default faz li $a0, 1
 *              CGEN(name = $a0)  -- esse salva de novo
 *              j start_for_n
 *              end_for_n:
 * 
 */

/* Loop type for */
const char mips_for_ini[] =
    "\t# v--------------------- For -------------------v #\n";

/* Begin of for */
const char mips_start_for[] =
    "start_for_%d:\n";

/* For condition check */
const char mips_for_check[] =
    "\t# --------- Condition check expression ---------- #\n"
    "\tsle $a0, $a0, $t1\n"
    "\tbeq $a0, $0, end_for_%d # Check iterator\n"
    "\t# ----------------------------------------------- #\n";
    
/* Store */
const char mips_for_load_inc[] =
    "\tmove $t1, $a0 # Copy value of $a0 into $t1\n";

/* Mips default for inc */
const char mips_for_inc[] =
    "\tadd $a0, $a0, $t1 # Increment iterator\n";

/* End of for defition */
const char mips_end_for[]=
    "\tj start_for_%d\n"
    "end_for_%d:\n"
    "\t# ^----------------- End of For ----------------^ #\n";

/* ------------------------------------------------------------- */
/*                    Function Definitions                       */
/* ------------------------------------------------------------- */

/**
 * Model for function call.
 * 
 * x = newSymbolTable(NULL)
 * for param in x1, ..., xn
 *      addSymbolNode(x, x1)
 * 
 * x - 4
 * y - 8
 * z - 12
 * 
 * u = 0
 * k(0, 1, 2)
 * 
 * 0 - 4
 * 1 - 8
 * 2 - 12
 * 
 * function k(x, y, z)
 *      local t = 0
 *      print(x)
 * end
 * 
 *  Default Model for functions declaration:
 *      CGEN(function f(x1, ..., xn) bloco [retorno]* end)
 *          function_name:
 *          move $fp, $sp
 *          sw $ra, 4($sp)
 *          addiu $sp, $sp, -4
 *          CGEN(bloco)
 *          end_function_name:
 *          lw $ra, 4($sp)
 *          addiu $sp, $sp, z
 *          lw $fp, 4($sp)
 *          jr $ra
 * 
 */

/* For some reason I'm receving segfault if i use these two strings concatenated */

/* Start of function definition */
const char mips_start_function_def[] = 
    "\tj end_definition_function_%s # Skip over Function definition\n"
    "\t# v------------ Function Definition ------------v #\n";
    
/* Start of function definition part 2 */
const char mips_start_function_def2[] =
    "function_%s:\n"
        "\t# Load Function Frame Pointer and Return Adress\n"
        "\tmove $fp, $sp\n"
        "\taddiu $sp, $sp, -8\n"
        "\tsw $ra, 4($sp)\n"
        "\tsw $fp, 8($sp)\n";

/* -- BLOCK -- */

/* Empty return */
const char mips_end_of_function[] =
    "\tj end_function_%s # Jump to the end of this void function\n";

/* End of function definition with nil return */
const char mips_end_function_def[] =
    "end_function_%s:\n"
        "\tlw $ra, 4($sp)\n"
        "\tlw $fp, 8($sp)\n"
        "\tlw $a0, " GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_val # Load nil as return\n"
        "\taddiu $sp, $sp, 8\n";
        
/* End of function definition without nil return and jr */
const char mips_end_function_defX[] =
        "\t# ^-------- End of Function Definition ---------^ #\n"
    "end_definition_function_%s: # Continue program from here\n";

/* -- POP PARAMETERS -- */

/* End of function definition part 2 */
const char mips_end_function_def2[] = 
        "\tjr $ra\n"
    "\t# ^-------- End of Function Definition ---------^ #\n"
    "end_definition_function_%s: # Continue program from here\n";

/* ------------------------------------------------------------- */
/*                        Function Calls                         */
/* ------------------------------------------------------------- */

/**
 * Model for function call.
 * 
 *  Default Model CGEN(function_name(listexp[a,b,...])):
 *      CGEN(listexp[1])
 *      push $a0
 *      CGEN(listexp[2])
 *      push $a0
 *      ...
 *      jal function_name
 */
 
/* Start of a function call */
const char mips_start_function_call[] =
    "\t# v--------------- Call Function ---------------v #\n"
    "\taddiu $sp, $sp, -4 # Push $fp\n"
    "\tsw $fp, 4($sp)\n";
    
/* End of a function call */
const char mips_end_function_call[] =
    "\tjal function_%s\n"
    "\tlw $fp, 4($sp)     # Load old $fp\n"
    "\taddiu $sp, $sp, 4  # Pop $fp\n"
    "\t# ^----------- End of Call Function ------------^ #\n";

/* ------------------------------------------------------------- */
/*                      Nil type definition                      */
/* ------------------------------------------------------------- */

/**
 * Model for nil numbers.
 * 
 *  Default Model CGEN(nil):
 *      lw $a0, _nil_val
 * 
 *  Description:
 *      Nil values is interpreted as the last signed number or 
 *      0x7FFFFFFF this number represent the number 0x0 after
 *      a xor with the mask 0x80000000 and a bitwise negation
 *      if this value is used it will interpreted as a nil number
 * 
 *  Warning:
 *      Arithmetic operations should don't use nil as a value since
 *      nil is not a number, nil is a type used to define that a
 *      variable hasn't received a value or it's 'NULL'.
 */

/* Load a nil into $a0 */
const char mips_nil[] =
    "\t# -------- Load global nil value in $a0 --------- #\n"
    "\tlw $a0, " GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_val\n"
    "\t# ----------------------------------------------- #\n";
    
/* Check if a given variable is nil */
const char mips_check_a0_nil[] =
    "\t# ----------- Check if variable is nil ---------- #\n"
    "\taddiu $sp, $sp, -8 # Push $fp\n"
    "\tsw $fp, 8($sp)\n"
    "\tsw $a0, 4($sp)\n"
	"\tjal function_check_nil\n"
    "\tlw $fp, 4($sp)     # Load old $fp\n"
    "\taddiu $sp, $sp, 4  # Pop $fp\n"
	"\t# ----------------------------------------------- #\n";
 
/* ------------------------------------------------------------- */
/*                Short-Circuit Evaluation Models                */
/* ------------------------------------------------------------- */

/**
 * Model for short-circuit 'and' and 'or' operations.  
 * 
 *  Default Model for 'and' (exp1 and exp2):
 *      CGEN(exp1)
 *      mips_check_a0_nil()
 *      sltiu $a0, $a0, 1
 *      xori $a0, $a0, 1
 *      beqz $a0, end_and_n / bnez $a0, end_or_n
 *      CGEN(exp2)
 *      mips_check_a0_nil()
 *      sltiu $a0, $a0, 1
 *      xori $a0, $a0, 1
 *      end_and_n:
 */
 
/* Begin of the and check */
const char mips_and_sc_header[] =
    "\t# v------------- Short Circuit 'and' -----------v #\n";
    
/* Skip the next evaluation if the first has failed */
const char mips_and_sc_skip[] =
    //"\tsltiu $a0, $a0, 1\n" // Uncoment this if you wanna use bool result
    //"\txori $a0, $a0, 1\n"
    "\tbeqz $a0, end_and_%d\n";

/* End of mips sc */
const char mips_and_sc_footer[] =
    //"\tsltiu $a0, $a0, 1\n" // Uncoment this if you wanna use bool result
    //"\txori $a0, $a0, 1\n"
	"\tend_and_%d:\n"
	"\t# ^-------- End of 'and' short-circuit ---------^ #\n";

/* Begin of the and check */
const char mips_or_sc_header[] =
    "\t# v------------- Short Circuit 'or' ------------v #\n";
    
/* Skip the next evaluation if the first has success */
const char mips_or_sc_skip[] =
    //"\tsltiu $a0, $a0, 1\n" // Uncoment this if you wanna use bool result
    //"\txori $a0, $a0, 1\n"
    "\tbnez $a0, end_or_%d\n";

/* End of mips sc */
const char mips_or_sc_footer[] =
    // "\tsltiu $a0, $a0, 1\n" // Uncoment this if you wanna use bool result
    //"\txori $a0, $a0, 1\n"
	"\tend_or_%d:\n"
	"\t# ^-------- End of 'or' short-circuit ----------^ #\n";
 
/* ------------------------------------------------------------- */
/*                         Assign Model                          */
/* ------------------------------------------------------------- */

/**
 * Model for assigns, exp == x {, y}+ = z {, k}.
 * 
 *  Default Model for assign CGEN(nomes = expressions)
 *      {
 *          CGEN(expressions[n - 1])
 *          push_a0
 *      }+
 *      ==
 *      {
 *          top_a0
 *          nome = a0
 *          pop
 *      }+
 */

/* Begin of a assign */
const char mips_start_assign[] =
    "\t# v------------------- Assign ------------------v #\n";

/* Expression Execution marker */
const char mips_marker_exp[] =
    "\t### Expression -- %d ###\n";

/* Assign marker */
const char mips_marker_assign[] =
    "\t### Assign -- %s -- %d ###\n";

/* End of a assign */
const char mips_end_assign[] =
	"\t# ^--------------- End of Assign ---------------^ #\n";
 
/* ------------------------------------------------------------- */
/*                      Local Variable Model                     */
/* ------------------------------------------------------------- */

/**
 * Model for assigns, exp == x {, y}+ = z {, k}.
 * 
 *  Default Model for assign CGEN(local nomes):
 *      {
 *          CGEN(define nomes[i])
 *          CGEN(mips_value)
 *          CGEN(store nomes[i])
 *      }+
 * 
 *  Default Model for assign CGEN(local nomes = expressions):
 *      {
 *          
 *      }
 */

/* Begin of a local assign */
const char mips_start_local_assign[] =
    "\t# v---------------- Local Assign ---------------v #\n";

/* Assign local assign marker */
const char mips_marker_local_assign[] =
    "\t### Local Assign -- %s -- %d ###\n";

/* End of a local assign */
const char mips_end_local_assign[] =
	"\t# ^------------ End of local assign ------------^ #\n";
 
/* ------------------------------------------------------------- */
/*                      Return Values Model                      */
/* ------------------------------------------------------------- */

/**
 * Model for returning values.
 * 
 *  Default Model CGEN(return exp_list):
 *      CGEN(exp_list)                  # Push exp list to stack
 *      lw $a0, 4($sp)                  # Load top of stack to $a0
 *      addiu $sp, $sp, 4               # pop stack
 *      move $s0, $sp                   # Store New $sp to $s0
 *      li $t2, 0                       # Load first value of 'i'
 *      li $t3, %d                      # Load final value of 'i'
 *      addiu $sp, $sp, -4              # Push old $fp
 *      lw $ra, %d($sp)                 # Load $ra
 *      lw $fp, %d($sp)                 # Load $fp
 *      lw $t1, %d($sp)                 # Load old $fp to $t1
 *      sw $t1, 4($sp)                  # Store old $fp to top of stack
 *      addiu $sp, $sp, %d              # Remove all values from stack
 * {
 *  return_loop_%d:                     # while (there are values on stack)
 *      lw $t1, 4($s0)                  # Pick bottom value of exp stack
 *      sw $t1, 4($sp)                  # and store on top of stack
 *      addiu $s0, $s0, -4              # Decrease exp stack
 *      addiu $sp, $sp, -4              # Increase stack
 *      addiu $t2, $t2, 1               # Increase 'i' counter
 *      bne $t2, $t3, return_loop_%d    # End Loop
 * } if (return values are more than one)
 *      jr $ra                          # Jump to register address
 */

/* Begin of the return values model */
const char mips_start_return[] =
    "\t# v-------------- Start of return --------------v #\n"
    "\tmove $s0, $sp        # Store $sp to $s0\n"
    "\tlw $a0, 4($sp)       # Load top of stack to $a0\n"
    "\tlw $ra, %d($sp)      # Load $ra\n"
    "\tlw $fp, %d($sp)      # Load $fp\n"
    "\tlw $t1, %d($sp)      # Load old $fp to $t1\n"
    "\tsw $t1, 4($sp)       # Store old $fp to top of stack\n"
    "\taddiu $sp, $sp, %d   # Remove all values from stack\n"
    "\taddiu $s0, $s0, %d   # Put s0 on bottom of the expression stack\n";

/* Multiple returns stack rearange */
const char mips_return_multiple[] =
    "\tli $t2, 0            # Load first value of 'i'\n"
    "\tli $t3, %d           # Load final value of 'i'\n"
    "\treturn_loop_%d:      # while (there are values on stack)\n"
    "\tlw $t1, 4($s0)       # Pick bottom value of exp stack\n"
    "\tsw $t1, 4($sp)       # and store on top of stack\n"
    "\taddiu $s0, $s0, -4   # Decrease exp stack\n"
    "\taddiu $sp, $sp, -4   # Increase stack\n"
    "\taddiu $t2, $t2, 1    # Increase 'i' counter\n"
    "\tbne $t2, $t3, return_loop_%d\n"
    "\taddiu $sp, $sp, 4    # Remove empty position on stack\n";

/* End of a return */
const char mips_end_return[] =
    "\tjr $ra\n"
    "\t# ^---------------- End of Return --------------^ #\n";

/* ------------------------------------------------------------- */