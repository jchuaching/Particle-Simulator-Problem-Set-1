//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <string>
//
//// Constants for the GUI
//const int WINDOW_WIDTH = 1280;
//const int WINDOW_HEIGHT = 720;
//const sf::Vector2f DISPLAY_AREA_SIZE(WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT);
//const sf::Vector2f INPUT_AREA_SIZE(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT);
//const sf::Vector2f BUTTON_SIZE(INPUT_AREA_SIZE.x * 0.8f, 40);
//const sf::Vector2f INPUT_BOX_SIZE(INPUT_AREA_SIZE.x * 0.8f, 30);
//const float BALL_RADIUS = 10.0f;
//
//// Forward declarations
//class Ball;
//class Wall;
//
//// Function to create a text input box
//sf::RectangleShape createInputBox(sf::Vector2f position, sf::Vector2f size);
//
//// Function to create a button
//sf::RectangleShape createButton(sf::Vector2f position, sf::Vector2f size);
//
//// Class to handle the ball object
//class Ball {
//public:
//    sf::CircleShape shape;
//    sf::Vector2f velocity;
//
//    Ball(float radius, sf::Vector2f position, sf::Vector2f velocity)
//        : shape(radius), velocity(velocity) {
//        shape.setPosition(position);
//        // Set additional properties if needed
//    }
//
//    void update(const sf::Time& dt) {
//        shape.move(velocity * dt.asSeconds());
//        // Add collision detection with walls here
//    }
//
//    void draw(sf::RenderWindow& window) {
//        window.draw(shape);
//    }
//};
//
//// Class to handle the wall object
//class Wall {
//public:
//    sf::RectangleShape shape;
//
//    Wall(sf::Vector2f position, sf::Vector2f size) {
//        shape.setSize(size);
//        shape.setPosition(position);
//        // Set additional properties if needed
//    }
//
//    void draw(sf::RenderWindow& window) {
//        window.draw(shape);
//    }
//};
//
//int main() {
//    // Create the main window
//    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Balls and Walls Simulation");
//
//    // Define the display area where the balls will bounce
//    sf::RectangleShape displayArea;
//    displayArea.setSize(DISPLAY_AREA_SIZE);
//    displayArea.setFillColor(sf::Color::White);
//    displayArea.setPosition(0, 0);
//
//    // Define GUI elements
//    std::vector<sf::RectangleShape> inputBoxes;
//    std::vector<sf::RectangleShape> buttons;
//
//    // Create input boxes and buttons
//    for (int i = 0; i < 4; ++i) {
//        inputBoxes.push_back(createInputBox(sf::Vector2f(WINDOW_WIDTH - INPUT_AREA_SIZE.x, i * (INPUT_BOX_SIZE.y + 5)), INPUT_BOX_SIZE));
//        buttons.push_back(createButton(sf::Vector2f(WINDOW_WIDTH - INPUT_AREA_SIZE.x, (i * (BUTTON_SIZE.y + 5)) + 200), BUTTON_SIZE));
//    }
//
//    // Main loop
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Handle other events like user input, button clicks, etc.
//        }
//
//        // Update logic for balls and walls
//
//        // Rendering
//        window.clear();
//        window.draw(displayArea);
//        for (auto& box : inputBoxes) {
//            window.draw(box);
//        }
//        for (auto& button : buttons) {
//            window.draw(button);
//        }
//        // Render balls and walls
//
//        window.display();
//    }
//
//    return 0;
//}
//
//sf::RectangleShape createInputBox(sf::Vector2f position, sf::Vector2f size) {
//    sf::RectangleShape box;
//    box.setSize(size);
//    box.setPosition(position);
//    box.setFillColor(sf::Color::Black);
//    // Additional styling can be added here
//    return box;
//}
//
//sf::RectangleShape createButton(sf::Vector2f position, sf::Vector2f size) {
//    sf::RectangleShape button;
//    button.setSize(size);
//    button.setPosition(position);
//    button.setFillColor(sf::Color(255, 0, 255)); // Sample color for the button
//    // Additional styling can be added here
//    return button;
//}
//
//#include <SFML/Graphics.hpp>
//#include <SFML/System/Clock.hpp>
//#include <iostream>
//#include <sstream>
//
//// Constants for GUI
//const unsigned int WINDOW_WIDTH = 1280;
//const unsigned int WINDOW_HEIGHT = 720;
//
//// Function Prototypes
//void setupGUI(sf::RenderWindow& window);
//
//int main() {
//    // Create the main application window
//    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close);
//
//    // Setup GUI elements
//    setupGUI(window);
//
//    // Main loop
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//            // Handle other events like user input, button clicks, etc.
//        }
//
//        // Clear the window with black color
//        window.clear(sf::Color::Black);
//
//        // Here, we would draw our GUI elements, particles, walls, and FPS counter
//
//        // Display the rendered frame on screen
//        window.display();
//    }
//
//    return 0;
//}
//
//void setupGUI(sf::RenderWindow& window) {
//    // Setup code for GUI elements such as input boxes, buttons, and FPS counter will go here.
//    // Placeholder for GUI setup code
//    // GUI Elements
//    sf::Font font;
//    sf::Text fpsCounter;
//    sf::Clock fpsClock; // Clock to measure the time between frames for FPS calculation
//    sf::Clock updateClock; // Clock to update the FPS counter every 0.5 seconds
//
//    // Load a font
//    if (!font.loadFromFile("C:\\Users\\Ayisha\\Documents\\GitHub\\poopsicle-particles\\poop\\res\\Inter-Regular.ttf")) {
//        std::cerr << "Error loading font 'arial.ttf'!" << std::endl;
//        exit(1);
//    }
//
//    // Setup FPS counter
//    fpsCounter.setFont(font);
//    fpsCounter.setCharacterSize(20); // in pixels
//    fpsCounter.setFillColor(sf::Color::White);
//    fpsCounter.setPosition(10.f, WINDOW_HEIGHT - 30.f);
//
//    // Setup input fields and buttons
//    // You will need to create sf::Text and sf::RectangleShape objects for each input field and button
//
//    // Placeholder for input fields and buttons setup code
//
//}
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

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

        window.display(); // Display everything we have drawn
    }

    return 0;
}

