#ifndef IK_TRANSFORM_H
#define IK_TRANSFORM_H

#include "ik/config.h"

C_BEGIN

struct vector_t;
struct ik_node_t;
struct chain_t;

enum ik_transform_mode_e
{
    IK_G2L            = 0x00,
    IK_L2G            = 0x01,
    IK_ROTATIONS      = 0x02,
    IK_TRANSLATIONS   = 0x04
};

IK_PUBLIC_API void
ik_transform_node(struct ik_node_t* root, uint8_t mode);

IK_PRIVATE_API void
ik_transform_chain(struct chain_t* chain, uint8_t mode);

IK_PRIVATE_API void
ik_transform_chain_list(const struct vector_t* chain_list, uint8_t mode);

C_END

#endif /* IK_TRANSFORM_H */
