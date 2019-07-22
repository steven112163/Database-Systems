#ifndef WHERE_CONDITIONS_H
#define WHERE_CONDITIONS_H

#include "User.h"
#include "Like.h"

class WhereConditions {
private:
    std::string firstField;
    std::string firstData;
    std::string firstOp;
    std::string secondField;
    std::string secondData;
    std::string secondOp;
    std::string op;


    static bool getPartialResult(User_t *user, const std::string &field, double data, const std::string &op);

    static bool getPartialResult(Like_t *like, const std::string &field, double data, const std::string &op);


    static bool compare(unsigned int target, double data, const std::string &op);

    static bool
    getPartialResult(User_t *user, const std::string &field, const std::string &data, const std::string &op);

    static bool compare(const std::string &target, const std::string &data, const std::string &op);

public:
    explicit WhereConditions(std::vector<std::string> conditions);
    bool getResult(User_t *user);
    bool getResult(Like_t *like);
    bool getResult(Tuple &tuple, Table_t &table);
};

#endif
