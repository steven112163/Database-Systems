# Simple DBMS                                                                                                   

This project is a simple DBMS implemented using C, as the education material of the Introduction to Database Systems in National Chiao Tung University

## Prerequisite

* make >= 4.1
* gcc >= 5.4.0
* g++ >= 5.4.0
* gtest >= 1.8.1
* python >= 3.6.0

## Unit test

`$ make check`

## System test

`$ python3 test/system/system_test.py ./shell [test_case..]`

> if no test_cases is specified, it will run all system tests by default

## Compile

`$ make`

## Quick Start
`$ ./shell`

## Query

***The current table schema is hard coded, the schema in the [include/User.h](include/User.h), you can check the User_t to get the detail of each field.***

### Table schema
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
| select | Print out the `User_t` records in specified format in the current database |
| update | Update tuples in the user table |
| delete | Delete tuples in the user table |

### Command Format
| Command | Format | Description |
|---|---|---|
| insert | `insert into user <id> <name> <email> <age>` | Insert a user with id, name, email and age |
| insert | `insert into like <id1> <id2>` | Insert a like with id1 and id2 |
| select | `select {field} from table [where <conditions>] [offset <offset_num>] [limit <limit_num>]` | Select specified field of required tuples from table starting from offset and up to limit rows |
| update | `update user set { <field> = <content> } [where <conditions>]` | Update specified field of required tuples only in user table |
| delete | `delete from user [where <conditions>]` | Delete specified tuples only in user table|


## Storage mode

### Memory only

The memory only will not store any record after the process terminate

#### Usage:
`$ ./shell`

### File mode

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
