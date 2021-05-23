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

u_int8_t decode_byte(char value[])
{
    u_int8_t result;

    result = 0;
    for(u_int8_t i = 0; i < 8; i++)
    {
        char letter = value[i];
        if((letter >= 0x41) && (letter <= 0x5a))
        {
            result |= 0x01 << (7 - i);
        }
    }

    return result;
}


void encode_chunk(u_int64_t size, FILE *input_stream, FILE *output_stream)
{
    u_int8_t *input_data;
    char *output_data;

    input_data = (u_int8_t *)malloc(size);
    output_data = (char *)malloc(9 * size);
    fread(input_data, size, 1, input_stream);

    for(u_int64_t i = 0; i < size; i++)
    {
        u_int8_t value = input_data[i];
        char *encoded_byte = encode_byte(value);
        memcpy(&(output_data[9*i]), encoded_byte, 9);
    }

    fwrite(output_data, size, 9, output_stream);
}

// Decode a chunk of iosifbin formatted data from a file and write to the output file
// size is in number of iosifbin encoded bytes so 9 * n
void decode_chunk(u_int64_t size, FILE *input_stream, FILE *output_stream)
{
    char *input_data;
    char *encoded_byte;
    u_int8_t *output_data;

    input_data = (char *)malloc(9 * size);
    output_data = (u_int8_t *)malloc(size);
    fread(input_data, size, 9, input_stream);

    for(u_int64_t i = 0; i < size; i++)
    {
        encoded_byte = &(input_data[9 * i]);
        u_int8_t decoded_byte = decode_byte(encoded_byte);
        output_data[i] = decoded_byte;
    }

    fwrite(output_data, size, 1, output_stream);

    return;
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
        printf("Cannot open file \"%s\"\n"
        "File does not exist or insufficient permissions.\n", input_file);
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

void decode(const char *input_file, const char *output_file)
{
    FILE *input_stream;
    FILE *output_stream;
    u_int64_t input_file_size;
    u_int64_t nchunks;
    u_int64_t leftover;

    input_stream = fopen(input_file, "r");
    if(!input_stream)
    {
        printf("Cannot open file \"%s\"\n"
        "File does not exist or insufficient permissions.\n", input_file);
        exit(1);
    }

    output_stream = fopen(output_file, "w");
    if(!output_stream)
    {
        perror("fopen");
        exit(1);
    }

    input_file_size = get_file_size(input_stream);
    nchunks = (input_file_size / 9) / CHUNK_SIZE;
    leftover = (input_file_size / 9) % CHUNK_SIZE;

    for(u_int64_t i = 0; i < nchunks; i++)
    {
        decode_chunk(CHUNK_SIZE, input_stream, output_stream);
    }
    decode_chunk(leftover, input_stream, output_stream);

    fclose(input_stream);
    fclose(output_stream);

    return;
}

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
        decode(input_filename, output_filename);
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