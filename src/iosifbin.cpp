#include "iosifbin.hpp"

#define CHUNK_SIZE 1024

u_int64_t get_file_size(FILE *file)
{
    u_int64_t size;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, 0);

    return size;
}

char *encode_byte(u_int8_t value)
{
    static char result[9];
    memcpy(result, "iosifbin ", 9);

    for(int i = 0; i < 8; i++)
    {
        u_int8_t bit = (value >> (7 - i)) & 0x01;
        result[i] -= bit * 0x20;
    }

    return result;
}

void encode_chunk(u_int64_t size, FILE *input_stream, FILE *output_stream)
{
    u_int8_t *input_data;

    input_data = (u_int8_t *)malloc(size);
    fread(input_data, size, 1, input_stream);

    for(u_int64_t i = 0; i < size; i++)
    {
        u_int8_t value = input_data[i];
        char *encoded_byte = encode_byte(value);
        fwrite(encoded_byte, 9, 1, output_stream);
    }
}

void encode(const char *input_file, const char *output_file)
{
    FILE *input_stream = fopen(input_file, "r");
    FILE *output_stream = fopen(output_file, "w");
    u_int64_t input_file_size;
    u_int64_t nchunks;
    u_int64_t leftover;

    if(!input_stream)
    {
        printf("File \"%s\" does not exist.\n", input_file);
        exit(1);
    }
    if(!output_stream)
    {
        perror("fopen");
        exit(1);
    }

    input_file_size = get_file_size(input_stream);
    nchunks = input_file_size / CHUNK_SIZE;
    leftover = input_file_size % CHUNK_SIZE;

    for(u_int64_t i = 0; i < nchunks; i++)
    {
        encode_chunk(CHUNK_SIZE, input_stream, output_stream);
    }

    encode_chunk(leftover, input_stream, output_stream);

    fclose(input_stream);
    fclose(output_stream);

    return;
}

void decode(const char *input_file, const char *output_file);

int main(int argc, char *argv[])
{
    const char *usage_menu = "Usage: \n"
        "\t./iosifbin [encode|decode] input-filename output-filename\n";

    if(argc != 4)
    {
        puts(usage_menu);
        return 1;
    }

    char *option = argv[1];
    char *input_filename = argv[2];
    char *output_filename = argv[3];

    if(!strcmp(option, "encode"))
    {
        encode(input_filename, output_filename);
    }
    else if(!strcmp(option, "decode"))
    {
        // To do.
        //decode(input_filename, output_filename);
        return 0;
    }
    else
    {
        puts("Invalid option.\n");
        puts(usage_menu);
        return 1;
    }

    return 0;
}