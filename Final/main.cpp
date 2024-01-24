#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "particle.h"
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<string>faces);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
bool showflower = false;
glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightAmbient(0.5f, 0.5f, 0.5f);
glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 lampPos(0.5f, 1.0f, 1.5f);
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //particlesystem
    glm::vec3 pink(0.937f, 0.545f, 0.667f);
    glm::mat4 cupRotate = glm::mat4(1.0f);
    ParticleSystem pinkflower(lightPos, pink);
    glm::vec3 white(1.0f, 1.0f, 1.0f);
    ParticleSystem fountain(lightPos, white,-20);

    


    // build and compile shaders
    // -------------------------
    Shader shader("V.vs", "F.fs");
    Shader skybox("skybox.vs", "skybox.fs");
    Shader floor("floor.vs", "floor.fs");
    floor.use(); floor.setVec3("lightPos", lightPos);floor.setVec3("lightColor", lightColor);
    floor.setVec3("lightAmbient", lightAmbient); floor.setVec3("lightDiffuse", lightDiffuse); floor.setVec3("lightSpecular", lightSpecular);
    Shader pavi("pavi.vs", "pavi.fs");
    pavi.use(); pavi.setVec3("lightPos", lightPos); pavi.setVec3("lightColor", lightColor);
    pavi.setVec3("lightAmbient", lightAmbient); pavi.setVec3("lightDiffuse", lightDiffuse); pavi.setVec3("lightSpecular", lightSpecular);
    Shader ts("tree.vs", "tree.fs");
    ts.use(); ts.setVec3("lightPos", lightPos); ts.setVec3("lightColor", lightColor);
    ts.setVec3("lightAmbient", lightAmbient); ts.setVec3("lightDiffuse", lightDiffuse); ts.setVec3("lightSpecular", lightSpecular);
    Shader scpt("sculpture.vs", "sculpture.fs");
    scpt.use(); scpt.setVec3("lightPos", lightPos); scpt.setVec3("lightColor", lightColor);
    scpt.setVec3("lightAmbient", lightAmbient); scpt.setVec3("lightDiffuse", lightDiffuse); scpt.setVec3("lightSpecular", lightSpecular);
    Shader particleshader("particle.vs", "particle.fs");
    particleshader.use(); particleshader.setVec3("lightPos", lightPos); particleshader.setVec3("lightColor", lightColor);

    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    float planeVertices[] = {
        // positions     // texture Coords   //normal
        -50.0f, -10.0f,  -50.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        -50.0f, -10.0f,  0.0f,  0.0f, 0.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 0.0f,  1.0f, 0.0f,0.0f,1.0f,0.0f,
        -50.0f, -10.0f,  -50.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f,  -50.0f,  1.0f, 1.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 0.0f,  1.0f, 0.0f,0.0f,1.0f,0.0f,

        0.0f, -10.0f,  -50.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 0.0f,  0.0f, 0.0f,0.0f,1.0f,0.0f,
        50.0f,-10.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, -50.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        50.0f,-10.0f,-50.0f,1.0f,1.0f,0.0f,1.0f,0.0f,
        50.0f,-10.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,

        -50.0f, -10.0f,  0.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        -50.0f, -10.0f,  50.0f,  0.0f, 0.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 50.0f,  1.0f, 0.0f,0.0f,1.0f,0.0f,
        -50.0f, -10.0f,  0.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f,  0.0f,  1.0f, 1.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 50.0f,  1.0f, 0.0f,0.0f,1.0f,0.0f,

        0.0f, -10.0f,  0.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 50.0f,  0.0f, 0.0f,0.0f,1.0f,0.0f,
        50.0f,-10.0f,50.0f,1.0f,0.0f,0.0f,1.0f,0.0f,
        0.0f, -10.0f, 0.0f,  0.0f, 1.0f,0.0f,1.0f,0.0f,
        50.0f,-10.0f,0.0f,1.0f,1.0f,0.0f,1.0f,0.0f,
        50.0f,-10.0f,50.0f,1.0f,0.0f,0.0f,1.0f,0.0f
    };

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLOAT, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLOAT, 3 * sizeof(float), (void*)0);

    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    vector<string>faces{
        "resource/skybox/lake2/left.tga",
        "resource/skybox/lake2/right.tga",
        "resource/skybox/lake2/up.tga",
        "resource/skybox/lake2/down.tga",
        "resource/skybox/lake2/front.tga",
        "resource/skybox/lake2/back.tga"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int floorTexture = loadTexture("resource/floor/floor.jpg");
    shader.use();
    shader.setInt("skybox", 0);
    skybox.use();
    skybox.setInt("skybox", 0);
    floor.use();
    floor.setInt("floor", 0);

    Model pavilion("resource/pavilion/file.obj");
    Model tree("resource/tree/file.obj");
    Model sculpture("resource/sculpture/file.obj");
    Model sculpture1("resource/sculpture1/file.obj");
    Model sculpture2("resource/sculpture2/file.obj");

    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();



        // don't forget to enable shader before setting uniforms
        shader.use();
        //// view/projection transformations
        model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("cameraPos", camera.Position);

        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // floor
        floor.use();
        floor.setVec3("viewPos", camera.Position);
        floor.setMat4("projection", projection);
        floor.setMat4("view", view);
        model = glm::mat4(1.0f);
        floor.setMat4("model", model);
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //tree
        ts.use();
        // view/projection transformations
        ts.setVec3("viewPos", camera.Position);
        ts.setMat4("projection", projection);
        ts.setMat4("view", view);
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(7.0f, -10.0f, 30.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));	// it's a bit too big for our scene, so scale it down
        ts.setMat4("model", model);
        tree.Draw(ts);

        

        //sculpture
        scpt.use();
        // view/projection transformations
        scpt.setVec3("viewPos", camera.Position);
        scpt.setMat4("projection", projection);
        scpt.setMat4("view", view);
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-15.0f, -10.0f, 40.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
        scpt.setMat4("model", model);
        sculpture.Draw(scpt);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-35.0f, -15.0f, 30.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(45.0f, 45.0f, 45.0f));	// it's a bit too big for our scene, so scale it down
        scpt.setMat4("model", model);
        sculpture1.Draw(scpt);

        // view/projection transformations
        scpt.setVec3("viewPos", camera.Position);
        scpt.setMat4("projection", projection);
        scpt.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, -10.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));	// it's a bit too big for our scene, so scale it down
        scpt.setMat4("model", model);
        sculpture2.Draw(scpt);



        //pavilion
        pavi.use();
        // view/projection transformations
        pavi.setVec3("viewPos", camera.Position);
        pavi.setMat4("projection", projection);
        pavi.setMat4("view", view);
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(25.0f, -13.0f, 25.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model,glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));	// it's a bit too big for our scene, so scale it down
        pavi.setMat4("model", model);
        pavilion.Draw(pavi);

        for (int i = 0; i < 30; i++) {
            fountain.insertParticle(Particle(glm::vec3(-35 , -15, 30 ), glm::vec3(1.0 * (rand() % 200 - 100) / 100, 8, 1.0 * (rand() % 200 - 100) / 100), 2));
        }
        fountain.simulate(deltaTime);
        fountain.render(view, projection, glm::mat4(1.0f), cupRotate);

        if (showflower) {
            //particlesystem
            pinkflower.insertParticle(Particle(glm::vec3(rand() % 100 - 50, 50, rand() % 100 - 50), glm::vec3(rand() % 10 - 5, -rand() % 8, -2.5 + rand() % 5),1));
            pinkflower.simulate(deltaTime);
            pinkflower.render(view, projection, glm::mat4(1.0f), cupRotate);
        }


        //skybox
        glDepthFunc(GL_LEQUAL);
        skybox.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox.setMat4("view", view);
        skybox.setMat4("projection", projection);
        
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        showflower = !showflower;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}