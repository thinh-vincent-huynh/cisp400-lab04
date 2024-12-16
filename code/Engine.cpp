#include "Engine.h"
#include "Particle.h"
#include <vector>

#include <SFML/Audio.hpp>

Image image;
Texture texture;

SoundBuffer buffer; // stores sounds
Sound fireworkSE; //SE = sound effect

vector<Sprite> fireworks; // vector of firework sprites
vector<float> fireworkTargets; // vector of respective target y positions
vector<vector<Particle>> particleGroups; // group particles per firework


bool wait = false; // pause flag
float target;

// Firework and Sound are free commons

Engine::Engine() {
    VideoMode desktop = VideoMode::getDesktopMode(); // get screen resolution
    m_Window.create(desktop, "Particle Engine", Style::Fullscreen); // fullscreen mode

    image.loadFromFile("firework1.png"); // load firework image 
    texture.loadFromImage(image); // load texture from image
    
    if (!buffer.loadFromFile("fireworkSE.wav")) {
        std::cerr << "Error: Could not load sound file." << std::endl;
    }
    fireworkSE.setBuffer(buffer);
    fireworkSE.setVolume(100.f);
 

}

void Engine::run() {
    Clock clock;
    while (m_Window.isOpen()) {
        float dtAsSeconds = clock.restart().asSeconds(); // calculate elapsed time
        input(); //user's input by kb or mouse
        update(dtAsSeconds); // update the state of all objects
        draw(); // render everything
    }
}

// Handle user interactions
void Engine::input() {
    Event event;
    // close the window
    while (m_Window.pollEvent(event)) {
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
            m_Window.close();
        }

        // toggle pause
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
            wait = !wait; // Toggle pause
        }

        //user's input (mouse click) for firework and particle pos
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            
            // create a new firework sprite at the clicked position
            Vector2i mouseClick = Mouse::getPosition(m_Window);


            Sprite newFirework;
            newFirework.setTexture(texture); // use the preloaded texture
            newFirework.setPosition(mouseClick.x - 260, VideoMode::getDesktopMode().height - 50); // starts from the bottom (og image: 520x520)

            // add the firework sprite and its target y position
            fireworks.push_back(newFirework);
            fireworkTargets.push_back(mouseClick.y - 105);//105 bc its kinda in the middle of sprite (firework)

            fireworkSE.play();
            //float ratio = (VideoMode::getDesktopMode().height - mouseClick.y) / static_cast<float>(VideoMode::getDesktopMode().height);
            //fireworkSE.setPitch(0.5f + ratio * 1.5f); // Pitch range [0.5, 2.0]
         

            // generate particles at the clicked position and group them
            vector<Particle> newParticleGroup; //for each firework made will havea group so its not doesn't effect others

            for (int i = 0; i < 5; ++i) {
                newParticleGroup.emplace_back(m_Window, rand() % 26 + 25, mouseClick);
            }
            particleGroups.push_back(newParticleGroup);
        }
    }
}

// updates all active objects
void Engine::update(float dtAsSeconds) {

    if (!wait) { //wait for firework to get to it's pos

        // update each firework sprite
        for (size_t i = 0; i < fireworks.size();) {
            fireworks[i].move(0, -1000 * dtAsSeconds); // move upward

            //case being over user's y value with firework's y value
            if (fireworks[i].getPosition().y < fireworkTargets[i]) {
                fireworkTargets[i] = -target; // mark target as triggered
                fireworks[i].setPosition(-9999, 9999); // idk how to remove it so put it away somewhere far lol
                //it = fireworks.erase(it); //removes it
                ++i;
            }
            else {
                ++i;
            }
        }
        // update particles for each group
        for (size_t i = 0; i < particleGroups.size(); ++i) {
            if (fireworkTargets[i] == -target) { // display particles only if target happens
                for (auto it = particleGroups[i].begin(); it != particleGroups[i].end();) {
                    if (it->getTTL() > 0.0f) {
                        it->update(dtAsSeconds); //always updating
                        ++it;
                    }
                    else {
                        it = particleGroups[i].erase(it); //remove particles
                    }
                }
            }
        }
    }
}

// renders everything
void Engine::draw() {
    m_Window.clear();
    
    // draw all active fireworks
    for (const auto& firework : fireworks) {
        m_Window.draw(firework);
    }

    // draw all particles for each group
    for (size_t i = 0; i < particleGroups.size(); ++i) {
        if (fireworkTargets[i] == -target) { // only draw particles if target happens
            for (const auto& particle : particleGroups[i]) {
                m_Window.draw(particle);
            }
        }
    }

    m_Window.display();
}
