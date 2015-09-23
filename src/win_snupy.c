#include <pebble.h>
#include "win_snupy.h"
#define KEY_COLOR 0
	
static int color = 0x5500FF;
	
static Layer *s_simple_bg_layer, *s_date_layer, *s_hands_layer;
static TextLayer *s_day_label, *s_num_label;

static char s_num_buffer[4], s_day_buffer[6];

	
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_img_minuti;
static GBitmap *s_res_img_snupy;
static GBitmap *s_res_img_ore;
static RotBitmapLayer *s_bitmaplayer_minuti;
static BitmapLayer *s_bitmaplayer_1;
static RotBitmapLayer *s_bitmaplayer_ore;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_COLOR:
      // color returned as a hex string
      if (t->value->int32 > 0) {
        color = t->value->int32;
        window_set_background_color(s_window, GColorFromHEX(color)); 
        persist_write_int(KEY_COLOR, color);
				color=persist_read_int(KEY_COLOR);
      }
			break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void initialise_ui(void) {
  s_res_img_minuti = gbitmap_create_with_resource(RESOURCE_ID_IMG_MINUTI);
  s_res_img_snupy = gbitmap_create_with_resource(RESOURCE_ID_IMG_SNUPY);
  s_res_img_ore = gbitmap_create_with_resource(RESOURCE_ID_IMG_ORE);
  
	// s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_img_snupy);
	bitmap_layer_set_compositing_mode(s_bitmaplayer_1, GCompOpSet);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_bitmaplayer_minuti
  s_bitmaplayer_minuti = rot_bitmap_layer_create(s_res_img_minuti);
  rot_bitmap_layer_set_corner_clip_color(s_bitmaplayer_minuti, GColorClear);
  rot_bitmap_set_compositing_mode(s_bitmaplayer_minuti, GCompOpSet);
	layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_minuti);
  
  // s_bitmaplayer_ore
  s_bitmaplayer_ore = rot_bitmap_layer_create(s_res_img_ore);
  rot_bitmap_layer_set_corner_clip_color(s_bitmaplayer_ore, GColorClear);
  rot_bitmap_set_compositing_mode(s_bitmaplayer_ore, GCompOpSet);
	layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_ore);
  }

static void destroy_ui(void) {
  window_destroy(s_window);
  rot_bitmap_layer_destroy(s_bitmaplayer_minuti);
  bitmap_layer_destroy(s_bitmaplayer_1);
  rot_bitmap_layer_destroy(s_bitmaplayer_ore);
  gbitmap_destroy(s_res_img_minuti);
  gbitmap_destroy(s_res_img_snupy);
  gbitmap_destroy(s_res_img_ore);
}
// END AUTO-GENERATED UI CODE



static void hands_update_proc(Layer *layer, GContext *ctx) {

	time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  // minute/hour hand

	//  "ORE"
  GPoint src_ic = GPoint(54,54);

  // Angle of rotation
  int angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);

	// Draw!
  rot_bitmap_set_src_ic(s_bitmaplayer_ore, src_ic);
	rot_bitmap_layer_set_angle(s_bitmaplayer_ore,angle);

	
	//  "Minuti"
  
  // Angle of rotation
  angle = (TRIG_MAX_ANGLE * t->tm_min / 60);
  src_ic = GPoint(55,60);
	
  // Draw!
	rot_bitmap_set_src_ic(s_bitmaplayer_minuti, src_ic);
	rot_bitmap_layer_set_angle(s_bitmaplayer_minuti,angle);
	  
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

static void handle_window_unload(Window *s_window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_date_layer);

  text_layer_destroy(s_day_label);
  text_layer_destroy(s_num_label);

  layer_destroy(s_hands_layer);
	destroy_ui();
}


static void init() {

	if (persist_exists(KEY_COLOR)) {
    color = persist_read_int(KEY_COLOR);
  }
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
	 s_window = window_create();
	
  window_set_background_color(s_window, GColorFromHEX(color));
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
		
  s_day_buffer[0] = '\0';
  s_num_buffer[0] = '\0';


// Register with TickTimerService
 tick_timer_service_subscribe(MINUTE_UNIT, handle_second_tick);
}

static void deinit() {
	tick_timer_service_unsubscribe();
}


static void window_load(Window *s_window) {
 Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);

  s_date_layer = layer_create(bounds);
  layer_set_update_proc(s_date_layer, date_update_proc);
  layer_add_child(window_layer, s_date_layer);

  s_day_label = text_layer_create(GRect(30, 116, 18, 16));

  s_num_label = text_layer_create(GRect(113, 116, 18, 24));
  text_layer_set_text(s_num_label, s_num_buffer);
  text_layer_set_background_color(s_num_label, GColorBlack);
  text_layer_set_text_color(s_num_label, GColorYellow);
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
  init();
	show_win_snupy();
 	app_event_loop();
	APP_LOG(APP_LOG_LEVEL_INFO, "hide winsnupy... thatisallfalk!");
	hide_win_snupy();
  deinit();
}