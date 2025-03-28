#include "bytecode_generator.hpp"

void generate_bytecode(Node* node, CompilerContext& context);
    
// BytecodeGenerationPass implementation
BytecodeGenerationPass::BytecodeGenerationPass() 
    : CompilerPass() {}

void BytecodeGenerationPass::run(CompilerContext& context) {
    if (!context.contains("input_file")) {
        CompilerContext::error("input_file not set in context");
    }
    std::string inputFile = context.get<std::string>("input_file");
    if (!context.contains("ast")) {
        CompilerContext::error("AST not set in context");
    }
    Node* ast = context.get<Node*>("ast");
    generate_bytecode(ast, context);

    if(context.contains("verboseB")){
        if(context.get<bool>("verboseB")){
            std::cout << "Bytecode:" << std::endl;
            display_bytecode(context.get<function*>("function"));
            std::cout << "Constants:" << std::endl;
            display_constants();
            std::cout << "Variables:" << std::endl;
            display_variables();
        }

    }
}

void BytecodeGenerationPass::gen_test_file(std::string test_file_name, CompilerContext& context) {
    // Get the directory where the current file (bytecode_generator.cpp) is located
    std::string current_file = __FILE__;
    std::string current_dir = current_file.substr(0, current_file.find_last_of("/\\"));
    
    // Create tests directory within the BytecodeGenerator directory
    std::string test_dir = current_dir + "/tests";
    std::filesystem::create_directories(test_dir);

    // Check if required data is in the context
    if (!context.contains("function")) {
        // CompilerContext::error("Function not found in context");
        std::cout << "Function not found in context" << std::endl;
        return;
    }
    if (!context.contains("variable_names")) {
        // CompilerContext::error("Variable names not found in context");
        std::cout << "Variable names not found in context" << std::endl;
        return;
    }
    if (!context.contains("constants")) {
        // CompilerContext::error("Constants not found in context");
        std::cout << "Constants not found in context" << std::endl;
        return;
    }

    // Generate the full path for the test file
    std::string full_path = test_dir + "/" + test_file_name + ".cl_exe";
    
    // Get the data from context
    function* func = context.get<function*>("function");
    std::vector<std::string> variable_names = context.get<std::vector<std::string>>("variable_names");
    std::vector<Value> constants = context.get<std::vector<Value>>("constants");

    // Write the bytecode file
    write_cl_exe(test_file_name + ".cl_exe", test_dir + "/", func, variable_names, constants);

    if (std::filesystem::exists(full_path)) {
        std::cout << "Test file generated: " << full_path << std::endl;
    }
}

CompilerContext& BytecodeGenerationPass::read_test_file(std::string test_file_name) {
    CompilerContext* context = new CompilerContext();
    // std::cout << "Reading test file: " << test_file_name << std::endl;
    cl_exe* exe = read_cl_exe(test_file_name + ".cl_exe");
    // std::cout << "Read test file: " << test_file_name << std::endl;
    context->set("function", exe->main);
    context->set("variable_names", exe->variable_names);
    context->set("constants", exe->constants);
    return *context;
}


std::tuple<bool, std::string> BytecodeGenerationPass::compare_out_to_expected(CompilerContext& out, CompilerContext& expected) {
    // check if function is in the context
    if (!out.contains("function") || !expected.contains("function")) {
        return std::make_tuple(false, "Function not found in context");
    }
    function* out_func = out.get<function*>("function");
    function* expected_func = expected.get<function*>("function");
    if (out_func->count != expected_func->count) {
        return std::make_tuple(false, "Different number of bytes in the function");
    }
    for (int i = 0; i < out_func->count; i++) {
        if (out_func->code[i] != expected_func->code[i]) {
            return std::make_tuple(false, "Different bytes in the function");
        }
    }

    // Compare constants
    // check if constants are in the context
    if (!out.contains("constants") || !expected.contains("constants")) {
        return std::make_tuple(false, "Constants not found in context");
    }
    std::vector<Value> out_constants = out.get<std::vector<Value>>("constants");
    std::vector<Value> expected_constants = expected.get<std::vector<Value>>("constants");
    if (out_constants.size() != expected_constants.size()) {
        return std::make_tuple(false, "Different number of constants");
    }
    for (int i = 0; i < (int)out_constants.size(); i++) {
        if (Value::equals(out_constants[i], expected_constants[i]) == false) {
            return std::make_tuple(false, "Different constants");
        }
    }

    // Compare variable names
    //check if variable names are in the context
    if (!out.contains("variable_names") || !expected.contains("variable_names")) {
        return std::make_tuple(false, "Variable names not found in context");
    }

    std::vector<std::string> out_variable_names = out.get<std::vector<std::string>>("variable_names");
    std::vector<std::string> expected_variable_names = expected.get<std::vector<std::string>>("variable_names");
    if (out_variable_names.size() != expected_variable_names.size()) {
        return std::make_tuple(false, "Different number of variable names");
    }
    for (int i = 0; i < (int)out_variable_names.size(); i++) {
        if (out_variable_names[i] != expected_variable_names[i]) {
            return std::make_tuple(false, "Different variable names");
        }
    }

    return std::make_tuple(true, "");
}


BytecodeGenerationPass::~BytecodeGenerationPass() {
    // Cleanup implementation if needed
}


void interpretation_error(std::string message, Node *node, function *func);

