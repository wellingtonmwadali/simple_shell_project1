#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() to get files*/
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg:  string generated from getline containing arguements
 *@argv: array of strings generated from arg
 *@path: string path for the current cmd
 *@argc: argument count
 *@line_count:  error count
 *@err_num: error code for exit()s in lin
 *@linecount_flag: if on count then this line of input
 *@shell_name:  program filename
 *@env:a  linked list local copy of environment
 *@environ:  modified copy of environ from LL env
 *@history:  history node
 *@alias:  alias node
 *@env_changed: on if environent was changed
 *@status: return status of the last exec'd cmd
 *@cmd_buf: address of ptr to cmd_buf,  if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd:  fd from  line to read input
 *@histcount:history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *shell_name;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* ptr to cmd ; , for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - has a builtin string and related function
 *@type: the builtin cmd flag
 *@func:  function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* loop.c */
int shell_loop(info_t *info, char **av);
int locate_builtin(info_t *info);
void locate_command(info_t *info);
void execute_command(info_t *info);

/* parse.c */
char duplicate_chars(charpath_string, int begin, int end);
int executable_cmd(info_t info_struct, charfile_path);
char locate_path(info_tinfo_struct, char path_string, charcommand);

/* loophsh.c */
int loophsh(char **);

/* errors1.c */
void print_to_stderr(char *str);
int write_to_stderr(char c);
int write_to_fd(char c, int fd);
int print_to_fd(char *str, int fd);

/* string1.c */
int string_length(char *str);
int string_compare(char *str1, char *str2);
char begins_with(const char *main_str, const char *sub_str);
char string_concat(char *dest_str, char *src_str);

/*string2.c */
int write_char(char ch);
void print_str(char *input_str);
char duplicate_str(const char *input_str);
char copy_str(char *destination, char *source);

/*exit_shell.c */
char *string_copy(char *destination, char *source, int num_chars);
char *string_concat(char *dest, char *src, int num_bytes);
char *find_char(char *str, char char_to_find);

/*tow.c */
char *split_string_by_delimiter(char *str, char *delim);
char *split_string_by_char(char *str, char delim);

/* realloc.c */
char *_memset(char *s, char b, unsigned int n);
void ffree(char **pp);
void _realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/*memory_buffer*/
int free_buffer(void **ptr);

/* atoi.c */
int check_alpha(int c);
int check_delimiter(char c, char *delimiter);
int convert_to_integer(char *string);
int is_interactive(info_t *information);

/*errors1.c */
int str_to_int(char *s);
void display_error(info_t info, charerror_str);
int print_decimal(int input, int fd);
char *num_to_str(long int num, int base, int flags);
void eliminate_comments(char *buff);

/* builtin1.c */
int exit_shell(info_t *info);
int change_directory(info_t *info);
int display_help(info_t *info);

/*builtin2.c */
int display_history(info_t *info);
int remove_alias(info_t *info, char *str);
int create_alias(info_t *info, char *str);
int show_alias(list_t *node);
int manage_alias(info_t *info);

/*getline.c */
ssize_t input_buf(info_t info, char **buf, size_tlen);
ssize_t get_input(info_t *info);
ssize_t read_buf(info_t info, charbuf, size_t *t);
int _getline(info_t info, char **ptr, size_tlength);
void sigintHandler(__attribute__((unused))int sig_num);

/* get_info.c */
void clear_information(info_t *info);
void set_information(info_t *info, char **argv);
void free_information(info_t *info, int free_all);

/*environ.c */
int display_env(info_t *info);
char find_env_var(info_tinfo, const char *var_name);
int create_or_modify_env(info_t *info);
int remove_env_var(info_t *info);
int fill_env_list(info_t *info);

/*getenv.c */
char **get_environment(info_tinfo);
int unset_environment_var(info_t info, charvariable);
int set_environment_var(info_t info, charvariable, char *value);

/*history.c */
char get_history_file(info_tinfo);
int build_history_list(info_t info, charbuffer, int linecount);
int renumber_history(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);

/* lists1.c */
list_t insert_node_begin(list_t **head_ptr, const charinput_str, int index);
list_t insert_node_end(list_t **head_ptr, const charinput_str, int index);
size_t display_list_str(const list_t *head);
int remove_node_at_index(list_t **head_ptr, unsigned int index);
void release_list(list_t **head_ptr);

/* lists2.c */
size_t linked_list_length(const list_t *first_node);
char **linked_list_to_string_array(list_thead);
size_t display_linked_list(const list_t *first_node);
list_t find_node_prefix(list_tcurrent_node, char *prefix, char next_char);
ssize_t find_node_index(list_t list_head, list_ttarget_node);

/* vars.c */
int test_chain_delimeter(info_t info, charbuf, size_t *p);
void verify_chain(info_t info, charbuf, size_t *p, size_t i, size_t len);
int substitute_alias(info_t *info);
int substitute_vars(info_t *info);
int swap_string(char *old, char *new);

#endif
