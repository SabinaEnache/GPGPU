#include "lab/GPGPU/GPGPU.h"
#include <stb/stb_image.h>
#include <random>
#include <ctime>


#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace lab;


GPGPU::GPGPU()
{
}


GPGPU::~GPGPU()
{
}

void GPGPU::DrawQuad() 
{
    vector<VertexFormat> quadVertices
    {
        VertexFormat(glm::vec3(-10, -10, 0.0), glm::vec3(0.7,0.8,0.96)),
        VertexFormat(glm::vec3(10, -10, 0.0), glm::vec3(0.7,0.8,0.96)),
        VertexFormat(glm::vec3(-10, 10, 0.0), glm::vec3(0.7,0.8,0.96)),
        VertexFormat(glm::vec3(10, 10, 0.0), glm::vec3(0.7,0.8,0.96))
    };

    vector<unsigned int> quadIndices
    {
        0, 1, 2,
        1, 3, 2
    };

    CreateMesh("quad", quadVertices, quadIndices);
}

void GPGPU::DrawCube() 
{
    vector<VertexFormat> cubeVertices
    {
        VertexFormat(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 0, 1)),
    };

    vector<unsigned int> cubeIndices
    {
        0, 1, 2,    // indices for first triangle
        1, 3, 2,    // indices for second triangle
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4
    };

    // Create the mesh from the data
    CreateMesh("cube", cubeVertices, cubeIndices);
}

void GPGPU::DrawSphere()
{
    float radius = 0.5;
    int stacks = 36;
    int sectors = 18;
    vector<VertexFormat> sphereVertices;
    vector<unsigned int> sphereIndices;

    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * 2 * M_PI / sectors;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            sphereVertices.push_back({ glm::vec3(x, y, z), glm::vec3(1,1,0) });
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                sphereIndices.push_back(k1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k1 + 1);
            }
            if (i != (stacks - 1))
            {
                sphereIndices.push_back(k1 + 1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k2 + 1);
            }
        }
    }
    CreateMesh("sphere", sphereVertices, sphereIndices);
}