/*
For athritmetic and boolean operations
*/
std::unordered_map<std::string, OpCode> opCodeMap = {
    {"(", OpCode::OP_FUNCTION_CALL},
    {"[", OpCode::OP_ACCESS},
    {"u-", OpCode::OP_U_SUB},
    {"+", OpCode::OP_ADD},
    {"-", OpCode::OP_SUB},
    {"*", OpCode::OP_MUL},
    {"/", OpCode::OP_DIV},
    {"%", OpCode::OP_MOD},
    {"^", OpCode::OP_EXP},
    {"==", OpCode::OP_EQ},
    {"!=", OpCode::OP_NEQ},
    {">", OpCode::OP_GT},
    {"<", OpCode::OP_LT},
    {">=", OpCode::OP_GTEQ},
    {"<=", OpCode::OP_LTEQ},
    {"&&", OpCode::OP_AND},
    {"||", OpCode::OP_OR},
    {"!", OpCode::OP_NOT}};

/*
This vector stores constant values Ex: let x = 5; 5 is a constant
Types of constants: NUMBER, BOOL, STRING, FUNCTION, NULL_VALUE
*/
std::vector<Value> constants; 

/*
This array stores the names of the variables
*/
std::vector<std::string> variable_names; 


// Visual Representation for debugging -------------------------------
void display_bytecode(function *func)
{
    for (int i = 0; i < func->count; i++)
    {
        std::cout << i << ": ";
        switch (func->code[i])
        {
        // Arithmetic
        case OpCode::OP_ADD:
            std::cout << "OP_ADD" << std::endl;
            break;
        case OpCode::OP_SUB:
            std::cout << "OP_SUB" << std::endl;
            break;
        case OpCode::OP_U_SUB:
            std::cout << "OP_U_SUB" << std::endl;
            break;
        case OpCode::OP_MUL:
            std::cout << "OP_MUL" << std::endl;
            break;
        case OpCode::OP_DIV:
            std::cout << "OP_DIV" << std::endl;
            break;
        case OpCode::OP_MOD:
            std::cout << "OP_MOD" << std::endl;
            break;
        case OpCode::OP_EXP:
            std::cout << "OP_EXP" << std::endl;
            break;

        // Boolean
        case OpCode::OP_AND:
            std::cout << "OP_AND" << std::endl;
            break;
        case OpCode::OP_OR:
            std::cout << "OP_OR" << std::endl;
            break;
        case OpCode::OP_NOT:
            std::cout << "OP_NOT" << std::endl;
            break;

        // Comparison
        case OpCode::OP_EQ:
            std::cout << "OP_EQ" << std::endl;
            break;
        case OpCode::OP_NEQ:
            std::cout << "OP_NEQ" << std::endl;
            break;
        case OpCode::OP_GT:
            std::cout << "OP_GT" << std::endl;
            break;
        case OpCode::OP_LT:
            std::cout << "OP_LT" << std::endl;
            break;
        case OpCode::OP_GTEQ:
            std::cout << "OP_GTEQ" << std::endl;
            break;
        case OpCode::OP_LTEQ:
            std::cout << "OP_LTEQ" << std::endl;
            break;

        // Variables
        case OpCode::OP_LOAD:
            std::cout << "OP_LOAD";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Value: " << VALUE_AS_STRING(constants[(int)func->code[i]]) << std::endl;
            break;
        case OpCode::OP_STORE_VAR:
        {
            std::cout << "OP_STORE_VAR";
            std::cout << "          ";
            int type = (int)func->code[++i];
            std::string assignment_type;
            switch (type)
            {
            case 0:
                assignment_type = "let";
                break;
            case 1:
                assignment_type = "const";
                break;
            case 2:
                assignment_type = "global";
                break;
            default:
                assignment_type = "unknown";
                break;
            }
            std::cout << "Assignment type: " << assignment_type << std::endl;
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;
        }
        case OpCode::OP_UPDATE_VAR:
            std::cout << "OP_UPDATE_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;
        case OpCode::OP_LOAD_VAR:
            std::cout << "OP_LOAD_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;
        case OpCode::OP_LOAD_FUNCTION_VAR:
            std::cout << "OP_LOAD_FUNCTION_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;

        // Arrays
        case OpCode::OP_CREATE_VECTOR:
            std::cout << "OP_CREATE_VECTOR" << std::endl;
            break;
        case OpCode::OP_VECTOR_PUSH:
            std::cout << "OP_VECTOR_PUSH" << std::endl;
            break;
        case OpCode::OP_UPDATE_VECTOR_ELEMENT:
            std::cout << "OP_UPDATE_VECTOR_ELEMENT";
            std::cout << "          ";
            std::cout << "Vector Name: " << variable_names[(int)func->code[++i]];
            std::cout << std::endl;
            break;

        // maps
        case OpCode::OP_CREATE_MAP:
            std::cout << "OP_CREATE_MAP" << std::endl;
            break;
        case OpCode::OP_UPDATE_MAP_ELEMENT:
            std::cout << "OP_UPDATE_MAP_ELEMENT";
            std::cout << "          ";
            std::cout << "Map Name: " << variable_names[(int)func->code[++i]];
            std::cout << "          ";
            std::cout << "Element Name: " << variable_names[(int)func->code[++i]];
            std::cout << std::endl;
            break;
        case OpCode::OP_ACCESS:
            std::cout << "OP_ACCESS" << std::endl;
            break;
        case OpCode::OP_ACCESS_FOR_UPDATE:
            std::cout << "OP_ACCESS_FOR_UPDATE" << std::endl;
            break;
        case OpCode::OP_DEFINE_OP_FOR_TYPE:
            std::cout << "OP_DEFINE_OP_FOR_TYPE" << std::endl;
            break;

        // Stack
        case OpCode::OP_UPDATE_STACK_ELEMENT:
            std::cout << "OP_UPDATE_STACK_ELEMENT" << std::endl;
            break;
        case OpCode::OP_ACCESS_STACK_ELEMENT_QUEUE:
            std::cout << "OP_ACCESS_STACK_ELEMENT_QUEUE" << std::endl;
            break;
        case OpCode::OP_NOT_EMPTY:
            std::cout << "OP_NOT_EMPTY" << std::endl;   
            break;
        case OpCode::OP_POP:
            std::cout << "OP_POP" << std::endl;
            break;
        

        // Control flow
        case OpCode::OP_RETURN:
            std::cout << "OP_RETURN" << std::endl;
            break;
        case OpCode::OP_JUMP:
            std::cout << "OP_JUMP    " << "Index: " << (int)func->code[++i] << std::endl;
            break;
        case OpCode::OP_JUMP_IF_FALSE:
            std::cout << "OP_JUMP_IF_FALSE    " << "Index: " << (int)func->code[++i] << std::endl;
            break;
        case OpCode::OP_FUNCTION_CALL:
            std::cout << "OP_FUNCTION_CALL" << std::endl;
            ;
            break;

        // Scope
        case OpCode::OP_INC_SCOPE:
            std::cout << "OP_INC_SCOPE" << std::endl;
            break;
        case OpCode::OP_DEC_SCOPE:
            std::cout << "OP_DEC_SCOPE" << std::endl;
            break;

        // Output
        case OpCode::OP_PRINT:
            std::cout << "OP_PRINT" << std::endl;
            break;

        // Stdlib
        case OpCode::OP_STD_LIB_CALL:
            std::cout << "OP_STD_LIB_CALL";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << STD_LIB_FUNCTIONS_DEFINITIONS[(int)func->code[i]].name << std::endl;
            break;

        default:
            std::cout << "Unknown opcode" << std::endl;
            break;
        }
    }
}

