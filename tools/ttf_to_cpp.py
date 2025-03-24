import sys

def convert_ttf_to_cpp(ttf_path, output_path):
    with open(ttf_path, 'rb') as f:
        font_data = f.read()
    
    with open(output_path, 'w') as f:
        f.write('// Auto-generated file from ttf_to_cpp.py\n')
        f.write('#include "embedded_font.hpp"\n\n')
        f.write('const unsigned char embedded_font[] = {')
        
        for i, byte in enumerate(font_data):
            if i % 12 == 0:
                f.write('\n    ')
            f.write(f'0x{byte:02x}, ')
        
        f.write('\n};\n\n')
        f.write(f'const unsigned int embedded_font_size = {len(font_data)};\n')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python ttf_to_cpp.py input.ttf output.cpp')
        sys.exit(1)
    
    convert_ttf_to_cpp(sys.argv[1], sys.argv[2])