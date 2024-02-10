#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 
#include <thread>
#include <mutex>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants for the GUI layout
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const unsigned int SIDEBAR_WIDTH = 320;
const unsigned int INPUT_HEIGHT = 30;

const sf::Color columbiaBlue(210, 224, 237);
const sf::Color peachFuzz(255, 215, 194);
const sf::Color flashWhite(246, 246, 248);
const sf::Color slateBlue(110, 110, 110);
const sf::Color mintTulip(195,224,244);
const sf::Color cornFlower(160,207,234);


// Function to create an input box
sf::RectangleShape createInputBox(float x, float y, float width, float height) {
    sf::RectangleShape box;
    box.setSize(sf::Vector2f(width, height));
    box.setPosition(x, y);
    box.setFillColor(flashWhite);
    return box;
}

sf::Text createLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y) {
    sf::Text label;
    label.setFont(font);
    label.setString(content);
    label.setCharacterSize(size);
    label.setFillColor(slateBlue);
    label.setPosition(x, y);
    return label;
}

sf::Text createInputLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y, float boxHeight) {
    sf::Text label;
    label.setFont(font);
    label.setString(content);
    label.setCharacterSize(size);
    label.setFillColor(slateBlue);

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
    button.setFillColor(cornFlower);
    return button;
}

// Helper function to create a button with text
sf::RectangleShape createTextButton(float x, float y, float width, float height, const std::string& textContent, sf::Font& font, std::vector<sf::Text>& buttonTexts) {
    sf::RectangleShape button = createButton(x, y, width, height);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(textContent);
    buttonText.setCharacterSize(20);  // Set to your desired size
    buttonText.setFillColor(slateBlue);
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
        shape.setFillColor(slateBlue); // Set wall color
    }

    void draw(sf::RenderWindow& window) const {
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

class RadioButton {
public:
    sf::CircleShape outerCircle;
    sf::CircleShape innerCircle;
    bool isSelected;
    sf::Text labelText; // Label text for the radio button
    std::string label;

    RadioButton(float x, float y, std::string labelText, sf::Font& font, unsigned int characterSize = 15) : isSelected(false), label(labelText) {
        outerCircle.setRadius(8); // Smaller radius for the outer circle
        outerCircle.setPosition(x, y);
        outerCircle.setFillColor(sf::Color::White);

        innerCircle.setRadius(4); // Smaller radius for the inner circle
        // Adjust position to ensure innerCircle remains centered within outerCircle
        innerCircle.setPosition(x + 4, y + 4); // Adjusted for the new radius
        innerCircle.setFillColor(slateBlue);

        // Initially not selected, so inner circle is not visible
        innerCircle.setFillColor(sf::Color::Transparent);

        // Setting up the label
        this->labelText.setFont(font);
        this->labelText.setString(label);
        this->labelText.setCharacterSize(characterSize);
        this->labelText.setFillColor(slateBlue);
        
        // Position the label right next to the radio button
        this->labelText.setPosition(x + outerCircle.getRadius() * 2 + 5, y);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(outerCircle);
        if (isSelected) {
            innerCircle.setFillColor(slateBlue);
        }
        else {
            innerCircle.setFillColor(sf::Color::Transparent);
        }
        window.draw(innerCircle);
        window.draw(labelText); // Draw the label text
    }

    void select() {
        isSelected = true;
    }
    void deselect(){
        isSelected = false;
    }

    void toggle() {
        isSelected = !isSelected;
    }

    bool contains(sf::Vector2f point) {
        return outerCircle.getGlobalBounds().contains(point);
    }
};


class Ball {
public:
    sf::CircleShape shape;
    float vx, vy; // Velocity components

    
    Ball(float x, float y, float radius, sf::Color color, float speed, float angleInDegrees)
    : shape(radius) {
        // Invert the y coordinate to match the requested coordinate system
        float invertedY = WINDOW_HEIGHT - y; // Subtract y from WINDOW_HEIGHT to invert

        shape.setPosition(x, invertedY - radius * 2); // Adjust for radius to ensure the ball spawns from the correct location
        shape.setFillColor(color);

        // Convert angle from degrees to radians
        float angleInRadians = angleInDegrees * (M_PI / 180.0f);

        // Calculate velocity components based on speed and angle
        vx = speed * std::cos(angleInRadians);
        vy = -speed * std::sin(angleInRadians); // Invert vy because the coordinate system is inverted
       }

    void draw(sf::RenderWindow& window) const {
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
        //Calculate the trajectory line of the ball for this frame
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

void updateBallsInParallel(std::vector<Ball>& balls, const sf::RectangleShape& boundary, const std::vector<Wall>& walls, float deltaTime) {
    const size_t numThreads = std::thread::hardware_concurrency();
    const size_t chunkSize = balls.size() / numThreads;
    std::vector<std::thread> threads;

    for (size_t i = 0; i < numThreads; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = (i == numThreads - 1) ? balls.size() : (i + 1) * chunkSize;

        threads.emplace_back([startIdx, endIdx, &balls, &boundary, &walls, deltaTime]() {
            for (size_t j = startIdx; j < endIdx; ++j) {
                balls[j].update(boundary, walls, deltaTime);
            }
        });
    }

    // Join threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

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
        box.setFillColor(flashWhite);

        label = createInputLabel(labelText, font, 16, position.x + 5, position.y, size.y);
        text.setFont(font);
        text.setCharacterSize(15);
        text.setFillColor(slateBlue);
        text.setPosition(position.x, position.y + (size.y - text.getCharacterSize()) / 2.0f);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(box);
        window.draw(label);
        window.draw(text);

        // Adjust the position of the text to be to the right of the label
        text.setPosition(box.getPosition().x + label.getLocalBounds().width + 10, text.getPosition().y);

        if (isActive && cursorVisible) {
            sf::RectangleShape cursor(sf::Vector2f(2, text.getCharacterSize()));
            cursor.setFillColor(slateBlue);
            cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 3, text.getPosition().y);
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

// Helper function to reset and update input boxes
void updateInputBoxes(std::vector<InputBox>& inputBoxes, sf::Font& font, float startY, int form) {
    inputBoxes.clear(); // Clear existing input boxes
    
    if (form == 1) {
        // Add new input boxes for "Form 1"
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "N:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Start X:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 70), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Start Y:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 105), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "End X:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 140), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "End Y:", font);

        // Keep "Angle:" and "Velocity:" input boxes
        float nextInputY = startY + 175;
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, nextInputY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Angle:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, nextInputY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Velocity:", font);
    }
    else if (form == 2) {
        // Add new input boxes for "Form 2"
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "N:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 70), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 105), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Start Angle:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 140), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "End Angle:", font);

        // Keep "Angle:" and "Velocity:" input boxes
        float nextInputY = startY + 175;
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, nextInputY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Velocity:", font);
    }
    else if (form == 3) {
        // Add new input boxes for "Form 3"
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "N:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 70), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 105), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Angle:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 140), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Start Velocity:", font);

        // Keep "Angle:" and "Velocity:" input boxes
        float nextInputY = startY + 175;
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, nextInputY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "End Velocity:", font);
    }
    else {
        // Create input boxes for Balls
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 70), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Angle:", font);
        inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, startY + 105), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Velocity:", font);
    }

    // Create input boxes for Walls
    float wallInputsStartY = 150 + 240 + 70; // Adjust based on the final position of the Balls input boxes

    // Create input boxes for Walls
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X1:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y1:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY + 70), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X2:", font);
    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY + 105), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y2:", font);

}

