//! Shows text rendering with moving, rotating and scaling text.
//!
//! Note that this uses [`Text2d`] to display text alongside your other entities in a 2D scene.
//!
//! For an example on how to render text as part of a user interface, independent from the world
//! viewport, you may want to look at `games/contributors.rs` or `ui/text.rs`.

use bevy::{
    prelude::*,
    sprite::{Text2dShadow},
    text::{LineBreak, TextBounds},
};

pub fn run() {
    App::new()
        .add_plugins(DefaultPlugins)
        .add_systems(Startup, setup)
        .run();
}

fn setup(mut commands: Commands, asset_server: Res<AssetServer>) {
    let font = asset_server.load("fonts/bubbletea.ttf");
    let text_font = TextFont {
        font: font.clone(),
        font_size: 50.0,
        ..default()
    };
    commands.spawn(Camera2d);
    let box_size = Vec2::new(300.0, 200.0);
    let box_position = Vec2::new(0.0, -250.0);
    let box_color = Color::srgb(0.25, 0.25, 0.55);
    let text_shadow_color = box_color.darker(0.05);
    commands.spawn((
        Sprite::from_color(Color::srgb(0.25, 0.25, 0.55), box_size),
        Transform::from_translation(box_position.extend(0.0)),
        children![(
            Text2d::new("Welcome to MOONCON"),
            text_font.clone(),
            TextLayout::new(Justify::Left, LineBreak::WordBoundary),
            // Wrap text in the rectangle
            TextBounds::from(box_size),
            // Ensure the text is drawn on top of the box
            Transform::from_translation(Vec3::Z),
            // Add a shadow to the text
            Text2dShadow {
                color: text_shadow_color,
                ..default()
            },
        )],
    ));
}