void display_constants()
{
    for (int i = 0; i < (int)constants.size(); i++)
    {
        std::cout << i << ": \n";
        print_value(constants[i]);
        std::cout << "\n"
                  << std::endl;
    }
}

void display_variables()
{
    for (int i = 0; i < (int)variable_names.size(); i++)
    {
        std::cout << i << ": " << variable_names[i] << std::endl;
    }
}
// -------------------------------------------------------------------


/*
Writes a byte to the end of the current function's code array
*/
inline void WRITE_BYTE(CODE_SIZE byte, function *func)
{
    func->code[func->count++] = byte;
}

/*
Changes a byte at a specific index in the current function's code array
*/
inline void CHANGE_BYTE(int index, CODE_SIZE byte, function *func)
{
    if (index >= func->count || index < 0)
    {
        std::cout << "Index out of bounds" << std::endl;
        exit(1);
    }
    func->code[index] = byte;
}

/*
Adds a flag to the current function's flags array
Used to siginify a specific byte to be changed later
*/
inline void FLAG_BYTE(int index, std::string flag, function *func)
{
    func->flags.push_back({index, flag});
}


// Writing values to the constants array ----------------------------
inline void WRITE_VALUE(double value)
{
    constants.push_back({Value_Type::NUMBER, value});
}

inline void WRITE_VALUE(bool value)
{
    constants.push_back({Value_Type::BOOL, value});
}

inline void WRITE_VALUE(const std::string &value)
{
    constants.push_back({Value_Type::STRING, value});
}

/*
TODO: fix, nullptr values go into this function as well, because of the null value type
*/
inline void WRITE_VALUE(function *value)
{
    if(value == nullptr){
        // std::cout << "Function is null" << std::endl;
        constants.push_back({Value_Type::NULL_VALUE, nullptr});
    } else {
    constants.push_back({Value_Type::FUNCTION, value});
    }
}

inline void WRITE_VALUE(Value value)
{
    constants.push_back(value);
}
// -------------------------------------------------------------------

/*
Returns the constant at the given index
*/
inline Value get_constant(int index)
{
    if(index < 0 || index >= (int)constants.size()){
        interpretation_error("Constant index out of bounds", nullptr, nullptr);
    }
    return constants[index];
}

/*
Writes the name of a variable to the variable names array
Names can be duplicated (Probably not the most space efficient, but it is faster than searching for the name every time O(n))
*/
inline void WRITE_VAR_NAME(const std::string &name)
{
    variable_names.push_back(name);
}

/*
Returns the index of the variable in the variable names array or -1 if it doesn't exist
*/
int get_variable_index(const std::string &name)
{ // returns the index of the variable in the variable names array
    for (int i = 0; i < (int)variable_names.size(); i++)
    {
        if (variable_names[i] == name)
        {
            return i;
        }
    }
    return -1;
}

/*
Writes the name of a variable to the variable names array if it doesn't already exist
*/
void WRITE_VAR_NAME_IF_NOT_EXISTS(const std::string &name)
{
    if (get_variable_index(name) == -1)
    {
        WRITE_VAR_NAME(name);
    }
}

/*
Returns the name of the variable at the given index
*/
std::string get_variable_name(int index)
{
    if(index < 0 || index >= (int)variable_names.size()){
        interpretation_error("Variable index out of bounds", nullptr, nullptr);
    }
    return variable_names[index];
}


/*
Creates a new function
Capacity is the initial size of the code array
*/
function *create_function(int capacity)
{
    function *func = new function;
    func->code = new CODE_SIZE[capacity];
    func->count = 0;
    func->capacity = capacity;

    return func;
}


