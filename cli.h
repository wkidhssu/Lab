#ifndef CSVSTAT_CLI_H
#define CSVSTAT_CLI_H

enum Command {
    CMD_HELP,    
    CMD_SCHEMA, 
    CMD_STATS    
};

int cli_parse_args(int argc, char **argv, enum Command *command, const char **file_name);

// Печатает краткую справку в stdout.
void cli_print_help(void);

#endif
