
/* --------------------------------------------------------
 *    File: main.cpp
 *  Author: George Stephen
 *   Class: Programming I, COP 2006
 * Purpose: Finishing the Pong Game
 * -------------------------------------------------------- */

#include "pong_defs.h"


// Function declarations (prototypes)
// --------------------------------------------------------

void setup(Ball &ball, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock AI_paddle);
Direction processInput();
bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &playerPaddle, bool &started, MovingBlock AI_paddle);
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders theWalls, MovingBlock playerPaddle, MovingBlock AI_paddle);
bool checkCollision(Ball theBall, Block theWalls);

/**
 * The main application
 * @return OS status message (0=Success)
 */
int main() {
    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello SFML");
    window.clear(WINDOW_COLOR);

    MovingBlock playerPaddle;
    MovingBlock AI_paddle;
    // declare a ball variable and populate it in the center of the window
    Ball theBall;
    // Declare a variable of type Borders
    Borders theWalls;
    setup(theBall, theWalls, playerPaddle, AI_paddle);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;
    bool started = false;
    bool gameOver = false;
    while (!gameOver)
    {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }
        // Process user input
        // ------------------------------------------------
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;
        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time
            update(userInput, theBall, delta, theWalls, playerPaddle, started, AI_paddle);
//            gameOver = update(&userInput, &theBall, theWalls, delta, playerPaddle, started);
            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }
        // Render the window
        // ------------------------------------------------
        render(window, theBall, delta, theWalls, playerPaddle, AI_paddle);
    } // end main game loop
    // game ending, close the graphics window
    window.close();
    return 0;   // return success to the OS
} // end main


/**
 *
 * @param theBall
 * @param theWalls
 * @param playerPaddle
 * @param AI_paddle
 */
void setup(Ball &theBall, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock AI_paddle){

    theWalls.leftWall.left = 0.0;
    theWalls.leftWall.top = 0.0;
    theWalls.leftWall.width = WALL_THICKNESS;
    theWalls.leftWall.height = WINDOW_HEIGHT;
    theWalls.leftWall.rect.setSize(sf::Vector2(theWalls.leftWall.width, theWalls.leftWall.height));
    theWalls.leftWall.rect.setPosition(theWalls.leftWall.left, theWalls.leftWall.top);
    theWalls.leftWall.rect.setFillColor(WALLCOLOR);

    theWalls.rightWall.left = WINDOW_WIDTH - WALL_THICKNESS;
    theWalls.rightWall.top = 0.0;
    theWalls.rightWall.width = WALL_THICKNESS;
    theWalls.rightWall.height = WINDOW_HEIGHT;
    theWalls.rightWall.rect.setSize(sf::Vector2(theWalls.rightWall.width, theWalls.rightWall.height));
    theWalls.rightWall.rect.setPosition(theWalls.rightWall.left, theWalls.rightWall.top);
    theWalls.rightWall.rect.setFillColor(WALLCOLOR);

    theWalls.topWall.left = 0.0;
    theWalls.topWall.top = 0.0;
    theWalls.topWall.width = WINDOW_WIDTH;
    theWalls.topWall.height = WALL_THICKNESS;
    theWalls.topWall.rect.setSize(sf::Vector2(theWalls.topWall.width, theWalls.topWall.height));
    theWalls.topWall.rect.setPosition(theWalls.topWall.left, theWalls.topWall.top);
    theWalls.topWall.rect.setFillColor(WALLCOLOR);

    theWalls.bottomWall.left = 0.0;
    theWalls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    theWalls.bottomWall.width = WINDOW_WIDTH;
    theWalls.bottomWall.height = WALL_THICKNESS;
    theWalls.bottomWall.rect.setSize(sf::Vector2(theWalls.bottomWall.width, theWalls.bottomWall.height));
    theWalls.bottomWall.rect.setPosition(theWalls.bottomWall.left, theWalls.bottomWall.top);
    theWalls.bottomWall.rect.setFillColor(WALLCOLOR);
    // <initialize the ball structure properties here>

    playerPaddle.block.left = PADDLE_THICKNESS;
    playerPaddle.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    playerPaddle.block.width = PADDLE_THICKNESS;
    playerPaddle.block.height = PADDLE_HEIGHT;
    playerPaddle.block.wallColor = PADDLE_COLOR;
    playerPaddle.block.rect.setSize(sf::Vector2f(playerPaddle.block.width, playerPaddle.block.height));
    playerPaddle.block.rect.setFillColor(PADDLE_COLOR);
    playerPaddle.velocityX = 0.0;
    playerPaddle.velocityY = 0.0;

    AI_paddle.block.left = WINDOW_WIDTH - 2 * PADDLE_THICKNESS - WALL_THICKNESS;
    AI_paddle.block.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    AI_paddle.block.width = PADDLE_THICKNESS;
    AI_paddle.block.height = PADDLE_HEIGHT;
    AI_paddle.block.wallColor = PADDLE_COLOR;
    AI_paddle.block.rect.setSize(sf::Vector2f(AI_paddle.block.width, AI_paddle.block.height));
    AI_paddle.block.rect.setFillColor(PADDLE_COLOR);
    AI_paddle.velocityX = 0.0;
    AI_paddle.velocityY = 0.0;


    theBall.radius = BALL_RADIUS;
    theBall.coordinateX = playerPaddle.block.left + playerPaddle.block.width + theBall.radius + 1;
    theBall.coordinateY = playerPaddle.block.top + (playerPaddle.block.height / 2.0);
    theBall.velocityX = 0.0;
    theBall.velocityY = 0.0;
    theBall.color = BALL_COLOR;

}