// Interpretation ----------------------------------------------------
void interpret(Node *node, function *func);
void interpret_stmt_list(Node *node, function *func);
void interpret_stmt(Node *node, function *func);
void interpret_if(Node *node, function *func);
void interpret_return(Node *node, function *func);
void interpret_expr(Node *node, function *func);
void interpret_op(Node *node, function *func);
void interpret_function(Node *node, function *func);
void interpret_function_call(Node *node, function *func);
void interpret_std_lib_call(Node *node, function *func);
void interpret_list(Node *node, function *func);
void interpret_map_assign(Node *node, function *func);

/*
Called when an error occurs during interpretation
Prints the error message, the node that caused the error, and the current state of the bytecode 
Exits the program
*/
void interpretation_error(std::string message, Node *node, function *func)
{
    std::cout << "Bytecode generation failed" << std::endl;

    std::cout << "Error at line " << node->get_line_number() << ": " << std::endl;
    std::cout << message << std::endl;
    node->print();

    // print the current state of the bytecode
    std::cout << "\nBytecode: " << std::endl;
    display_bytecode(func);
    std::cout << "\nConstants: " << std::endl;
    display_constants();
    std::cout << "\nVariable names: " << std::endl;
    display_variables();

    exit(1); // TODO: Handle errors better
}

/*
Takes a node that contains some kind of value
Evaluates the value and pushes it to the stack
Types: EXPR_NODE, FUNCTION_NODE, LIST_NODE, NULL_NODE, MAP_NODE
*/
void evaluate(Node *value, function *func)
{
    if(value->get_type() != NodeType::EXPR_NODE){
        interpretation_error("Invalid node type for evaluate", value, func);
    }
    interpret_expr(value, func);
}

/*
Pushes the arguments of an stdlib call to the stack
Calls the stdlib function if it is found in STD_LIB_FUNCTIONS_DEFINITIONS
*/
void interpret_std_lib_call(Node *node, function *func)
{
    if (node->get_type() != NodeType::STD_LIB_CALL_NODE)
    {
        interpretation_error("Std Lib call doesn't start with STD_LIB_CALL Node", node, func);
    }

    std::string function_name = node->get_value(1);

    // push the arguments to the stack
    Node *arg_list = node->get_child(0);
    for (int i = 0; i < (int)arg_list->get_children().size(); i++)
    {
        evaluate(arg_list->get_child(i), func);
    }

    WRITE_BYTE(OpCode::OP_STD_LIB_CALL, func);
    // look up the function in the std lib function names array
    for (int i = 0; i < (int)STD_LIB_FUNCTIONS_DEFINITIONS.size(); i++)
    {
        if (function_name == STD_LIB_FUNCTIONS_DEFINITIONS[i].name)
        {
            WRITE_BYTE(i, func);
            return;
        }
    }
    interpretation_error("Std Lib function not found: " + function_name, node, func);
}

/*
Takes a node and writes its value to the stack
*/
void choose_expr_operand(Node *node, function *func)
{
    std::string opStr = node->get_value();
    switch (node->get_type())
    {
    case NodeType::OP_NODE:
        interpret_op(node, func);
        break;
    case NodeType::NUM_NODE:
        WRITE_VALUE(std::stod(opStr));
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    case NodeType::NULL_NODE:
        WRITE_VALUE(nullptr);
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    case NodeType::BOOL_NODE:
        WRITE_VALUE(opStr == "true"); // Convert the string to a bool
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    case NodeType::VAR_NODE:
        if (node->get_children().size() == 0)
        { // Variable access
            WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
            if (get_variable_index(opStr) == -1)
            {
                interpretation_error("Variable not found", node, func);
            }
            WRITE_BYTE(get_variable_index(node->get_value()), func);
        }
        else
        {
            interpretation_error("Invalid number of children for VAR Node", node, func);
        }
        break;
    case NodeType::FUNCTION_NODE:
        interpret_function(node, func);
        break;
    case NodeType::MAP_NODE:
    {
        // Create the map
        WRITE_BYTE(OpCode::OP_CREATE_MAP, func); // Create an empty map

        // Assign the values to the map
        Node *list = node->get_child(0);
        for (int i = 0; i < (int)list->get_children().size(); i++)
        {
            // assignment nodes
            Node *assign = list->get_child(i);
            if (assign->get_type() != NodeType::ASSIGN_NODE)
            {
                interpretation_error("Map assignment doesn't start with ASSIGN Node", assign, func);
            }
            //assign children
            std::vector<Node*> assign_children = assign->get_children();
            if(assign_children.size() != 2){
                interpretation_error("Invalid number of children for ASSIGN Node", assign, func);
            }
            // interpret the key which can be any value, so evaluate it and leave it on the stack
            evaluate(assign_children[0], func);
            Node* assign_value = assign_children[1];
            interpret_map_assign(assign_value, func);
        }
    }
        break;
    case NodeType::LIST_NODE:
        WRITE_BYTE(OpCode::OP_CREATE_VECTOR, func); // Create an empty vector and push it to the stack
        interpret_list(node, func); // interpret the list and leave the vector on the stack
        break;
    case NodeType::STD_LIB_CALL_NODE:
        interpret_std_lib_call(node, func);
        break;
    case NodeType::EXPR_NODE:
        interpret_expr(node, func);
        break;
    case NodeType::STRING_NODE:
        WRITE_VALUE(node->get_value()); // Add the string to the constants array
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    default:
        interpretation_error("Invalid child type for OP Node", node, func);
        break;
    }
}

