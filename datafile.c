#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <lctp.h>
#include "datafile.h"
const char const *datadir = "C:\\ProgramData\\Clockio\\data\\";
const char const *suffix = ".dat";
int write_time_entry(char *employeename, char *prefix)
{
	if(employeename == NULL || prefix == NULL)
		return EINVAL;
	char *filename = malloc(strlen(datadir) + strlen(employeename) + strlen(suffix) + 1);
	if(filename == NULL)
		return EINVAL;
	strcpy(filename, datadir);
	strcat(filename, employeename);
	strcat(filename, suffix);
	FILE *datfile = fopen(filename, "a+");
	free(filename);
	if(datfile == NULL)
		return errno;
	char timeentry[28];
	time_t t = time(NULL);
	struct tm *tmp = localtime(&t);
	strftime(timeentry, sizeof(timeentry), "   %m/%d/%Y        %H:%M\n", tmp);
	fputs(prefix, datfile);
	fputs(timeentry, datfile);
	fflush(datfile);
	fclose(datfile);
	return 0;
}

char *get_status(char *employeename)
{
	char *err = {"(error)"};
	if(employeename == NULL)
		return err;
	char *filename = malloc(strlen(datadir) + strlen(employeename) + strlen(suffix) + 1);
	if(filename == NULL)
		return err;
	strcpy(filename, datadir);
	strcat(filename, employeename);
	strcat(filename, suffix);
	FILE *datfile = fopen(filename, "a+");
	free(filename);
	if(datfile == NULL)
		return err;
	if(fseek(datfile, -LCTP_DATA_LINE_LEN, SEEK_END) < 0)
		return err;
	char buf[LCTP_DATA_LINE_LEN+1];
	if(fgets(buf, LCTP_DATA_LINE_LEN, datfile) == NULL)
		return err;
	fclose(datfile);
	return lctp_getstatus(buf);
}
