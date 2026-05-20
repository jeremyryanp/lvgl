/**
 * @file lv_frameseq.h
 *
 */

#ifndef LV_FRAMESEQ_H
#define LV_FRAMESEQ_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../core/lv_obj.h"
#include "../../lv_conf_internal.h"

#ifndef LV_USE_FRAMESEQ
#define LV_USE_FRAMESEQ 0
#endif

#if LV_USE_FRAMESEQ

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

LV_ATTRIBUTE_EXTERN_DATA extern const lv_obj_class_t lv_frameseq_class;

#if LV_USE_OBJ_PROPERTY
enum _lv_property_frameseq_id_t {
    LV_PROPERTY_ID(FRAMESEQ, CURRENT_FRAME_INDEX, LV_PROPERTY_TYPE_INT,  0),
    LV_PROPERTY_ID(FRAMESEQ, DURATION,            LV_PROPERTY_TYPE_INT,  1),
    LV_PROPERTY_ID(FRAMESEQ, END_INDEX,           LV_PROPERTY_TYPE_INT,  2),
    LV_PROPERTY_ID(FRAMESEQ, FORMAT,              LV_PROPERTY_TYPE_TEXT, 3),
    LV_PROPERTY_ID(FRAMESEQ, FRAME_COUNT,         LV_PROPERTY_TYPE_INT,  4),
    LV_PROPERTY_ID(FRAMESEQ, FRAME_HEIGHT,        LV_PROPERTY_TYPE_INT,  5),
    LV_PROPERTY_ID(FRAMESEQ, FRAME_WIDTH,         LV_PROPERTY_TYPE_INT,  6),
    LV_PROPERTY_ID(FRAMESEQ, MODE,                LV_PROPERTY_TYPE_INT,  7),
    LV_PROPERTY_ID(FRAMESEQ, SRC,                 LV_PROPERTY_TYPE_TEXT, 8),
    LV_PROPERTY_ID(FRAMESEQ, START_INDEX,         LV_PROPERTY_TYPE_INT,  9),
    LV_PROPERTY_FRAMESEQ_END,
};
#endif
/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a frame sequence object.
 * @param parent pointer to an object, it will be the parent of the new frame sequence.
 * @return pointer to the created frame sequence object.
 */
lv_obj_t * lv_frameseq_create(lv_obj_t * parent);

/**
 * Set the frame path pattern. Supports printf integer patterns, "{}", and "{index}".
 * @param obj pointer to a frame sequence object.
 * @param src frame path pattern.
 */
void lv_frameseq_set_src(lv_obj_t * obj, const char * src);

/**
 * Get the frame path pattern.
 * @param obj pointer to a frame sequence object.
 * @return frame path pattern.
 */
char * lv_frameseq_get_src(lv_obj_t * obj);

/**
 * Set the frame format. Use "rgb565" or "raw565" for direct raw frame reads.
 * @param obj pointer to a frame sequence object.
 * @param format frame format.
 */
void lv_frameseq_set_format(lv_obj_t * obj, const char * format);

/**
 * Get the frame format.
 * @param obj pointer to a frame sequence object.
 * @return frame format.
 */
char * lv_frameseq_get_format(lv_obj_t * obj);

void lv_frameseq_set_frame_width(lv_obj_t * obj, int32_t frame_width);
int32_t lv_frameseq_get_frame_width(lv_obj_t * obj);

void lv_frameseq_set_frame_height(lv_obj_t * obj, int32_t frame_height);
int32_t lv_frameseq_get_frame_height(lv_obj_t * obj);

void lv_frameseq_set_frame_count(lv_obj_t * obj, int32_t frame_count);
int32_t lv_frameseq_get_frame_count(lv_obj_t * obj);

void lv_frameseq_set_duration(lv_obj_t * obj, int32_t duration);
int32_t lv_frameseq_get_duration(lv_obj_t * obj);

void lv_frameseq_set_mode(lv_obj_t * obj, int32_t mode);
int32_t lv_frameseq_get_mode(lv_obj_t * obj);

void lv_frameseq_set_start_index(lv_obj_t * obj, int32_t start_index);
int32_t lv_frameseq_get_start_index(lv_obj_t * obj);

void lv_frameseq_set_end_index(lv_obj_t * obj, int32_t end_index);
int32_t lv_frameseq_get_end_index(lv_obj_t * obj);

void lv_frameseq_set_current_frame_index(lv_obj_t * obj, int32_t current_frame_index);
int32_t lv_frameseq_get_current_frame_index(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_FRAMESEQ*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_FRAMESEQ_H*/
