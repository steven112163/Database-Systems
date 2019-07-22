#include <string.h>
#include <stdlib.h>
#include "UpdateState.h"
#include "WhereConditions.h"
#include <vector>

void field_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;
    cmd->cmd_args.sel_args.idxList.clear();
    cmd->cmd_args.sel_args.idxListLen = 0;
    while(arg_idx < cmd->args_len) {
        if (cmd->args[arg_idx] == "table") {
            set_update_handler(cmd, arg_idx+1, table);
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

void set_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len && (cmd->args[arg_idx] == "set")) {

        arg_idx++;
        std::string setCondition = cmd->args[arg_idx];
        size_t idx;
        for (idx = arg_idx + 1; idx < cmd->args_len; idx++) {
            if (cmd->args[idx] == "where") break;
            setCondition += cmd->args[idx];
        }
        
        size_t opLoc = setCondition.find("=");
        std::string field = setCondition.substr(0, opLoc);
        std::string data = setCondition.substr(opLoc + 1, std::string::npos);
        arg_idx = idx;
        
        if (arg_idx == cmd->args_len) {
            if (field == "id")
                return;
            else if (field == "age") {
                unsigned int age = atoi(data.c_str());
                size_t idx;
                for (idx = 0; idx < table->len; idx++) {
                    User_t *user = get_User(table, idx);
                    user->age = age;
                }
            } else {
                size_t idx;
                for (idx = 0; idx < table->len; idx++) {
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
    return;
}

void where_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table, std::string field, std::string data) {
    if (arg_idx < cmd->args_len) {
        std::string conditions = cmd->args[arg_idx];
        size_t next_idx;
        for (next_idx = arg_idx + 1; next_idx < cmd->args_len; next_idx++)
            conditions += cmd->args[next_idx];
        WhereConditions whereConditions(conditions);
        
        size_t idx;
        std::vector<size_t> updateIdx;
        for (idx = 0; idx < table->len; idx++) {
            User_t *user = get_User(table, idx);
            if (whereConditions.getResult(user))
                updateIdx.push_back(idx);
        }
        
        if (field == "id") {
            if (updateIdx.size() > 1)
                return;
            
            unsigned int id = atoi(data.c_str());
            for (idx = 0; idx < table->len; idx++) {
                User_t *user = get_User(table, idx);
                if (user->id == id)
                    return;
            }
            
            User_t *user = get_User(table, updateIdx[0]);
            user->id = id;
        } else if (field == "age") {
            unsigned int age = atoi(data.c_str());
            for (idx = 0; idx < updateIdx.size(); idx++) {
                User_t *user = get_User(table, updateIdx[idx]);
                user->age = age;
            }
        } else {
            for (idx = 0; idx < updateIdx.size(); idx++) {
                User_t *user = get_User(table, updateIdx[idx]);
                if (field == "name")
                    strncpy(user->name, data.c_str(), MAX_USER_NAME);
                else
                    strncpy(user->email, data.c_str(), MAX_USER_EMAIL);
            }
        }
        
        if (next_idx == cmd->args_len) {
            return;
        } 
    }
    cmd->type = UNRECOG_CMD;
    return;
}
