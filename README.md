# Raylib Gremlin

![Sprite Animation](sprite-animation.gif)

This is a desktop pet project created with Raylib. It features a gremlin character that can be moved around the screen and has various animations.

## Credits

The multimedia assets used in this project are from [linux-desktop-gremlin](https://github.com/iluvgirlswithglasses/linux-desktop-gremlin).

## Features

- A gremlin character that lives on your desktop.
- The window is transparent, undecorated, and always on top.
- The gremlin has multiple animations for different states (idle, walking, sleeping, etc.).
- You can drag the gremlin around with your mouse.
- The gremlin will fall asleep if you don't interact with it.

## Building and Running

This project uses CMake. To build and run it, you will need to have Raylib installed.

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd Raylib-Gremlin
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure the project with CMake:**
    ```bash
    cmake ..
    ```

4.  **Build the project:**
    ```bash
    make
    ```

5.  **Run the application:**
    ```bash
    ./Raylib-Gremlin
    ```

## Controls

- **Mouse Drag**: Click and drag the gremlin to move it around.
- **W**: Move backward.
- **S**: Move forward.
- **A**: Move left.
- **D**: Move right.
- **E**: Emote animation.
- **Q**: Intro animation.
- **ESC**: Quit the application.

The gremlin will automatically go to sleep after 30 seconds of inactivity. Any interaction will wake it up.
