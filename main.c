#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "copy_ls.h"

const char *short_options = "alsh";
const struct option long_options[] = {  
	{ "all", 0, NULL, 'a'},
	{ "long", 0, NULL, 'l'},
	{ "size", 0, NULL, 's'},
	{ "help", 0, NULL, 'h'},
	{ 0, 0, 0, 0},  
};

int flag_a = 0;
int flag_l = 0;  
int flag_s = 0;

int main(int argc, char *argv[])
{ 
	struct stat st = {0};
	//memset(&st,0,sizeof(struct stat));

	int option;
	while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
	{
		switch (option) 
		{
			case 'a':
				flag_a=1;
				break;
			case 'l':
				flag_l=1;
				break;
			case 's':
				flag_s=1;
				break;
			case 'h':
				print_usage(stdout);
				break;
			case '?':
				printf("optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			default :
				printf("optopt = %c\n", (char)optopt);
				printf("opterr = %d\n", opterr);
				fprintf(stderr, "usage: %s [-t nsecs] [-n] name\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
//	printf("flag_a=%d flag_l=%d argc=%d\n", flag_a, flag_l, argc);
//	for(int i=0; i<argc; ++i)
//	{
//		printf("%s\n", argv[i]);
//	}
	
	if(argc == 1 || *argv[argc-1]=='-')
	{
		display_dir(".");
	}
	else
	{
		for(int i=1; i<argc; ++i)
		{
			stat(argv[i], &st);

			if(*argv[i] == '-')
			{
				continue;
			}
			else if((st.st_mode & S_IFMT) == S_IFDIR)
			{
				printf("%s:\n", argv[i]);
				display_dir(argv[i]);
			}	
			else if((st.st_mode & S_IFMT) == S_IFREG)
			{
				display_file(argv[i]);
			}
			else
			{
				printf("ls: 无法访问'%s': 没有那个文件或目录\n", argv[i]);
				continue;
			}	
		}
	}
	

	return 0;
}
