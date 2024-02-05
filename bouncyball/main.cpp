#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
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

    // Function to update the ball position and check for boundary collisions
    void update(const sf::RectangleShape& boundary) {
        // Move the ball
        shape.move(vx, vy);

        // Get the position of the ball
        sf::Vector2f position = shape.getPosition();

        // Check for left or right wall collisions
        if (position.x < boundary.getPosition().x) {
            position.x = boundary.getPosition().x;
            vx = -vx;
        }
        else if (position.x + shape.getRadius() * 2 > boundary.getPosition().x + boundary.getSize().x) {
            position.x = boundary.getPosition().x + boundary.getSize().x - shape.getRadius() * 2;
            vx = -vx;
        }

        // Check for top or bottom wall collisions
        if (position.y < boundary.getPosition().y) {
            position.y = boundary.getPosition().y;
            vy = -vy;
        }
        else if (position.y + shape.getRadius() * 2 > boundary.getPosition().y + boundary.getSize().y) {
            position.y = boundary.getPosition().y + boundary.getSize().y - shape.getRadius() * 2;
            vy = -vy;
        }

        // Wall collision detection and response
        //for (const auto& wall : walls) {
            // Here you need to implement the logic for line-circle intersection
            // and reflect the ball's velocity if a collision is detected
            // This is a more complex problem that involves some math
            // You may need to refer to an algorithm for line-circle collision detection
        //}


        // Set the new position of the ball
        shape.setPosition(position);
    }
};

class Wall {
public:
    sf::Vertex line[2];

    Wall(sf::Vector2f start, sf::Vector2f end) {
        line[0] = sf::Vertex(start);
        line[1] = sf::Vertex(end);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(line, 2, sf::Lines);
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
    if (!font.loadFromFile("res/Inter-Regular.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
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

    // Main event loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for mouse clicks to activate input boxes
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
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

                    for (auto& box : inputBoxes) {
                        if (box.box.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                            box.setActive(true);
                        }
                        else {
                            box.setActive(false);
                        }
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

        // Update the main loop to call the update method for each ball
        for (auto& ball : balls) {
            ball.update(displayArea); // Update the position of each ball
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

        window.display(); // Display everything we have drawn
    }

    return 0;
}