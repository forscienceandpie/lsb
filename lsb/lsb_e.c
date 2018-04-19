#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_bit(char _byte,int which_bit){
	return((_byte >> 8 - (which_bit + 1)) & 1);
}

int get_image_data_offset(FILE *bmp_offset){
	fseek(bmp_offset, 10, 0);
	int offset;
	offset = (int)fgetc(bmp_offset);
	return offset;
}

int get_message_length(FILE *fp){
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return(size);
}

int main(int argc,char** argv){

	FILE *file_handle;
	FILE *message_handle;
	FILE *hidden_message_handle;

	if(argc != 4){
		printf("LSB substitution\nYou should type something like this: <source file> <output file> <text file>\n");
		exit(1);
	}

	file_handle = fopen(argv[1], "r");
	if (file_handle == NULL){
		printf("Can't open input file %s\n", argv[1]);
		exit(1);
	}

	hidden_message_handle = fopen(argv[2], "w");
	if (hidden_message_handle == NULL){
		printf("Can`t create output file %s\n", argv[2]);
		exit(1);
	}
	
	fseek(file_handle, 28, SEEK_SET);
	int depth = (int)fgetc(file_handle);
	if(depth < 24){
		printf("Too depressive colors for me. Try anything else, please.\n");
		return 1;
	} 
	rewind(file_handle);
	
	char buf_sig_cpy;
	int offset = get_image_data_offset(file_handle);
	rewind(file_handle);

	for(int i = 0; i < offset; i++){
		buf_sig_cpy = fgetc(file_handle);
		fputc(buf_sig_cpy, hidden_message_handle);
	}

	// Made file as .bmp

	char file_buffer; 			// Temp variable for one byte from file
	char message_buffer;		// Temp buffer for one byte of message

	message_handle = fopen(argv[3], "r");
	if (message_handle == NULL){
		printf("Can't open text input file %s\n", argv[3]);
		exit(1);
	}
	int hidden_message_length = get_message_length(message_handle);

	do {
		int bit_of_message;

		if(!feof(message_handle)){		
			message_buffer = fgetc(message_handle);
			for(int i = 0; i < 8; i++){  //Do this for every bit in every byte of the original text
				file_buffer = fgetc(file_handle);
				int file_byte_lsb = file_buffer & 1; 

				bit_of_message = get_bit(message_buffer, i);

				if(file_byte_lsb == bit_of_message){
					fputc(file_buffer, hidden_message_handle);
				}
				else{ 
					if(file_byte_lsb == 0)
						file_buffer = (file_buffer | 1);
					else
						file_buffer = (file_buffer & 254);
					//  flip the LSB bit of file_buffer and put it into a file with putc()
					fputc(file_buffer, hidden_message_handle);
				}
			}
		}
		else{
			buf_sig_cpy = fgetc(file_handle);
			fputc(buf_sig_cpy, hidden_message_handle);
		}
	} while(!feof(file_handle));

	fclose(message_handle);
	fclose(file_handle);
	fclose(hidden_message_handle);
}
