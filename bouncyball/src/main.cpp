#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <future>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants for the GUI layout
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const unsigned int SIDEBAR_WIDTH = 320;
const unsigned int INPUT_HEIGHT = 30;

// Color for display
const sf::Color columbiaBlue(210, 224, 237);
const sf::Color peachFuzz(255, 215, 194);
const sf::Color flashWhite(246, 246, 248);
const sf::Color slateBlue(110, 110, 110);
const sf::Color cornFlower(160,207,234);

// Function declarations 
sf::RectangleShape createButton(float x, float y, float width, float height);
sf::RectangleShape createTextButton(float x, float y, float width, float height, const std::string& textContent, sf::Font& font, std::vector<sf::Text>& buttonTexts);
sf::Text createLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y);
sf::Vector2f reflect(const sf::Vector2f& velocity, const sf::Vector2f& normal);
sf::Text createInputLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y, float boxHeight);

// Wall Class
// Represents a wall in the simulation, defined by start and end points. It calculates its own shape, size, and orientation based on these points and can draw itself on a render window.
class Wall {
public:
    sf::Vector2f start;
    sf::Vector2f end;
    sf::RectangleShape shape;

    Wall(sf::Vector2f start, sf::Vector2f end) : start(start), end(end) {
        sf::Vector2f direction = end - start;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        shape.setSize(sf::Vector2f(length, 2)); // The wall has a thickness of 2 units
        shape.setOrigin(0, 1); // Origin is at the top left corner
        shape.setPosition(start);

        // Calculation for the angle to rotate the wall
        float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
        shape.setRotation(angle);
        shape.setFillColor(slateBlue);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    } 
};


// Radio Button Class
// Represents a radio button with an outer circle, an inner circle, and a label. It can draw itself, handle selection/deselection, and detect if it contains a given point (for mouse interaction).
class RadioButton {
public:
    sf::CircleShape outerCircle;
    sf::CircleShape innerCircle;
    sf::Text labelText; // Label text for the radio button
    std::string label;
     bool isSelected;

    RadioButton(float x, float y, std::string labelText, sf::Font& font, unsigned int characterSize = 15) : isSelected(false), label(labelText) {
        outerCircle.setRadius(8); // Smaller radius for the outer circle
        outerCircle.setPosition(x, y);
        outerCircle.setFillColor(sf::Color::White);

        innerCircle.setRadius(4); // Smaller radius for the inner circle
        // Position to ensure innerCircle remains centered within outerCircle
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

    bool contains(sf::Vector2f point) {
        return outerCircle.getGlobalBounds().contains(point);
    }
};

sf::Vector2f getWallNormal(const Wall& wall);

// Ball Class
// Represents a moving ball in the simulation. It keeps track of its position, velocity, and can calculate its new position based on collisions with boundaries and walls. It can also draw itself and check for line intersections (for collision detection).
class Ball {
public:
    sf::CircleShape shape;
    float vx, vy; // Velocity components

    Ball(float x, float y, float radius, sf::Color color, float speed, float angleInDegrees)
    : shape(radius) {
        // Invert the y coordinate to match the coordinate system
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

        // Wall collision handling
        bool collisionDetected = false;
        sf::Vector2f collisionPoint;
        sf::Vector2f wallNormal;
        for (const auto& wall : walls) {
            if (lineIntersect(startPosition, endPosition, wall.start, wall.end, &collisionPoint)) {
                collisionDetected = true;                 // Collision detected
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

            // Adjust the ball's position to the point of collision plus a bit back,
            // so it won't collide again in the next frame because of numerical errors
            sf::Vector2f newPosition = collisionPoint - (incomingVelocity * deltaTime * 0.5f);
            shape.setPosition(newPosition);
        } else {
            shape.setPosition(endPosition);             // If no collision was detected,  move the ball to its new position
        }
    }
};


// Input Box Class
// Represents an interactive input box where users can type in data. It includes a text label, handles keyboard input, cursor visibility, and can be activated or deactivated based on user interaction.
class InputBox {
public:
    sf::RectangleShape box;
    sf::Text text;
    sf::Text label;
    std::string inputString;
    sf::Clock cursorClock;
    bool isActive;
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

