#include <stdio.h>
#include <stdlib.h>

char start_string[] = "vpn=0x10514";
char stop_string[] = "vpn=0x105c2";

long long unsigned filter(FILE* original, FILE* filtered);

int main()
{
	FILE* original = fopen("filtered.txt", "r");
	FILE* filtered = fopen("tmp1.txt", "w");
	long long unsigned byte_filtered = 0;
	if(!original | !filtered)
	{
		printf("Error in opening or creating the file!");
		return -1;
	}
	
	byte_filtered = filter(original, filtered);
	printf("Filtered: %llu B\n", byte_filtered);
	
	fclose(original);
	fclose(filtered);
	return 0;
}


long long unsigned filter(FILE* original, FILE* filtered)
{
	long long written = 0;
	long long original_size = 0;
	int found = 1;
	int filter_flag = 0;
	char curr_char = fgetc(original);
	
	while(curr_char!=EOF)
	{
		++original_size;
		if(filter_flag)
		{
			if(curr_char=='(')
			{
				found=1;
				for(int i=0; i<sizeof(stop_string)-1 && found; i++)
				{
					curr_char = fgetc(original);
					if(curr_char!=stop_string[i])
						found=0;
				}
				if(found)
				{
					filter_flag=0;
					//printf("Fine pattern\n");
					while(fgetc(original)!='\n');
					fprintf(filtered, "Finita la funzione do_stuff()...\n");
				}
			}
		}
		else if(curr_char=='(')
		{
			fputc(curr_char, filtered);
			++written;
			found=1;
			for(int i=0; i<sizeof(start_string)-1 && found; i++)
			{
				curr_char = fgetc(original);
				fputc(curr_char, filtered);
				++written;
				if(curr_char!=start_string[i])
					found=0;
			}
			if(found)
			{
				filter_flag=1;
				//printf("Pattern trovato!\n");
				fprintf(filtered, "\nInizia la funzione do_stuff()...\n");
			}
		}
		else
		{
			fputc(curr_char, filtered);
			++written;
		}
		curr_char = fgetc(original);
	}
	
	return original_size-written;
}
