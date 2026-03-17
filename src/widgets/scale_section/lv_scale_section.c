/**
 * @file lv_scale_section.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_scale_section_private.h"
#include "../scale/lv_scale_private.h"

#if LV_USE_SCALE != 0

/*********************
 *      DEFINES
 *********************/
#define LV_SCALE_TICK_IDX_DEFAULT_ID ((uint32_t)255U)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void invalidate_section_scale(lv_scale_section_t * section);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_scale_section_t * lv_scale_section_create(lv_obj_t * scale_obj)
{
    LV_ASSERT_NULL(scale_obj);
    LV_ASSERT_OBJ(scale_obj, &lv_scale_class);

    lv_scale_t * scale = (lv_scale_t *)scale_obj;
    lv_scale_section_t * section = lv_ll_ins_head(&scale->section_ll);
    LV_ASSERT_MALLOC(section);
    if(section == NULL) return NULL;

    lv_memzero(section, sizeof(lv_scale_section_t));
    section->scale = scale_obj;
    section->first_tick_idx_in_section = LV_SCALE_TICK_IDX_DEFAULT_ID;
    section->last_tick_idx_in_section = LV_SCALE_TICK_IDX_DEFAULT_ID;
    section->range_max = -1;

    return section;
}

lv_scale_section_t * lv_scale_add_section(lv_obj_t * obj)
{
    return lv_scale_section_create(obj);
}

void lv_scale_set_section_range(lv_obj_t * scale, lv_scale_section_t * section, int32_t min, int32_t max)
{
    LV_ASSERT_OBJ(scale, &lv_scale_class);
    LV_ASSERT_NULL(section);

    lv_scale_section_set_range(section, min, max);
}

void lv_scale_set_section_min_value(lv_obj_t * scale, lv_scale_section_t * section, int32_t min)
{
    LV_ASSERT_OBJ(scale, &lv_scale_class);
    LV_ASSERT_NULL(section);

    lv_scale_section_set_range_min(section, min);
}

void lv_scale_set_section_max_value(lv_obj_t * scale, lv_scale_section_t * section, int32_t max)
{
    LV_ASSERT_OBJ(scale, &lv_scale_class);
    LV_ASSERT_NULL(section);

    lv_scale_section_set_range_max(section, max);
}

void lv_scale_section_set_range(lv_scale_section_t * section, int32_t range_min, int32_t range_max)
{
    if(section == NULL) return;

    section->range_min = range_min;
    section->range_max = range_max;
    invalidate_section_scale(section);
}

void lv_scale_section_set_range_min(lv_scale_section_t * section, int32_t range_min)
{
    if(section == NULL) return;
    if(section->range_min == range_min) return;

    section->range_min = range_min;
    invalidate_section_scale(section);
}

void lv_scale_section_set_range_max(lv_scale_section_t * section, int32_t range_max)
{
    if(section == NULL) return;
    if(section->range_max == range_max) return;

    section->range_max = range_max;
    invalidate_section_scale(section);
}

void lv_scale_set_section_style_main(lv_obj_t * scale, lv_scale_section_t * section, const lv_style_t * style)
{
    LV_ASSERT_OBJ(scale, &lv_scale_class);
    LV_ASSERT_NULL(section);

    lv_scale_section_set_style_main(section, style);
}

void lv_scale_set_section_style_indicator(lv_obj_t * scale, lv_scale_section_t * section, const lv_style_t * style)
{
    LV_ASSERT_OBJ(scale, &lv_scale_class);
    LV_ASSERT_NULL(section);

    lv_scale_section_set_style_indicator(section, style);
}

void lv_scale_set_section_style_items(lv_obj_t * scale, lv_scale_section_t * section, const lv_style_t * style)
{
    LV_ASSERT_OBJ(scale, &lv_scale_class);
    LV_ASSERT_NULL(section);

    lv_scale_section_set_style_items(section, style);
}

void lv_scale_section_set_style(lv_scale_section_t * section, lv_part_t part, lv_style_t * section_part_style)
{
    if(section == NULL) return;

    switch(part) {
        case LV_PART_MAIN:
            section->main_style = section_part_style;
            break;
        case LV_PART_INDICATOR:
            section->indicator_style = section_part_style;
            break;
        case LV_PART_ITEMS:
            section->items_style = section_part_style;
            break;
        default:
            return;
    }

    invalidate_section_scale(section);
}

void lv_scale_section_set_style_main(lv_scale_section_t * section, const lv_style_t * style)
{
    if(section == NULL) return;

    section->main_style = style;
    invalidate_section_scale(section);
}

void lv_scale_section_set_style_indicator(lv_scale_section_t * section, const lv_style_t * style)
{
    if(section == NULL) return;

    section->indicator_style = style;
    invalidate_section_scale(section);
}

void lv_scale_section_set_style_items(lv_scale_section_t * section, const lv_style_t * style)
{
    if(section == NULL) return;

    section->items_style = style;
    invalidate_section_scale(section);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void invalidate_section_scale(lv_scale_section_t * section)
{
    if(section->scale != NULL) {
        lv_obj_invalidate(section->scale);
    }
}

#endif
