#include <stdlib.h>
#include <string.h>
#include "Command.h"

CMD_t cmd_list[] = {
    { ".exit", 5, BUILT_IN_CMD },
    { ".output", 7, BUILT_IN_CMD },
    { ".load", 5, BUILT_IN_CMD },
    { ".help", 5, BUILT_IN_CMD },
    { "insert", 6, QUERY_CMD },
    { "select", 6, QUERY_CMD },
    { "update", 6, QUERY_CMD },
    { "delete", 6, QUERY_CMD },
    { "", 0, UNRECOG_CMD },
};

Command_t* new_Command() {
    Command_t *cmd = (Command_t*) malloc(sizeof(Command_t));
    memset(cmd, 0, sizeof(Command_t));

    cmd->type = UNRECOG_CMD;
    cmd->args_len = 0;
    cmd->args_cap = 0;
    return cmd;
}

///
/// Add argument to Command_t
/// Reallocate buffer if it is full.
///
int add_Arg(Command_t *cmd, const char *arg) {
    std::string inputArg = arg;
    if (cmd->args.size() == 0)
        cmd->args_cap = 5;
    else if (cmd->args_cap == cmd->args_len)
        cmd->args_cap += 5;
    cmd->args.push_back(inputArg);
    cmd->args_len++;
    return 0;
}

///
/// Add select fields
///
int add_select_field(Command_t *cmd, const char *argument) {
    size_t fields_len = cmd->cmd_args.sel_args.fields_len;
    char **buf = (char**)malloc(sizeof(char*) * (fields_len+1));
    if (buf == NULL) {
        return 0;
    }

    if (cmd->cmd_args.sel_args.fields) {
        memcpy(buf, cmd->cmd_args.sel_args.fields, sizeof(char*) * fields_len);
        free(cmd->cmd_args.sel_args.fields);
    }

    cmd->cmd_args.sel_args.fields = buf;
    cmd->cmd_args.sel_args.fields[fields_len] = strdup(argument);
    cmd->cmd_args.sel_args.fields_len++;
    return 1;
}

///
/// Free the allocated arguments, but without free the argument buffer
///
void cleanup_Command(Command_t *cmd) {
    size_t idx;
    cmd->args.clear();
    cmd->cmd_args.sel_args.idxList.clear();
    cmd->cmd_args.sel_args.idxListLen = 0;
    if (cmd->type == SELECT_CMD) {
        for (idx = 0; idx < cmd->cmd_args.sel_args.fields_len; idx++) {
            free(cmd->cmd_args.sel_args.fields[idx]);
            cmd->cmd_args.sel_args.fields[idx] = NULL;
        }
        free(cmd->cmd_args.sel_args.fields);
        cmd->cmd_args.sel_args.fields = NULL;
        cmd->cmd_args.sel_args.fields_len = 0;
    }
    cmd->type = UNRECOG_CMD;
    cmd->args_len = 0;
}
