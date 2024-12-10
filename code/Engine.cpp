#include "Engine.h"
#include "Particle.h"

bool wait = false;

Engine::Engine() {
    VideoMode desktop = VideoMode::getDesktopMode(); // res
    m_Window.create(desktop, "Particle Engine", Style::Fullscreen); // be fullscreen
}


void Engine::run() {
    Clock clock; 
    while (m_Window.isOpen()) {
        float dtAsSeconds = clock.restart().asSeconds(); // Calculate the time since the last frame in seconds
        input();
        update(dtAsSeconds); // update the state of all particles
        draw(); // rendering particles
    }
}

// users interactions
void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) { // doing stuff in a queue
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
            m_Window.close(); // closing if "esc" or closed by top right X
        }
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
            wait = !wait;
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mouseClick = Mouse::getPosition(m_Window); // Get mouse click position relative to the window.
            for (int i = 0; i < 5; ++i) {
                // 5 particles at the click position with random 
                m_particles.emplace_back(m_Window, rand() % 26 + 25, mouseClick);
            }
        }
    }
}

// Updates all active particles, removing expired ones.
void Engine::update(float dtAsSeconds) {
    if(!wait){
        for (auto it = m_particles.begin(); it != m_particles.end(); ) {
            if (it->getTTL() > 0.0f) {
                it->update(dtAsSeconds); // update the particles state based on elapsed time
                ++it; 
            }
            else {
                it = m_particles.erase(it); // remove particles whose time-to-live (TTL) has expired
            }
        }
    }
}

// rendering
void Engine::draw() {
    m_Window.clear(); 
    for (const auto& particle : m_particles) {
        m_Window.draw(particle); // draw each particle
    }
    m_Window.display();
}