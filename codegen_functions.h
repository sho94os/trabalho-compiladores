#ifndef CODEGEN_FUNCTIONS_H
#define CODEGEN_FUNCTIONS_H

#include <stdbool.h>
#include "token_struct.h"

/* ------------------------------------------------------------- */
/*                     Code Generator Constants                  */
/* ------------------------------------------------------------- */

/* Global Start Address */
#define GLOBAL_START_ADRESS                     -1

/* Function Prefix */
#define FUNCTION_PREFIX                         "function_"

/* Global reserved variables Prefix */
#define GLOBAL_SYSTEM_VARIABLE_PREFIX           "_"

/* Global variables Prefix */
#define GLOBAL_VARIABLE_PREFIX                  "__"

/* Value of a nil variable */
#define NIL_TYPE_MASK                           "0x80000000"
#define NIL_TYPE_VALUE                          "0x7FFFFFFF"

/* ------------------------------------------------------------- */
/*                     Code Generator Functions                  */
/* ------------------------------------------------------------- */

/* Generate all code */
bool cgenAllCode(TokenNode *root_token);

/* Generate blocks of code */
bool cgenBlockCode(TokenNode *block_token, SymbolTable *previous_scope);

/* Copy global variable definitions */
bool copyGlobalVariables();

/* ------------------------------------------------------------- */
/*                         System Templates                      */
/* ------------------------------------------------------------- */

/* Generated code Header */
const char mips_header[] = 
    "# ### START OF GENERATED CODE ### #\n"
    "\n"
    "# Code Generated By Jeferson Lima and Jeferson Rene Compiler\n"
    "# LUA MIPS Compiler Version 0.3 \n"
    "\n"
    "# Variable Declarations\n"
    ".data\n"
    "\n"
    "# System default variables \n"
    GLOBAL_SYSTEM_VARIABLE_PREFIX "newline: .asciiz \"\\n\"\n"
    GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_str: .asciiz \"nil\"\n"
    GLOBAL_SYSTEM_VARIABLE_PREFIX "nil_val: .word   " NIL_TYPE_VALUE
    "\n"
    "# User Global Variables";

/* -- GLOBAL VARS -- */

/* System Functions */        
const char mips_main[] =
    "# Start of mips code\n"
    ".globl main\n"
    ".text\n"
    "\n"
    "# -- Main Definition -- #\n"
    "main:\n"
    "\n";
    
/* -- ALL INSTRUCTIONS -- "%s" */

/* Close main */
const char mips_footer[] =
    "\n"
    "# Close Main Declaration\n"
    "close_main:\n"
        "\tli $v0, 10\n"
        "\tsyscall\n"
    "\n"
    "# -- End of Main -- #\n"
    "\n"
    "# System Defined Functions\n"
    "\n"
    "# -- Print Function -- #\n"
    "function_print:\n"
        "\t# Load Function Frame Pointer and Return Adress\n"
        "\tmove $fp, $sp\n"
        "\tsw $ra, 0($sp)\n"
        "\taddiu $sp, $sp, 4\n"
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
        "\tli $v0, 1\n"
        "\tsyscall\n"
        "\tj end_print\n"
        "\n"
    "print_nil_value:\n"
    "   	# Print Value nil"
        "\tli $v0, 4\n"
        "\tla $a0, _nil_str\n"
        "\tsyscall\n"
        "\n"
    "end_print:\n"
        "\t# Print linefeed\n"
        "\tli $v0, 4\n"
        "\tla $a0, _newline\n"
        "\tsyscall\n"
        "\n"
        "\t# Close Print Function \n"
        "\tlw $ra, 0($fp)\n"
        "\taddiu $sp, $sp, 12\n"
        "\tlw $fp, 0($sp)\n"
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
    "" GLOBAL_VARIABLE_PREFIX "%s .word " NIL_TYPE_VALUE "\n"
    "# ----------------------------------------------- #\n";

/* Define a local variable */    
const char mips_global_define[] =
    "\t# ------------ Define a local variable ---------- #\n"
    "\taddiu $sp, $sp, -4\n"
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
    "\tsw $a0, %d($sp)\n"
    "\t# ----------------------------------------------- #\n";

/* Load a local variable into $a0 */
const char mips_local_load[] =
    "\t# --------- Load local variable in $a0 ---------- #\n"
    "\tlw $a0, %d($sp)\n"
    "\t# ----------------------------------------------- #\n";

/* Push temporary return of a expression */
const char mips_push_a0[] =
    "\t# -------------- Push $a0 to stack -------------- #\n"
    "\tsw $a0, 0($sp)\n"
    "\taddiu $sp, $sp, -" TO_STRING(BYTE_VARIABLE_SIZE) "\n"
    "\t# ----------------------------------------------- #\n";
    
/* Pop stack value */
const char mips_pop[] =
    "\t# ------------------- Pop stack ----------------- #\n"
    "\taddiu $sp, $sp, " TO_STRING(BYTE_VARIABLE_SIZE) "\n"
    "\t# ----------------------------------------------- #\n";

/* Load top value to $t1 */
const char mips_top_t1[] =
    "\t# ------------- Top of stack to $t1 ------------- #\n"
    "\tlw $t1, " TO_STRING(BYTE_VARIABLE_SIZE) "($sp)\n"
    "\t# ----------------------------------------------- #\n";

