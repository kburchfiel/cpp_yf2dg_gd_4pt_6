// Source: https://github.com/j-dax/gd-cpp/blob/main/src/scene/hud.h
// Copyright Matthew Pizza; released under the BSD 3-Clause license


#include "hud.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/variant/signal.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Hud::Hud() { reset = false; }

void Hud::_bind_methods() {
ADD_SIGNAL(MethodInfo("game_started"));
  ClassDB::bind_method(D_METHOD("_on_loss"), &Hud::_on_loss);
  ClassDB::bind_method(D_METHOD("_on_score_change"), &Hud::_on_score_change);
  ClassDB::bind_method(D_METHOD("_on_start"), &Hud::_on_start);
  ClassDB::bind_method(D_METHOD("_on_message_timer_timeout"),
                       &Hud::_on_message_timer_timeout);
ClassDB::bind_method(D_METHOD("_on_reset"), &Hud::_on_reset);
}

void Hud::show_message(String text) {
  auto message = get_node<Label>("Message");
  message->set_text(text);
  message->show();
  get_node<Timer>("MessageTimer")->start(2.0);
}

void Hud::_on_loss() {
  reset = true;
  show_message("Game Over");
}

void Hud::_on_score_change(int64_t score) {
  get_node<Label>("ScoreLabel")->set_text(String::num_uint64(score));
}

void Hud::_on_start() {
  get_node<Button>("StartButton")->hide();
  emit_signal("game_started");
}

void Hud::_on_message_timer_timeout() {
  get_node<Label>("Message")->hide();
  if (reset) {
    _on_reset();
    reset = false;
  }
}

void Hud::_on_reset() {
    show_message("Dodge the Creeps!");
    get_node<Button>("StartButton")->show();
}