        // Adjusts position of the text to be to the right of the label
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

        if (event.type == sf::Event::TextEntered) {         // Handle keyboard input
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
        if (isActive) {         // Blinking cursor logic
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

// Variables
std::vector<Wall> walls; // global vector to store walls
std::vector<Ball> balls;
std::mutex vectorMutex; // Mutex to protect shared vectors
int updateInterval = 5; // Update every 5 frames
sf::Text errorMessage;
bool showError = false;
sf::Clock errorClock; // Tracks how long the error message has been displayed
const float errorDisplayTime = 3.0f; // Error message display time in seconds

// Functions
void updateInputBoxes(std::vector<InputBox>& inputBoxes, sf::Font& font, float startY, int form);
void updateBallsInParallel(std::vector<Ball>& balls, const sf::RectangleShape& boundary, const std::vector<Wall>& walls, float deltaTime);
void addBallSafely(const Ball& ball); 
void updateBalls(float deltaTime, const sf::RectangleShape& displayArea, const std::vector<Wall>& walls, int currentFrame);
void drawBalls(sf::RenderWindow& window);
void triggerErrorMessage();

// Main Function
int main() {

    unsigned int frameCount = 0;
    sf::Clock fpsClock;
    sf::Clock displayClock; // Clock to update the display every 0.5 seconds
    sf::Clock clock; 
    sf::Text fpsText;
    
    sf::Font font;
    std::vector<RadioButton> radioButtons;
    std::vector<InputBox> inputBoxes; // Define input boxes for the balls and walls
    std::vector<sf::RectangleShape> buttons;   // Define buttons and their labels
    std::vector<sf::Text> buttonTexts;

    //std::vector<Ball> balls;  // Vector to hold all the balls

    // Create the main window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulator");

    // Set the frame rate limit to 60 frames per second
    window.setFramerateLimit(60);

    // Define the display area for the simulation
    sf::RectangleShape displayArea(sf::Vector2f(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT));
    displayArea.setFillColor(peachFuzz);  // background for the simulation area
    displayArea.setPosition(0, 0);
    
    //   if (!font.loadFromFile("C:/Users/Ayisha/Documents/GitHub/bouncyball/bouncyball/res/Inter-Regular.ttf")) {
    //       std::cout << "Failed to load font!" << std::endl;
    //       return -1;
    //   }
    
    /*if (!font.loadFromFile("/Users/janinechuaching/Desktop/rawr/Inter-Regular.ttf")) {
       std::cout << "Failed to load font!" << std::endl;
       return -1;
    }*/

    // if (!font.loadFromFile("../res/Inter-Regular.ttf")) {
    // std::cerr << "Failed to load font!" << std::endl;
    // return -1;
    // }

    if (!font.loadFromFile("res/Inter-Regular.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
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

    radioButtons.emplace_back(startX, 75, "Form 1", font, 14);
    radioButtons.emplace_back(startX + radioButtonSpacing, 75, "Form 2", font, 14);
    radioButtons.emplace_back(startX + 2 * radioButtonSpacing, 75, "Form 3", font, 14);

    // Create input boxes for Balls
    float ballInputsStartY = 110; // Makes space for radio buttons
    updateInputBoxes(inputBoxes, font, ballInputsStartY, -1);

    // Add Ball button with label
    buttons.push_back(createTextButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 360, SIDEBAR_WIDTH - 20, INPUT_HEIGHT, "Add", font, buttonTexts));
    // Add Wall button with label
    buttons.push_back(createTextButton(WINDOW_WIDTH - SIDEBAR_WIDTH + 10, 605, SIDEBAR_WIDTH - 20, INPUT_HEIGHT, "Add", font, buttonTexts));

    // Initialize FPS text
    fpsText.setFont(font);
    fpsText.setCharacterSize(20); 
    fpsText.setFillColor(slateBlue);
    fpsText.setPosition(WINDOW_WIDTH - 210, WINDOW_HEIGHT - 50); // Position it at the top-right corner
    int activeForm = -1;
    float deltaTime = clock.restart().asSeconds();

    //Error Message
    errorMessage.setFont(font); // Assuming 'font' is already loaded
    errorMessage.setString("Input Error");
    errorMessage.setCharacterSize(16); // Example size
    errorMessage.setFillColor(sf::Color::Red); // Error message color
    // Position the error message in the upper right corner of the displayArea
    errorMessage.setPosition(WINDOW_WIDTH - SIDEBAR_WIDTH - errorMessage.getLocalBounds().width - 10, 10);

    
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
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                for (auto& radio : radioButtons) {
                    if (radio.contains(mousePos)) {
                        bool wasSelected = radio.isSelected;

                        for (auto& otherRadio : radioButtons) {
                            otherRadio.deselect(); // Deselect all buttons
                        }
                        if (!wasSelected) {
                            radio.select(); // If the clicked radio button was not already selected, select it
                        }
                
                        // If no radio button is selected (i.e., the clicked button was deselected),revert input boxes to their original state.
                        if (!radio.isSelected) {     
                            updateInputBoxes(inputBoxes, font, 110, -1);
                            activeForm = -1;
                        }
                        else { // If a radio button is selected, update the input boxes based on the selected form
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
                    
                    int N;
                    float x, y, angle, speed;
                    float startX, startY, endX, endY;
                    float startAngle, endAngle;
                    float startVelocity, endVelocity;
                    float radius = 3.0f; 
                    sf::Color color = sf::Color{ slateBlue }; 

                    switch (activeForm) {
                    case 1: // Handling for Form 1
                        try {
                            N = std::stoi(inputBoxes[0].inputString);
                            startX = std::stof(inputBoxes[1].inputString);
                            startY = std::stof(inputBoxes[2].inputString);
                            endX = std::stof(inputBoxes[3].inputString);
                            endY = std::stof(inputBoxes[4].inputString);
                            angle = std::stof(inputBoxes[5].inputString);
                            speed = std::stof(inputBoxes[6].inputString);
                            
                        }
                        catch (std::invalid_argument const& e) {
                            std::cerr << "Invalid input for X: not a number" << std::endl;
                            triggerErrorMessage();
                        }
                        catch (std::out_of_range const& e) {
                            std::cerr << "Invalid input for X: out of range" << std::endl;
                            triggerErrorMessage();
                        }

                        if (N > 0 && startX >= 0 && startY >= 0 && endX >= 0 && endY >= 0 && speed >= 0 &&  
                         startX <= WINDOW_WIDTH-SIDEBAR_WIDTH && startY <= WINDOW_HEIGHT && endX <= WINDOW_WIDTH-SIDEBAR_WIDTH && endY <= WINDOW_HEIGHT) {
                            for (int i = 0; i < N; ++i) {
                                float t = (float)i / (N - 1); // Calculate interpolation parameter
                                x = startX + t * (endX - startX); // Interpolate X
                                y = startY + t * (endY - startY); // Interpolate Y
                                Ball newBall(x, y, radius, color, speed, angle);
                                addBallSafely(newBall);
                            } 
                        } else{
                            std::cerr << "Invalid input: values must be non-negative and within screen bounds." << std::endl;
                            triggerErrorMessage();
                        }
                    
                        break;
                    case 2: // Handling for Form 2
                        try {
                            N = std::stoi(inputBoxes[0].inputString); // Number of balls
                            x = std::stof(inputBoxes[1].inputString); // X position
                            y = std::stof(inputBoxes[2].inputString); // Y position
                            startAngle = std::stof(inputBoxes[3].inputString);
                            endAngle = std::stof(inputBoxes[4].inputString);
                            speed = std::stof(inputBoxes[5].inputString);
                    
                        }   
                        catch (std::invalid_argument const& e) {
                            std::cerr << "Invalid input for X: not a number" << std::endl;
                            triggerErrorMessage();
                        }
                        catch (std::out_of_range const& e) {
                            std::cerr << "Invalid input for X: out of range" << std::endl;
                            triggerErrorMessage();
                        }

                        if(N > 0 && x >= 0 && y >= 0 && startAngle >= 0 && endAngle >= 0 && speed >= 0 && 
                            x <= WINDOW_WIDTH-SIDEBAR_WIDTH && y <= WINDOW_HEIGHT){
                            for (int i = 0; i < N; ++i) {
                                float t = (float)i / (N - 1); // Calculate interpolation parameter
                                angle = startAngle + t * (endAngle - startAngle); // Interpolate Angle
                                Ball newBall(x, y, radius, color, speed, angle);
                                addBallSafely(newBall);
                            }                  
                        }
                        else{
                            std::cerr << "Invalid input: values must be non-negative and within screen bounds." << std::endl;
                            triggerErrorMessage();
                        }
            
                        break;
                    case 3: // Handling for Form 3
                        try {
                            N = std::stoi(inputBoxes[0].inputString); // Number of balls
                            x = std::stof(inputBoxes[1].inputString); // X position
                            y = std::stof(inputBoxes[2].inputString); // Y position
                            angle = std::stof(inputBoxes[3].inputString);
                            startVelocity = std::stof(inputBoxes[4].inputString);
                            endVelocity = std::stof(inputBoxes[5].inputString);
                        }
                        catch (std::invalid_argument const& e) {
                            std::cerr << "Invalid input for X: not a number" << std::endl;
                            triggerErrorMessage();
                        }
                        catch (std::out_of_range const& e) {
                            std::cerr << "Invalid input for X: out of range" << std::endl;
                            triggerErrorMessage();
                        }

                        if(N > 0 && x >= 0 && y >= 0 && angle >= 0 && startVelocity >= 0 && endVelocity >= 0 &&
                            x <= WINDOW_WIDTH - SIDEBAR_WIDTH && y <= WINDOW_HEIGHT){
                            for (int i = 0; i < N; ++i) {
                                float t = (float)i / (N - 1); // Calculate interpolation parameter
                                speed = startVelocity + t * (endVelocity - startVelocity); // Interpolate Velocity
                                Ball newBall(x, y, radius, color, speed, angle);
                                addBallSafely(newBall);
                            }
                        } else {
                            std::cerr << "Invalid input: values must be non-negative and within screen bounds." << std::endl;
                            triggerErrorMessage();
                        }
                        
                        break;
                    default:
                        try{
                            x = std::stof(inputBoxes[0].inputString);
                            y = std::stof(inputBoxes[1].inputString);
                            angle = std::stof(inputBoxes[2].inputString); // Angle in degrees
                            speed = std::stof(inputBoxes[3].inputString); // Speed
                        }
                        catch (std::invalid_argument const& e) {
                            std::cerr << "Invalid input for X: not a number" << std::endl;
                            triggerErrorMessage();
                        }
                        catch (std::out_of_range const& e) {
                            std::cerr << "Invalid input for X: out of range" << std::endl;
                            triggerErrorMessage();
                        }
                        
                        if(x >= 0 && y >= 0 && angle >= 0 && speed >= 0 &&
                            x <= WINDOW_WIDTH - SIDEBAR_WIDTH && y <= WINDOW_HEIGHT){
                            Ball newBall(x, y, radius, color, speed, angle);
                            addBallSafely(newBall);
                        } else{
                            std::cerr << "Invalid input: values must be non-negative and within screen bounds." << std::endl;
                            triggerErrorMessage();
                        }
                        
                        break;
                    }

                    // Clear the input fields
                    for (int i = 0; i < n_input; ++i) {
                        inputBoxes[i].inputString.clear();
                        inputBoxes[i].text.setString("");
                    }
                }

                // Check if the "Add" button for walls was clicked
                if (buttons[1].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Get wall input values
                    float x1, y1,x2, y2;

                    try{
                        x1 = std::stof(inputBoxes[n_input].inputString);
                        y1 = std::stof(inputBoxes[n_input+1].inputString);
                        x2 = std::stof(inputBoxes[n_input+2].inputString);
                        y2 = std::stof(inputBoxes[n_input+3].inputString);
                        triggerErrorMessage();
                    }
                    catch (std::invalid_argument const& e) {
                        std::cerr << "Invalid input: not a number" << std::endl;
                        triggerErrorMessage();
                    }
                    catch (std::out_of_range const& e) {
                        std::cerr << "Invalid input: out of range" << std::endl;
                        triggerErrorMessage();
                    }
                    
                    if(x1 < WINDOW_WIDTH && y1 < WINDOW_HEIGHT && x2 < WINDOW_WIDTH && y2 < WINDOW_HEIGHT)
                    {
                        // Check if the input values are within the display area
                        if (displayArea.getGlobalBounds().contains(x1, y1) && displayArea.getGlobalBounds().contains(x2, y2)) {
                            walls.emplace_back(sf::Vector2f(x1, WINDOW_HEIGHT - y1), sf::Vector2f(x2, WINDOW_HEIGHT - y2)); // Create a new wall and add it to the vector
                        
                            // Clear the input fields
                            for (int i = n_input; i < n_input+4; ++i) {
                                inputBoxes[i].inputString.clear();
                                inputBoxes[i].text.setString("");
                            }
                        } else {
                            std::cout << "Wall coordinates must be within the display area!" << std::endl;
                            triggerErrorMessage();
                            // Clear the input fields
                            for (int i = n_input; i < n_input+4; ++i) {
                                inputBoxes[i].inputString.clear();
                                inputBoxes[i].text.setString("");
                            }
                        }

                    }else {
                            std::cerr << "Invalid input: values must be non-negative and within screen bounds." << std::endl;
                            triggerErrorMessage();
                            // Clear the input fields
                            for (int i = n_input; i < n_input+4; ++i) {
                                inputBoxes[i].inputString.clear();
                                inputBoxes[i].text.setString("");
                            }
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

        // Update balls in parallel
        updateBallsInParallel(balls, displayArea, walls, deltaTime);

        window.clear(columbiaBlue); 

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
            box.update(); 
            box.draw(window);  
        }
        
        // Draw buttons and their labels
        for (auto& button : buttons) {
            window.draw(button);
        }
        for (auto& buttonText : buttonTexts) {
            window.draw(buttonText);
        }

        updateBalls(static_cast<float>(deltaTime), displayArea, walls, frameCount); 
        drawBalls(window); // Draw balls and walls inside the display area

        for (auto& wall : walls) {
            wall.draw(window);
        }

        frameCount++; // Increment frame count

        // Check if half a second has passed to update the FPS display
        if (displayClock.getElapsedTime().asSeconds() >= 1.f) {
            float fps = frameCount / fpsClock.restart().asSeconds(); // Calculate FPS
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frameCount = 0; // Reset frame count and restart the display clock
            displayClock.restart();
        }

        window.draw(fpsText);

        // Inside the main loop, before window.display()

        if (showError) {
            // Check if the error display time has elapsed
            if (errorClock.getElapsedTime().asSeconds() >= errorDisplayTime) {
                showError = false; // Stop showing the error message
            }
            else {
                // Render the error message
                window.draw(errorMessage);
            }
        }

        window.display(); 
    }

    return 0;
}

// FUNCTIONS --------------------------------------------------------------

// Iteratively updates a subset of all balls' positions and checks for collisions to maintain performance across frames.
void updateBalls(float deltaTime, const sf::RectangleShape& displayArea, const std::vector<Wall>& walls, int currentFrame) {
    // Only update a subset of balls to maintain high FPS
    for (int i = currentFrame % updateInterval; i < balls.size(); i += updateInterval) {
        balls[i].update(displayArea, walls, deltaTime); // Update the ball's position and check for collisions
    }
}

// Draws all balls on the window without updating their state, ensuring visual representation is consistent with their physical model.
void drawBalls(sf::RenderWindow& window) {
    for (const auto& ball : balls) {
        ball.draw(window); 
    }
}

// Generates a text label at a specified position with a given font, size, and content, setting its color to slateBlue.
sf::Text createLabel(const std::string& content, sf::Font& font, unsigned int size, float x, float y) {
    sf::Text label;
    label.setFont(font);
    label.setString(content);
    label.setCharacterSize(size);
    label.setFillColor(slateBlue);
    label.setPosition(x, y);
    return label;
}

// Creates a text label for input boxes, aligning the text vertically within the input field for better UI consistency.
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

// Creates a basic rectangular button with specified dimensions and position, setting its fill color to cornFlower
sf::RectangleShape createButton(float x, float y, float width, float height) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(cornFlower);
    return button;
}

// Helper function that extends createButton to include text on the button, positioning the text centrally within the button and adding it to a vector for later rendering.
sf::RectangleShape createTextButton(float x, float y, float width, float height, const std::string& textContent, sf::Font& font, std::vector<sf::Text>& buttonTexts) {
    sf::RectangleShape button = createButton(x, y, width, height);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(textContent);
    buttonText.setCharacterSize(20); 
    buttonText.setFillColor(slateBlue);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(x + width / 2.0f, y + height / 2.0f);

    buttonTexts.push_back(buttonText); 

    return button;
}

// Calculates the reflection of a vector (velocity) off a surface with a given normal vector, using the reflection formula.
sf::Vector2f reflect(const sf::Vector2f& velocity, const sf::Vector2f& normal) {
    return velocity - 2 * (velocity.x * normal.x + velocity.y * normal.y) * normal;
}

// Computes the normal (perpendicular) vector to a Wall object, which is used in collision reflection calculations.
sf::Vector2f getWallNormal(const Wall& wall) {
    sf::Vector2f direction = wall.end - wall.start;     // Calculate direction vector of the wall
    sf::Vector2f normal(-direction.y, direction.x);     // Calculate normal (perpendicular) vector
    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);     // Normalize the normal vector
    normal /= length;
    return normal;
}

// Helper function that dynamically updates the set of input boxes displayed in the sidebar based on the active form selection, clearing any existing boxes and adding new ones as needed.
void updateInputBoxes(std::vector<InputBox>& inputBoxes, sf::Font& font, float startY, int form) {
    inputBoxes.clear(); 
    
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

// Updates the positions of all Ball objects in parallel using multiple threads to handle large numbers of balls efficiently.
void updateBallsInParallel(std::vector<Ball>& balls, const sf::RectangleShape& boundary, const std::vector<Wall>& walls, float deltaTime) {
    const size_t numThreads = std::thread::hardware_concurrency();
    const size_t totalBalls = balls.size();
    const size_t chunkSize = totalBalls / numThreads;
    size_t remainingBalls = totalBalls % numThreads;

    std::vector<std::future<void>> futures(numThreads);

    size_t startIdx = 0;
    for (size_t i = 0; i < numThreads; ++i) {
        size_t ballsToProcess = chunkSize + (remainingBalls > 0 ? 1 : 0);
        if (remainingBalls > 0) {
            --remainingBalls;
        }

        size_t endIdx = startIdx + ballsToProcess;
        futures[i] = std::async(std::launch::async, [startIdx, endIdx, &balls, &boundary, &walls, deltaTime]() {
            for (size_t j = startIdx; j < endIdx; ++j) {
                balls[j].update(boundary, walls, deltaTime);
            }
        });
        startIdx = endIdx;
    }

    for (auto& future : futures) {
        future.get();
    }
}

// Safely adds a new ball to the global balls vector using mutex locking to prevent concurrent access issues in a multithreaded environment.
void addBallSafely(const Ball& ball) {
    std::lock_guard<std::mutex> guard(vectorMutex);
    balls.push_back(ball);
}

void triggerErrorMessage() {
    showError = true;
    errorClock.restart();
}