/* Load a static number into $a0 */
const char mips_static_number_load[] =
    "\t# --------- Load static number into $a0 --------- #\n"
    "\tli $a0, %s\n"
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
 *          CGEN(operand)
 */

/* Not a number in $a0 */
const char mips_not_a0_a0[] =
    "\t# ---------------- Not $a0 number --------------- #\n"
    "\tnot $a0, $a0\n"
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
 *          push_a0
 *          CGEN(exp2)
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
    "\tdiv $a0, $t1, $a0\n"
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

/* Check if is greater than, the value of $t1 with $a0 and store in $a0 */
const char mips_gt_a0_t1_a0[] =
    "\t# ------------- Gt $a0 = $t1 > $a0 -------------- #\n"
    "\tslt $a0, $a0, $t1\n"
    "\t# ----------------------------------------------- #\n";

/* Check if is less than, the value of $t1 with $a0 and store in $a0 */
const char mips_lt_a0_t1_a0[] =
    "\t# ------------- Lt $a0 = $t1 < $a0 -------------- #\n"
    "\tslt $a0, $t1, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is equal $t1 */
const char mips_eq_a0_t1_a0[] =
    "\t# ------------- Eq $a0 = $t1 == $a0 ------------- #\n"
    "\tsubu $a0, $t1, $a0\n"
    "\tsltu $a0, $0, $a0\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is not equal $t1 */
const char mips_neq_a0_t1_a0[] =
    "\t# ------------- Neq $a0 = $t1 ~= $a0 ------------ #\n"
    "\tsubu $a0, $t1, $a0\n"
    "\tsltu $a0, $0, $a0\n"
    "\t# ----------------------------------------------- #\n";

/* Check if $a0 is greater or equal $t1 */
const char mips_gte_a0_t1_a0[] =
    "\t# ------------- Gte $a0 = $t1 >= $a0 ------------ #\n"
    "\tslt $a0, $t1, $a0\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";
    
/* Check if $a0 is less or equal $t1 */
const char mips_lte_a0_t1_a0[] =
    "\t# ------------- Lte $a0 = $t1 <= $a0 ------------ #\n"
    "\tslt $a0, $a0, $t1\n"
    "\txori $a0, $a0, 1\n"
    "\t# ----------------------------------------------- #\n";


/* ------------------------------------------------------------- */
/*               Conditional Operations Template                 */
/* ------------------------------------------------------------- */

/**
 * Model for conditional operations
 * 
 *  Default Model:
 *      CGEN(if exp1 then bloco1 else bloco2) ->
 *          CGEN(exp1)
 *          push_a0 
 *          li t1 1
 *          bne a0 t1 else
 *          CGEN(bloco1)
 *          j end
 *  label   else
 *             CGEN(bloco2)
 *  label   end    
 */
    
 /* Conditional type If-elseif */
const char mips_if =
    "\t# -------------------- If ----------------------- #\n"
    "\tbne $a0, $t1, endif%d\n"
    "\t%s\n" //"then comes here"
    "\tjal endif\n"
    "endif:\n"
    "\t# ----------------------------------------------- #\n";

const char mips_elseif =
    "\t# -------------------- Elseif ------------------- #\n"
    "\tbne $a0, $t1, elseif%d\n"
    "\t%s\n" //"then comes here"
    "\tjal end\n"
    "elseif%d\n"
    "\t# ----------------------------------------------- #\n";
    
 /* Conditional type Else */
const char mips_else =
    "\t# --------------------- Else -------------------- #\n"
    "\telse%d:\n"
    "\t%s\n" //"else comes here"
    "end%d:\n"
    "\t# ----------------------------------------------- #\n";
 
/* ------------------------------------------------------------- */
/*                   Loops Operations Template                   */
/* ------------------------------------------------------------- */

/**
 * Model for loop operations
 *     Default Model While:
 *        CGEN(while(exp1) do bloco end) ->
 *  label   while  
 *          CGEN(exp1)
 *          beq $a0, $a0, end
 *          CGEN(bloco)
 *          b while
 *   label      end
 * 
 * Default Model For:
 *       
 */

/* Loop type while */
const char mips_while =
    "\t# -------------------- WHILE -------------------- #\n"
    "while%d:\n"
    "\t%s\n"
    "\tbeq $a0, $ao, end_while\n"
    "\t%s\n"
    "\tb while%d\n"
    "end_while%d:"
    "\t# ----------------------------------------------- #\n";
    
/* Loop type for */
const char mips_for =
    "\t# ---------------------- FOR -------------------- #\n"
  
    "\t# ----------------------------------------------- #\n";
    
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
 const char mips_start_function_call[] =
    "\t# ---------------- CALL FUNCTION ---------------- #\n"
    "\tsw $fp, 0($sp)\n"
    "\taddiu $sp, $sp, -4\n";
    
const char mips_end_function_call[] =
    "\tjal function_%s\n"
    "\t# ----------------------------------------------- #\n";

/* ------------------------------------------------------------- */

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
 *      Operations should test if this number can be subtracted,
 *      multiplied, addition and division is possible but should
 *      be avoided as much as it's possible.
 */

/* ------------------------------------------------------------- */

 
/* ------------------------------------------------------------- */
/*                  ..........................                   */
/* ------------------------------------------------------------- */

/**
 * Model ...
 * 
 *  Default Model:
 *      ...
 */

/* ------------------------------------------------------------- */

#endif