void GPGPU::DrawCylinder()
{
    const int sectorCount = 32;
    const float radius = 0.5f;
    const float height = 1.0f;
    const float halfHeight = height / 2.0f;
    const float pi = 3.14159265359f;

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    // Side vertices.
    for (int yIndex = 0; yIndex <= 1; yIndex++) {
        float y = yIndex == 0 ? -halfHeight : halfHeight;

        for (int sector = 0; sector <= sectorCount; sector++) {
            float sectorAngle = 2.0f * pi * sector / sectorCount;
            float x = radius * cos(sectorAngle);
            float z = radius * sin(sectorAngle);

            glm::vec3 position(x, y, z);


            vertices.push_back(VertexFormat(position, glm::vec3(1,0,0)));
        }
    }

    // Bottom and top center vertices.
    unsigned int bottomCenter = static_cast<unsigned int>(vertices.size());
    vertices.push_back(VertexFormat(
        glm::vec3(0.0f, -halfHeight, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    ));

    unsigned int topCenter = static_cast<unsigned int>(vertices.size());
    vertices.push_back(VertexFormat(
        glm::vec3(0.0f, halfHeight, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    ));

    // Cylinder side.
    for (int sector = 0; sector < sectorCount; sector++) {
        unsigned int bottomCurrent = sector;
        unsigned int bottomNext = sector + 1;
        unsigned int topCurrent = sectorCount + 1 + sector;
        unsigned int topNext = sectorCount + 1 + sector + 1;

        indices.push_back(bottomCurrent);
        indices.push_back(topCurrent);
        indices.push_back(bottomNext);

        indices.push_back(bottomNext);
        indices.push_back(topCurrent);
        indices.push_back(topNext);
    }

    // Bottom and top caps.
    for (int sector = 0; sector < sectorCount; sector++) {
        unsigned int bottomCurrent = sector;
        unsigned int bottomNext = sector + 1;
        unsigned int topCurrent = sectorCount + 1 + sector;
        unsigned int topNext = sectorCount + 1 + sector + 1;

        indices.push_back(bottomCenter);
        indices.push_back(bottomCurrent);
        indices.push_back(bottomNext);

        indices.push_back(topCenter);
        indices.push_back(topNext);
        indices.push_back(topCurrent);
    }

    // Create the mesh from the data
    CreateMesh("cylinder", vertices, indices);
}



void GPGPU::Init()
{

    // Create a shader program 
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab06", "shaders", "LabShader.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab06", "shaders", "LabShader.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("TransparentShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab06", "shaders", "LabShader.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab06", "shaders", "TransparentShader.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    DrawQuad();
    DrawCube();
    DrawSphere();
    DrawCylinder();

}

Texture2D* GPGPU::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D* GPGPU::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();


    // TODO(student): Use the "glTexImage2D" directive to load the information
    // into the graphics processor's memory. Use the correct format based on
    // the number of components:
    //   - 1 color channel - GL_RED
    //   - 2 color channels - GL_RG
    //   - 3 color channels - GL_RGB
    //   - 4 color channels - GL_RGBA
    GLint internalFormat[] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
    GLint pixelFormat[] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[channels], width, height, 0, pixelFormat[channels], GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

Mesh* GPGPU::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void GPGPU::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void GPGPU::Update(float deltaTimeSeconds)

{   glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-2, 1, 0));
    RenderSimpleMesh(meshes["cube"], shaders["LabShader"], model, glm::vec3(1));
   
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], model, glm::vec3(1));

    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(2, 1, 0));
    RenderSimpleMesh(meshes["cylinder"], shaders["LabShader"], model, glm::vec3(1));

    //cub
    {
        //spate
        //glm::mat4 spate = glm::mat4(1);
        //spate = glm::translate(spate, glm::vec3(0, 0, -10));
        //RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], spate, glm::vec3(1));

        //stanga
        glm::mat4 stanga = glm::mat4(1);
        stanga = glm::translate(stanga, glm::vec3(-10, 0, 0));
        stanga = glm::rotate(stanga, glm::radians(90.0f), glm::vec3(0,1,0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], stanga, glm::vec3(1));

        //dreapta
        glm::mat4 dreapta = glm::mat4(1);
        dreapta = glm::translate(dreapta, glm::vec3(10, 0, 0));
        dreapta = glm::rotate(dreapta, glm::radians(90.0f), glm::vec3(0,1,0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], dreapta, glm::vec3(1));

        //sus
        glm::mat4 sus = glm::mat4(1);
        sus = glm::translate(sus, glm::vec3(0, 10, 0));
        sus = glm::rotate(sus, glm::radians(90.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], sus, glm::vec3(1));

        //jos
        glm::mat4 jos = glm::mat4(1);
        jos = glm::translate(jos, glm::vec3(0, -10, 0));
        jos = glm::rotate(jos, glm::radians(90.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], jos, glm::vec3(1));

        //fata
        glm::mat4 fata = glm::mat4(1);
        fata = glm::translate(fata, glm::vec3(0, 0, 10));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], fata, glm::vec3(1));
    }
}


void GPGPU::FrameEnd()
{
    //DrawCoordinateSystem();
}

void GPGPU::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, const glm::vec3& object_color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eye_position));

    glm::vec3 material_ka = object_color;
    glm::vec3 material_kd = object_color;
    glm::vec3 material_ks = object_color;
    int material_shininess = 30;
    // TODO(student): Set material property uniforms (shininess, ka, kd, ks)
    glUniform3fv(glGetUniformLocation(shader->program, "material_ka"), 1, glm::value_ptr(material_ka));
    glUniform3fv(glGetUniformLocation(shader->program, "material_kd"), 1, glm::value_ptr(material_kd));
    glUniform3fv(glGetUniformLocation(shader->program, "material_ks"), 1, glm::value_ptr(material_ks));
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), material_shininess);

    // Send the model matrix uniform
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Send the view matrix unfirom
    glm::mat4 view = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    // Send the projection matrix uniform
    glm::mat4 projection = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void GPGPU::CreateFramebuffer(int width, int height)
{
    // TODO(student): In this method, use the attributes
    // 'framebuffer_object', 'color_texture' and 'depth_texture'
    // declared in lab09.h

    // TODO(student): Generate and bind the framebuffer
    glGenFramebuffers(1, &framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);

    // TODO(student): Generate, bind and initialize the color texture
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // TODO(student): Bind the color texture to the
    // framebuffer as a color attachment at position 0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);

    // TODO(student): Generate, bind and initialize the depth texture
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // TODO(student): Bind the depth texture to the framebuffer as a depth attachment
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    // TODO(student): Set the color texture as the draw texture
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    // TODO(student): Check the status of the framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void GPGPU::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        const float speed = 2;

        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

    }

}

void GPGPU::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void GPGPU::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void GPGPU::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void GPGPU::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void GPGPU::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void GPGPU::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void GPGPU::OnWindowResize(int width, int height)
{
}
