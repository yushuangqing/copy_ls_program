#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <grp.h>         //getgrgid()
#include <pwd.h>         //getpwuid()
#include "copy_ls.h"
extern int flag_a;
extern int flag_l;  
extern int flag_s;
void display_dir(char *dirname)
{
    DIR *dir;
    struct dirent *dirent;

    char buf[1024];
	
    dir = opendir(dirname);
	if(dir == NULL)
	{
		perror("opendir");
		return;
	}
    while((dirent = readdir(dir)) != NULL)
    {
        strcpy(buf, dirname);
        strcat(buf, "/");
        strcat(buf, dirent->d_name);
		if(flag_a == 1)
		{
			display_file(buf);
		}
		else
		{
			if(dirent->d_name[0] == '.')
				continue;
			else
				display_file(buf);
		}
    }
	printf("\n");
	closedir(dir);
}


void display_file(char *filename)
{
	char *fn = NULL;
	int foreground = 0;
	int background = 1;
	int colormode = 0;
	
	
	if(flag_l == 0)
	{
		struct stat st = {0};
		//memset(&st,0,sizeof(struct stat));
		stat(filename, &st);
		
		if(flag_s == 1)
			printf("%3d ", (int)(st.st_blocks)/2);
		
		switch(st.st_mode & S_IFMT)    
		{
			case S_IFREG:  
			{
				if((st.st_mode & S_IXUSR) == S_IXUSR)
					foreground = 32;  
				break;
			}
			case S_IFDIR:  
			{
				foreground = 34;    
				break;
			}
			default : printf("没有此类文件");
		}
		
		fn = filename+strlen(filename)-1;
		while(*fn != '/')
		{
			fn--;
		}
		fn++;
		
		printf("\033[%d;%d;%dm%s  \033[0m", colormode, foreground, background, fn);		
	}
	else
	{
		struct stat st = {0};
		//memset(&st,0,sizeof(struct stat));
		
		struct passwd *pw;
		struct group *gr;
		struct tm *tm;
		
		stat(filename, &st);
		
		if(flag_s == 1)
			printf("%3d ", (int)(st.st_blocks)/2);
		
		switch(st.st_mode & S_IFMT)    
		{
			case S_IFREG:  
			{
				if((st.st_mode & S_IXUSR) == S_IXUSR)
					foreground = 32;
				printf("-");    
				break;
			}
			case S_IFDIR:  
			{
				foreground = 34;
				printf("d");    
				break;
			}
			
			case S_IFLNK:  printf("l");    break;
			case S_IFBLK:  printf("b");    break;
			case S_IFCHR:  printf("c");    break;
			case S_IFIFO:  printf("p");    break;
			case S_IFSOCK: printf("s");    break;
			default : printf("没有此类文件");
		}

		for(int i = 8; i >= 0; i--)
		{    
			if(st.st_mode & (1 << i))
			{
				switch(i%3)
				{
				case 2: printf("r"); break;
				case 1: printf("w"); break;
				case 0: printf("x"); break;
				default : printf("没有此类权限");
				}
			}
			else
				printf("-");
		}

		pw = getpwuid(st.st_uid);
		gr = getgrgid(st.st_gid);

		printf("%3d %s %s %5ld", (int)st.st_nlink, pw->pw_name, gr->gr_name, st.st_size);

		tm = localtime(&st.st_ctime);
		printf(" %04d-%02d-%02d %02d:%02d",tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
		
		char *fn = filename+strlen(filename)-1;
		while(*fn != '/')
		{
			fn--;
		}
		fn++;
		printf("\033[%d;%d;%dm %s\033[0m\n", colormode, foreground, background, fn);
	}		
    
}

void print_usage(FILE *stream)  
{  
	printf("用法：ls [选项]... [文件]...\n"); 
    fprintf(stream, 
            "-a  --all   不隐藏任何以. 开始的项目\n"  
            "-l          使用较长格式列出信息\n" 
			"-s  --size  以块数形式显示每个文件分配的尺寸\n"
			"-h  --help  显示此帮助信息并退出\n"
			);  
    exit(0);  
}

