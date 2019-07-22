#include <string.h>
#include <stdlib.h>
#include "DeleteState.h"
#include "WhereConditions.h"
#include <vector>

void field_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;
    cmd->cmd_args.sel_args.idxList.clear();
    cmd->cmd_args.sel_args.idxListLen = 0;
    while(arg_idx < cmd->args_len) {
        if (cmd->args[arg_idx] == "from") {
            table_delete_handler(cmd, arg_idx+1, table);
            return;
        } else {
            cmd->type = UNRECOG_CMD;
            return;
        }
        arg_idx += 1;
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void table_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len && (cmd->args[arg_idx] == "table")) {

        arg_idx++;
        if (arg_idx == cmd->args_len) {
            table->users.clear();
            table->len = 0;
            return;
        } else if (cmd->args[arg_idx] == "where") {
            where_delete_handler(cmd, arg_idx+1, table);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void where_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len) {
        std::string conditions = cmd->args[arg_idx];
        size_t next_idx;
        for (next_idx = arg_idx + 1; next_idx < cmd->args_len; next_idx++)
            conditions += cmd->args[next_idx];
        WhereConditions whereConditions(conditions);
        
        int idx;
        for (idx = 0; (size_t)idx < table->len; idx++) {
            User_t *user = get_User(table, idx);
            if (whereConditions.getResult(user)) {
                table->users.erase(table->users.begin()+idx);
                idx--;
                table->len--;
            }
        }
        
        if (next_idx == cmd->args_len) {
            return;
        } 
    }
    cmd->type = UNRECOG_CMD;
    return;
}
