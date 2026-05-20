/**
 * @file LV_NEEDLE_private.h
 *
 */

#ifndef LV_NEEDLE_PRIVATE_H
#define LV_NEEDLE_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../core/lv_obj_private.h"
#include "lv_needle.h"

#if LV_USE_NEEDLE != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/** Internal state for the needle widget. */
struct _lv_needle_t {
    lv_obj_t obj;

    lv_value_precise_t pivot_x;
    lv_value_precise_t pivot_y;

    lv_value_precise_t min_value;    /**< Logical value mapped to start_angle. */
    lv_value_precise_t max_value;    /**< Logical value mapped to end_angle. */
    lv_value_precise_t start_angle;  /**< Starting angle in degrees. */
    lv_value_precise_t end_angle;    /**< Ending angle in degrees. */
    lv_value_precise_t value;        /**< Current value. */
    lv_value_precise_t length;       /**< Forward length from the pivot. */
    lv_value_precise_t back_length;  /**< Backward length from the pivot. */

    bool is_segment;                 /**< True when using explicit endpoint drawing. */
    lv_value_precise_t end_x;        /**< Segment endpoint x coordinate. */
    lv_value_precise_t end_y;        /**< Segment endpoint y coordinate. */
    lv_area_t cached_area;           /**< Previously invalidated draw area. */
    bool cached_area_valid;          /**< True once cached_area has been initialized. */
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#endif /* LV_USE_NEEDLE != 0 */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_NEEDLE_PRIVATE_H*/
