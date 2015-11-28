static char* current_directory;
pid_t pid;
pid_t pid2;

void what_to_do(char * args[]);
void regular_commands(char *args[]);
void multiple_commands(char *args[]);
void change_directory(char *args[]);
void simple_redirection(char *args[]);
void simple_piping(char *args[]);
