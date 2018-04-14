#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_image_data_offset(FILE* bmp_offset) {
	fseek(bmp_offset, 10, 0);
	int offset;
	offset = (int)fgetc(bmp_offset);
	return offset;
}

int main(int argc, char** argv){
	FILE *message_handle;
	FILE *hidden_message_handle;

	if(argc != 4) {
		printf("LSB substitution\nYou should type something like this: <message_handle> <hidden_message_handle> <key>\n");
        exit(1);
	}

	message_handle = fopen(argv[1], "r");
	if (message_handle == NULL){
		printf("Can't open input file %s\n", argv[1]);
	}

	/*Must be checking of the key. I haven't done it yet*/

	hidden_message_handle = fopen(argv[2], "w");
	if (message_handle == NULL) {
		printf("Can't open text output file %s\n", argv[2]);
		exit(1);
	}

	int offset = get_image_data_offset(message_handle);

	rewind(message_handle);

	fseek(message_handle, offset, 0);

	/* Grab LSB of all bytes for length specified at fgetc */
	int message_length = fgetc(message_handle);
	char message_buffer;
	for(int i = 0; i < message_length; i++) {
		char temp_ch = '\0';
		for(int j = 0; j < 8 ; j++) {
			temp_ch = temp_ch << 1;
			message_buffer = fgetc(message_handle);
			int file_byte_lsb = message_buffer & 1; 
			temp_ch |= file_byte_lsb;
		}
		fputc(temp_ch, hidden_message_handle);
	}
	fclose(message_handle);	
	fclose(hidden_message_handle);
    return 0;
}
