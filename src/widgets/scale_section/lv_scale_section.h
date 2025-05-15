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
#include "../line/lv_line.h"
#include "../image/lv_image.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

LV_ATTRIBUTE_EXTERN_DATA extern const lv_obj_class_t lv_scale_section_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create an scale object
 * @param parent    pointer to an object, it will be the parent of the new scale
 * @return          pointer to the created scale
 */
lv_obj_t * lv_scale_section_create(lv_obj_t * parent);

/*======================
 * Add/remove functions
 *=====================*/

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the range for the given scale section
 * @param section       pointer to a scale section object
 * @param minor_range   section new minor range
 * @param major_range   section new major range
 */
void lv_scale_section_set_range(lv_scale_section_t * section, int32_t minor_range, int32_t major_range);

/**
 * Set the range for the given scale section
 * @param section       pointer to a scale section object
 * @param minor_range   section new minor range
 */
void lv_scale_section_set_range_min(lv_scale_section_t * section, int32_t range_min);

/**
 * Set the range for the given scale section
 * @param section       pointer to a scale section object
 * @param major_range   section new major range
 */
void lv_scale_section_set_range_max(lv_scale_section_t * section, int32_t range_max);

/**
 * Set the style of the part for the given scale section
 * @param section   pointer to a scale section object
 * @param part      the part for the section, e.g. LV_PART_INDICATOR
 * @param section_part_style Pointer to the section part style
 */
void lv_scale_section_set_style(lv_scale_section_t * section, lv_part_t part, lv_style_t * section_part_style);

/*=====================
 * Getter functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_SCALE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_SCALE_H*/
