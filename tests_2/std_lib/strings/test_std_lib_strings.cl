// String functions

print $string_len("Hello, World!"); // Output: 13
print $string_concat("Hello, ", "World!"); // Output: Hello, World!
print $string_substr("Hello, World!", 7, 5); // Output: World
print $char_at("Hello, World!", 7); // Output: W
print $replace_char("Hello, World!", 0, "J"); // Output: Jello, World!
print $string_to_vector("Hello, World!"); // Output: [H, e, l, l, o, ,,  , W, o, r, l, d, !]
print $string_split("Hello, World!", ","); // Output: [Hello,  World!]
