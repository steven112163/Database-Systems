#include <string.h>
#include <stdlib.h>
#include "DeleteState.h"
#include "WhereConditions.h"

void field_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    cmd->cmd_args.sel_args.fields.clear();
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
    }
    cmd->type = UNRECOG_CMD;
}

void table_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len && (cmd->args[arg_idx] == "user")) {

        arg_idx++;
        if (arg_idx == cmd->args_len) {
            table->users.clear();
            return;
        } else if (cmd->args[arg_idx] == "where") {
            where_delete_handler(cmd, arg_idx+1, table);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
}

void where_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len) {
        std::vector<std::string> conditions;
        for (; arg_idx < cmd->args_len; arg_idx++)
            conditions.push_back(cmd->args[arg_idx]);
        WhereConditions whereConditions(conditions);
        
        int idx;
        for (idx = 0; (size_t)idx < table->users.size(); idx++) {
            User_t *user = get_User(table, idx);
            if (whereConditions.getResult(user)) {
                table->users.erase(table->users.begin() + idx);
                idx--;
            }
        }
        
        if (arg_idx == cmd->args_len) {
            return;
        } 
    }
    cmd->type = UNRECOG_CMD;
}
