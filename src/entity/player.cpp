#include "player.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Player::_bind_methods() {
    // expose speed in the editor
    ClassDB::bind_method(D_METHOD("get_speed"), &Player::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &Player::set_speed);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "speed"), "set_speed", "get_speed");

    ClassDB::bind_method(D_METHOD("_on_body_entered", "node"), 
    &Player::_on_body_entered);

    // The signal to emit when the player collides with a Mob
    ADD_SIGNAL(MethodInfo("player_hit"));
    // This code comes from j-dax's script, but a similar example
    // can be found within
    // https://docs.godotengine.org/en/4.5/tutorials/scripting/cpp/gdextension_cpp_example.html .
}

Player::Player() {
    speed = 400;
}

void Player::set_speed(const int pSpeed) {
    speed = pSpeed;
}

int Player::get_speed() const {
    return speed;
}

void Player::start(Vector2 position) {
    UtilityFunctions::print("start() just got called within player.cpp.");
    set_position(position);
    show();
    get_node<CollisionShape2D>("CollisionShape2D")->set_disabled(false);
}

void Player::_ready() {
    UtilityFunctions::print("_ready just got called within player.cpp.");
    // https://github.com/godotengine/godot/issues/74993
    // workaround available: in _ready
    auto im = InputMap::get_singleton();
    im->load_from_project_settings();

    // Note: the following line isn't present within version 3.5
    // of the C++ code for the Your First 2D Game tutorial--
    // so it's possible that I won't
    // need it within this version either.

    start(get_position());

    screen_size = get_viewport_rect().size;

    // You may find it helpful to comment out the following line
    // at times for debugging/testing purposes, especially 
    // earlier in the tutorial.
    hide();
    // From https://docs.godotengine.org/en/3.5/getting_started/first_2d_game/03.coding_the_player.html
}

void Player::_process(double delta) {
    //UtilityFunctions::print("Process just got called within player.cpp.");
    auto input = Input::get_singleton();
    auto velocity = Vector2(0, 0);
    if (input->is_action_pressed("move_up")) {
        velocity.y -= 1;
    }
    if (input->is_action_pressed("move_down")) {
        velocity.y += 1;
    }
    if (input->is_action_pressed("move_left")) {
        velocity.x -= 1;
    }
    if (input->is_action_pressed("move_right")) {
        velocity.x += 1;
    }

    auto animated_sprite_2d = Node::get_node<AnimatedSprite2D>(
        "AnimatedSprite2D");

    if (velocity.x != 0) {
        animated_sprite_2d->set_animation("walk");
        animated_sprite_2d->set_flip_v(false);
        animated_sprite_2d->set_flip_h(velocity.x < 0);
    } else if (velocity.y != 0) {
        animated_sprite_2d->set_animation("up");
        animated_sprite_2d->set_flip_v(velocity.y > 0);
    }

    if (velocity.length() > 0) {
        velocity = velocity.normalized() * speed;
        animated_sprite_2d->play();
    } else {
        animated_sprite_2d->stop();
    }

    auto new_position = get_position();
    new_position += velocity * delta;

    new_position.x = Math::clamp(new_position.x, 0.0f, screen_size.x);
    new_position.y = Math::clamp(new_position.y, 0.0f, screen_size.y);

    set_position(new_position);
}

void Player::_on_body_entered(Node2D *node) {
    UtilityFunctions::print("on_body_entered() just got called within player.cpp.");
    hide();
    get_node<CollisionShape2D>("CollisionShape2D")->set_deferred(
        StringName("set_disabled"), true);
    // Let listeners respond to hit
    emit_signal("player_hit");
}