# Simple DBMS                                                                                                   

This project is a simple DBMS implemented using C, as the education material of the Introduction to Database Systems in National Chiao Tung University

## Prerequisite

* make >= 4.1
* gcc >= 5.4.0
* g++ >= 5.4.0
* gtest >= 1.8.1
* python >= 3.6.0

## Unit Test

`$ make check`

## System Test

`$ python3 test/system/system_test.py ./shell [test_case..]`

> if no test_cases is specified, it will run all system tests by default

## Compile

`$ make`

## Quick Start
`$ ./shell`

## Query

***The current table schema is hard coded, the schema in the [include/User.h](include/User.h), you can check the User_t to get the detail of each field.***

### Table Schema
#### `User` table
| field_name | data_type |
|---|---|
| id | int |
| name | char[255] |
| email | char[255] |
| age | int |

### Supported Command
| Command | Description |
|---|---| 
| insert | Only allows inserting one `User_t` record at a time |
| select | Only prints out all the `User_t` records in the current database |

### Command Format
| Command | Format | Description |
|---|---|---|
| insert | insert <id> <name> <email> <age> | insert a user with id, name, email and age |
| select | select {field} from table [offset <offset_num>] [limit <limit_num>] | select specified field from table starting from offset and up to limit rows |

## Storage Mode

### Memory Only

The memory only will not store any record after the process terminate

#### Usage:
`$ ./shell`

### File Mode

The File mode will store the record into the specify \<DB file\>, the
next execute can automatically reload the record by the same
execution command.

#### Usage:
`$ ./shell <DB file>`

## Project Layout
    ├─ include/           Header files
    ├─ src/               Source files
    │  ├─ shell.c         Entry point, which contains main()
    │  └─ Util.c          Some utility functions
    ├─ test/              Unit tests & system tests
    │  ├─ include/        Test header files
    │  ├─ system/         System tests
    │  └─ *.c             Unit tests source file
    └─ Makefile           Makefile for make
