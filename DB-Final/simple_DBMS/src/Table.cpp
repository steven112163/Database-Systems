#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Table.h"

///
/// Allocate a Table_t structure, then initialize some attributes, and
/// load table if the `file_name` is given
///
Table_t *new_Table(char *file_name) {
    auto *table = (Table_t *) malloc(sizeof(Table_t));
    //memset((void *) table, 0, sizeof(Table_t));
    //table->capacity = INIT_TABLE_SIZE;
    //table->cache_map = nullptr;
    //table->cache_map = (unsigned char *) malloc(sizeof(char) * INIT_TABLE_SIZE);
    //memset(table->cache_map, 0, sizeof(char) * INIT_TABLE_SIZE);
    //table->fp = nullptr;
    //table->file_name = nullptr;
    //load_table(table, file_name);
    return table;
}

///
/// Add the `User_t` data to the given table
/// If the table is full, it will allocate new space to store more
/// user data
/// return 1 when the data successfully add to table
///
int add_User(Table_t *table, User_t *user) {
    size_t idx;
    User_t *usr_ptr;
    if (!table || !user) {
        return 0;
    }
    // Check id doesn't exist in the table
    for (idx = 0; idx < table->users.size(); idx++) {
        usr_ptr = get_User(table, idx);
        if (usr_ptr->id == user->id) {
            return 0;
        }
    }
    //idx = table->users.size();
    table->users.push_back(*user);
    //table->cache_map[idx] = 1;
    return 1;
}

///
/// Add the `Like_t` data to the given table
/// If the table is full, it will allocate new space to store more
/// like data
/// return 1 when the data successfully add to table
///
int add_Like(Table_t *table, Like_t *like) {
    //size_t idx;
    if (!table || !like) {
        return 0;
    }
    //idx = table->likes.size();
    table->likes.push_back(*like);
    //table->cache_map[idx] = 1;
    return 1;
}

///
/// Return value is the archived table len
///
//int archive_table(Table_t *table) {
//    size_t archived_len;
//    struct stat st{};
//
//    if (table->fp == nullptr) {
//        return 0;
//    }
//    if (stat(table->file_name, &st) == 0) {
//        archived_len = st.st_size / sizeof(User_t);
//    } else {
//        archived_len = 0;
//    }
//    User_t *users = &table->users[0];
//    fwrite((void *) (users + archived_len), \
//            sizeof(User_t), table->users.size() + table->likes.size() - archived_len, \
//            table->fp);
//
//    fclose(table->fp);
//    free(table->file_name);
//    table->fp = nullptr;
//    table->file_name = nullptr;
//    return table->users.size() + table->likes.size();
//}

///
/// Loading the db file will overwrite the existed records in table,
/// only if the ``file_name`` is NULL
/// Return: the number of records in the db file
///
//int load_table(Table_t *table, char *file_name) {
//    size_t archived_len;
//    struct stat st{};
//    if (table->fp != nullptr) {
//        fclose(table->fp);
//        free(table->file_name);
//        table->fp = nullptr;
//        table->file_name = nullptr;
//    }
//    if (file_name != nullptr) {
//        memset(table->cache_map, 0, sizeof(char) * INIT_TABLE_SIZE);
//        if (stat(file_name, &st) != 0) {
//            //Create new file
//            table->fp = fopen(file_name, "wb");
//        } else {
//            archived_len = st.st_size / sizeof(User_t);
//            if (archived_len > table->capacity) {
//                auto *new_cache_buf = (unsigned char *) malloc(sizeof(unsigned char) * (archived_len + EXT_LEN));
//
//                memset(new_cache_buf, 0, sizeof(unsigned char) * (archived_len + EXT_LEN));
//
//                free(table->cache_map);
//                table->cache_map = new_cache_buf;
//                table->capacity = archived_len + EXT_LEN;
//            }
//            table->fp = fopen(file_name, "a+b");
//        }
//        table->file_name = strdup(file_name);
//    }
//    return table->users.size() + table->likes.size();
//}

///
/// Return the user in table by the given index
///
User_t *get_User(Table_t *table, size_t idx) {
    return &table->users[idx];
}

///
/// Return the like in table by the given index
///
Like_t *get_Like(Table_t *table, size_t idx) {
    return &table->likes[idx];
}
