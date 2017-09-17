/**
 * bmpldmp - a bitmap color inspection helper
 *
 * Please refer to LICENSE for legal issues.
 */

#include <stdio.h>
#include <stdint.h>

#define FILE_HEADER_SIZE 14
#define MAX_BMP_HEADER_SIZE 124

int main(int argc, char **argv) {
  uint8_t error_case = 0;

  if (argc <= 1) {
    printf("USE: bmpldmp FILE\n");
    return 0;
  }

  FILE *bmp_file = fopen(argv[1], "rb");
  if (NULL == bmp_file) {
    fprintf(stderr, "Could not open file.\n");
    return -1;
  }

  uint8_t magic_number[2] = {0};
  if (sizeof(magic_number) != fread(&magic_number, 1, sizeof(magic_number), bmp_file)) {
    error_case = 1;
    goto error;
  }

  if (0x42 != magic_number[0] || 0x4D != magic_number[1]) {
    error_case = 2;
    goto error;
  }

  uint32_t data_offset = 0;
  fseek(bmp_file, 8, SEEK_CUR);
  if (sizeof(data_offset) != fread(&data_offset, 1, sizeof(data_offset), bmp_file)) {
    error_case = 3;
    goto error;
  }

  uint32_t header_size = 0;
  if (sizeof(header_size) != fread(&header_size, 1, sizeof(header_size), bmp_file)) {
    error_case = 4;
    goto error;
  }

  if (header_size < 40) {
    printf("No. Bitmap does not support palette.\n");
    goto close;
  }

  uint16_t color_depth = 0;
  fseek(bmp_file, 10, SEEK_CUR);
  if (sizeof(color_depth) != fread(&color_depth, 1, sizeof(color_depth), bmp_file)) {
    error_case = 5;
    goto error;
  }

  uint32_t compression_setting = 0;
  if (sizeof(compression_setting) != fread(&compression_setting, 1, sizeof(compression_setting), bmp_file)) {
    error_case = 6;
    goto error;
  }

  fseek(bmp_file, 12, SEEK_CUR);
  uint32_t palette_size = 0, actual_size = 0;
  if (sizeof(palette_size) != fread(&palette_size, 1, sizeof(palette_size), bmp_file)) {
    error_case = 7;
    goto error;
  }

  if (0 == palette_size && color_depth <= 8) {
    actual_size = 1 << color_depth;
  } else {
    actual_size = palette_size;
  }

  if (0 == actual_size) {
    printf("No. There is no palette.\n");
    goto close;
  }

  uint8_t mask_list_size = 0;
  if (3 == compression_setting) {
    mask_list_size = 12;
  }

  printf("   #,   R,   G,   B\n");

  fseek(bmp_file, FILE_HEADER_SIZE + header_size + mask_list_size, SEEK_SET);
  for (uint32_t i = 0; i < actual_size; ++i) {
    uint8_t value[4] = {0};

    if (sizeof(value) != fread(&value, 1, sizeof(value), bmp_file)) {
      error_case = 8;
      goto error;
    }

    printf("%4d, %3hhu, %3hhu, %3hhu\n", i, value[2], value[1], value[0]);
  }

  goto close;

error:
  fprintf(stderr, "Bitmap parser error: %hhu\n", error_case);
  fclose(bmp_file);

  return -1;

close:
  fclose(bmp_file);

  return 0;
}
