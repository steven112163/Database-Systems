#ifndef UPDATE_STATE_H
#define UPDATE_STATE_H

#include "Command.h"
#include "Table.h"
#include <string>

void field_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void set_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void where_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table, std::string field, std::string data);

#endif