/*
Determines the type and class of the operator and gets the operands accordingly
*/
void interpret_op(Node *node, function *func)
{
    if (node->get_type() == NodeType::OP_NODE)
    {
        std::string opStr = node->get_value();

        if (opCodeMap.find(opStr) == opCodeMap.end())
        {
            interpretation_error("Invalid operator", node, func);
        }

        if (get_operators().find(opStr) == get_operators().end())
        {
            interpretation_error("Operator not found", node, func);
        }

        if (std::get<1>(get_operators()[opStr]) == "binary")
        {
            Node *l_child = node->get_child(0);
            choose_expr_operand(l_child, func);

            Node *r_child = node->get_child(1);
            choose_expr_operand(r_child, func);
        }
        else if (std::get<1>(get_operators()[opStr]) == "unary")
        {
            Node *child = node->get_child(0);
            choose_expr_operand(child, func);
        }
        else if (std::get<1>(get_operators()[opStr]) == "access")
        {
            Node *l_child = node->get_child(0);
            choose_expr_operand(l_child, func);

            Node *r_child = node->get_child(1);
            choose_expr_operand(r_child, func);
        }
        else if (std::get<1>(get_operators()[opStr]) == "call")
        {
            // push args
            Node *r_child = node->get_child(1);
            for (int i = 0; i < (int)r_child->get_children().size(); i++)
            {
                interpret_expr(r_child->get_child(i), func);
            }
        
            // push function
            Node *l_child = node->get_child(0);
            choose_expr_operand(l_child, func);
        }
        else
        {
            interpretation_error("Invalid operator type", node, func);
        }

        WRITE_BYTE(opCodeMap[opStr], func);
    }
    else
    {
        interpretation_error("Operator doesn't start with OP Node", node, func);
    }
}

/*
Checks if the node is an expression node and then calls choose_expr_operand
*/
void interpret_expr(Node *node, function *func)
{
    if (node->get_type() == NodeType::EXPR_NODE)
    {
        choose_expr_operand(node->get_child(0), func);
    }
    else
    {
        interpretation_error("Expression doesn't start with EXPR Node", node, func);
    }
}

/*
Writes the return value to the stack and returns to caller
*/
void interpret_return(Node *node, function *func)
{
    if (node->get_type() != NodeType::RETURN_NODE)
    {
        interpretation_error("Return doesn't start with RETURN Node", node, func);
    }

    evaluate(node->get_child(0), func);

    WRITE_BYTE(OpCode::OP_RETURN, func);
}

/*
Interprets an if, else if, else block chain
*/
void interpret_if(Node *node, function *func)
{
    if (node->get_type() != NodeType::IF_NODE)
    {
        interpretation_error("If doesn't start with IF Node", node, func);
    }

    int start_byte = func->count;

    // if block
    interpret_expr(node->get_child(0), func);
    WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);
    WRITE_BYTE(0, func); // Placeholder for the jump index
    int jump_if_false_byte = func->count - 1;

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the if block
    interpret_stmt_list(node->get_child(1), func);
    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the if block
    WRITE_BYTE(OpCode::OP_JUMP, func); // Jump to the end of the if, else if, else block becasue the if block was executed
    WRITE_BYTE(0, func); // Placeholder for the jump index
    FLAG_BYTE(func->count - 1, "if_executed", func);

    CHANGE_BYTE(jump_if_false_byte, func->count - 1, func); // Jump to the end of the if block

    // else if blocks
    int current_index = 2;
    while (node->get_children().size() > current_index && node->get_child(current_index)->get_type() == NodeType::EXPR_NODE)
    {
        interpret_expr(node->get_child(current_index), func);
        WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);
        WRITE_BYTE(0, func); // Placeholder for the jump index
        jump_if_false_byte = func->count - 1;

        WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the else if block
        interpret_stmt_list(node->get_child(current_index + 1), func);
        WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the else if block
        WRITE_BYTE(OpCode::OP_JUMP, func); // Jump to the end of the if, else if, else block becasue the else if block was executed
        WRITE_BYTE(0, func); // Placeholder for the jump index
        FLAG_BYTE(func->count - 1, "if_executed", func);

        CHANGE_BYTE(jump_if_false_byte, func->count - 1, func); // Jump to the end of the else if block

        current_index+=2;
    }

    // check if there is an else block
    if (node->get_children().size() % 2 == 1) // There is an else block (no condition, just stmt_list)
    {
        CHANGE_BYTE(jump_if_false_byte, func->count - 1, func); // Jump to the else block

        WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the else block
        interpret_stmt_list(node->get_child(node->get_children().size() - 1), func);
        WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the else block
    }

    //find all if_executed falgs from the start of the if block to the end of the else block
    // find all flagged bytes from start_byte to the end of the for loop
    std::vector<int> indices_to_remove;
    for(int i = 0; i < (int)func->flags.size(); i++){
        if(std::get<1>(func->flags[i]) == "if_executed"){
            if(std::get<0>(func->flags[i]) >= start_byte && std::get<0>(func->flags[i]) < func->count){
                // change the jump index to the end of the else block
                CHANGE_BYTE(std::get<0>(func->flags[i]), func->count - 1, func); // -1 because the vm will increment the ip
                indices_to_remove.push_back(i);
            }
        }
    }

    for(int i = 0; i < (int)indices_to_remove.size(); i++){
        func->flags.erase(func->flags.begin() + indices_to_remove[i]);
    }
}

