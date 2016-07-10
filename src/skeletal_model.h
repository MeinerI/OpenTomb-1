
#ifndef SKELETAL_MODEL_H
#define SKELETAL_MODEL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ANIM_CMD_MOVE               0x01
#define ANIM_CMD_CHANGE_DIRECTION   0x02
#define ANIM_CMD_JUMP               0x04

/*
 * Animation control flags
 */
#define ANIM_NORMAL_CONTROL             (0)
#define ANIM_LOOP_LAST_FRAME            (1)
#define ANIM_FRAME_LOCK                 (2)
#define ANIM_FRAME_REVERSE              (4)

#define ANIM_EXT_TARGET_TO              (1)
    
#define ANIM_TARGET_USE_AXIS_MOD        (0x0001)
#define ANIM_TARGET_OWERRIDE_ANIM       (0x0002)

#define ANIM_TYPE_BASE                  (0x0000)
#define ANIM_TYPE_HEAD_TRACK            (0x0001)
#define ANIM_TYPE_WEAPON_LH             (0x0002)
#define ANIM_TYPE_WEAPON_RH             (0x0003)
#define ANIM_TYPE_WEAPON_TH             (0x0004)
#define ANIM_TYPE_MISK_1                (0x0100)
#define ANIM_TYPE_MISK_2                (0x0101)
#define ANIM_TYPE_MISK_3                (0x0102)
#define ANIM_TYPE_MISK_4                (0x0103)

#include <stdint.h>

struct base_mesh_s;

/*
 * Animated skeletal model. Taken from openraider.
 * model -> animation -> frame -> bone
 * thanks to Terry 'Mongoose' Hendrix II
 */

/*
 * SMOOTHED ANIMATIONS STRUCTURES
 * stack matrices are needed for skinned mesh transformations.
 */
typedef struct ss_bone_tag_s
{
    struct ss_bone_tag_s   *parent;
    uint16_t                index;
    struct base_mesh_s     *mesh_base;                                          // base mesh - pointer to the first mesh in array
    struct base_mesh_s     *mesh_skin;                                          // base skinned mesh for ТР4+
    struct base_mesh_s     *mesh_slot;
    struct ss_animation_s  *alt_anim;
    float                   offset[3];                                          // model position offset

    float                   qrotate[4];                                         // quaternion rotation
    float                   transform[16]      __attribute__((packed, aligned(16)));    // 4x4 OpenGL matrix for stack usage
    float                   full_transform[16] __attribute__((packed, aligned(16)));    // 4x4 OpenGL matrix for global usage

    uint32_t                body_part;                                          // flag: BODY, LEFT_LEG_1, RIGHT_HAND_2, HEAD...
}ss_bone_tag_t, *ss_bone_tag_p;


typedef struct ss_animation_s
{
    uint16_t                    type : 15;
    uint16_t                    enabled : 1;
    int16_t                     last_state;
    int16_t                     next_state;
    int16_t                     last_animation;
    int16_t                     current_animation;                              //
    int16_t                     next_animation;                                 //
    int16_t                     current_frame;                                  //
    int16_t                     next_frame;                                     //

    uint16_t                    anim_frame_flags;                               // base animation control flags
    uint16_t                    anim_ext_flags;                                 // additional animation control flags

    uint16_t                    targeting_bone;
    uint16_t                    targeting_flags;
    float                       bone_direction[3];
    float                       targeting_limit[4];                             // x, y, z, cos(alpha_limit)
    float                       targeting_axis_mod[3];
    float                       current_mod[4];
    float                       target[3];

    float                       period;                                         // one frame change period
    float                       frame_time;                                     // current time
    float                       lerp;

    int                       (*onFrame)(struct entity_s *ent, struct ss_animation_s *ss_anim, float time);
    void                      (*onEndFrame)(struct entity_s *ent, struct ss_animation_s *ss_anim, int state);
    struct skeletal_model_s    *model;                                          // pointer to the base model
    struct ss_animation_s      *next;
}ss_animation_t, *ss_animation_p;

