#include <pebble.h>
#include "win_snupy.h"

#include "simple_analog.h"



	// ricorda di aggiungere : bitmap_layer_set_compositing_mode(s_bitmaplayer_1, GCompOpSet);
	// per la trasparenza!!!
	
static Layer *s_simple_bg_layer, *s_date_layer, *s_hands_layer;
static TextLayer *s_day_label, *s_num_label;

static GPath *s_tick_paths[NUM_CLOCK_TICKS];
static GPath *s_minute_arrow, *s_hour_arrow;
static char s_num_buffer[4], s_day_buffer[6];

	
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_img_minuti;
static GBitmap *s_res_img_snupy;
static GBitmap *s_res_img_ore;
static BitmapLayer *s_bitmaplayer_minuti;
static BitmapLayer *s_bitmaplayer_1;
static BitmapLayer *s_bitmaplayer_ore;
static GColor bkColore;
static GColor Colore;

static void initialise_ui(void) {
  s_window = window_create();
	Colore = GColorFromRGB(0, 170, 255);
	Colore = GColorVividCerulean;
  window_set_background_color(s_window, Colore);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_img_minuti = gbitmap_create_with_resource(RESOURCE_ID_IMG_MINUTI);
  s_res_img_snupy = gbitmap_create_with_resource(RESOURCE_ID_IMG_SNUPY);
  s_res_img_ore = gbitmap_create_with_resource(RESOURCE_ID_IMG_ORE);
  // s_bitmaplayer_minuti
  s_bitmaplayer_minuti = bitmap_layer_create(GRect(75, 72, 58, 40));
  bitmap_layer_set_bitmap(s_bitmaplayer_minuti, s_res_img_minuti);
	bitmap_layer_set_compositing_mode(s_bitmaplayer_minuti, GCompOpSet);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_minuti);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_img_snupy);
	bitmap_layer_set_compositing_mode(s_bitmaplayer_1, GCompOpSet);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_bitmaplayer_ore
  s_bitmaplayer_ore = bitmap_layer_create(GRect(20, 72, 58, 40));
  bitmap_layer_set_bitmap(s_bitmaplayer_ore, s_res_img_ore);
	bitmap_layer_set_compositing_mode(s_bitmaplayer_ore, GCompOpSet);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_ore);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_minuti);
  bitmap_layer_destroy(s_bitmaplayer_1);
  bitmap_layer_destroy(s_bitmaplayer_ore);
  gbitmap_destroy(s_res_img_minuti);
  gbitmap_destroy(s_res_img_snupy);
  gbitmap_destroy(s_res_img_ore);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}



static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorYellow);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, GColorClear);
  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_draw_filled(ctx, s_tick_paths[i]);
  }
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  //int16_t second_hand_length = bounds.size.w / 2;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  //int32_t second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  //GPoint second_hand = {
  //  .x = (int16_t)(sin_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
  //  .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
  //};

  
  // minute/hour hand
  // graphics_context_set_fill_color(ctx, GColorClear);
  // graphics_context_set_stroke_color(ctx, GColorBlack);

  // gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  // gpath_draw_filled(ctx, s_minute_arrow);
  // gpath_draw_outline(ctx, s_minute_arrow);

  // gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  // gpath_draw_filled(ctx, s_hour_arrow);
  // gpath_draw_outline(ctx, s_hour_arrow);

	// Get image center "ORE"
  GRect img_bounds = gbitmap_get_bounds(s_res_img_ore);
  // GPoint src_ic = grect_center_point(&img_bounds);
	GPoint src_ic = GPoint(48,20);
	

  // Get context center "ORE"
  GRect ctx_bounds = layer_get_bounds(layer);
  // GPoint ctx_ic = grect_center_point(&ctx_bounds);
  GPoint ctx_ic = GPoint(72,96);
  // Angle of rotation
  int angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);

	bitmap_layer_set_compositing_mode(s_bitmaplayer_ore, GCompOpSet);
  // Draw!
  graphics_draw_rotated_bitmap(ctx, s_res_img_ore, src_ic, angle, ctx_ic);
	  
	// Get context center "Minuti"
  
  // Angle of rotation
  angle = (TRIG_MAX_ANGLE * t->tm_min / 60);
  src_ic = GPoint(10,20);
	
	bitmap_layer_set_compositing_mode(s_bitmaplayer_minuti, GCompOpSet);
  // Draw!
  graphics_draw_rotated_bitmap(ctx, s_res_img_minuti, src_ic, angle, ctx_ic);
	
}

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", t);
  text_layer_set_text(s_day_label, s_day_buffer);

  strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
  text_layer_set_text(s_num_label, s_num_buffer);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(s_window));
}

static void window_unload(Window *s_window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_date_layer);

  text_layer_destroy(s_day_label);
  text_layer_destroy(s_num_label);

  layer_destroy(s_hands_layer);
}

static void init() {
 // window = window_create();
 // window_set_window_handlers(window, (WindowHandlers) {
 //   .load = window_load,
 //   .unload = window_unload,
 // });
 // window_stack_push(window, true);

  s_day_buffer[0] = '\0';
  s_num_buffer[0] = '\0';

  // init hand paths
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);

  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);

  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    s_tick_paths[i] = gpath_create(&ANALOG_BG_POINTS[i]);
  }

  tick_timer_service_subscribe(MINUTE_UNIT, handle_second_tick);
}

static void deinit() {
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);

  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_destroy(s_tick_paths[i]);
  }

  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}


static void window_load(Window *s_window) {
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);

 s_simple_bg_layer = layer_create(bounds);
 layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
 // layer_add_child(window_layer, s_simple_bg_layer);

  s_date_layer = layer_create(bounds);
  layer_set_update_proc(s_date_layer, date_update_proc);
  layer_add_child(window_layer, s_date_layer);

  s_day_label = text_layer_create(GRect(46, 114, 27, 20));
  text_layer_set_text(s_day_label, s_day_buffer);
  text_layer_set_background_color(s_day_label, GColorClear);
  text_layer_set_text_color(s_day_label, GColorWhite);
  text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));

  layer_add_child(s_date_layer, text_layer_get_layer(s_day_label));

  s_num_label = text_layer_create(GRect(73, 114, 18, 20));
  text_layer_set_text(s_num_label, s_num_buffer);
  text_layer_set_background_color(s_num_label, GColorBlack);
  text_layer_set_text_color(s_num_label, GColorWhite);
  text_layer_set_font(s_num_label, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  layer_add_child(s_date_layer, text_layer_get_layer(s_num_label));

  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
}

void show_win_snupy(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
		.load = window_load,
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_win_snupy(void) {
  window_stack_remove(s_window, true);
}


int main() {
  show_win_snupy();
	init();
 	app_event_loop();
	hide_win_snupy();
  deinit();
}