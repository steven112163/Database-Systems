#include <Table.h>
#include "WhereConditions.h"

WhereConditions::WhereConditions(std::vector<std::string> conditions) {
    if (conditions.size() > 3) {
        this->firstField = conditions[0];
        this->firstOp = conditions[1];
        this->firstData = conditions[2];
        this->op = conditions[3];
        this->secondField = conditions[4];
        this->secondOp = conditions[5];
        this->secondData = conditions[6];
    } else {
        this->firstField = conditions[0];
        this->firstOp = conditions[1];
        this->firstData = conditions[2];
        this->op = "";
        this->secondField = "";
        this->secondOp = "";
        this->secondData = "";
    }
}

// getResult for user
bool WhereConditions::getResult(User_t *user) {
    bool firstResult;
    if (this->firstField == "id" || this->firstField == "age") {
        double data = strtod(this->firstData.c_str(), nullptr);
        firstResult = WhereConditions::getPartialResult(user, this->firstField, data, this->firstOp);
    } else {
        firstResult = getPartialResult(user, this->firstField, this->firstData, this->firstOp);
    }

    if (this->secondField.empty())
        return firstResult;

    bool secondResult;
    if (this->secondField == "id" || this->secondField == "age") {
        double data = strtod(this->secondData.c_str(), nullptr);
        secondResult = WhereConditions::getPartialResult(user, this->secondField, data, this->secondOp);
    } else {
        secondResult = getPartialResult(user, this->secondField, this->secondData, this->secondOp);
    }

    if (this->op == "or")
        return (firstResult || secondResult);
    else
        return (firstResult && secondResult);
}


// getResult for like
bool WhereConditions::getResult(Like_t *like) {
    bool firstResult = false;
    if (this->firstField == "id1" || this->firstField == "id2") {
        double data = strtod(this->firstData.c_str(), nullptr);
        firstResult = getPartialResult(like, this->firstField, data, this->firstOp);
    }

    if (this->secondField.empty())
        return firstResult;

    bool secondResult = false;
    if (this->secondField == "id1" || this->secondField == "id2") {
        double data = strtod(this->secondData.c_str(), nullptr);
        secondResult = getPartialResult(like, this->secondField, data, this->secondOp);
    }

    if (this->op == "or")
        return (firstResult || secondResult);
    else
        return (firstResult && secondResult);
}

// getResult for join
bool WhereConditions::getResult(Tuple &tuple, Table_t &table) {
    bool firstResult;
    if (this->firstField == "id" || this->firstField == "name" ||
        this->firstField == "email" || this->firstField == "age") {
        User_t *user = &table.users[tuple.userIndex];
        if (this->firstField == "id" || this->firstField == "age") {
            double data = strtod(this->firstData.c_str(), nullptr);
            firstResult = getPartialResult(user, this->firstField, data, this->firstOp);
        } else {
            firstResult = getPartialResult(user, this->firstField, this->firstData, this->firstOp);
        }
    } else {
        Like_t *id = &table.likes[tuple.likeIndex];
        double data = strtod(this->firstData.c_str(), nullptr);
        firstResult = getPartialResult(id, this->firstField, data, this->firstOp);
    }

    if (this->secondField.empty())
        return firstResult;


    bool secondResult;

    if (this->firstField == "id" || this->firstField == "name" ||
        this->firstField == "email" || this->firstField == "age") {

        User_t *user = &table.users[tuple.userIndex];
        if (this->secondField == "id" || this->secondField == "age") {
            double data = strtod(this->secondData.c_str(), nullptr);
            secondResult = getPartialResult(user, this->secondField, data, this->secondOp);
        } else {
            secondResult = getPartialResult(user, this->secondField, this->secondData, this->secondOp);
        }

    } else {

        Like_t *id = &table.likes[tuple.likeIndex];
        double data = strtod(this->firstData.c_str(), nullptr);
        secondResult = getPartialResult(id, this->firstField, data, this->firstOp);

    }

    if (this->op == "or")
        return (firstResult || secondResult);
    else
        return (firstResult && secondResult);
}

// for user
bool WhereConditions::getPartialResult(User_t *user, const std::string &field, double data, const std::string &op) {
    if (field == "id")
        return compare(user->id, data, op);
    else
        return compare(user->age, data, op);
}

// for like
bool WhereConditions::getPartialResult(Like_t *like, const std::string &field, double data, const std::string &op) {
    if (field == "id1")
        return compare(like->id1, data, op);
    else
        return compare(like->id2, data, op);
}

bool WhereConditions::compare(unsigned int target, double data, const std::string &op) {
    if (op == "=")
        return (double) target == data;
    else if (op == "!=")
        return (double) target != data;
    else if (op == ">")
        return (double) target > data;
    else if (op == "<")
        return (double) target < data;
    else if (op == ">=")
        return (double) target >= data;
    else
        return (double) target <= data;
}

bool WhereConditions::getPartialResult(User_t *user, const std::string &field, const std::string &data,
                                       const std::string &op) {
    if (field == "name") {
        std::string name = user->name;
        return compare(name, data, op);
    } else {
        std::string email = user->email;
        return compare(email, data, op);
    }
}

bool WhereConditions::compare(const std::string &target, const std::string &data, const std::string &op) {
    if (op == "=")
        return target == data;
    else
        return target != data;
}
