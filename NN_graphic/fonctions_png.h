void read_png_file(char *filename, int *width, int *height, png_byte *color_type, png_byte *bit_depth, png_bytep **row_pointers);
void write_png_file(char *filename, int width, int height, png_byte color_type, png_byte bit_depth, png_bytep *row_pointers) ;
void process_png_file(int width, int height, png_bytep *row_pointers);