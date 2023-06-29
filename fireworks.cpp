#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include "params.h"

struct Particle {
    sf::Vector2f originalPos;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifespan;
    bool mustDelete;
};

int main() {
    constexpr int windowWidth = 1600;
    constexpr int windowHeight = 900;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Firworks Simulation");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> velDist(100.0f, 300.0f);
    std::uniform_real_distribution<float> angleDist(0.0f, 2 * 3.141592f);
    std::uniform_int_distribution<int> colorDist(0, 255);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        // Create new fireworks
        sf::Color c = sf::Color(colorDist(gen), colorDist(gen), colorDist(gen));
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            for(int i = 0; i < PARTICLE_MULTIPLIER; ++i) {
                Particle particle;
                particle.originalPos = sf::Vector2f(sf::Mouse::getPosition(window));
                particle.position = particle.originalPos;
                particle.velocity = sf::Vector2f(velDist(gen) * std::cos(angleDist(gen)), velDist(gen) * std::sin(angleDist(gen)));
                particle.color = sf::Color(colorDist(gen), colorDist(gen), colorDist(gen));
                particle.lifespan = 10.0f;
                particle.mustDelete = false;

                particles.push_back(particle);
            }
        }

        // Update and draw particles
        for(auto it = particles.begin(); it != particles.end();) {
            it->position += it->velocity * 0.01f;
            it->velocity.y += 50.0f * 0.01f; // Apply gravity
            
            sf::CircleShape shape(PARTICLE_SIZE);
            shape.setPosition(it->position);
            shape.setFillColor(it->color);
            window.draw(shape);

            it->lifespan -= 0.01f;
            float dist = std::sqrt(std::pow(it->originalPos.x - it->position.x, 2) + std::pow(it->originalPos.y - it->position.y, 2));
            if(dist >= MAX_RADIUS) {
                it->mustDelete = true;
            }
            if(it->lifespan <= 0 || it->mustDelete)
                it = particles.erase(it);
            else
                ++it;
        }
        window.display();
    }
    return 0;
}
