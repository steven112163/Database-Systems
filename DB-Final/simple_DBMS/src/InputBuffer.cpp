#include <stdio.h>
#include <stdlib.h>
#include "InputBuffer.h"

///
/// Allocate InputBuffer_t and initialize some attributes
/// Return: ptr of InputBuffer_t
///
//InputBuffer_t* new_InputBuffer() {
//    auto *input_buffer = (InputBuffer_t *) malloc(sizeof(InputBuffer_t));
//    input_buffer->buffer = nullptr;
//    input_buffer->buffer_len = 0;
//    input_buffer->input_len = 0;
//    return input_buffer;
//}

///
/// Read the input from stdin, then store into InputBuffer_t
///
void read_input(char *input_buffer) {
    size_t bufsize = 200;
    getline(&input_buffer, &bufsize, stdin);
}

///
/// Free the allocated buffer to store input string
///
//void clean_InputBuffer(InputBuffer_t *input_buffer) {
//    free(input_buffer->buffer);
//    input_buffer->buffer = nullptr;
//    input_buffer->buffer_len = 0;
//    input_buffer->input_len = 0;
//}