// Global mutex for rendering synchronization
std::mutex renderMutex;

// Thread function for rendering balls
void renderBalls(sf::RenderWindow& window, const std::vector<Ball>& balls) {
    std::lock_guard<std::mutex> lock(renderMutex);
    for (const auto& ball : balls) {
        ball.draw(window);
    }
}

// Thread function for rendering walls
void renderWalls(sf::RenderWindow& window, const std::vector<Wall>& walls) {
    std::lock_guard<std::mutex> lock(renderMutex);
    for (const auto& wall : walls) {
        wall.draw(window);
    }
}

int main() {

// FPS calculation and display
    unsigned int frameCount = 0;
    sf::Clock fpsClock;
    sf::Text fpsText;
    sf::Clock displayClock; // Clock to update the display every 0.5 seconds

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulator");

    // Set the frame rate limit to 60 frames per second
    window.setFramerateLimit(60);

    // Define the display area for the simulation
    sf::RectangleShape displayArea(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT));
    displayArea.setFillColor(peachFuzz);  // background for the simulation area
    displayArea.setPosition(0, 0);

    sf::Font font;

    if (!font.loadFromFile("/Users/janinechuaching/Desktop/rawr/Inter-Regular.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        return -1;
    }

    // Initialize text labels for the sections
    sf::Text ballsTitle = createLabel("Balls", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 20);
    sf::Text wallsTitle = createLabel("Walls", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 360+70);
    
    // Initialize text label for the "Batch Form:" subtitle
    sf::Text batchFormTitle = createLabel("Batch Form:", font, 18, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 45);

    // Position radio buttons side by side in a single line
    float startX = WINDOW_WIDTH - SIDEBAR_WIDTH + 10;
    float radioButtonSpacing = 80; // Horizontal spacing between radio buttons
    
    std::vector<RadioButton> radioButtons;

    // Assuming font and characterSize are already defined and appropriate
    radioButtons.emplace_back(startX, 75, "Form 1", font, 14);
    radioButtons.emplace_back(startX + radioButtonSpacing, 75, "Form 2", font, 14);
    radioButtons.emplace_back(startX + 2 * radioButtonSpacing, 75, "Form 3", font, 14);

    // Define input boxes for the balls and walls
    std::vector<InputBox> inputBoxes;

    // Create input boxes for Balls
    float ballInputsStartY = 110; // Adjust this value as needed to make space for radio buttons
    updateInputBoxes(inputBoxes, font, ballInputsStartY, -1);

    // Create input boxes for Walls
    //float wallInputsStartY = 150 + 240 + 70; // Adjust based on the final position of the Balls input boxes

    //// Create input boxes for Walls
    //inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X1:", font);
    //inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY + 35), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y1:", font);
    //inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY + 70), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X2:", font);
    //inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, wallInputsStartY + 105), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y2:", font);
    //
    // Define buttons and their labels
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;

    // Add Ball button with label
    buttons.push_back(createTextButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 360, SIDEBAR_WIDTH - 20, INPUT_HEIGHT, "Add", font, buttonTexts));
    // Add Wall button with label
    buttons.push_back(createTextButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 605, SIDEBAR_WIDTH - 20, INPUT_HEIGHT, "Add", font, buttonTexts));

    std::vector<Ball> balls;  // Vector to hold all the balls

    sf::Clock clock; 

    // Initialize FPS text
    fpsText.setFont(font);
    fpsText.setCharacterSize(20); // Adjust size as needed
    fpsText.setFillColor(slateBlue); // Choose a color that stands out
    fpsText.setPosition(WINDOW_WIDTH - 210, WINDOW_HEIGHT - 50); // Position it at the top-right corner or wherever you prefer
    int activeForm = -1;
    
    // Main event loop
    while (window.isOpen()) {
        sf::Event event;

        sf::Time elapsed = clock.restart(); // Restart the clock and get the elapsed time
        float deltaTime = elapsed.asSeconds(); // Convert elapsed time to seconds

        // Update balls in parallel
        updateBallsInParallel(balls, displayArea, walls, deltaTime);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for mouse clicks to activate input boxes
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                // Example usage in your main loop when handling radio button selection
                for (auto& radio : radioButtons) {
                    if (radio.contains(mousePos)) {
                        bool wasSelected = radio.isSelected;

                        for (auto& otherRadio : radioButtons) {
                            otherRadio.deselect(); // Deselect all buttons
                        }
                        if (!wasSelected) {
                            // If the clicked radio button was not already selected, select it
                            radio.select();
                        }
                        // After updating the selection state, check which form should be active or if we should revert to default
                        if (!radio.isSelected) {
                            // If no radio button is selected (i.e., the clicked button was deselected),
                            // revert input boxes to their original state.
                            updateInputBoxes(inputBoxes, font, 110, -1);
                            activeForm = -1;
                        }
                        else {
                            // If a radio button is selected, update the input boxes based on the selected form
                            if (radio.label == "Form 1") {
                                activeForm = 1;
                                updateInputBoxes(inputBoxes, font, 110, 1);
                            }
                            else if (radio.label == "Form 2") {
                                activeForm = 2;
                                updateInputBoxes(inputBoxes, font, 110, 2);
                            }
                            else if (radio.label == "Form 3") {
                                activeForm = 3;
                                updateInputBoxes(inputBoxes, font, 110, 3);
                            }
                        }
                        break;
                    }
                }
                int n_input;

                if (activeForm == 1) {
                    n_input = 7;
                }
                else if (activeForm == 2 || activeForm == 3) {
                    n_input = 6;
                }
                else {
                    n_input = 4;
                }

                // Check if the "Add" button for balls was clicked
                if (buttons[0].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Then, modify the section where you add a new ball
                    // Assuming inputs are valid numbers for simplicity; add validation as needed
                    int N;
                    float x, y, angle, speed;
                    float startX, startY, endX, endY;
                    float startAngle, endAngle;
                    float startVelocity, endVelocity;
                    float radius = 3.0f; // Example radius, you would get this from input or define it
                    sf::Color color = sf::Color{ slateBlue }; // Example color, you could make this selectable

                    switch (activeForm) {
                    case 1:
                        N = std::stoi(inputBoxes[0].inputString); // Number of balls
                        startX = std::stof(inputBoxes[1].inputString);
                        startY = std::stof(inputBoxes[2].inputString);
                        endX = std::stof(inputBoxes[3].inputString);
                        endY = std::stof(inputBoxes[4].inputString);
                        angle = std::stof(inputBoxes[5].inputString);
                        speed = std::stof(inputBoxes[6].inputString);

                        for (int i = 0; i < N; ++i) {
                            float t = (float)i / (N - 1); // Calculate interpolation parameter
                            x = startX + t * (endX - startX); // Interpolate X
                            y = startY + t * (endY - startY); // Interpolate Y
                            std::cout << x << " " << y;
                            balls.emplace_back(x, y, radius, color, speed, angle);
                        }
                        break;
                    case 2:
                        // Handling for Form 2
                        N = std::stoi(inputBoxes[0].inputString); // Number of balls
                        x = std::stof(inputBoxes[1].inputString); // X position
                        y = std::stof(inputBoxes[2].inputString); // Y position
                        startAngle = std::stof(inputBoxes[3].inputString);
                        endAngle = std::stof(inputBoxes[4].inputString);
                        speed = std::stof(inputBoxes[5].inputString);

                        for (int i = 0; i < N; ++i) {
                            float t = (float)i / (N - 1); // Calculate interpolation parameter
                            angle = startAngle + t * (endAngle - startAngle); // Interpolate Angle
                            std::cout << angle;
                            balls.emplace_back(x, y, radius, color, speed, angle);
                        }
                        break;
                    case 3:
                        // Handling for Form 3
                        N = std::stoi(inputBoxes[0].inputString); // Number of balls
                        x = std::stof(inputBoxes[1].inputString); // X position
                        y = std::stof(inputBoxes[2].inputString); // Y position
                        angle = std::stof(inputBoxes[3].inputString);
                        startVelocity = std::stof(inputBoxes[4].inputString);
                        endVelocity = std::stof(inputBoxes[5].inputString);

                        for (int i = 0; i < N; ++i) {
                            float t = (float)i / (N - 1); // Calculate interpolation parameter
                            speed = startVelocity + t * (endVelocity - startVelocity); // Interpolate Velocity
                            std::cout << speed;
                            balls.emplace_back(x, y, radius, color, speed, angle);
                        }
                        break;
                    default:
                        x = std::stof(inputBoxes[0].inputString);
                        y = std::stof(inputBoxes[1].inputString);
                        angle = std::stof(inputBoxes[2].inputString); // Angle in degrees
                        speed = std::stof(inputBoxes[3].inputString); // Speed
                        // No form is selected or handling for default case
                        balls.emplace_back(x, y, radius, color, speed, angle);
                        break;
                    }
                    
                    //float radius = 4.0f; // Example radius, you would get this from input or define it
                    //sf::Color color = sf::Color{ pink2 }; // Example color, you could make this selectable


                    //// Create a new ball and add it to the vector
                    //balls.emplace_back(x, y, radius, color, speed, angle);

                    // Clear the input fields

                    for (int i = 0; i < n_input; ++i) {
                        inputBoxes[i].inputString.clear();
                        inputBoxes[i].text.setString("");
                    }
                }

                // Check if the "Add" button for walls was clicked
                if (buttons[1].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Get wall input values
                    float x1 = std::stof(inputBoxes[n_input].inputString);
                    float y1 = std::stof(inputBoxes[n_input+1].inputString);
                    float x2 = std::stof(inputBoxes[n_input+2].inputString);
                    float y2 = std::stof(inputBoxes[n_input+3].inputString);

                    // Check if the input values are within the display area
                    if (displayArea.getGlobalBounds().contains(x1, y1) && displayArea.getGlobalBounds().contains(x2, y2)) {
                        // Create a new wall and add it to the vector
                        walls.emplace_back(sf::Vector2f(x1, WINDOW_HEIGHT - y1), sf::Vector2f(x2, WINDOW_HEIGHT - y2));
                    
                        // Clear the input fields
                        for (int i = n_input; i < n_input+4; ++i) {
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
        // for (auto& ball : balls) {
        //     ball.update(displayArea, walls, deltaTime); // Pass deltaTime to update method
        // }
        // Update balls in parallel
        updateBallsInParallel(balls, displayArea, walls, deltaTime);

        window.clear(columbiaBlue); // Clear the window

        // Draw the display area
        window.draw(displayArea);

        // Draw the titles
        window.draw(ballsTitle);
        window.draw(wallsTitle);
        window.draw(batchFormTitle);

        // Before window.display(); to ensure they are drawn in this frame
        for (auto& radio : radioButtons) {
            radio.draw(window);
        }

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

        // Increment frame count
        frameCount++;

        // Check if half a second has passed to update the FPS display
        if (displayClock.getElapsedTime().asSeconds() >= 0.5f) {
            // Calculate FPS
            float fps = frameCount / fpsClock.restart().asSeconds();
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

            // Reset frame count and restart the display clock
            frameCount = 0;
            displayClock.restart();
        }

        // Draw the FPS text
        window.draw(fpsText);

        // Inside your main loop, before window.display();
        std::thread ballsThread(renderBalls, std::ref(window), std::ref(balls));
        std::thread wallsThread(renderWalls, std::ref(window), std::ref(walls));

        // Make sure to join the threads
        ballsThread.join();
        wallsThread.join();

        window.display(); // Display everything we have drawn
    }

    return 0;
}
