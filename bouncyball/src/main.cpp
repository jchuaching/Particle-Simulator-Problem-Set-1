#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants for the GUI layout
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const unsigned int SIDEBAR_WIDTH = 320;
const unsigned int INPUT_HEIGHT = 40;
const sf::Color INPUT_BG_COLOR(255, 255, 255);  // Light grey
const sf::Color BUTTON_BG_COLOR(255, 192, 203); // Light pink
const sf::Color pink5(230, 139, 190);
const sf::Color pink4(238, 161, 205);
const sf::Color pink3(244, 184, 218);
const sf::Color pink2(249, 206, 231);
const sf::Color pink1(253, 228, 242);


// Function to create an input box
sf::RectangleShape createInputBox(float x, float y, float width, float height) {
    sf::RectangleShape box;
    box.setSize(sf::Vector2f(width, height));
    box.setPosition(x, y);
    box.setFillColor(pink3);
    return box;
}

sf::Text createLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y) {
    sf::Text label;
    label.setFont(font);
    label.setString(content);
    label.setCharacterSize(size);
    label.setFillColor(sf::Color::Black);
    label.setPosition(x, y);
    return label;
}

sf::Text createInputLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y, float boxHeight) {
    sf::Text label;
    label.setFont(font);
    label.setString(content);
    label.setCharacterSize(size);
    label.setFillColor(sf::Color::Black);

    // Calculate the vertical center
    sf::FloatRect textRect = label.getLocalBounds();
    float verticalCenter = y + (boxHeight - textRect.height) / 2.0f - textRect.top;

    label.setPosition(x, verticalCenter);
    return label;
}

// Function to create a button
sf::RectangleShape createButton(float x, float y, float width, float height) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(pink4);
    return button;
}

// Helper function to create a button with text
sf::RectangleShape createTextButton(float x, float y, float width, float height, const std::string& textContent, sf::Font& font, std::vector<sf::Text>& buttonTexts) {
    sf::RectangleShape button = createButton(x, y, width, height);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(textContent);
    buttonText.setCharacterSize(20);  // Set to your desired size
    buttonText.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(x + width / 2.0f, y + height / 2.0f);

    buttonTexts.push_back(buttonText);  // Add to the vector for drawing later

    return button;
}

sf::Vector2f reflect(const sf::Vector2f& velocity, const sf::Vector2f& normal) {
    // Reflect the velocity vector off the normal vector
    return velocity - 2 * (velocity.x * normal.x + velocity.y * normal.y) * normal;
}

float distanceFromPointToLine(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P) {
    return std::abs((B.y - A.y) * P.x - (B.x - A.x) * P.y + B.x * A.y - B.y * A.x) /
        std::sqrt((B.y - A.y) * (B.y - A.y) + (B.x - A.x) * (B.x - A.x));
}


class Wall {
public:
    sf::Vector2f start;
    sf::Vector2f end;
    sf::RectangleShape shape;

