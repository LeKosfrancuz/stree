#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define CYAN "\x1B[1;34m"
#define WHITE "\x1B[0;37m"

#define INTERSECT "\u251C\u2500"
#define LAST_INTERSECT "\u2514\u2500"
#define LINE "\u2502"

#define INDENT 4

void print_content_of_dir(char *path, const int padding);
char *append_to_path(char *path, char *suffix);

int main(int argc, char **argv) {

	char* pwd = getenv("PWD");
	char* path = malloc(strlen(pwd) + 1);

	if (argc > 1) {
		path = realloc(path, strlen(argv[1]) + 1);
		strcpy(path, argv[1]);
	} else {
		strcpy(path, pwd);
	}

	printf(CYAN"\n%s/\n", path);
	print_content_of_dir(path, strlen(path));
	printf("\n");
}

size_t count_files_of_dir(char *path) {

	size_t file_count = 0;

	DIR *dp;
	struct dirent *dir_ent;

	dp = opendir(path);
	while ((dir_ent = readdir(dp)) != NULL) {
	    if (dir_ent->d_type == DT_REG) {
		 file_count++;
	    }
	}
	closedir(dp);

	return file_count;
}

void print_content_of_dir(char *path, const int padding) {

	DIR *dp;
	struct dirent *dir_ent;
	size_t file_count = count_files_of_dir(path);
	dp = opendir(path);

	while ((dir_ent = readdir(dp)) != NULL) {
		switch(dir_ent->d_type) {
			case DT_DIR: {
				if (dir_ent->d_name[0] == '.') {
					if (dir_ent->d_name[1] == '.' || dir_ent->d_name[1] == '\0') continue;
				}

				printf(CYAN"%*s"LINE"\n%*s"LAST_INTERSECT" %s"WHITE"\n", padding, "", padding, "", dir_ent->d_name);
				print_content_of_dir(append_to_path(path, dir_ent->d_name), padding + INDENT);

			} break;
			case DT_REG: {
				printf("%*s", padding, "");

				if (file_count > 1) {
					printf(CYAN INTERSECT WHITE" %s\n", dir_ent->d_name);
				} else {
					printf(CYAN LAST_INTERSECT WHITE" %s\n", dir_ent->d_name);
				}
				file_count--;
			} break;
		}
	}
	closedir(dp);
	free(path);

	return;
}

char *append_to_path(char *path, char *suffix) {

	int path_len = strlen(path);
	char* result = malloc( path_len + strlen(suffix) + 2);
	strcpy(result, path);
	result[path_len] = '/';
	strcpy(result + path_len + 1, suffix);

	return result;
}