/*
Creates a new function and adds it to the constants array
Interprets the stmt_list of the function and adds the bytecode to the function
*/
void interpret_function(Node *node, function *func)
{
    if (node->get_type() != NodeType::FUNCTION_NODE)
    {
        interpretation_error("Function doesn't start with FUNCTION Node", node, func);
    }

    function *new_func = create_function(1000);
    WRITE_VALUE(new_func); // Add the function to the constants array

    WRITE_BYTE(OpCode::OP_LOAD, func); // push function pointer to stack
    WRITE_BYTE(constants.size() - 1, func);

    // add the arguments to the variables map
    for (int i = 0; i < (int)node->get_child(0)->get_children().size(); i++)
    {
        std::string arg_name = node->get_child(0)->get_child(i)->get_value();
        WRITE_VAR_NAME_IF_NOT_EXISTS(arg_name);

        new_func->arguments.push_back(arg_name);
    }

    // assign the stack values to the arguments
    for (int i = new_func->arguments.size() - 1; i >= 0; i--)
    { // reverse loop to keep the order of the arguments
        WRITE_BYTE(OpCode::OP_STORE_VAR, new_func);
        WRITE_BYTE(get_variable_index(new_func->arguments[i]), new_func);
    }

    // make sure the function isn't empty
    if (node->get_children().size() == 0)
    {
        interpretation_error("Function is empty", node, func);
    }
    interpret_stmt_list(node->get_child(1), new_func);
}

/*
Evaluates each value in the list and pushes it to the vector
*/
void interpret_list(Node *node, function *func)
{
    if (node->get_type() != NodeType::LIST_NODE)
    {
        interpretation_error("List doesn't start with LIST Node", node, func);
    }

    for (int i = 0; i < (int)node->get_children().size(); i++)
    {
        Node* child = node->get_child(i);
        evaluate(child, func);
        WRITE_BYTE(OpCode::OP_VECTOR_PUSH, func);    // Insert the value into the vector
    }
}

/*
Interprets a value and updates the map on the stack with the value
*/
void interpret_map_assign(Node *node, function *func)
{
    evaluate(node, func);

    // Store the value in the map thats on the stack
    WRITE_BYTE(OpCode::OP_UPDATE_STACK_ELEMENT, func);
}

/*
Evaluates a value and assigns it to a variable
*/
void interpret_assign(Node *node, function *func)
{
    if (node->get_type() != NodeType::ASSIGN_NODE)
    {
        interpretation_error("Assign doesn't start with ASSIGN Node", node, func);
    }

    std::string assign_type = node->get_value(0); // let, const, global
    Node *var = node->get_child(0);
    std::string var_name = var->get_value();
    Node *value = node->get_child(1);

    if (var->get_type() != NodeType::VAR_NODE)
    {
        interpretation_error("Assign doesn't have a VAR Node as the first child", node, func);
    }

    WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);
    
    evaluate(value, func);

    WRITE_BYTE(OpCode::OP_STORE_VAR, func);
    int type = -1;
    if (assign_type == "let")
    {
        type = 0;
    }
    else if (assign_type == "const")
    {
        type = 1;
    }
    else if (assign_type == "global")
    {
        type = 2;
    }
    else
    {
        interpretation_error("Invalid assign type", node, func);
    }
    WRITE_BYTE(type, func); // 0 = let, 1 = const, 2 = global
    WRITE_BYTE(get_variable_index(var_name), func);
}

/*
Evaluates a value and updates the variable with the value
Variable can be normal, Ex: x, or a map or vector, Ex: x[0]["key"]
*/
void interpret_update(Node *node, function *func)
{
    if (node->get_type() != NodeType::UPDATE_NODE)
    {
        interpretation_error("Update doesn't start with UPDATE Node", node, func);
    }

    Node *variable = node->get_child(0);

    if (variable->get_type() != NodeType::VAR_NODE)
    {
        interpretation_error("Update doesn't have a VAR Node as the first child", node, func);
    }

    std::vector<Node *> node_children = node->get_children();

    if (node_children.size() != 2)
    { 
        interpretation_error("Invalid number of children for UPDATE Node", node, func);
    }
    std::vector<Node *> variable_children = variable->get_children();

    if (variable_children.size() == 0)
    {                                           // Normal update
        evaluate(node_children[1], func);

        WRITE_BYTE(OpCode::OP_UPDATE_VAR, func); // takes the value from the stack and updates the value in the variables map
        int index = get_variable_index(variable->get_value());
        if (index == -1)
        {
            interpretation_error("Trying to update a variable that hasn't been defined", node, func);
        }
        WRITE_BYTE(index, func);
    }
    else
    {                                           // map or vector update
        std::vector<Node *> variable_children = variable->get_children();

        WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
        int index = get_variable_index(variable->get_value());
        if (index == -1)
        {
            interpretation_error("Trying to access a variable that hasn't been defined", node, func);
        }
        WRITE_BYTE(index, func);

        int num_accesses = variable_children.size();
        for(int i = 0; i < num_accesses - 1; i++){ // reverse loop to keep the order of the accesses (want to access the elements closer to the varibale first)
            interpret_expr(variable_children[i], func);
            WRITE_BYTE(OpCode::OP_ACCESS_FOR_UPDATE, func);
        }

        interpret_expr(variable_children[num_accesses - 1], func); // Index to update

        evaluate(node_children[1], func);

        for(int i = 0; i < num_accesses; i++){
            WRITE_BYTE(OpCode::OP_UPDATE_STACK_ELEMENT, func);
        }

        WRITE_BYTE(OpCode::OP_UPDATE_VAR, func); // takes the value from the stack and updates the value in the variables map
        WRITE_BYTE(index, func);
    }
}

/*
Evaluates a value and prints it to the stdout
*/
void interpret_print(Node *node, function *func)
{
    if (node->get_type() != NodeType::PRINT_NODE)
    {
        interpretation_error("Print doesn't start with PRINT Node", node, func);
    }

    //interpret_expr(node->get_child(0), func); // Expression to print
    evaluate(node->get_child(0), func);

    WRITE_BYTE(OpCode::OP_PRINT, func);
}

