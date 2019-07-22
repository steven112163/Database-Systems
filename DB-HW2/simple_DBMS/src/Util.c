#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Util.h"
#include "Command.h"
#include "Table.h"

///
/// Allocate State_t and initialize some attributes
/// Return: ptr of new State_t
///
State_t* new_State() {
    State_t *state = (State_t*)malloc(sizeof(State_t));
    state->saved_stdout = -1;
    return state;
}

///
/// Print shell prompt
///
void print_prompt(State_t *state) {
    if (state->saved_stdout == -1) {
        printf("db > ");
    }
}

///
/// Print the user in the specific format
///
void print_user(User_t *user, char **columns, size_t col_len) {
    if (col_len == 0)
        printf("(%d, %s, %s, %d)\n", user->id, user->name, user->email, user->age);
    else {
        printf("(");
        for (size_t i = 0; i < col_len; i++) {
            if (strncmp(columns[i], "id", 2) == 0)
                printf("%d", user->id);
            else if (strncmp(columns[i], "name", 4) == 0)
                printf("%s", user->name);
            else if (strncmp(columns[i], "email", 5) == 0)
                printf("%s", user->email);
            else
                printf("%d", user->age);
            
            if (i != col_len - 1)
                printf(", ");
        }
        printf(")\n");
    }
}

///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " \n");
    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " \n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
void handle_builtin_cmd(Table_t *table, Command_t *cmd, State_t *state) {
    if (!strncmp(cmd->args[0], ".exit", 5)) {
        archive_table(table);
        exit(0);
    } else if (!strncmp(cmd->args[0], ".output", 7)) {
        if (cmd->args_len == 2) {
            if (!strncmp(cmd->args[1], "stdout", 6)) {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1], 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
                __fpurge(stdout); //This is used to clear the stdout buffer
            }
        }
    } else if (!strncmp(cmd->args[0], ".load", 5)) {
        if (cmd->args_len == 2) {
            load_table(table, cmd->args[1]);
        }
    } else if (!strncmp(cmd->args[0], ".help", 5)) {
        print_help_msg();
    }
}

///
/// Handle query type commands
/// Return: command type
///
int handle_query_cmd(Table_t *table, Command_t *cmd) {
    if (!strncmp(cmd->args[0], "insert", 6)) {
        handle_insert_cmd(table, cmd);
        return INSERT_CMD;
    } else if (!strncmp(cmd->args[0], "select", 6)) {
        handle_select_cmd(table, cmd);
        return SELECT_CMD;
    } else {
        return UNRECOG_CMD;
    }
}

///
/// The return value is the number of rows insert into table
/// If the insert operation success, then change the input arg
/// `cmd->type` to INSERT_CMD
///
int handle_insert_cmd(Table_t *table, Command_t *cmd) {
    int ret = 0;
    User_t *user = command_to_User(cmd);
    if (user) {
        int found = 0;
        for (size_t i = 0; i < table->len; i++) {
            User_t *duplicate = get_User(table, i); // Find duplicate userId
            if (duplicate) {
                if (duplicate->id == user->id) {
                    found = 1;
                    break;
                }
            }
        }
        if (found == 0)
        	ret = add_User(table, user);
        if (ret > 0) {
            cmd->type = INSERT_CMD;
        }
    }
    return ret;
}

///
/// The return value is the number of rows select from table
/// If the select operation success, then change the input arg
/// `cmd->type` to SELECT_CMD
///
int handle_select_cmd(Table_t *table, Command_t *cmd) {
    size_t idx;
    size_t limit = table->len;
    size_t offset = 0;
    char **columns = (char **) malloc(sizeof(char*) * 4);
    size_t col_len = 0;
    for (size_t i = 0; i < cmd->args_len; i++) {
        if (strncmp(cmd->args[i], "limit", 5) == 0) // Get limit
            limit = atoi(cmd->args[i + 1]);
        else if (strncmp(cmd->args[i], "offset", 6) == 0) // Get offset
            offset = atoi(cmd->args[i + 1]);
        else if (strncmp(cmd->args[i], "select", 6) == 0) { // Get selected fields
            size_t j = i + 1;
            for (; j < cmd->args_len && j < i + 5; j++) {
                if ((strncmp(cmd->args[j], "id", 2) == 0) || (strncmp(cmd->args[j], "name", 4) == 0) ||
                    (strncmp(cmd->args[j], "email", 5) == 0) || (strncmp(cmd->args[j], "age", 3) == 0)) {
                    columns[col_len] = strdup(cmd->args[j]);
                    col_len++;
                } else
                    break;
            }
            i = j - 1;
        }
    }
    for (idx = offset; idx < table->len && idx < limit; idx++) {
        print_user(get_User(table, idx), columns, col_len);
    }
    cmd->type = SELECT_CMD;
    free(columns);
    return table->len;
}

///
/// Show the help messages
///
void print_help_msg() {
    const char msg[] = "# Supported Commands\n"
    "\n"
    "## Built-in Commands\n"
    "\n"
    "  * .exit\n"
    "\tThis cmd archives the table, if the db file is specified, then exit.\n"
    "\n"
    "  * .output\n"
    "\tThis cmd change the output strategy, default is stdout.\n"
    "\n"
    "\tUsage:\n"
    "\t    .output (<file>|stdout)\n\n"
    "\tThe results will be redirected to <file> if specified, otherwise they will display to stdout.\n"
    "\n"
    "  * .load\n"
    "\tThis command loads records stored in <DB file>.\n"
    "\n"
    "\t*** Warning: This command will overwrite the records already stored in current table. ***\n"
    "\n"
    "\tUsage:\n"
    "\t    .load <DB file>\n\n"
    "\n"
    "  * .help\n"
    "\tThis cmd displays the help messages.\n"
    "\n"
    "## Query Commands\n"
    "\n"
    "  * insert\n"
    "\tThis cmd inserts one user record into table.\n"
    "\n"
    "\tUsage:\n"
    "\t    insert <id> <name> <email> <age>\n"
    "\n"
    "\t** Notice: The <name> & <email> are string without any whitespace character, and maximum length of them is 255. **\n"
    "\n"
    "  * select\n"
    "\tThis cmd will display all user records in the table.\n"
    "\n";
    printf("%s", msg);
}