/**
 * @Description: This function will determine if the ball collides with the walls
 * @param theBall - Access to theBall structure
 * @param theWalls - Access to theWalls structure
 * @return bool, checks if the ball touches the borders.
 */
bool checkCollision(Ball theBall, Block theWalls){
    bool collision = false;
    float ballLeft = theBall.coordinateX - theBall.radius;
    float ballTop = theBall.coordinateY - theBall.radius;
    float ballRight = theBall.coordinateX + theBall.radius;
    float ballBottom = theBall.coordinateY + theBall.radius;

    float boxLeft = theWalls.left;
    float boxTop = theWalls.top;
    float boxRight = theWalls.left + theWalls.width;
    float boxBottom = theWalls.top + theWalls.height;

    if(ballLeft < boxRight &&
       ballRight > boxLeft &&
       ballTop < boxBottom &&
       ballBottom > boxTop){
        collision = true;
    }
    return collision;
};

/**
 *
 * @param moving
 * @param stationary
 * @return
 */
bool checkBlockCollision(Block moving, Block stationary) {
    bool collision = false;
    // Walls
    float stationaryLeft = stationary.left;
    float stationaryTop = stationary.top;
    float stationaryRight = stationary.left + stationary.width;
    float stationaryBottom = stationary.top + stationary.height;

    // Player paddle block
    float movingRight = moving.left + moving.width;
    float movingLeft = moving.left;
    float movingBottom = moving.top + moving.height;
    float movingTop = moving.top;

    // Moving block = playerPaddle.block
    //  stationary block = walls.topWall
    if(movingLeft < stationaryRight &&
       movingRight > stationaryLeft &&
       movingTop < stationaryBottom &&
       movingBottom > stationaryTop){
        collision = true;
    }
}

/**
 * convert user keyboard input into recognized integer values
 * for left=1/up=2/right=3/down=4
 * @return int - user input (default no-input=0, quit=-1)
 */
Direction processInput() {
    Direction input = None;  // no input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        input = Start;
    }
    return input;
} // end getUserInput

/**
 *
 * @param gameOver
 * @param ball
 * @param playerPaddle
 * @param walls
 * @param AI_paddle
 * @return
 */
bool collisionChecks(bool gameOver, Ball &ball, MovingBlock &playerPaddle, Borders &walls, MovingBlock AI_paddle){
    gameOver = false;

    if(checkCollision(ball, playerPaddle.block)){
        // If hit
        ball.velocityX *= - 1;
        ball.coordinateX = playerPaddle.block.left + playerPaddle.block.width + ball.radius + 1;
    }

    else if(checkCollision(ball, walls.leftWall)) {
        ball.velocityX *= -1;
        ball.coordinateX = walls.leftWall.left + walls.leftWall.width + ball.radius;
        gameOver = true;
    }

    else if(checkCollision(ball, walls.rightWall)){
        ball.velocityX *= -1;
        ball.coordinateX = walls.rightWall.left - ball.radius -1;
        gameOver = true;
    };
    if(checkCollision(ball, walls.topWall)){
        ball.velocityY *= -1;
    }
    else if(checkCollision(ball, walls.bottomWall)){
        ball.velocityY *= -1;
        ball.coordinateY = walls.bottomWall.top - ball.radius -1;
    }



    // PLAYER PADDLE
    if(checkBlockCollision(playerPaddle.block, walls.topWall)){
//        playerPaddle.velocityY = 0.0;
//        playerPaddle.block.top = walls.topWall.top + walls.topWall.height + 1;
    }
    else if(checkBlockCollision(playerPaddle.block, walls.bottomWall)){
//        playerPaddle.velocityY = 0.0;
//        playerPaddle.block.top = walls.bottomWall.top + playerPaddle.block.height + 1;
    }

    // AI PADDLE
    if(checkBlockCollision(AI_paddle.block, walls.topWall)){
//        AI_paddle.velocityY = 0.0;
//        AI_paddle.block.top = walls.topWall.top + walls.topWall.height + 1;
    }
    else if(checkBlockCollision(AI_paddle.block, walls.bottomWall)){
//        AI_paddle.velocityY = 0.0;
//        AI_paddle.block.top = walls.bottomWall.top + AI_paddle.block.height + 1;

    }

    return gameOver;
}

