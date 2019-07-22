#include "WhereConditions.h"

WhereConditions::WhereConditions(std::string conditions) {
    size_t orLoc = conditions.find("or");
    size_t andLoc = conditions.find("and");
    
    std::string firstCondition, secondCondition;
    if (orLoc != std::string::npos) {
        this->op = "or";
        firstCondition = conditions.substr(0, orLoc);
        secondCondition = conditions.substr(orLoc + 2, std::string::npos);
    } else if (andLoc != std::string::npos) {
        this->op = "and";
        firstCondition = conditions.substr(0, andLoc);
        secondCondition = conditions.substr(andLoc + 3, std::string::npos);
    } else {
        this->op = "";
        size_t opLoc = this->findOp(conditions, this->firstOp);
        this->firstField = conditions.substr(0, opLoc);
        this->firstData = conditions.substr(opLoc + this->firstOp.length(), std::string::npos);
    }
    
    if (orLoc != std::string::npos || andLoc != std::string::npos) {
        size_t firstOpLoc = this->findOp(firstCondition, this->firstOp);
        this->firstField = firstCondition.substr(0, firstOpLoc);
        this->firstData = firstCondition.substr(firstOpLoc + this->firstOp.length(), std::string::npos);
        
        size_t secondOpLoc = this->findOp(secondCondition, this->secondOp);
        this->secondField = secondCondition.substr(0, secondOpLoc);
        this->secondData = secondCondition.substr(secondOpLoc + this->secondOp.length(), std::string::npos);
    }
}

size_t WhereConditions::findOp(std::string condition, std::string& op) {
    size_t location = condition.find("!=");
    if (location == std::string::npos) {
        location = condition.find(">=");
        if (location == std::string::npos) {
            location = condition.find("<=");
            if (location == std::string::npos) {
                location = condition.find("=");
                if (location == std::string::npos) {
                    location = condition.find(">");
                    if (location == std::string::npos) {
                        location = condition.find("<");
                        if (location != std::string::npos)
                            op = "<";
                    } else {
                        op = ">";
                    }
                } else {
                    op = "=";
                }
            } else {
                op = "<=";
            }
        } else {
            op = ">=";
        }
    } else {
        op = "!=";
    }
    
    return location;
}

bool WhereConditions::getResult(User_t *user) {
    bool partialResult;
    if (this->firstField == "id" || this->firstField == "age") {
        double data = stod(this->firstData);
        partialResult = this->getPartialResult(user, this->firstField, data, this->firstOp);
    } else {
        partialResult = this->getPartialResult(user, this->firstField, this->firstData, this->firstOp);
    }
    
    if (this->secondField.size() == 0)
        return partialResult;
    
    bool secondResult;
    if (this->secondField == "id" || this->secondField == "age") {
        double data = stod(this->secondData);
        secondResult = this->getPartialResult(user, this->secondField, data, this->secondOp);
    } else {
        secondResult = this->getPartialResult(user, this->secondField, this->secondData, this->secondOp);
    }
    
    if (this->op == "or")
        return (partialResult || secondResult);
    else
        return (partialResult && secondResult);
}

bool WhereConditions::getPartialResult(User_t *user, std::string field, double data, std::string op) {
    if (field == "id")
        return this->compare(user->id, data, op);
    else
        return this->compare(user->age, data, op);
}

bool WhereConditions::compare(unsigned int target, double data, std::string op) {
    if (op == "=")
        return (double)target == data;
    else if (op == "!=")
        return (double)target != data;
    else if (op == ">")
        return (double)target > data;
    else if (op == "<")
        return (double)target < data;
    else if (op == ">=")
        return (double)target >= data;
    else
        return (double)target <= data;
}

bool WhereConditions::getPartialResult(User_t *user, std::string field, std::string data, std::string op) {
    if (field == "name") {
        std::string name = user->name;
        return this->compare(name, data, op);
    } else {
        std::string email = user->email;
        return this->compare(email, data, op);
    }
}

bool WhereConditions::compare(std::string target, std::string data, std::string op) {
    if (op == "=")
        return target == data;
    else
        return target != data;
}