/*
Interprets a loop 
Starts with a variable assignment (optional)
Then an expression to check if the loop should continue (optional)
Then a statement list (optional)
Then an update statement (optional)
*/
void interpret_for(Node *node, function *func)
{
    if (node->get_type() != NodeType::FOR_NODE)
    {
        interpretation_error("For doesn't start with FOR Node", node, func);
    }

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the for loop

    std::vector<Node *> children = node->get_children();
    std::vector<NodeType> child_types;
    for (int i = 0; i < (int)children.size(); i++)
    {
        child_types.push_back(children[i]->get_type());
    }

    // find the index of the first assign node
    int assign_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::ASSIGN_NODE)
        {
            assign_index = i;
            break;
        }
    }
    if (assign_index != -1)
    {
        interpret_assign(node->get_child(assign_index), func); // Initialize the for loop
    }

    // get the index to jump back to
    int start_byte = func->count - 1;

    // find if there is an expression node
    int expr_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::EXPR_NODE)
        {
            expr_index = i;
            break;
        }
    }
    if (expr_index != -1)
    {
        interpret_expr(node->get_child(expr_index), func); // Interpret the condition for the for loop

        // jump if the condition is false
        WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);

        // get the index to jump to the end of the for loop
        WRITE_BYTE(0, func); // Placeholder for the end of for loop jump
    }
    int jump_to_end_byte = func->count - 1;

    // find the index of the statement list node
    int stmt_list_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::STMT_LIST_NODE)
        {
            stmt_list_index = i;
            break;
        }
    }
    if (stmt_list_index != -1)
    {
        interpret_stmt_list(node->get_child(stmt_list_index), func); // Interpret the body of the for loop
    }

    // find the index of the update node
    int update_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::UPDATE_NODE)
        {
            update_index = i;
            break;
        }
    }
    int update_byte = func->count - 1;
    if (update_index != -1)
    {
        interpret_update(node->get_child(update_index), func); // Update the for loop
    }

    // jump back to the condition
    WRITE_BYTE(OpCode::OP_JUMP, func);
    WRITE_BYTE(start_byte, func);

    // find all flagged bytes from start_byte to the end of the for loop
    for (int i = start_byte; i < func->count; i++)
    {
        if (func->flags.size() == 0)
        {
            break;
        }
        for (int j = 0; j < (int)func->flags.size(); j++)
        {
            if (std::get<0>(func->flags[j]) == i)
            {
                if (std::get<1>(func->flags[j]) == "continue")
                {
                    CHANGE_BYTE(i, update_byte, func); // jump to the update part of the for loop
                    func->flags.erase(func->flags.begin() + j);
                }
                else if (std::get<1>(func->flags[j]) == "break")
                {
                    CHANGE_BYTE(i, func->count - 1, func); // jump to the end of the for loop
                    func->flags.erase(func->flags.begin() + j);
                }
                break;
            }
        }
    }

    // jump to the end of the for loop
    if (expr_index != -1)
    {
        CHANGE_BYTE(jump_to_end_byte, func->count - 1, func);
    }

    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the for loop
}

/*
Interprets a foreach loop
Starts with a key and value assignment
Then a map/vector to iterate over
Then a statement list (optional)
*/
void interpret_foreach(Node *node, function *func)
{
    if (node->get_type() != NodeType::FOREACH_NODE)
    {
        interpretation_error("Foreach doesn't start with FOREACH Node", node, func);
    }

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the foreach loop

    std::vector<Node *> children = node->get_children();
    std::vector<NodeType> child_types;
    for (int i = 0; i < (int)children.size(); i++)
    {
        child_types.push_back(children[i]->get_type());
    }

    // check that there is at least 3 children
    if (children.size() < 3)
    {
        interpretation_error("Invalid number of children for FOREACH Node", node, func);
    }

    // Assign the key and value to the variables map
    std::string key_name = node->get_child(0)->get_value();
    std::string value_name = node->get_child(1)->get_value();

    WRITE_VAR_NAME_IF_NOT_EXISTS(key_name);
    WRITE_VAR_NAME_IF_NOT_EXISTS(value_name);

    //push a 0 to the stack, this will be the current iteration index
    WRITE_BYTE(OpCode::OP_LOAD, func);
    WRITE_BYTE(constants.size(), func);
    WRITE_VALUE(0.0);


    // push the map to the stack
    evaluate(node->get_child(2), func);

    // get the index to jump back to
    int start_byte = func->count - 1;

    // check if the map is empty
    WRITE_BYTE(OpCode::OP_NOT_EMPTY, func);
    WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);
    WRITE_BYTE(0, func); // Placeholder for the jump index
    int jump_to_end_byte = func->count - 1;

    // get a key value pair from the map
    WRITE_BYTE(OpCode::OP_ACCESS_STACK_ELEMENT_QUEUE, func);

    // assign pair to identifiers
    WRITE_BYTE(OpCode::OP_STORE_VAR, func);
    WRITE_BYTE(get_variable_index(key_name), func);

    WRITE_BYTE(OpCode::OP_STORE_VAR, func);
    WRITE_BYTE(get_variable_index(value_name), func);

    // interpret the statement list
    if(children.size() == 4){
        interpret_stmt_list(node->get_child(3), func);
    }

    // jump back to the start
    WRITE_BYTE(OpCode::OP_JUMP, func);
    WRITE_BYTE(start_byte, func);

    // find all flagged bytes from start_byte to the end of the for loop
    for (int i = start_byte; i < func->count; i++)
    {
        if (func->flags.size() == 0)
        {
            break;
        }
        for (int j = 0; j < (int)func->flags.size(); j++)
        {
            if (std::get<0>(func->flags[j]) == i)
            {
                if (std::get<1>(func->flags[j]) == "continue")
                {
                    CHANGE_BYTE(i, func->count - 1, func); // jump to the update part of the for loop
                    func->flags.erase(func->flags.begin() + j);
                }
                else if (std::get<1>(func->flags[j]) == "break")
                {
                    CHANGE_BYTE(i, func->count - 1, func); // jump to the end of the for loop
                    func->flags.erase(func->flags.begin() + j);
                }
                break;
            }
        }
    }

    // jump to the end of the for loop
    CHANGE_BYTE(jump_to_end_byte, func->count - 1, func);

    // remove the map and the index from the stack
    WRITE_BYTE(OpCode::OP_POP, func);
    WRITE_BYTE(OpCode::OP_POP, func);

    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the for loop
}

