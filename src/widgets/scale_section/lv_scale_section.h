/**
 * @file lv_scale_section.h
 *
 */

#ifndef LV_SCALE_SECTION_H
#define LV_SCALE_SECTION_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_conf_internal.h"

#if LV_USE_SCALE != 0

#include "../../core/lv_obj.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a scale section owned by `scale`.
 * @param scale     pointer to a scale object.
 * @return          pointer to the created section.
 */
lv_scale_section_t * lv_scale_section_create(lv_obj_t * scale);

/**
 * Set the range for the given scale section.
 * @param section       pointer to a scale section.
 * @param range_min     section new minimum range value.
 * @param range_max     section new maximum range value.
 */
void lv_scale_section_set_range(lv_scale_section_t * section, int32_t range_min, int32_t range_max);

/**
 * Set the minimum range value for the given scale section.
 * @param section       pointer to a scale section.
 * @param range_min     section new minimum range value.
 */
void lv_scale_section_set_range_min(lv_scale_section_t * section, int32_t range_min);

/**
 * Set the maximum range value for the given scale section.
 * @param section       pointer to a scale section.
 * @param range_max     section new maximum range value.
 */
void lv_scale_section_set_range_max(lv_scale_section_t * section, int32_t range_max);

/**
 * Set a section style for the specified scale part.
 * @param section               pointer to a scale section.
 * @param part                  part to style.
 * @param section_part_style    pointer to the section style.
 */
void lv_scale_section_set_style(lv_scale_section_t * section, lv_part_t part, lv_style_t * section_part_style);

/**
 * Set the main line style for a section.
 * @param section       pointer to a scale section.
 * @param style         pointer to a style.
 */
void lv_scale_section_set_style_main(lv_scale_section_t * section, const lv_style_t * style);

/**
 * Set the major tick and label style for a section.
 * @param section       pointer to a scale section.
 * @param style         pointer to a style.
 */
void lv_scale_section_set_style_indicator(lv_scale_section_t * section, const lv_style_t * style);

/**
 * Set the minor tick style for a section.
 * @param section       pointer to a scale section.
 * @param style         pointer to a style.
 */
void lv_scale_section_set_style_items(lv_scale_section_t * section, const lv_style_t * style);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_SCALE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_SCALE_SECTION_H*/
