#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "find_datafiles.h"
char **find_datafiles(char *directory)
{
	DIR *datadir = opendir(directory);
	if(datadir == NULL)
		return NULL; // caller can check errno to get details, let's not worry about that in this function...
	struct dirent *ent = NULL;
	char **list = NULL; // Array of file names, minus directory prefix and suffix (.dat)
	size_t listi = 0, listlen = 8;
	list = malloc(sizeof(char*) * listlen);
	if(list == NULL)
		return NULL;
	while((ent = readdir(datadir)) != NULL)
	{
		size_t l = strlen(ent->d_name);
		if(!strcmp(".dat", ent->d_name + l - 4))
		{
			if(listi + 2 > listlen)
			{
				size_t l = listlen * 1.6;
				void *n = realloc(list, l);
				if(n == NULL)
					return NULL;
				list = n;
				listlen = l;
			}
			list[listi] = malloc(l - 4 + 1); // Name, without .dat and with a terminating NULL
			if(list[listi] == NULL)
				return NULL;
			strncpy(list[listi], ent->d_name, l - 4);
			listi++;
		}
	}
	closedir(datadir);
	list[listi++] = NULL;
	return list; // This will need to get free()'d by the caller!
}