/*
 * base frame of animated skeletal model
 */
typedef struct ss_bone_frame_s
{
    uint16_t                    bone_tag_count;                                 // number of bones
    struct ss_bone_tag_s       *bone_tags;                                      // array of bones
    float                       pos[3];                                         // position (base offset)
    float                       bb_min[3];                                      // bounding box min coordinates
    float                       bb_max[3];                                      // bounding box max coordinates
    float                       centre[3];                                      // bounding box centre
    float                      *transform;

    struct ss_animation_s       animations;                                     // animations list
}ss_bone_frame_t, *ss_bone_frame_p;

/*
 * ORIGINAL ANIMATIONS
 */
typedef struct bone_tag_s
{
    float               offset[3];                                              // bone vector
    float               qrotate[4];                                             // rotation quaternion
}bone_tag_t, *bone_tag_p;

/*
 * base frame of animated skeletal model
 */
typedef struct bone_frame_s
{
    uint16_t            bone_tag_count;                                         // number of bones
    uint16_t            command;                                                // & 0x01 - move need, &0x02 - 180 rotate need
    struct bone_tag_s  *bone_tags;                                              // bones data
    float               pos[3];                                                 // position (base offset)
    float               bb_min[3];                                              // bounding box min coordinates
    float               bb_max[3];                                              // bounding box max coordinates
    float               centre[3];                                              // bounding box centre
    float               move[3];                                                // move command data
    float               v_Vertical;                                             // jump command data
    float               v_Horizontal;                                           // jump command data
}bone_frame_t, *bone_frame_p ;

/*
 * mesh tree base element structure
 */
typedef struct mesh_tree_tag_s
{
    struct base_mesh_s         *mesh_base;                                      // base mesh - pointer to the first mesh in array
    struct base_mesh_s         *mesh_skin;                                      // base skinned mesh for ТР4+
    float                       offset[3];                                      // model position offset
    uint16_t                    flag;                                           // 0x0001 = POP, 0x0002 = PUSH, 0x0003 = POP + PUSH
    uint16_t                    parent;                                         // parent index
    uint32_t                    body_part;
    uint8_t                     replace_mesh;                                   // flag for shoot / guns animations (0x00, 0x01, 0x02, 0x03)
    uint8_t                     replace_anim;
}mesh_tree_tag_t, *mesh_tree_tag_p;

/*
 * animation switching control structure
 */
typedef struct anim_dispatch_s
{
    uint16_t    next_anim;                                                      // "switch to" animation
    uint16_t    next_frame;                                                     // "switch to" frame
    uint16_t    frame_low;                                                      // low border of state change condition
    uint16_t    frame_high;                                                     // high border of state change condition
}anim_dispatch_t, *anim_dispatch_p;

typedef struct state_change_s
{
    uint32_t                    id;
    uint16_t                    anim_dispatch_count;
    struct anim_dispatch_s     *anim_dispatch;
}state_change_t, *state_change_p;

/*
 * one animation frame structure
 */
typedef struct animation_frame_s
{
    uint32_t                    id;
    uint8_t                     original_frame_rate;
    float                       speed_x;                // Forward-backward speed
    float                       accel_x;                // Forward-backward accel
    float                       speed_y;                // Left-right speed
    float                       accel_y;                // Left-right accel
    uint32_t                    anim_command;
    uint32_t                    num_anim_commands;
    uint16_t                    state_id;
    uint16_t                    frames_count;           // Number of frames
    struct bone_frame_s        *frames;                 // Frame data

    uint16_t                    state_change_count;     // Number of animation statechanges
    struct state_change_s      *state_change;           // Animation statechanges data

    struct animation_frame_s   *next_anim;              // Next default animation
    int                         next_frame;             // Next default frame
}animation_frame_t, *animation_frame_p;

/*
 * skeletal model with animations data.
 */

