#ifndef WHERE_CONDITIONS_H
#define WHERE_CONDITIONS_H

#include "User.h"
#include <string>

class WhereConditions {
private:
    std::string firstField;
    std::string firstData;
    std::string firstOp;
    std::string secondField;
    std::string secondData;
    std::string secondOp;
    std::string op;
    size_t findOp(std::string condition, std::string& op);
    bool getPartialResult(User_t *user, std::string field, double data, std::string op);
    bool compare(unsigned int target, double data, std::string op);
    bool getPartialResult(User_t *user, std::string field, std::string data, std::string op);
    bool compare(std::string target, std::string data, std::string op);

public:
    WhereConditions(std::string conditions);
    bool getResult(User_t *user);
};

#endif
