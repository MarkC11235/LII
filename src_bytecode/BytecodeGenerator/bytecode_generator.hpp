#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <cstdint>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>

#include "../Compiler.hpp"
#include "../helpers/Function.hpp"
#include "../helpers/Value.hpp"
#include "../helpers/Node.hpp"
#include "../std_lib/std_lib.hpp"
#include "../helpers/cl_exe_file.hpp"
#include "../helpers/opcodes.hpp"
#include "../helpers/operators.hpp"

// Forward declarations
function* generate_bytecode(Node* node, std::string file_name);
void display_bytecode(function* func);
void display_constants();
void display_variables();

class BytecodeGenerationPass : public CompilerPass {
public:
    BytecodeGenerationPass(CompilerContext& ctx);
    void run() override;
    void test() override;
    ~BytecodeGenerationPass() override;
};

// External declarations for global variables
extern std::unordered_map<std::string, OpCode> opCodeMap;
extern std::vector<Value> constants;
extern std::vector<std::string> variable_names;

// Function declarations
void interpretation_error(std::string message, Node* node, function* func);
inline void WRITE_BYTE(CODE_SIZE byte, function* func);
inline void CHANGE_BYTE(int index, CODE_SIZE byte, function* func);
inline void FLAG_BYTE(int index, std::string flag, function* func);
inline void WRITE_VALUE(double value);
inline void WRITE_VALUE(bool value);
inline void WRITE_VALUE(const std::string& value);
inline void WRITE_VALUE(function* value);
inline void WRITE_VALUE(Value value);
inline Value get_constant(int index);
inline void WRITE_VAR_NAME(const std::string& name);
int get_variable_index(const std::string& name);
void WRITE_VAR_NAME_IF_NOT_EXISTS(const std::string& name);
std::string get_variable_name(int index);
function* create_function(int capacity);

// Interpretation function declarations
void interpret(Node* node, function* func);
void interpret_stmt_list(Node* node, function* func);
void interpret_stmt(Node* node, function* func);
void interpret_if(Node* node, function* func);
void interpret_return(Node* node, function* func);
void interpret_expr(Node* node, function* func);
void interpret_op(Node* node, function* func);
void interpret_function(Node* node, function* func);
void interpret_function_call(Node* node, function* func);
void interpret_std_lib_call(Node* node, function* func);
void interpret_list(Node* node, function* func);
void interpret_map_assign(Node* node, function* func);
void evaluate(Node* value, function* func);
void interpret_assign(Node* node, function* func);
void interpret_update(Node* node, function* func);
void interpret_print(Node* node, function* func);
void interpret_for(Node* node, function* func);
void interpret_foreach(Node* node, function* func);
void interpret_define(Node* node, function* func);
void choose_expr_operand(Node* node, function* func);

#endif // BYTECODE_GENERATOR_HPP