typedef struct skeletal_model_s
{
    uint32_t                    id;                                             // ID
    uint8_t                     transparency_flags;                             // transparancy flags; 0 - opaque; 1 - alpha test; other - blending mode
    uint8_t                     hide;                                           // do not render
    float                       bbox_min[3];                                    // bbox info
    float                       bbox_max[3];
    float                       centre[3];                                      // the centre of model

    uint16_t                    animation_count;                                // number of animations
    struct animation_frame_s   *animations;                                     // animations data

    uint16_t                    mesh_count;                                     // number of model meshes
    struct mesh_tree_tag_s     *mesh_tree;                                      // base mesh tree.
    uint16_t                   *collision_map;
}skeletal_model_t, *skeletal_model_p;


void SkeletalModel_Clear(skeletal_model_p model);
void SkeletalModel_GenParentsIndexes(skeletal_model_p model);
void SkeletalModel_InterpolateFrames(skeletal_model_p models);

void SkeletalModel_FillTransparency(skeletal_model_p model);
void SkeletalModel_FillSkinnedMeshMap(skeletal_model_p model);
void SkeletalModel_CopyMeshes(mesh_tree_tag_p dst, mesh_tree_tag_p src, int tags_count);
void SkeletalModel_CopyMeshesToSkinned(mesh_tree_tag_p dst, mesh_tree_tag_p src, int tags_count);
void BoneFrame_Copy(bone_frame_p dst, bone_frame_p src);

void SSBoneFrame_CreateFromModel(ss_bone_frame_p bf, skeletal_model_p model);
void SSBoneFrame_Clear(ss_bone_frame_p bf);
void SSBoneFrame_Update(struct ss_bone_frame_s *bf, float time);
void SSBoneFrame_RotateBone(struct ss_bone_frame_s *bf, const float q_rotate[4], int bone);
int  SSBoneFrame_CheckTargetBoneLimit(struct ss_bone_frame_s *bf, struct ss_animation_s *ss_anim);
void SSBoneFrame_TargetBoneToSlerp(struct ss_bone_frame_s *bf, struct ss_animation_s *ss_anim, float time);
void SSBoneFrame_SetTrget(struct ss_animation_s *ss_anim, uint16_t targeted_bone, const float target_pos[3], const float bone_dir[3]);
void SSBoneFrame_SetTargetingAxisMod(struct ss_animation_s *ss_anim, const float mod[3]);
void SSBoneFrame_SetTargetingLimit(struct ss_animation_s *ss_anim, const float limit[4]);
void SSBoneFrame_SetAnimation(struct ss_bone_frame_s *bf, int anim_type, int animation, int frame);
struct ss_animation_s *SSBoneFrame_AddOverrideAnim(struct ss_bone_frame_s *bf, struct skeletal_model_s *sm, uint16_t anim_type);
struct ss_animation_s *SSBoneFrame_GetOverrideAnim(struct ss_bone_frame_s *bf, uint16_t anim_type);
void SSBoneFrame_EnableOverrideAnimByType(struct ss_bone_frame_s *bf, uint16_t anim_type);
void SSBoneFrame_EnableOverrideAnim(struct ss_bone_frame_s *bf, struct ss_animation_s *ss_anim);
void SSBoneFrame_DisableOverrideAnim(struct ss_bone_frame_s *bf, uint16_t anim_type);

struct state_change_s *Anim_FindStateChangeByAnim(struct animation_frame_s *anim, int state_change_anim);
struct state_change_s *Anim_FindStateChangeByID(struct animation_frame_s *anim, uint32_t id);
int  Anim_GetAnimDispatchCase(struct ss_bone_frame_s *bf, uint32_t id);
void Anim_GetNextFrame(struct ss_animation_s *ss_anim, float time, struct state_change_s *stc, int16_t *frame, int16_t *anim, uint16_t anim_flags);

#ifdef	__cplusplus
}
#endif

#endif //SKELETAL_MODEL_H
