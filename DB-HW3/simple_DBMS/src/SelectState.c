#include <string.h>
#include <stdlib.h>
#include "SelectState.h"

void field_state_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;
    cmd->cmd_args.sel_args.idxList.clear();
    cmd->cmd_args.sel_args.idxListLen = 0;
    table->aggreTypes.clear();
    table->aggreFields.clear();
    table->aggreResults.clear();
    while(arg_idx < cmd->args_len) {
        std::string field;
        std::string type = check_aggregation(cmd->args[arg_idx], field);
        if (type != "") {
            table->aggreTypes.push_back(type);
            table->aggreFields.push_back(field);
        } else if (cmd->args[arg_idx] == "*") {
            add_select_field(cmd, cmd->args[arg_idx].c_str());
        } else if (cmd->args[arg_idx] == "id") {
            add_select_field(cmd, cmd->args[arg_idx].c_str());
        } else if (cmd->args[arg_idx] == "name") {
            add_select_field(cmd, cmd->args[arg_idx].c_str());
        } else if (cmd->args[arg_idx] == "email") {
            add_select_field(cmd, cmd->args[arg_idx].c_str());
        } else if (cmd->args[arg_idx] == "age") {
            add_select_field(cmd, cmd->args[arg_idx].c_str());
        } else if (cmd->args[arg_idx] == "from") {
            table_state_handler(cmd, arg_idx+1, table);
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

std::string check_aggregation(std::string aggregation, std::string& field) {
    size_t aggreLoc = aggregation.find("avg");
    if (aggreLoc != std::string::npos) {
        field = aggregation.substr(aggreLoc + 4, aggregation.length() - 5);
        return "avg";
    }
    
    aggreLoc = aggregation.find("count");
    if (aggreLoc != std::string::npos) {
        field = aggregation.substr(aggreLoc + 6, aggregation.length() - 7);
        return "count";
    }
    
    aggreLoc = aggregation.find("sum");
    if (aggreLoc != std::string::npos) {
        field = aggregation.substr(aggreLoc + 4, aggregation.length() - 5);
        return "sum";
    }
    
    field = "";
    return "";
}

void table_state_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len && (cmd->args[arg_idx] == "table")) {

        arg_idx++;
        if (arg_idx == cmd->args_len) {
            if (table->aggreTypes.size() > 0) {
                std::vector<size_t> targetIdx;
                size_t idx;
                for (idx = 0; idx < table->len; idx++)
                    targetIdx.push_back(idx);
                get_aggregation_result(targetIdx, table);
            }
            return;
        } else if (cmd->args[arg_idx] == "where") {
            where_state_handler(cmd, arg_idx+1, table);
            return;
        } else if (cmd->args[arg_idx] == "offset") {
            offset_state_handler(cmd, arg_idx+1, NULL, table);
            return;
        } else if (cmd->args[arg_idx] == "limit") {
            limit_state_handler(cmd, arg_idx+1, NULL, table);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void where_state_handler(Command_t *cmd, size_t arg_idx, Table_t *table) {
    if (arg_idx < cmd->args_len) {
        size_t next_arg;
        for (next_arg = arg_idx; next_arg < cmd->args_len; next_arg++)
            if (cmd->args[next_arg] == "offset" ||
                cmd->args[next_arg] == "limit")
                break;
        
        std::string conditions = cmd->args[arg_idx];
        size_t next_idx;
        for (next_idx = arg_idx + 1; next_idx < next_arg; next_idx++)
            conditions += cmd->args[next_idx];
        WhereConditions whereConditions(conditions);
        
        if (table->aggreTypes.size() > 0) {
            std::vector<size_t> targetIdx;
            size_t idx;
            for (idx = 0; idx < table->len; idx++) {
                User_t *user = get_User(table, idx);
                if (whereConditions.getResult(user))
                    targetIdx.push_back(idx);
            }
            
            get_aggregation_result(targetIdx, table);
        } else {
            size_t idx;
            for (idx = 0; idx < table->len; idx++) {
                User_t *user = get_User(table, idx);
                if (whereConditions.getResult(user)) {
                    cmd->cmd_args.sel_args.idxList.push_back(idx);
                    cmd->cmd_args.sel_args.idxListLen++;
                }
            }
            if (cmd->cmd_args.sel_args.idxListLen == 0)
                cmd->cmd_args.sel_args.idxListLen = 1;
        }
        
        if (next_arg == cmd->args_len) {
            return;
        }
        if (next_arg < cmd->args_len && cmd->args[next_arg] == "offset") {
            offset_state_handler(cmd, next_arg+1, &whereConditions, table);
            return;
        } else if (next_arg < cmd->args_len && cmd->args[next_arg] == "limit") {
            limit_state_handler(cmd, next_arg+1, &whereConditions, table);
            return;
        }
            
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void get_aggregation_result(std::vector<size_t> targetIdx, Table_t *table) {
    size_t aggreIdx;
    for (aggreIdx = 0; aggreIdx < table->aggreTypes.size(); aggreIdx++) {
        if (table->aggreTypes[aggreIdx] == "count") {
            table->aggreResults.push_back(std::to_string(targetIdx.size()));
        } else {
            unsigned int sum = 0;
            size_t idx;
            for (idx = 0; idx < targetIdx.size(); idx++) {
                User_t *user = get_User(table, targetIdx[idx]);
                if (table->aggreFields[aggreIdx] == "id")
                    sum += user->id;
                else
                    sum += user->age;
            }
            if (table->aggreTypes[aggreIdx] == "sum")
                table->aggreResults.push_back(std::to_string(sum));
            else
                table->aggreResults.push_back(std::to_string((double)sum / targetIdx.size()));
        }
    }
}

void offset_state_handler(Command_t *cmd, size_t arg_idx, WhereConditions *whereConditions, Table_t *table) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.offset = atoi(cmd->args[arg_idx].c_str());

        arg_idx++;

        if (arg_idx == cmd->args_len) {
        	if (whereConditions != NULL) {
        	    if (table->aggreTypes.size() > 0) {
                    std::vector<size_t> targetIdx;
                    size_t idx;
                    for (idx = 0; idx < table->len; idx++) {
                        User_t *user = get_User(table, idx);
                        if (whereConditions->getResult(user))
                            targetIdx.push_back(idx);
                    }
                    
                    get_aggregation_result(targetIdx, table);
                }
            } else {
                if (table->aggreTypes.size() > 0) {
                    std::vector<size_t> targetIdx;
                    size_t idx;
                    for (idx = 0; idx < table->len; idx++)
                        targetIdx.push_back(idx);
                    get_aggregation_result(targetIdx, table);
                }
            }
            return;
        } else if (arg_idx < cmd->args_len && cmd->args[arg_idx] == "limit") {

            limit_state_handler(cmd, arg_idx+1, whereConditions, table);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void limit_state_handler(Command_t *cmd, size_t arg_idx, WhereConditions *whereConditions, Table_t *table) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.limit = atoi(cmd->args[arg_idx].c_str());

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            if (whereConditions != NULL) {
        	    if (table->aggreTypes.size() > 0) {
                    std::vector<size_t> targetIdx;
                    size_t idx;
                    for (idx = 0; idx < table->len; idx++) {
                        User_t *user = get_User(table, idx);
                        if (whereConditions->getResult(user))
                            targetIdx.push_back(idx);
                    }
                    
                    get_aggregation_result(targetIdx, table);
                }
            } else {
                if (table->aggreTypes.size() > 0) {
                    std::vector<size_t> targetIdx;
                    size_t idx;
                    for (idx = 0; idx < table->len; idx++)
                        targetIdx.push_back(idx);
                    get_aggregation_result(targetIdx, table);
                }
            }
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}
