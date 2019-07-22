#ifndef SELECT_STATE_H
#define SELECT_STATE_H

#include "Command.h"
#include "Table.h"
#include "WhereConditions.h"

void field_state_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
std::string check_aggregation(std::string aggregation, std::string& field);
void table_state_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void where_state_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void get_aggregation_result(std::vector<size_t> targetIdx, Table_t *table);
void offset_state_handler(Command_t *cmd, size_t arg_idx, WhereConditions *whereConditions, Table_t *table);
void limit_state_handler(Command_t *cmd, size_t arg_idx, WhereConditions *whereConditions, Table_t *table);

#endif
