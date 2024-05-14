/*
 * Owen Milke
 * CS2300B
 */

#include "engine.h"

// Program states
enum state {start, normal, challenge, over};
state screen;

// Colors
color targetColor, targetColorHover, targetColorPress;

// Initializes base window, time, and color values
Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();

    // Default fastest times for each game mode
    fastestTimeNormal = 1000000.0;
    fastestTimeChallenge = 1000000.0;

    // Default target color is red
    targetColor = {1, 0, 0, 1};
    targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
    targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "Target Practice", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // Base target for starting the game
    startingTarget = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{65, 65}, color{1, 0, 0, 1});
    // The real game target. Originally spawns offscreen
    target = make_unique<Rect>(shapeShader, vec2{-100, -100}, vec2{65, 65}, color{1, 0, 0, 1});
}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // Sets game state to normal mode and initializes with normal mode settings
    if (keys[GLFW_KEY_N] && screen == start) {
        screen = normal;
        totalClicks = 0;
        totalTargets = 20;
        targetsRemaining = (int)totalTargets;
        startingTarget->setPos({width/2,height/2});
    }

    // Sets game state to challenge mode and initializes with challenge mode settings
    if (keys[GLFW_KEY_C] && screen == start) {
        screen = challenge;
        totalClicks = 0;
        totalTargets = 30;
        targetsRemaining = (int)totalTargets;
        startingTarget->setPos({width/2,height/2});
    }

    // Sets the color of the targets to red
    if (keys[GLFW_KEY_1] && screen == start) {
        targetColor = {1, 0, 0, 1};
        targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
        targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
    }
    // Sets the color of the targets to green
    if (keys[GLFW_KEY_2] && screen == start) {
        targetColor = {0, 1, 0, 1};
        targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
        targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
    }
    // Sets the color of the targets to blue
    if (keys[GLFW_KEY_3] && screen == start) {
        targetColor = {0, 0, 1, 1};
        targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
        targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
    }
    // Sets the color of the targets to yellow
    if (keys[GLFW_KEY_4] && screen == start) {
        targetColor = {1, 1, 0, 1};
        targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
        targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
    }
    // Sets the color of the targets to cyan
    if (keys[GLFW_KEY_5] && screen == start) {
        targetColor = {0, 1, 1, 1};
        targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
        targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
    }
    // Sets the color of the targets to magenta
    if (keys[GLFW_KEY_6] && screen == start) {
        targetColor = {1, 0, 1, 1};
        targetColorHover.vec = targetColor.vec + vec4{0.5, 0.5, 0.5, 0};
        targetColorPress.vec = targetColor.vec - vec4{0.5, 0.5, 0.5, 0};
    }

    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position

    // Checks to see if the mouse is pressed
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // Starting target mouse interaction
    // If the mouse is hovering over the starting target, set its color to the hover variant
    if ((startingTarget->isOverlapping(vec2(MouseX, MouseY)) && ((screen == normal) || (screen == challenge)))) {
        startingTarget->setColor(targetColorHover);
    }
    // If the mouse is not touching the starting target, set its color back to normal
    if (!(startingTarget->isOverlapping(vec2(MouseX, MouseY)) && ((screen == normal) || (screen == challenge)))) {
        startingTarget->setColor(targetColor);
    }
    // If the mouse is touching the starting target and being clicked, set its color to the pressed variant
    if ((startingTarget->isOverlapping(vec2(MouseX, MouseY)) && mousePressed && ((screen == normal) || (screen == challenge)))) {
        startingTarget->setColor(targetColorPress);
    }
    // In normal mode, if the mouse has been clicked on the starting target, hide the starting target offscreen, move the game target to the playable area, and start the normal mode timer
    if ((startingTarget->isOverlapping(vec2(MouseX, MouseY)) && mousePressedLastFrame && !mousePressed && screen == normal)) {
        startingTarget->setPos({-100, -100});
        moveTarget();
        totalClicks--;
        beginNormal = std::chrono::steady_clock::now();
    }
    // In challenge mode, if the mouse has been clicked on the starting target, hide the starting target offscreen, move the game target to the playable area, and start the challenge mode timer
    if ((startingTarget->isOverlapping(vec2(MouseX, MouseY)) && mousePressedLastFrame && !mousePressed && screen == challenge)) {
        startingTarget->setPos({-100, -100});
        moveTarget();
        totalClicks--;
        beginChallenge = std::chrono::steady_clock::now();
    }

    // Target mouse interaction
    // If the mouse is hovering over the target, set its color to the hover variant
    if ((target->isOverlapping(vec2(MouseX, MouseY)) && ((screen == normal) || (screen == challenge)))) {
        target->setColor(targetColorHover);
    }
    // If the mouse is not touching the target, set its color back to normal
    if (!(target->isOverlapping(vec2(MouseX, MouseY)) && ((screen == normal) || (screen == challenge)))) {
        target->setColor(targetColor);
    }
    // If the mouse is touching the target and being clicked, set its color to the pressed variant
    if ((target->isOverlapping(vec2(MouseX, MouseY)) && mousePressed && ((screen == normal) || (screen == challenge)))) {
        target->setColor(targetColorPress);
    }
    // In normal mode, if the mouse has been clicked on the target, move the target to a random spot in the playable area and count a successful target click
    if ((target->isOverlapping(vec2(MouseX, MouseY)) && mousePressedLastFrame && !mousePressed && screen == normal)) {
        moveTarget();
        targetsRemaining--;
    }
    // In challenge mode, if the mouse has been clicked on the target, move the target to a random spot in the playable area, randomly select either 0 or 1 for target movement direction, and count a successful target click
    if ((target->isOverlapping(vec2(MouseX, MouseY)) && mousePressedLastFrame && !mousePressed && screen == challenge)) {
        moveTarget();
        targetsRemaining--;
        direction = rand()%2;
    }

    // If the mouse is clicked anywhere onscreen, add that click to the total number of mouse clicks
    if (mousePressedLastFrame && !mousePressed) {
        totalClicks++;
    }

    // Save mousePressed for next frame
    mousePressedLastFrame = mousePressed;
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // If in challenge mode, depending on if 0 or 1 was chosen, move the target across the screen
    if (screen == challenge) {
        if (direction == 0) {
            // Moves the target to the left
            target->moveX(-4);
            if (target->getPosX() < target->getSize().x/2) {
                // Sets target to the right side of the screen once it collides with the left edge
                target->setPosX(800);
            }
        } else if (direction == 1) {
            // Moves the target to the right
            target->moveX(4);
            if ((800 - target->getPosX()) < target->getSize().x/2) {
                // Sets target to the left side of the screen once it collides with the right edge
                target->setPosX(0);
            }
        }
    }

    // If the user has successfully clicked all targets in normal mode, transition to the over state
    if (targetsRemaining == 0 && screen == normal) {
        screen = over;
        // If the normal mode time is faster that the current fastest normal mode time, set the fastest normal mode time equal to the normal mode time
        if (totalTimeNormal < fastestTimeNormal) {
            fastestTimeNormal = totalTimeNormal;
        }
        // Move the target back offscreen
        target->setPos({-100, -100});
    }
    // If the user has successfully clicked all targets in challenge mode, transition to the over state
    if (targetsRemaining == 0 && screen == challenge) {
        screen = over;
        // If the challenge mode time is faster than the current fastest challenge mode time, set the fastest challenge mode time equal to the challenge mode time
        if (totalTimeChallenge < fastestTimeChallenge) {
            fastestTimeChallenge = totalTimeChallenge;
        }
        // Move the target back offscreen
        target->setPos({-100, -100});
    }

}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    // Render differently depending on screen
    switch (screen) {
        case start: {
            // Displays the game introduction and instructions for both normal and challenge mode
            string intro1 = "Welcome to Target Practice! Similar to other";
            this->fontRenderer->renderText(intro1, 22, 570, 0.7, vec3{1, 1, 1});
            string intro2 = "virtual aim trainers, this game lets you test";
            this->fontRenderer->renderText(intro2, 22, 550, 0.7, vec3{1, 1, 1});
            string intro3 = "both your clicking speed and accuracy on";
            this->fontRenderer->renderText(intro3, 22, 530, 0.7, vec3{1, 1, 1});
            string intro4 = "randomly positioned targets.";
            this->fontRenderer->renderText(intro4, 22, 510, 0.7, vec3{1, 1, 1});
            string intro5 = "Normal mode spawns a total of 20 targets with";
            this->fontRenderer->renderText(intro5, 22, 470, 0.7, vec3{1, 1, 1});
            string intro6 = "no time limit between hits, which is great";
            this->fontRenderer->renderText(intro6, 22, 450, 0.7, vec3{1, 1, 1});
            string intro7 = "for learning the mechanics of the game.";
            this->fontRenderer->renderText(intro7, 22, 430, 0.7, vec3{1, 1, 1});
            string intro8 = "Challenge mode spawns a total of 30 targets";
            this->fontRenderer->renderText(intro8, 22, 390, 0.7, vec3{1, 1, 1});
            string intro9 = "that can move either left or right across";
            this->fontRenderer->renderText(intro9, 22, 370, 0.7, vec3{1, 1, 1});
            string intro10 = "the game screen, making them harder to hit.";
            this->fontRenderer->renderText(intro10, 22, 350, 0.7, vec3{1, 1, 1});
            string normal = "Press n for Normal Mode";
            this->fontRenderer->renderText(normal, width/2 - (12 * normal.length()), height/2.2, 1, vec3{1, 1, 1});
            string challenge = "Press c for Challenge Mode";
            this->fontRenderer->renderText(challenge, width/2 - (12 * challenge.length()), height/2.6, 1, vec3{1, 1, 1});
            // If the fastest normal mode time is not the default value, display the user's current fastest normal mode time
            if (fastestTimeNormal != 1000000.0) {
                string fastest = "Best Normal Time: " + std::to_string(fastestTimeNormal);
                this->fontRenderer->renderText(fastest, width/2 - (12 * fastest.length()), height/3.5, 1, vec3{1, 1, 1});
            }
            // if the fastest challenge mode time is not the default value, display the user's current fastest challenge mode time
            if (fastestTimeChallenge != 1000000.0) {
                string fastest = "Best Challenge Time: " + std::to_string(fastestTimeChallenge);
                this->fontRenderer->renderText(fastest, width/2 - (12 * fastest.length()), height/4.6, 1, vec3{1, 1, 1});
            }
            // Displays the different target color options and tells the user how to select them
            string color1 = "Press a number 1-6 to choose target color!";
            this->fontRenderer->renderText(color1, 49, 55, 0.7, vec3{1, 1, 1});
            string color2 = "1=Red 2=Green 3=Blue 4=Yellow 5=Cyan 6=Magenta";
            this->fontRenderer->renderText(color2, 14, 25, 0.7, vec3{1, 1, 1});
            break;
        }
        case normal: {
            // Renders the starting target
            startingTarget->setUniforms();
            startingTarget->draw();

            // Renders the game target
            target->setUniforms();
            target->draw();

            // Takes another time value in order to constantly update the in-game timer
            endNormal = std::chrono::steady_clock::now();
            // Gets the current timer value in seconds
            totalTimeNormal = double(std::chrono::duration_cast<std::chrono::microseconds>(endNormal - beginNormal).count() / 1000000.0);
            totalTime = double(std::chrono::duration_cast<std::chrono::microseconds>(endNormal - beginNormal).count() / 1000000.0);

            // If the starting target is offscreen (meaning it has been clicked), display the number of targets remaining and the current time elapsed. Otherwise, display normal mode instructions
            if (startingTarget->getPosX() == -100 && startingTarget->getPosY() == -100) {
                string remaining = "Targets Remaining: " + std::to_string(targetsRemaining);
                this->fontRenderer->renderText(remaining, width/2 - (12 * remaining.length()), height/1.075, 1, vec3{1, 1, 1});
                string timer = "Time Elapsed: " + std::to_string(totalTimeNormal);
                this->fontRenderer->renderText(timer, width/2 - (12 * timer.length()), 20, 1, vec3{1, 1, 1});
            } else {
                string directions1 = "Hit all 20 targets as fast as you can!";
                this->fontRenderer->renderText(directions1, width/2 - (10 * directions1.length()) + 64, height/1.25, 0.7, vec3{1, 1, 1});
                string directions2 = "Click on the target below to begin.";
                this->fontRenderer->renderText(directions2, width/2 - (10 * directions2.length()) + 64, height/1.35, 0.7, vec3{1, 1, 1});
            }
            break;
        }
        case challenge: {
            // Renders the starting target
            startingTarget->setUniforms();
            startingTarget->draw();

            // Renders the game target
            target->setUniforms();
            target->draw();

            // Takes another time value in order to constantly update the in-game timer
            endChallenge = std::chrono::steady_clock::now();
            // Gets the current timer value in seconds
            totalTimeChallenge = double(std::chrono::duration_cast<std::chrono::microseconds>(endChallenge - beginChallenge).count() / 1000000.0);
            totalTime = double(std::chrono::duration_cast<std::chrono::microseconds>(endChallenge - beginChallenge).count() / 1000000.0);

            // If the starting target is offscreen (meaning it has been clicked), display the number of targets remaining and the current time elapsed. Otherwise, display challenge mode instructions
            if (startingTarget->getPosX() == -100 && startingTarget->getPosY() == -100) {
                string remaining = "Targets Remaining: " + std::to_string(targetsRemaining);
                this->fontRenderer->renderText(remaining, width/2 - (12 * remaining.length()), height/1.075, 1, vec3{1, 1, 1});
                string timer = "Time Elapsed: " + std::to_string(totalTimeChallenge);
                this->fontRenderer->renderText(timer, width/2 - (12 * timer.length()), 20, 1, vec3{1, 1, 1});
            } else {
                string directions1 = "Hit all 30 targets as fast as you can!";
                this->fontRenderer->renderText(directions1, width/2 - (10 * directions1.length()) + 64, height/1.25, 0.7, vec3{1, 1, 1});
                string directions2 = "Click on the target below to begin.";
                this->fontRenderer->renderText(directions2, width/2 - (10 * directions2.length()) + 64, height/1.35, 0.7, vec3{1, 1, 1});
            }
            break;
        }
        case over: {
            // Display confirmation that the user has successfully clicked all targets alongside their accuracy and final time
            string message = "Congratulations!";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/1.6, 1, vec3{1, 1, 1});
            string clicks = "Accuracy: " + std::to_string((totalTargets / totalClicks) * 100) + "%";
            this->fontRenderer->renderText(clicks, width/2 - (12 * clicks.length()), height/1.8, 1, vec3{1, 1, 1});
            string back = "Press b to return to Main Menu";
            this->fontRenderer->renderText(back, width/2 - (12 * back.length()), height/2.05, 1, vec3{1, 1, 1});
            string timer = "Time Elapsed: " + std::to_string(totalTime);
            this->fontRenderer->renderText(timer, width/2 - (12 * timer.length()), 20, 1, vec3{1, 1, 1});
            // Returns the user back to the main menu if the b key is pressed
            if (keys[GLFW_KEY_B]) {
                screen = start;
            }
            break;
        }
    }

    glfwSwapBuffers(window);
}

void Engine::moveTarget() {
    vec2 pos = {rand() % (int)width, rand() % (int)height};
    target->setPos(pos);
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

GLenum Engine::glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}