    Wall(sf::Vector2f start, sf::Vector2f end) : start(start), end(end) {
        sf::Vector2f direction = end - start;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        shape.setSize(sf::Vector2f(length, 2)); // Assuming the wall has a thickness of 2 units
        shape.setOrigin(0, 1); // Set origin to the top left corner
        shape.setPosition(start);

        // Calculate the angle to rotate the wall
        float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
        shape.setRotation(angle);
        shape.setFillColor(sf::Color::Black); // Set wall color
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

sf::Vector2f getWallNormal(const Wall& wall) {
    // Calculate direction vector of the wall
    sf::Vector2f direction = wall.end - wall.start;
    // Calculate normal (perpendicular) vector
    sf::Vector2f normal(-direction.y, direction.x);
    // Normalize the normal vector
    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
    normal /= length;
    return normal;
}

class Ball {
public:
    sf::CircleShape shape;
    float vx, vy; // Velocity components

    Ball(float x, float y, float radius, sf::Color color, float speed, float angleInDegrees) : shape(radius) {
        shape.setPosition(x, y);
        shape.setFillColor(color);

        // Convert angle from degrees to radians
        float angleInRadians = angleInDegrees * (M_PI / 180.0f);

        // Calculate velocity components based on speed and angle
        vx = speed * std::cos(angleInRadians);
        vy = speed * std::sin(angleInRadians);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

bool lineIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Vector2f* intersection = nullptr) {
    float s1_x = p2.x - p1.x;
    float s1_y = p2.y - p1.y;
    float s2_x = p4.x - p3.x;
    float s2_y = p4.y - p3.y;

    float s, t;
    s = (-s1_y * (p1.x - p3.x) + s1_x * (p1.y - p3.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p1.y - p3.y) - s2_y * (p1.x - p3.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        if (intersection != nullptr) {
            intersection->x = p1.x + (t * s1_x);
            intersection->y = p1.y + (t * s1_y);
        }
        return true;
    }

    return false; // No collision
}

// Function to update the ball position and check for boundary collisions
void update(const sf::RectangleShape& boundary, const std::vector<Wall>& walls, float deltaTime) {
    // Calculate the trajectory line of the ball for this frame
    sf::Vector2f startPosition = shape.getPosition();
    sf::Vector2f endPosition = startPosition + sf::Vector2f(vx * deltaTime, vy * deltaTime);

    // Check boundary collision with adjusted ball radius
    float leftBound = boundary.getPosition().x + shape.getRadius();
    float rightBound = boundary.getPosition().x + boundary.getSize().x - shape.getRadius() * 2;
    float topBound = boundary.getPosition().y + shape.getRadius();
    float bottomBound = boundary.getPosition().y + boundary.getSize().y - shape.getRadius() * 2;

    if (endPosition.x < leftBound || endPosition.x > rightBound) {
        vx = -vx; // Reverse horizontal velocity
        endPosition.x = (endPosition.x < leftBound) ? leftBound : rightBound;
    }

    if (endPosition.y < topBound || endPosition.y > bottomBound) {
        vy = -vy; // Reverse vertical velocity
        endPosition.y = (endPosition.y < topBound) ? topBound : bottomBound;
    }

    // Now, handle wall collisions
    bool collisionDetected = false;
    sf::Vector2f collisionPoint;
    sf::Vector2f wallNormal;
    for (const auto& wall : walls) {
        if (lineIntersect(startPosition, endPosition, wall.start, wall.end, &collisionPoint)) {
            // Collision detected
            collisionDetected = true;
            wallNormal = getWallNormal(wall);
            break;
        }
    }

    if (collisionDetected) {
        // Reflect the velocity vector off the wall's normal vector
        sf::Vector2f incomingVelocity(vx, vy);
        sf::Vector2f reflectedVelocity = reflect(incomingVelocity, wallNormal);
        vx = reflectedVelocity.x;
        vy = reflectedVelocity.y;

        // Adjust the ball's position to the point of collision plus a little bit back,
        // so it won't collide again in the next frame due to numerical errors
        sf::Vector2f newPosition = collisionPoint - (incomingVelocity * deltaTime * 0.5f);
        shape.setPosition(newPosition);
    } else {
        // If no collision was detected, simply move the ball to its new position
        shape.setPosition(endPosition);
    }
}


};


class InputBox {
public:
    sf::RectangleShape box;
    sf::Text text;
    sf::Text label;
    std::string inputString;
    bool isActive;
    sf::Clock cursorClock;
    bool cursorVisible;

    InputBox(sf::Vector2f position, sf::Vector2f size, const std::string& labelText, sf::Font& font)
        : isActive(false), cursorVisible(false) {
        box.setSize(size);
        box.setPosition(position);
        box.setFillColor(pink3);

        label = createInputLabel(labelText, font, 20, position.x + 5, position.y, size.y);
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(position.x + 5, position.y + (size.y - text.getCharacterSize()) / 2.0f);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(box);
        window.draw(label);
        window.draw(text);

        // Adjust the position of the text to be to the right of the label
        text.setPosition(box.getPosition().x + label.getLocalBounds().width + 15, text.getPosition().y);

        if (isActive && cursorVisible) {
            sf::RectangleShape cursor(sf::Vector2f(2, text.getCharacterSize()));
            cursor.setFillColor(sf::Color::Black);
            cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 5, text.getPosition().y);
            window.draw(cursor);
        }
    }

    void handleEvent(const sf::Event& event) {
        if (!isActive) return;

        // Handle keyboard input
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                if (event.text.unicode == '\b') { // Handle backspace
                    if (!inputString.empty()) {
                        inputString.pop_back();
                        text.setString(inputString);
                    }
                }
                else {
                    inputString += static_cast<char>(event.text.unicode);
                    text.setString(inputString);
                }
            }
        }
    }

    void update() {
        // Blinking cursor logic
        if (isActive) {
            if (cursorClock.getElapsedTime().asSeconds() >= 0.5f) {
                cursorVisible = !cursorVisible;
                cursorClock.restart();
            }
        }
    }

    void setActive(bool active) {
        isActive = active;
        cursorVisible = active;
        cursorClock.restart();
    }

    void setPosition(float x, float y) {
        box.setPosition(x, y);
        text.setPosition(x + 5, y + (box.getSize().y - text.getCharacterSize()) / 2.0f);
        label.setPosition(x - label.getLocalBounds().width - 10, y + (box.getSize().y - label.getCharacterSize()) / 2.0f);
    }
};

