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

/** Data of line */
struct lv_needle_t {
    lv_obj_t obj;

    lv_value_precise_t pivot_x;
    lv_value_precise_t pivot_y;

    lv_value_precise_t start_angle;  // Starting angle in degrees
    lv_value_precise_t end_angle;    // Ending angle in degrees
    lv_value_precise_t value;        // Current value (0-100)
    lv_value_precise_t length;       // Length of the needle
    lv_value_precise_t back_length;  // Length of the back part of the needle

    bool is_segment;
    lv_value_precise_t end_x;
    lv_value_precise_t end_y;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#endif /* LV_USE_LINE != 0 */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_NEEDLE_PRIVATE_H*/
