#ifndef EFFECTOR_H
#define EFFECTOR_H

#include "ik/config.h"
#include "ik/vec3.h"
#include "ik/quat.h"

/*!
 * WEIGHT_NLERP Causes intermediary weight values to rotate the target around the
 * chain's base instead of linearly interpolating the target. Can be more
 * appealing if the solved tree diverges a lot from the original tree
 * during weight transitions.
 */
#define IK_EFFECTOR_FLAGS_LIST \
    X(WEIGHT_NLERP, 0x01) \
    X(KEEP_ORIENTATION, 0x02)

C_BEGIN

struct ik_node_t;

enum ik_effector_flags_e
{
#define X(arg, value) IK_EFFECTOR_##arg = value,
    IK_EFFECTOR_FLAGS_LIST
#undef X
};

/*!
 * @brief Specifies how a chain of nodes should be solved. The effector can
 * be attached to any node in a tree using ik_node_attach_effector(). The
 * effector specifies the target position and rotation of that node, as well
 * as how much influence the solver has on the tree (weight) and how many
 * child nodes are affected (chain_length).
 */
struct ik_effector_t
{
    struct ik_node_t* node;

    /*!
     * @brief Can be set at any point, and should be updated whenever you have
     * a new target position to solve for. Specifies the global (world)
     * position where the node it is attached to should head for.
     * @note Default value is (0, 0, 0).
     */
    struct ik_vec3_t target_position;

    /*!
     * @brief Can be set at any point, and should be updated whenever you have
     * a new target rotation to solve for. Specifies the global (world)
     * rotation where the node it is attached to should head for.
     * @note Default value is the identity quaternion.
     */
    struct ik_quat_t target_rotation;

    /*!
     * Used internally to hold the actual target position/rotation, which will
     * be different from target_position/target_rotation if the weight is not
     * 1.0. This value is updated right after calling solve() and before the
     * solving algorithm begins.
     */
    struct ik_vec3_t _actual_target;

    /*!
     * @brief Specifies how much influence the solver has on the chain of
     * nodes. A value of 0.0 will cause the solver to completely ignore the
     * chain, while a value of 1.0 will cause the solver to try to place the
     * target node directly at target_position/target_rotation.
     *
     * This is useful for blending the solver in and out. For instance, if you
     * wanted to ground the legs of an animated character, you would want the
     * solver to do nothing during the time when the foot is in the air
     * (weight=0.0) and be fully active when the foot is on the ground
     * (weight=1.0).
     */
    ikreal_t weight;

    ikreal_t rotation_weight;
    ikreal_t rotation_decay;

    /*!
     * @brief Specifies how many parent nodes should be affected. A value of
     * 0 means all of the parents, including the base node.
     * @note Changing the chain length requires the solver tree to be rebuilt
     * with ik_solver_rebuild_tree().
     */
    uint16_t chain_length;

    /*!
     * @brief Various behavioural settings. Check the enum effector_flags_e for
     * more information.
     */
    uint8_t flags;
};

#if defined(IK_BUILDING)

/*!
 * @brief Creates a new effector object. It can be attached to any node in the
 * tree using ik_node_attach_effector().
 */
IK_PRIVATE_API struct ik_effector_t*
ik_effector_create(void);

/*!
 * @brief Destroys and frees an effector object. This should **NOT** be called
 * on effectors that are attached to nodes. Use ik_node_destroy_effector()
 * instead.
 */
IK_PRIVATE_API void
ik_effector_destroy(struct ik_effector_t* effector);

/*!
 * @brief Duplicates the specified effector object and returns it.
 */
IK_PRIVATE_API struct ik_effector_t*
ik_effector_duplicate(const struct ik_effector_t* effector);

/*!
 * @brief Attaches an effector object to the node. The node gains ownership
 * of the effector and is responsible for its deallocation. If the node
 * already owns an effector, then it is first destroyed.
 * @return Returns IK_ALREADY_HAS_ATTACHMENT if the target node already has
 * an effector attached. IK_OK if otherwise.
 * @note You will need to rebuild the solver's tree before solving.
 */
IK_PRIVATE_API ikret_t
ik_effector_attach(struct ik_effector_t* effector, struct ik_node_t* node);

/*!
 * @brief Removes effector from the node it is attached to, if it exists.
 * The field node->effector  is set to NULL.
 * @note You regain ownership of the object and must destroy it manually when
 * done with it. You may also attach it to another node.
 * @note You will need to rebuild the solver's tree before solving.
 */
IK_PRIVATE_API void
ik_effector_detach(struct ik_effector_t* effector);

#endif /* IK_BUILDING */

C_END

#endif /* EFFECTOR_H */