//int main() {
//    // Create the main window
//    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulator");
//
//    // Define the display area for the simulation
//    sf::RectangleShape displayArea(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT));
//    displayArea.setFillColor(sf::Color{ pink4 }); // Assuming white background for the simulation area
//    displayArea.setPosition(0, 0);
//
//    sf::Font font;
//    if (!font.loadFromFile("C:\\Users\\Ayisha\\Documents\\GitHub\\poopsicle-particles\\poop\\res\\Inter-Regular.ttf")) {
//        std::cout << "Failed to load font!" << std::endl;
//        return -1;
//    }
//
//    // Initialize text labels for the sections
//    sf::Text ballsTitle = createLabel("Balls", font, 24, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 20);
//    sf::Text wallsTitle = createLabel("Walls", font, 24, WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 380 - 30);
//
//
//    // Define input boxes and buttons for the balls and walls
//    std::vector<sf::RectangleShape> inputBoxes;
//    std::vector<sf::RectangleShape> buttons;
//
//    // Create input boxes for Balls
//    for (int i = 0; i < 4; ++i) {
//        inputBoxes.push_back(createInputBox(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 60 + i * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));
//    }
//    // Create the Add Ball button
//    buttons.push_back(createButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 60 + 4 * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));
//
//    // Create input boxes for Walls
//    for (int i = 0; i < 4; ++i) {
//        inputBoxes.push_back(createInputBox(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 390 + i * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));
//    }
//    // Create the Add Wall button
//    buttons.push_back(createButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 390 + 4 * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));
//    
//    // Initialize input box labels
//    std::vector<sf::Text> inputLabels;
//    std::vector<InputBox> inputBoxes;
//
//    // Labels for the Balls input boxes
//    inputLabels.push_back(createInputLabel("X:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 60, INPUT_HEIGHT));
//    inputLabels.push_back(createInputLabel("Y:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 110, INPUT_HEIGHT));
//    inputLabels.push_back(createInputLabel("Angle:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 160, INPUT_HEIGHT));
//    inputLabels.push_back(createInputLabel("Velocity:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 210, INPUT_HEIGHT));
//
//    // Labels for the Walls input boxes
//    inputLabels.push_back(createInputLabel("X1:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 390, INPUT_HEIGHT));
//    inputLabels.push_back(createInputLabel("Y1:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 440, INPUT_HEIGHT));
//    inputLabels.push_back(createInputLabel("X2:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 490, INPUT_HEIGHT));
//    inputLabels.push_back(createInputLabel("Y2:", font, 20, WINDOW_WIDTH - SIDEBAR_WIDTH + 17, 540, INPUT_HEIGHT));
//
//    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 60), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "X:", font);
//    inputBoxes.emplace_back(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 110), sf::Vector2f(SIDEBAR_WIDTH - 20, INPUT_HEIGHT), "Y:", font);
//    // Create strings to hold the input for each box
//    std::vector<std::string> inputStrings(8, ""); // Initialize with 8 empty strings
//
//    // Main event loop
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            // Handle text input
//            if (event.type == sf::Event::TextEntered) {
//                // Assuming the first input box is active, you'll need to add logic to manage which box is active
//                if (event.text.unicode < 128) { // Only handle standard ASCII characters
//                    inputStrings[0] += static_cast<char>(event.text.unicode);
//                }
//            }
//            
//            // Check for mouse clicks to activate input boxes
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    for (auto& box : inputBoxes) {
//                        if (box.box.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
//                            box.setActive(true);
//                        }
//                        else {
//                            box.setActive(false);
//                        }
//                    }
//                }
//            }
//
//            // Handle text input for the active input box
//            for (auto& box : inputBoxes) {
//                box.handleEvent(event);
//            }
//        }
//
//        // ... (clearing the window and drawing the display area)
//
//        // Draw the input boxes
//        for (auto& box : inputBoxes) {
//            box.update();  // Update each box (for blinking cursor)
//            box.draw(window);  // Draw each box
//        }
//
//        window.clear(sf::Color{ pink5 }); // Clear the window
//
//        // Draw the display area, titles, input boxes, input labels, and buttons
//        window.draw(displayArea);
//        window.draw(ballsTitle);
//        window.draw(wallsTitle);
//
//        // Draw input boxes and buttons
//        for (const auto& box : inputBoxes) {
//            window.draw(box);
//        }
//        for (const auto& button : buttons) {
//            window.draw(button);
//        }
//        for (const auto& label : inputLabels) {
//            window.draw(label);
//        }
//
//        // TODO: Draw balls and walls inside the display area
//        // TODO: Draw the current input text in the input boxes
//
//        window.display(); // Display everything we have drawn
//    }
//
//    return 0;
//}