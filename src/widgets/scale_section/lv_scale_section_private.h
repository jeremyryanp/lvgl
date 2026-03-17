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

struct _lv_scale_section_t {
    lv_obj_t * scale;                    /**< Owning scale. */
    const lv_style_t * main_style;       /**< Style for the scale line inside this range. */
    const lv_style_t * indicator_style;  /**< Style for major ticks and labels inside this range. */
    const lv_style_t * items_style;      /**< Style for minor ticks inside this range. */
    int32_t range_min;                   /**< Draw section when tick value >= range_min. */
    int32_t range_max;                   /**< Draw section when tick value <= range_max. */
    uint32_t first_tick_idx_in_section;
    uint32_t last_tick_idx_in_section;
    int32_t first_tick_in_section_width;
    int32_t last_tick_in_section_width;
    lv_point_t first_tick_in_section;
    lv_point_t last_tick_in_section;
    uint32_t first_tick_idx_is_major : 1;
    uint32_t last_tick_idx_is_major  : 1;
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
