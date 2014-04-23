#include "pebble.h"

Window *window;
TextLayer *text_time_layer;
Layer *top_layer;

int top_layer_height = 28;
int top_line_height = 4;

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char time_text[] = "09.05.77, 15:56";
  strftime(time_text, sizeof(time_text), "%m.%d.%y, %I:%M", tick_time);
  text_layer_set_text(text_time_layer, time_text);
  
}

void init_time(void){
  Layer *window_layer = window_get_root_layer(window);
  
  text_time_layer = text_layer_create(GRect(0, 0, 144-7, 168-92));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FALLOUT_28)));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

void screen_text(void){
  
  
}

void draw_top(Layer *layer, GContext *ctx){
  GRect bounds = layer_get_bounds(layer);
  GRect top_line = GRect(bounds.origin.x, bounds.size.h/2 - top_line_height/2, bounds.size.w, top_line_height);
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  //graphics_draw_rect(ctx, top_line);
  graphics_fill_rect(ctx, top_line, 0, GCornersAll);
}

void handle_init(void) {
  
  window = window_create();
  window_set_fullscreen(window, true);
  window_stack_push(window, false);
  window_set_background_color(window, GColorBlack);
  
  Layer *window_layer = window_get_root_layer(window);
  
  GRect bounds = layer_get_bounds(window_layer);
  GSize size = GSize(bounds.size.w, top_layer_height);
  
  top_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, top_layer_height));
  layer_set_update_proc(top_layer, draw_top);
  
  layer_add_child(window_layer, top_layer);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
  text_layer_destroy(text_time_layer);
  layer_destroy(top_layer);
  window_destroy(window);
}
  
int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}