/**
 * @file lv_needle.h
 *
 */

#ifndef LV_NEEDLE_H
#define LV_NEEDLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../core/lv_obj.h"

#if LV_USE_NEEDLE != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a needle object
 * @param parent pointer to an object, it will be the parent of the new needle
 * @return pointer to the created needle
 */
lv_obj_t * lv_needle_create(lv_obj_t * parent);

/**********************
 *      TYPEDEFS
 **********************/

LV_ATTRIBUTE_EXTERN_DATA extern const lv_obj_class_t lv_needle_class;

/*=====================
 * Setter functions
 *====================*/

/**
 * Create a needle object
 * @param parent    pointer to an object, it will be the parent of the new needle
 * @return          pointer to the created needle
 */
lv_obj_t * lv_needle_create(lv_obj_t * parent);

/**
 * Set the pivot point of the needle
 * @param obj       pointer to a needle object
 * @param x         x coordinate of the pivot point
 * @param y         y coordinate of the pivot point
 */
void lv_needle_set_pivot(lv_obj_t * obj, lv_value_precise_t x, lv_value_precise_t y);

/**
 * Set the x coordinate of the pivot point
 * @param obj       pointer to a needle object
 * @param x         x coordinate of the pivot point
 */
void lv_needle_set_pivot_x(lv_obj_t * obj, lv_value_precise_t x);

/**
 * Set the y coordinate of the pivot point
 * @param obj       pointer to a needle object
 * @param y         y coordinate of the pivot point
 */
void lv_needle_set_pivot_y(lv_obj_t * obj, lv_value_precise_t y);

/**
 * Set the start angle of the needle
 * @param obj       pointer to a needle object
 * @param angle     angle in degrees (0-360)
 */
void lv_needle_set_start_angle(lv_obj_t * obj, lv_value_precise_t angle);

/**
 * Set the end angle of the needle
 * @param obj       pointer to a needle object
 * @param angle     angle in degrees (0-360)
 */
void lv_needle_set_end_angle(lv_obj_t * obj, lv_value_precise_t angle);

/**
 * Set the value of the needle
 * @param obj       pointer to a needle object
 * @param value     value of the needle
 */
void lv_needle_set_value(lv_obj_t * obj, lv_value_precise_t value);

/**
 * Set the length of the needle
 * @param obj       pointer to a needle object
 * @param length    length of the needle
 */
void lv_needle_set_length(lv_obj_t * obj, lv_value_precise_t length);

/**
 * Set the back length of the needle
 * @param obj       pointer to a needle object
 * @param length    length of the back part of the needle
 */
void lv_needle_set_back_length(lv_obj_t * obj, lv_value_precise_t length);

/**
 * Set the width of the needle
 * @param obj       pointer to a needle object
 * @param width     width of the needle
 */
void lv_needle_set_width(lv_obj_t * obj, lv_value_precise_t width);

/**
 * Set the color of the needle
 * @param obj       pointer to a needle object
 * @param color     color of the needle
 */

void lv_needle_set_color(lv_obj_t * obj, lv_color_t color);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the x coordinate of the pivot point
 * @param obj       pointer to a needle object
 * @return          x coordinate of the pivot point
 */
lv_value_precise_t lv_needle_get_pivot_x(lv_obj_t * obj);

/**
 * Get the y coordinate of the pivot point
 * @param obj       pointer to a needle object
 * @return          y coordinate of the pivot point
 */
lv_value_precise_t lv_needle_get_pivot_y(lv_obj_t * obj);

/**
 * Get the start angle of the needle
 * @param obj       pointer to a needle object
 * @return          angle in degrees (0-360)
 */
lv_value_precise_t lv_needle_get_start_angle(lv_obj_t * obj);

/**
 * Get the end angle of the needle
 * @param obj       pointer to a needle object
 * @return          angle in degrees (0-360)
 */
lv_value_precise_t lv_needle_get_end_angle(lv_obj_t * obj);

/**
 * Get the value of the needle
 * @param obj       pointer to a needle object
 * @return          value of the needle
 */
lv_value_precise_t lv_needle_get_value(lv_obj_t * obj);

/**
 * Get the length of the needle
 * @param obj       pointer to a needle object
 * @return          length of the needle
 */
lv_value_precise_t lv_needle_get_length(lv_obj_t * obj);

/**
 * Get the back length of the needle
 * @param obj       pointer to a needle object
 * @return          length of the back part of the needle
 */
lv_value_precise_t lv_needle_get_back_length(lv_obj_t * obj);

/**
 * Get the width of the needle
 * @param obj       pointer to a needle object
 * @return          width of the needle
 */
lv_value_precise_t lv_needle_get_width(lv_obj_t * obj);

/**
 * Get the color of the needle
 * @param obj       pointer to a needle object
 * @return          color of the needle
 */
lv_color_t lv_needle_get_color(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_NEEDLE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_NEEDLE_H*/
