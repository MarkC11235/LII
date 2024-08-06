#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <any>

// int inc(int i){
//     return i + 1;
// }

// int add(int a, int b){
//     return a + b;
// }

// void* func = (void*)&inc;
// void* func2 = (void*)&add;

// std::tuple<int, int> tuple = {1, 2};

// int main(){
//     int (*f)(int) = (int (*)(int))func;
//     int (*f2)(int, int) = (int (*)(int, int))func2;

//     int result = f(5);
//     std::cout << result << std::endl;

//     result = f2(5, 10);
//     std::cout << result << std::endl;

//     std::cout << std::apply(add, tuple) << std::endl; // use this to call a function with a tuple of arguments

//     return result;
// }

// template<typename... Args>
// void print(Args... args){
//     (std::cout << ... << args);
// }

// std::function<void()> test(){
//     return []() -> void{
//         print("Hello, ", "World!\n");
//     };
// }   

// int main(){
//     std::function<void()> f = test();
//     f();
//     return 0;
// }

#include <any>
#include <tuple>
#include <vector>
#include <iostream>
#include <functional>

// Error handling function
void std_lib_error(const std::string& context, const std::string& message) {
    std::cerr << context << ": " << message << std::endl;
}

// Helper to cast std::any to the correct type and call the function
template<typename Function, typename Tuple, std::size_t... I>
decltype(auto) callWithCastedArgs(Function func, const std::vector<std::any>& args, Tuple&& tuple, std::index_sequence<I...>) {
    return func(std::any_cast<std::tuple_element_t<I, std::decay_t<Tuple>>>(args[I])...);
}

// Main function to convert std::vector<std::any> to a tuple of args and call the function
template<typename Return, typename... Args>
std::function<std::any(std::vector<std::any>, std::vector<std::string>)> make_std_lib_function(Return (*function)(Args...)) {
    return [function](std::vector<std::any> args, std::vector<std::string> arg_types) -> std::any {
        if(args.size() != sizeof...(Args)) {
            std_lib_error("make_std_lib_function", "invalid number of arguments");
            return std::any();
        }

        // Assuming args are in the correct order and type
        try {
            auto result = callWithCastedArgs(function, args, std::tuple<Args...>{}, std::index_sequence_for<Args...>{});
            if constexpr(!std::is_same_v<Return, void>) {
                return std::any(result);
            }
        } catch(const std::bad_any_cast& e) {
            std_lib_error("make_std_lib_function", "bad any_cast");
        }

        return std::any();
    };
}

// Example usage
double exampleFunction(int a, double b) {
    return a + b;
}

int main() {
    auto funcWrapper = make_std_lib_function(exampleFunction);
    std::vector<std::any> args = {1, 2.0};
    std::vector<std::string> argTypes = {"int", "double"}; // Not used in this simplified example

    auto result = funcWrapper(args, argTypes);
    if(result.has_value()) {
        std::cout << "Result: " << std::any_cast<double>(result) << std::endl;
    }

    return 0;
}