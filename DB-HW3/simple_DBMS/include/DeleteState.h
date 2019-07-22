#ifndef DELETE_STATE_H
#define DELETE_STATE_H

#include "Command.h"
#include "Table.h"

void field_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void table_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table);
void where_delete_handler(Command_t *cmd, size_t arg_idx, Table_t *table);

#endif
