#include <string.h>
#include <stdlib.h>
#include "Like.h"

///
/// Allocate new space for Like_t
/// The caller should free the allocated space
///
Like_t* new_Like() {
    auto *new_like = (Like_t *) malloc(sizeof(Like_t));
    new_like->id1 = 0;
    new_like->id2 = 0;
    return new_like;
}

///
/// Transform from the input cmd to the Like_t
///
Like_t* command_to_Like(Command_t *cmd) {
    Like_t *like = new_Like();
    if (!like || !cmd) {
        return nullptr;
    }
    if (cmd->args_len != 5) {
        return nullptr;
    }
    if (cmd->args[2] == "like") {
        like->id1 = std::stoi(cmd->args[3]);
        like->id2 = std::stoi(cmd->args[4]);
    } else {
        return nullptr;
    }
    
    return like;
}

