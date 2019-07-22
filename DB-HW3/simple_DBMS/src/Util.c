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
#include "SelectState.h"
#include "DeleteState.h"
#include "UpdateState.h"

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
void print_user(User_t *user, SelectArgs_t *sel_args) {
    size_t idx;
    printf("(");
    for (idx = 0; idx < sel_args->fields_len; idx++) {
        if (!strncmp(sel_args->fields[idx], "*", 1)) {
            printf("%d, %s, %s, %d", user->id, user->name, user->email, user->age);
        } else {
            if (idx > 0) printf(", ");

            if (!strncmp(sel_args->fields[idx], "id", 2)) {
                printf("%d", user->id);
            } else if (!strncmp(sel_args->fields[idx], "name", 4)) {
                printf("%s", user->name);
            } else if (!strncmp(sel_args->fields[idx], "email", 5)) {
                printf("%s", user->email);
            } else if (!strncmp(sel_args->fields[idx], "age", 3)) {
                printf("%d", user->age);
            }
        }
    }
    printf(")\n");
}

///
/// Print the users for given offset and limit restriction
///
void print_users(Table_t *table, std::vector<size_t> idxList, size_t idxListLen, Command_t *cmd) {
    size_t idx;
    int limit = cmd->cmd_args.sel_args.limit;
    int offset = cmd->cmd_args.sel_args.offset;

    if (offset == -1)
        offset = 0;
    
    if (limit == -1 && table->aggreResults.size() > 0)
        limit = 1;

    if (table->aggreResults.size() > 0) {
        if (offset == 0 && limit > 0) {
            printf("(");
            size_t idx;
            for (idx = 0; idx < table->aggreResults.size(); idx++) {
                if (idx > 0) printf(", ");
                
                if (table->aggreTypes[idx] == "avg") {
                    double output = atof(table->aggreResults[idx].c_str());
                    printf("%.3f", output);
                } else {
                    int output = atoi(table->aggreResults[idx].c_str());
                    printf("%d", output);
                }
            }
            printf(")\n");
        }
    } else if (idxList.empty() && idxListLen == 1) {
        return;
    } else if (idxListLen != 0) {
        for (idx = offset; idx < idxListLen; idx++) {
            if (limit != -1 && ((int)idx - offset) >= limit) {
                break;
            }
            print_user(get_User(table, idxList[idx]), &(cmd->cmd_args.sel_args));
        }
    } else {
        for (idx = offset; idx < table->len; idx++) {
            if (limit != -1 && ((int)idx - offset) >= limit) {
                break;
            }
            print_user(get_User(table, idx), &(cmd->cmd_args.sel_args));
        }
    }
}

///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " ,\n");
    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " ,\n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
void handle_builtin_cmd(Table_t *table, Command_t *cmd, State_t *state) {
    if (cmd->args[0] == ".exit") {
        archive_table(table);
        exit(0);
    } else if (cmd->args[0] == ".output") {
        if (cmd->args_len == 2) {
            if (cmd->args[1] == "stdout") {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1].c_str(), 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
                __fpurge(stdout); //This is used to clear the stdout buffer
            }
        }
    } else if (cmd->args[0] == ".load") {
        if (cmd->args_len == 2) {
            char* filename = (char*) cmd->args[1].c_str();
            load_table(table, filename);
        }
    } else if (cmd->args[0] == ".help") {
        print_help_msg();
    }
}

///
/// Handle query type commands
/// Return: command type
///
int handle_query_cmd(Table_t *table, Command_t *cmd) {
    if (cmd->args[0] == "insert") {
        handle_insert_cmd(table, cmd);
        return INSERT_CMD;
    } else if (cmd->args[0] == "select") {
        handle_select_cmd(table, cmd);
        return SELECT_CMD;
    } else if (cmd->args[0] == "update") {
        handle_update_cmd(table, cmd);
        return UPDATE_CMD;
    } else if (cmd->args[0] == "delete") {
        handle_delete_cmd(table, cmd);
        return DELETE_CMD;
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
    cmd->type = SELECT_CMD;
    field_state_handler(cmd, 1, table);
    
    print_users(table, cmd->cmd_args.sel_args.idxList, cmd->cmd_args.sel_args.idxListLen, cmd);
    return table->len;
}

///
/// The return value is the number of rows updated from table
/// If the update operation success, then change the input arg
/// `cmd->type` to UPDATE_CMD
///
int handle_update_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = UPDATE_CMD;
    field_update_handler(cmd, 1, table);
    return table->len;
}

///
/// The return value is the number of rows deleted from table
/// If the delete operation success, then change the input arg
/// `cmd->type` to DELETE_CMD
///
int handle_delete_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = DELETE_CMD;
    size_t originalTableLen = table->len;
    field_delete_handler(cmd, 1, table);
    return originalTableLen - table->len;
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

