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

// value?
//min & max?

    lv_value_precise_t angle;
    lv_value_precise_t length;
    lv_value_precise_t back_length;
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
