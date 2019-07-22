#include <string.h>
#include <stdlib.h>
#include "UpdateState.h"
#include "WhereConditions.h"

void field_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    cmd->cmd_args.sel_args.fields.clear();
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;
    cmd->cmd_args.sel_args.idxList.clear();
    cmd->cmd_args.sel_args.idxListLen = 0;
    while(arg_idx < cmd->args_len) {
        if (cmd->args[arg_idx] == "user") {
            set_update_handler(cmd, arg_idx+1, table);
            return;
        } else {
            cmd->type = UNRECOG_CMD;
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
}

void set_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len && (cmd->args[arg_idx] == "set")) {

        arg_idx++;
        std::vector<std::string> setCondition;
        for (; arg_idx < cmd->args_len; arg_idx++) {
            if (cmd->args[arg_idx] == "where") break;
            setCondition.push_back(cmd->args[arg_idx]);
        }
        
        std::string field = setCondition[0];
        std::string data = setCondition[2];
        
        if (arg_idx == cmd->args_len) {
            if (field == "id")
                return;
            else if (field == "age") {
                unsigned int age = std::stoi(data);
                size_t idx, len = table->users.size();
                for (idx = 0; idx < len; idx++) {
                    User_t *user = get_User(table, idx);
                    user->age = age;
                }
            } else {
                size_t idx;
                size_t len = table->users.size();
                for (idx = 0; idx < len; idx++) {
                    User_t *user = get_User(table, idx);
                    if (field == "name") {
                        memset(user->name, 0, MAX_USER_NAME+1);
                        strncpy(user->name, data.c_str(), MAX_USER_NAME);
                    }
                    else {
                        memset(user->email, 0, MAX_USER_EMAIL+1);
                        strncpy(user->email, data.c_str(), MAX_USER_EMAIL);
                    }
                }
            }
            
            return;
        } else if (cmd->args[arg_idx] == "where") {
            where_update_handler(cmd, arg_idx+1, table, field, data);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
}

void where_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table, const std::string &field,
                          const std::string &data) {
    if (arg_idx < cmd->args_len) {
        std::vector<std::string> conditions;
        for (; arg_idx < cmd->args_len; arg_idx++)
            conditions.push_back(cmd->args[arg_idx]);
        WhereConditions whereConditions(conditions);
        
        size_t idx;
        std::vector<size_t> updateIdx;
        size_t len = table->users.size();
        for (idx = 0; idx < len; idx++) {
            User_t *user = get_User(table, idx);
            if (whereConditions.getResult(user))
                updateIdx.push_back(idx);
        }
        
        if (field == "id") {
            if (updateIdx.size() > 1)
                return;

            unsigned int id = std::stoi(data);
            size_t len = table->users.size();
            for (idx = 0; idx < len; idx++) {
                User_t *user = get_User(table, idx);
                if (user->id == id)
                    return;
            }
            
            User_t *user = get_User(table, updateIdx[0]);
            user->id = id;
        } else if (field == "age") {
            unsigned int age = std::stoi(data);
            size_t len = updateIdx.size();
            for (idx = 0; idx < len; idx++) {
                User_t *user = get_User(table, updateIdx[idx]);
                user->age = age;
            }
        } else {
            size_t len = updateIdx.size();
            for (idx = 0; idx < len; idx++) {
                User_t *user = get_User(table, updateIdx[idx]);
                if (field == "name")
                    strncpy(user->name, data.c_str(), MAX_USER_NAME);
                else
                    strncpy(user->email, data.c_str(), MAX_USER_EMAIL);
            }
        }
        
        if (arg_idx == cmd->args_len) {
            return;
        } 
    }
    cmd->type = UNRECOG_CMD;
}
