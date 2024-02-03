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
const sf::Color INPUT_BG_COLOR(200, 200, 200);  // Light grey
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
    box.setFillColor(INPUT_BG_COLOR);
    return box;
}

// Function to create a button
sf::RectangleShape createButton(float x, float y, float width, float height) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(BUTTON_BG_COLOR);
    return button;
}

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulator");

    // Define the display area for the simulation
    sf::RectangleShape displayArea(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT));
    displayArea.setFillColor(sf::Color{ pink4 }); // Assuming white background for the simulation area
    displayArea.setPosition(0, 0);

    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\Ayisha\\Documents\\GitHub\\poopsicle-particles\\poop\\res\\Inter-Regular.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        return -1;
    }

    // Initialize text with content, font, and size
    sf::Text ballsTitle("Balls", font, 24);
    sf::Text wallsTitle("Walls", font, 24);
    ballsTitle.setFillColor(sf::Color::Black);
    wallsTitle.setFillColor(sf::Color::Black);

    ballsTitle.setPosition(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 50 * (INPUT_HEIGHT + 10));

    // Define input boxes and buttons for the balls and walls
    std::vector<sf::RectangleShape> inputBoxes;
    std::vector<sf::RectangleShape> buttons;

    // Create input boxes for Balls
    for (int i = 0; i < 4; ++i) {
        inputBoxes.push_back(createInputBox(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 50 + i * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));
    }
    // Create the Add Ball button
    buttons.push_back(createButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 50 + 4 * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));

    // Create input boxes for Walls
    for (int i = 0; i < 4; ++i) {
        inputBoxes.push_back(createInputBox(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 300 + i * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));
    }
    // Create the Add Wall button
    buttons.push_back(createButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 300 + 4 * (INPUT_HEIGHT + 10), SIDEBAR_WIDTH - 20, INPUT_HEIGHT));

    // Main event loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // TODO: Implement event handling for input and buttons
        }

        window.clear(sf::Color{ pink5 }); // Clear the window

        // Draw the display area
        window.draw(displayArea);

        // Draw input boxes and buttons
        for (const auto& box : inputBoxes) {
            window.draw(box);
        }
        for (const auto& button : buttons) {
            window.draw(button);
        }

        // TODO: Draw balls and walls inside the display area

        window.display(); // Display everything we have drawn
    }

    return 0;
}