/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 */
bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &playerPaddle, bool &started, MovingBlock AI_paddle) {
    // adjust velocity directions for user input
    bool gameOver = false;
    if (input) {
        switch (input) {
            case 1: // Up
                //<ball's velocityY here>
                playerPaddle.velocityY -= PADDLE_SPEED;
                break;
            case 2: // Down
                //<ball's velocityY here>
                playerPaddle.velocityY += PADDLE_SPEED;
                break;
            case 3:
                if(!started){
                    ball.velocityY = BALL_SPEED_Y;
                    ball.velocityX = BALL_SPEED_X;
                    /* randomly set vertical velocity to positive or negative
                     by seeing if the tenths place of current delta
                    (i.e. 1st digit after the decimal) is odd or even.  */
                    if ((int(delta * 10) & 1) % 2) {
                        ball.velocityY *= -1;  // ball goes down if odd
                    }
                    started = true;
                }
                break;

        }
        // clear input
        input = None;
    }

    // Checks the "started" flag variable
    if (started == 1) {
        ball.velocityX * delta + ball.coordinateX;
        ball.velocityY * delta + ball.coordinateY;
        if(ball.coordinateY < AI_paddle.block.top + AI_paddle.block.height){
            AI_paddle.velocityY - PADDLE_SPEED;
            playerPaddle.block.top + playerPaddle.block.height / 2.0;
        }
        else if(ball.coordinateY > playerPaddle.block.top + playerPaddle.block.height / 2.0){
            AI_paddle.velocityY + PADDLE_SPEED;
        }
    }
    else{
        ball.coordinateX = playerPaddle.block.left + playerPaddle.block.width + ball.radius + 1;
        ball.coordinateY = playerPaddle.block.top + (playerPaddle.block.height / 2.0);
    }

    AI_paddle.block.top += AI_paddle.velocityY * delta;

    playerPaddle.block.top += playerPaddle.velocityY * delta;

    // adjust the location of the ball for speed * time
    //<ball's cooridnateX here>
    ball.coordinateX += ball.velocityX * delta;
    //<ball's cooridnateY here>
    ball.coordinateY += ball.velocityY * delta;

    collisionChecks(gameOver, ball, playerPaddle, walls, AI_paddle);

    return gameOver;
} // end update


/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 */
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders theWalls, MovingBlock playerPaddle, MovingBlock AI_paddle) {
    // Render drawing objects
    // ------------------------------------------------
    // clear the window with the background color
    window.clear(WINDOW_COLOR);
    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;
    circle.setFillColor(BALL_COLOR);
    circle.setRadius(ball.radius);
    // set screen coordinates relative to the center of the circle
    circle.setOrigin(ball.radius, ball.radius);
    // calculate current drawing location relative to speed and frame-time
    float xPosition;
    float yPosition;
    //  Calculate xPosition and yPosition
    xPosition = ball.coordinateX + ball.velocityX * delta;
    yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    float X;
    float Y;

    X = playerPaddle.block.left + playerPaddle.velocityX * delta;
    Y = playerPaddle.block.top + playerPaddle.velocityY * delta;
    playerPaddle.block.rect.setPosition(X, Y);
    window.draw(playerPaddle.block.rect);

    float AI_X;
    float AI_Y;

    AI_X = AI_paddle.block.left + AI_paddle.velocityX * delta;
    AI_Y = AI_paddle.block.top + AI_paddle.velocityY * delta;
    AI_paddle.block.rect.setPosition(AI_X, AI_Y);
    window.draw(AI_paddle.block.rect);



    window.draw(theWalls.leftWall.rect);
    window.draw(theWalls.rightWall.rect);
    window.draw(theWalls.topWall.rect);
    window.draw(theWalls.bottomWall.rect);
    // show the new window
    // ------------------------------------------------
    window.display();
} // end render