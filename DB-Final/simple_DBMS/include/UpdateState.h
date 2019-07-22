#ifndef UPDATE_STATE_H
#define UPDATE_STATE_H

#include "Command.h"
#include "Table.h"

void field_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void set_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table);

void
where_update_handler(Command_t *cmd, size_t arg_idx, Table_t *table, const std::string &field, const std::string &data);

#endif
