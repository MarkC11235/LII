import sys

def generate_header(output_path):
    with open(output_path, 'w') as f:
        f.write('#ifndef EMBEDDED_FONT_HPP\n')
        f.write('#define EMBEDDED_FONT_HPP\n\n')
        f.write('extern const unsigned char embedded_font[];\n')
        f.write('extern const unsigned int embedded_font_size;\n\n')
        f.write('#endif\n')

def generate_source(font_data, output_path):
    with open(output_path, 'w') as f:
        f.write('#include "embedded_font.hpp"\n\n')
        f.write('const unsigned char embedded_font[] = {')
        for i, byte in enumerate(font_data):
            # if i % 12 == 0:
            #     f.write('\n    ')
            f.write(f'0x{byte:02x}, ')
        f.write('\n};\n\n')
        f.write(f'const unsigned int embedded_font_size = {len(font_data)};\n')

def main():
    if len(sys.argv) != 4:
        print('Usage: python ttf_to_cpp.py input.ttf output.cpp output.hpp')
        sys.exit(1)
    
    ttf_path = sys.argv[1]
    cpp_path = sys.argv[2]
    hpp_path = sys.argv[3]
    
    # Read font data
    with open(ttf_path, 'rb') as f:
        font_data = f.read()
    
    # Generate files
    generate_header(hpp_path)
    generate_source(font_data, cpp_path)

if __name__ == '__main__':
    main()