// Add a global vector to store walls
std::vector<Wall> walls;

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulator");

    // Define the display area for the simulation
    sf::RectangleShape displayArea(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT));
    displayArea.setFillColor(pink4); // Assuming pink background for the simulation area
    displayArea.setPosition(0, 0);

    sf::Font font;

    if (!font.loadFromFile("/Users/janinechuaching/Desktop/rawr/Inter-Regular.ttf")) {
    std::cout << "Failed to load font!" << std::endl;
    return -1;
}


    // Initialize text labels for the sections
    sf::Text ballsTitle = createLabel("Balls", font, 24, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 20);
    sf::Text wallsTitle = createLabel("Walls", font, 24, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 350);

    // Define input boxes for the balls and walls
    std::vector<InputBox> inputBoxes;

    // Create input boxes for Balls
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 60), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 110), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 160), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Angle:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 210), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Velocity:", font);

    // Create input boxes for Walls
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 390), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X1:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 440), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y1:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 490), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X2:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 540), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y2:", font);

    // Define buttons and their labels
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;

    // Add Ball button with label
    buttons.push_back(createTextButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 260, SIDEBAR_WIDTH - 20, INPUT_HEIGHT, "Add", font, buttonTexts));
    // Add Wall button with label
    buttons.push_back(createTextButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 590, SIDEBAR_WIDTH - 20, INPUT_HEIGHT, "Add", font, buttonTexts));

    std::vector<Ball> balls;  // Vector to hold all the balls

    sf::Clock clock; 

    // Main event loop
    while (window.isOpen()) {
        sf::Event event;

        sf::Time elapsed = clock.restart(); // Restart the clock and get the elapsed time
        float deltaTime = elapsed.asSeconds(); // Convert elapsed time to seconds

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for mouse clicks to activate input boxes
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Check if the "Add" button for balls was clicked
                if (buttons[0].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Then, modify the section where you add a new ball
                    // Assuming inputs are valid numbers for simplicity; add validation as needed
                    float x = std::stof(inputBoxes[0].inputString);
                    float y = std::stof(inputBoxes[1].inputString);
                    float angle = std::stof(inputBoxes[2].inputString); // Angle in degrees
                    float speed = std::stof(inputBoxes[3].inputString); // Speed
                    float radius = 10.0f; // Example radius, you would get this from input or define it
                    sf::Color color = sf::Color{ pink2 }; // Example color, you could make this selectable


                    // Create a new ball and add it to the vector
                    balls.emplace_back(x, y, radius, color, speed, angle);

                    // Clear the input fields
                    for (int i = 0; i < 4; ++i) {
                        inputBoxes[i].inputString.clear();
                        inputBoxes[i].text.setString("");
                    }
                }

                // Check if the "Add" button for walls was clicked
                if (buttons[1].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Get wall input values
                    float x1 = std::stof(inputBoxes[4].inputString);
                    float y1 = std::stof(inputBoxes[5].inputString);
                    float x2 = std::stof(inputBoxes[6].inputString);
                    float y2 = std::stof(inputBoxes[7].inputString);

                    // Check if the input values are within the display area
                    if (displayArea.getGlobalBounds().contains(x1, y1) && displayArea.getGlobalBounds().contains(x2, y2)) {
                        // Create a new wall and add it to the vector
                        walls.emplace_back(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2));

                        // Clear the input fields
                        for (int i = 4; i < 8; ++i) {
                            inputBoxes[i].inputString.clear();
                            inputBoxes[i].text.setString("");
                        }
                    }

                    else {
                        std::cout << "Wall coordinates must be within the display area!" << std::endl;
                    }
                }

                for (auto& box : inputBoxes) {
                    if (box.box.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                        box.setActive(true);
                    }
                    else {
                        box.setActive(false);
                    }
                }
            
            }

            // Handle text input for the active input box
            for (auto& box : inputBoxes) {
                if (box.isActive) {
                    box.handleEvent(event);
                }
            }
        }

         // Update each ball with deltaTime
        for (auto& ball : balls) {
        ball.update(displayArea, walls, deltaTime); // Pass deltaTime to update method
    }

        window.clear(pink5); // Clear the window

        // Draw the display area
        window.draw(displayArea);

        // Draw the titles
        window.draw(ballsTitle);
        window.draw(wallsTitle);

        // Draw input boxes
        for (auto& box : inputBoxes) {
            box.update();  // Update each box (for blinking cursor)
            box.draw(window);  // Draw each box
        }
        
        // Draw buttons and their labels
        for (auto& button : buttons) {
            window.draw(button);
        }
        for (auto& buttonText : buttonTexts) {
            window.draw(buttonText);
        }

        // TODO: Draw balls and walls inside the display area
        for (auto& ball : balls) {
            ball.draw(window);
        }

        // TODO: Draw balls and walls inside the display area
        for (auto& wall : walls) {
            wall.draw(window);
        }

        window.display(); // Display everything we have drawn
    }

    return 0;
}
