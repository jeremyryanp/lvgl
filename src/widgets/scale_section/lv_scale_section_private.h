/**
 * @file lv_scale_section_private.h
 *
 */

#ifndef LV_SCALE_SECTION_PRIVATE_H
#define LV_SCALE_SECTION_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lv_scale_section.h"

#if LV_USE_SCALE != 0
#include "../../core/lv_obj_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct lv_scale_section_t {
    lv_style_t * main_style;
    lv_style_t * indicator_style;
    lv_style_t * items_style;
    int32_t minor_range;
    int32_t major_range;
    uint32_t first_tick_idx_in_section;
    uint32_t last_tick_idx_in_section;
    uint32_t first_tick_idx_is_major;
    uint32_t last_tick_idx_is_major;
    int32_t first_tick_in_section_width;
    int32_t last_tick_in_section_width;
    lv_point_t first_tick_in_section;
    lv_point_t last_tick_in_section;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#endif /* LV_USE_SCALE != 0 */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_SCALE_SECTION_PRIVATE_H*/
