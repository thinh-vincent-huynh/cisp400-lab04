#include "Engine.h"
#include "Particle.h"
sf::Image image;
sf::Texture texture;
sf::Sprite sprite;
bool wait = false; // Pause flag
bool fireworkActive = false; // Flag to track firework activity
int user_x;

Engine::Engine() {
    VideoMode desktop = VideoMode::getDesktopMode(); // Get screen resolution
    m_Window.create(desktop, "Particle Engine", Style::Fullscreen); // Fullscreen mode
    image.loadFromFile("firework1.png"); // Load firework image
    texture.loadFromImage(image); // Load texture from image
    sprite.setTexture(texture); // Assign texture to sprite
    sprite.setPosition(-9999, -9999); // Initialize off-screen
}

void Engine::run() {
    Clock clock;
    while (m_Window.isOpen()) {
        float dtAsSeconds = clock.restart().asSeconds(); // Calculate elapsed time
        input();
        update(dtAsSeconds); // Update the state of all objects
        draw(); // Render everything
    }
}

// Handle user interactions
void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) { // Handle events
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
            m_Window.close(); // Close the window
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
            wait = !wait; // Toggle pause
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            // Set firework position and activate it
            Vector2i mouseClick = Mouse::getPosition(m_Window);
            user_x = mouseClick.x;
            sprite.setPosition(user_x - 260, VideoMode::getDesktopMode().height - 50);
            fireworkActive = true; // Activate the firework
        }
    }
}

// Updates all active objects
void Engine::update(float dtAsSeconds) {
    if (!wait && fireworkActive) {
        // Move the sprite upward
        sprite.move(0, -750 * dtAsSeconds);

        // Deactivate firework when it goes off-screen
        if (sprite.getPosition().y + sprite.getGlobalBounds().height < 0) {
            fireworkActive = false; // Deactivate the sprite
            sprite.setPosition(-9999, -9999); // Move it off-screen
        }
    }

    // Update particles and remove expired ones
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        if (it->getTTL() > 0.0f) {
            it->update(dtAsSeconds);
            ++it;
        }
        else {
            it = m_particles.erase(it);
        }
    }
}

// Renders everything
void Engine::draw() {
    m_Window.clear();

    // Only draw the firework if it is active
    if (fireworkActive) {
        m_Window.draw(sprite);
    }

    // Draw all active particles
    for (const auto& particle : m_particles) {
        m_Window.draw(particle);
    }

    m_Window.display();
}