/*
Interprets a define statement
Starts with an operation
Then a type
Then a function
All of the Nodes must be EXPR Nodes
*/
void interpret_define(Node *node, function *func)
{
    if (node->get_type() != NodeType::DEFINE_NODE)
    {
        interpretation_error("Define doesn't start with DEFINE Node", node, func);
    }

    // first child is the operation
    Node *operation = node->get_child(0);
    // second child is the type
    Node *type = node->get_child(1);
    // third child is the function 
    Node *function = node->get_child(2);

    if (operation->get_type() != NodeType::EXPR_NODE)
    {
        interpretation_error("Define operation doesn't start with EXPR Node", node, func);
    }

    if (type->get_type() != NodeType::EXPR_NODE)
    {
        interpretation_error("Define type doesn't start with EXPR Node", node, func);
    }

    if (function->get_type() != NodeType::EXPR_NODE)
    {
        interpretation_error("Define function doesn't start with EXPR Node", node, func);
    }

    // evaluate function because it needs to be on the bottom of the stack
    evaluate(function, func);

    // evaluate type
    evaluate(type, func);

    // evaluate operation
    evaluate(operation, func);

    WRITE_BYTE(OpCode::OP_DEFINE_OP_FOR_TYPE, func);
}

/*
Determines the type of statement and calls the appropriate function
*/
void interpret_stmt(Node *node, function *func)
{
    if (node->get_type() == NodeType::STMT_NODE)
    {
        Node *child = node->get_child(0);
        switch (child->get_type())
        {
        case NodeType::EXPR_NODE:
            interpret_expr(child, func);
            break;
        case NodeType::RETURN_NODE:
            interpret_return(child, func);
            break;
        case NodeType::IF_NODE:
            interpret_if(child, func);
            break;
        case NodeType::ASSIGN_NODE:
            interpret_assign(child, func);
            break;
        case NodeType::UPDATE_NODE:
            interpret_update(child, func);
            break;
        case NodeType::DEFINE_NODE:
            interpret_define(child, func);
            break;
        case NodeType::PRINT_NODE:
            interpret_print(child, func);
            break;
        case NodeType::FOR_NODE:
            interpret_for(child, func);
            break;
        case NodeType::FOREACH_NODE:
            interpret_foreach(child, func);
            break;
        case NodeType::CONTINUE_NODE:
            // add a jump and flag the bytecode
            WRITE_BYTE(OpCode::OP_JUMP, func);
            WRITE_BYTE(0, func); // Placeholder for the jump index
            FLAG_BYTE(func->count - 1, "continue", func);
            break;
        case NodeType::BREAK_NODE:
            // add a jump and flag the bytecode
            WRITE_BYTE(OpCode::OP_JUMP, func);
            WRITE_BYTE(0, func); // Placeholder for the jump index
            FLAG_BYTE(func->count - 1, "break", func);
            break;
        case NodeType::STD_LIB_CALL_NODE:
            interpret_std_lib_call(child, func);
            break;
        default:
            interpretation_error("Invalid statement type", node, func);
            break;
        }
    }
    else
    {
        interpretation_error("Statement doesn't start with STMT Node", node, func);
    }
}

/*
Interprets a list of statements
*/
void interpret_stmt_list(Node *node, function *func)
{
    if (node->get_children().size() == 0)
    {
        return;
    }

    if (node->get_type() != NodeType::STMT_LIST_NODE)
    {
        interpretation_error("Statement List doesn't start with STMT_LIST Node", node, func);
    }

    interpret_stmt(node->get_child(0), func);
    if (node->get_children().size() == 2)
    {
        interpret_stmt_list(node->get_child(1), func);
    }
}

/*
Interprets the program
*/
void interpret(Node *node, function *func)
{
    if (node->get_type() != NodeType::STMT_LIST_NODE)
    {
        interpretation_error("Program doesn't start with STMT_LIST Node", node, func);
    }

    interpret_stmt_list(node, func);
}

/*
Generates the bytecode for the given AST
Writes the bytecode to a cl_exe file with the given name
*/
void generate_bytecode(Node *ast, CompilerContext& context)
{
    function *func = create_function(1000);

    WRITE_VAR_NAME_IF_NOT_EXISTS("argc");
    WRITE_VAR_NAME_IF_NOT_EXISTS("argv");

    interpret(ast, func);

    context.set("function", func);
    context.set("variable_names", variable_names);
    context.set("constants", constants);

    // write_cl_exe(name, "./", func, variable_names, constants);
}

// -------------------------------------------------------------------
