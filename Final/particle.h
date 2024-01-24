#pragma once
#include "shader.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#define PARTICLE_LIFE 200

class Particle {
private:
    unsigned int particleVAO, particleVBO;

public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::mat4 rotate;
    float size;
    float age;
    Particle(glm::vec3 _position, glm::vec3 _velocity,int type) {
        position = _position;
        velocity = _velocity;
        rotate = glm::mat4(1.0f);
        rotate = glm::rotate(rotate, glm::radians((float)(rand() % 360)), glm::vec3(1.0f, 0.0f, 0.0f));
        rotate = glm::rotate(rotate, glm::radians((float)(rand() % 360)), glm::vec3(0.0f, 1.0f, 0.0f));
        rotate = glm::rotate(rotate, glm::radians((float)(rand() % 360)), glm::vec3(0.0f, 0.0f, 1.0f));

        size = (rand() % 450 + 50) / 500.0;

        age = 0;


        float flower[] = { 
            0.000, 0.000, -0.300, 0.0999, -0.2001,  0.167,
            0.667, 0.333, -0.300, 0.0999, -0.2001,  0.167,
            0.500, 0.500,  0.000, 0.0999, -0.2001,  0.167,
            0.500, 0.500,  0.000, 0.2001, -0.0999,  0.167,
            0.667, 0.333, -0.300, 0.2001, -0.0999,  0.167,
            1.000, 1.000, -0.300, 0.2001, -0.0999,  0.167,
            0.000, 0.000, -0.300, -0.2001,  0.0999,  0.167,
            0.500, 0.500,  0.000, -0.2001,  0.0999,  0.167,
            0.333, 0.667, -0.300, -0.2001,  0.0999,  0.167,
            0.500, 0.500,  0.000,  0.0999, -0.2001, -0.167,
            0.333, 0.667, -0.300,  0.0999, -0.2001, -0.167,
            1.000, 1.000, -0.300,  0.0999, -0.2001, -0.167,


            -0.000, 0.000, -0.300,  0.0999, 0.2001,  -0.167,
            -0.667, 0.333, -0.300,  0.0999, 0.2001,  -0.167,
            -0.500, 0.500,  0.000,  0.0999, 0.2001,  -0.167,
            -0.500, 0.500,  0.000,  0.2001, 0.0999,  -0.167,
            -0.667, 0.333, -0.300,  0.2001, 0.0999,  -0.167,
            -1.000, 1.000, -0.300,  0.2001, 0.0999,  -0.167,
            -0.000, 0.000, -0.300, -0.2001, -0.0999,-0.167,
            -0.500, 0.500,  0.000, -0.2001, -0.0999,-0.167,
            -0.333, 0.667, -0.300, -0.2001, -0.0999,-0.167,
            -0.500, 0.500,  0.000,  0.0999, 0.2001, 0.167,
            -0.333, 0.667, -0.300,  0.0999, 0.2001, 0.167,
            -1.000, 1.000, -0.300,  0.0999, 0.2001, 0.167,


            0.000, -0.000, -0.300, -0.0999, -0.2001, -0.167,
            0.667, -0.333, -0.300, -0.0999, -0.2001, -0.167,
            0.500, -0.500,  0.000, -0.0999, -0.2001, -0.167,
            0.500, -0.500,  0.000, -0.2001, -0.0999, -0.167,
            0.667, -0.333, -0.300, -0.2001, -0.0999, -0.167,
            1.000, -1.000, -0.300, -0.2001, -0.0999, -0.167,
            0.000, -0.000, -0.300,  0.2001,  0.0999, -0.167,
            0.500, -0.500,  0.000,  0.2001,  0.0999, -0.167,
            0.333, -0.667, -0.300,  0.2001,  0.0999, -0.167,
            0.500, -0.500,  0.000, -0.0999, -0.2001, 0.167,
            0.333, -0.667, -0.300, -0.0999, -0.2001, 0.167,
            1.000, -1.000, -0.300, -0.0999, -0.2001, 0.167,


            -0.000, -0.000, -0.300, -0.0999,  0.2001,  0.167,
            -0.667, -0.333, -0.300, -0.0999,  0.2001,  0.167,
            -0.500, -0.500,  0.000, -0.0999,  0.2001,  0.167,
            -0.500, -0.500,  0.000, -0.2001,  0.0999,  0.167,
            -0.667, -0.333, -0.300, -0.2001,  0.0999,  0.167,
            -1.000, -1.000, -0.300, -0.2001,  0.0999,  0.167,
            -0.000, -0.000, -0.300,  0.2001, -0.0999,  0.167,
            -0.500, -0.500,  0.000,  0.2001, -0.0999,  0.167,
            -0.333, -0.667, -0.300,  0.2001, -0.0999,  0.167,
            -0.500, -0.500,  0.000, -0.0999,  0.2001, -0.167,
            -0.333, -0.667, -0.300, -0.0999,  0.2001, -0.167,
            -1.000, -1.000, -0.300, -0.0999,  0.2001, -0.167
        };
        float drop[] = {
            0.0f,   0.0f, -0.05f,   0.0f,  0.0f, -1.0f,
            0.0667f, 0.0333f, -0.05f, 0.0f,  0.0f, -1.0f,
            0.1f,   0.1f,-0.05f,0.0f,  0.0f, -1.0f,
            0.1f,   0.1f,-0.05f,0.0f,  0.0f, -1.0f,
            0.0333f, 0.0667f, -0.05f, 0.0f,  0.0f, -1.0f,
            0.0f,   0.0f, -0.05f,   0.0f,  0.0f, -1.0f
        };
        

        glGenVertexArrays(1, &particleVAO);
        glGenBuffers(1, &particleVBO);
        glBindVertexArray(particleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

        if(type==1)glBufferData(GL_ARRAY_BUFFER, sizeof(flower), &flower, GL_STATIC_DRAW);
        if(type==2)glBufferData(GL_ARRAY_BUFFER, sizeof(drop), &drop, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    ~Particle() {}
    void renderParticle() {
        glBindVertexArray(particleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 48);
        glBindVertexArray(0);
    }
};

class ParticleSystem {
private:
    float gravity;
    std::vector<Particle> particles;
public:

    Shader particleShader;
    ParticleSystem(glm::vec3 lightPos, glm::vec3 lightColor,float _gravity = -9) :
        particleShader("particle.vs", "particle.fs") {
        gravity = _gravity;
        particleShader.use();
        particleShader.setVec3("lightColor", lightColor);
        particleShader.setVec3("lightPos", lightPos);
        particleShader.setVec3("particleColor", 0.7f, 0.7f, 0.7f);
    }
    ~ParticleSystem() {}
    void init() {
        this->particles.clear();
    }

    void simulate(float dt) {
        aging(dt);
        updateParticle(dt);
    }
    void aging(float dt) {
        for (std::vector<Particle>::iterator iter = particles.begin(); iter != particles.end();)
        {
            iter->age += dt;
            if (iter->age >= PARTICLE_LIFE)
            {
                particles.erase(iter);
                continue;
            }
            iter++;
        }
    }
    void insertParticle(Particle p) {
        particles.push_back(p);
    }
    void updateParticle(float dt) {
        for (std::vector<Particle>::iterator iter = particles.begin(); iter != particles.end(); iter++)
        {
            iter->position = iter->position + iter->velocity * dt;
            iter->velocity = glm::vec3(iter->velocity.x, iter->velocity.y + gravity * dt / 8, iter->velocity.z);
        }
    }

    void render(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::mat4 rotate) {

        glm::mat4 startModel = model;
        particleShader.use();
        particleShader.setMat4("view", view);
        particleShader.setMat4("projection", projection);
        for (std::vector<Particle>::iterator iter = particles.begin(); iter != particles.end(); iter++)
        {

            model = glm::translate(startModel, iter->position);
            model = glm::scale(model, glm::vec3(iter->size));
            particleShader.setMat4("model", model);
            particleShader.setMat4("rotate", iter->rotate);
            iter->renderParticle();
        }
    }
};
