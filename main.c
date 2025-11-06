#include <assert.h>
#include <raylib.h>
#include <raymath.h>


typedef struct {
    Texture2D texture;
    Rectangle rectangle;
    int frames;
    int rows;
    int cols;
} SpriteAnimation;

typedef struct {
    SpriteAnimation sprite_animation;
    int reverse;
    int fps;
    int current_frame;
    float time_per_frame;
    float time_counter;
} SpriteAnimationPlayer;


SpriteAnimation LoadSpriteAnimation(const char *path, const Rectangle size, const int frames) {
    assert(path);
    const Texture2D texture = LoadTexture(path);
    if (texture.width == 0 || texture.height == 0) {
        return (SpriteAnimation){};
    }
    SpriteAnimation sprite_animation = {
        .texture = texture,
        .rectangle = size,
        .frames = frames,
        .cols = (int) (((float) texture.width - size.x) / size.width),
        .rows = (int) (((float) texture.height - size.y) / size.height),
    };
    if (sprite_animation.frames == 0) {
        sprite_animation.frames = sprite_animation.rows * sprite_animation.cols;
    }
    return sprite_animation;
}


SpriteAnimationPlayer GetSpriteAnimationPlayer(const SpriteAnimation sprite_animation, const int fps) {
    assert(sprite_animation.frames >0);
    const SpriteAnimationPlayer player = {
        .sprite_animation = sprite_animation,
        .fps = fps,
        .time_per_frame = 1.0 / (float) fps,
        .current_frame = 0,
    };
    return player;
}

void SpriteAnimationPlayerDraw(SpriteAnimationPlayer *player, Rectangle place) {
    player->time_counter += GetFrameTime();
    assert(player->time_per_frame >0);
    while (player->time_counter > player->time_per_frame) {
        player->current_frame += player->reverse ? -1 : 1;
        player->time_counter -= player->time_per_frame;
    }
    const SpriteAnimation sprite = player->sprite_animation;
    if (false) {
        // 好像用倒放的方式有点
        if (player->current_frame < 0) {
            player->current_frame *= -1;
            player->reverse = 0;
        }
        if (player->current_frame > player->sprite_animation.frames) {
            player->current_frame = player->sprite_animation.frames - (
                                        player->current_frame % player->sprite_animation.frames);
            player->reverse = 1;
        }
    } else if (player->current_frame >= player->sprite_animation.frames) {
        player->current_frame = player->current_frame % player->sprite_animation.frames;
    }
    const int row = player->current_frame / player->sprite_animation.cols;
    const int col = player->current_frame % player->sprite_animation.cols;
    const Rectangle src = {
        .x = sprite.rectangle.x + (float) col * sprite.rectangle.width,
        .y = sprite.rectangle.y + (float) row * sprite.rectangle.width,
        .width = sprite.rectangle.width,
        .height = sprite.rectangle.height,
    };
    DrawTexturePro(sprite.texture, src, place, (Vector2){}, 0.0, WHITE);
}

#define CREATE_ANIMATION_PLAYER(PATH_SUFFIX, FRAME_PARAM) \
GetSpriteAnimationPlayer(LoadSpriteAnimation("spritesheet/mambo/" PATH_SUFFIX, \
(Rectangle){.height = 300, .width = 300}, \
FRAME_PARAM), 30)

const int width = 300;
const int height = 300;

int main(void) {
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST); // 如果希望透明窗口，必须在 InitWindows 之前执行
    InitAudioDevice();
    InitWindow(width, height, "Raylib Gremlin");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    bool dragging = false;
    Vector2 origin = {};
    Vector2 last = {};
    float idle_time = 0;
    const float sleep_timer = 30; // after 3s goto sleep
    struct {
        SpriteAnimationPlayer backward;
        SpriteAnimationPlayer click;
        SpriteAnimationPlayer emote;
        SpriteAnimationPlayer forward;
        SpriteAnimationPlayer grab;
        SpriteAnimationPlayer hover;
        SpriteAnimationPlayer idle;
        SpriteAnimationPlayer intro;
        SpriteAnimationPlayer left;
        SpriteAnimationPlayer outro;
        SpriteAnimationPlayer right;
        SpriteAnimationPlayer sleep;
        SpriteAnimationPlayer walk_idle;
    } Gremlin = {
                .backward = CREATE_ANIMATION_PLAYER("backward.png", 18),
                .click = CREATE_ANIMATION_PLAYER("click.png", 120),
                .emote = CREATE_ANIMATION_PLAYER("emote.png", 54),
                .forward = CREATE_ANIMATION_PLAYER("forward.png", 18),
                .grab = CREATE_ANIMATION_PLAYER("grab.png", 60),
                .hover = CREATE_ANIMATION_PLAYER("hover.png", 18),
                .idle = CREATE_ANIMATION_PLAYER("idle.png", 18),
                .intro = CREATE_ANIMATION_PLAYER("intro.png", 56),
                .left = CREATE_ANIMATION_PLAYER("left.png", 18),
                .outro = CREATE_ANIMATION_PLAYER("outro.png", 18),
                .right = CREATE_ANIMATION_PLAYER("right.png", 18),
                .sleep = CREATE_ANIMATION_PLAYER("sleep.png", 74),
                .walk_idle = CREATE_ANIMATION_PLAYER("walk-idle.png", 55),
            };


    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    SpriteAnimationPlayer *state = &Gremlin.walk_idle;
    while (!WindowShouldClose()) {
        // 用户闲置状态检查
        if (GetKeyPressed() != 0
            || IsMouseButtonDown(MOUSE_LEFT_BUTTON)
            || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)
            || IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            idle_time = 0;
        } else if (idle_time < sleep_timer) {
            idle_time += GetFrameTime();
        }


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            dragging = true;
            origin = GetMousePosition();
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
        } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            const Vector2 current = GetMousePosition();
            if (current.x == last.x && current.y == last.y) {
            } else {
                last = GetMousePosition();
                const Vector2 w = Vector2Add(GetWindowPosition(), Vector2Subtract(GetMousePosition(), origin));
                SetWindowPosition((int) w.x, (int) w.y);
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        if (dragging) {
            state = &Gremlin.grab;
        } else if (IsKeyDown(KEY_W)) {
            state = &Gremlin.backward;
        } else if (IsKeyDown(KEY_S)) {
            state = &Gremlin.forward;
        } else if (IsKeyDown(KEY_A)) {
            state = &Gremlin.left;
        } else if (IsKeyDown(KEY_D)) {
            state = &Gremlin.right;
        } else if (IsKeyDown(KEY_E)) {
            state = &Gremlin.emote;
        } else if (IsKeyDown(KEY_Q)) {
            state = &Gremlin.intro;
        } else if (idle_time > sleep_timer) {
            state = &Gremlin.sleep;
        } else {
            state = &Gremlin.walk_idle;
        }
        BeginDrawing();
        ClearBackground(BLANK);
        SpriteAnimationPlayerDraw(state, (Rectangle){
                                      .width = width,
                                      .height = height
                                  });
        EndDrawing();
    }
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
