#include "lab/GPGPU/GPGPU.h"
#include <stb/stb_image.h>
#include <random>
#include <ctime>
#include <chrono>

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace lab;

static const float EPS = 1e-6f;
static bool testMoving = false;       
static bool testWasTouching = false;

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
    std::cout << "Numarul de vertecsi ai cubului: " << cubeVertices.size() << endl;
    // Create the mesh from the data
    CreateMesh("cube", cubeVertices, cubeIndices);
}

void GPGPU::DrawSphere()
{
    /*float radius = 0.5;
    int stacks = 16;
    int sectors = 8;
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
    std::cout << "Numarul de vertecsi ai sferei: " << sphereVertices.size() << endl;
    CreateMesh("sphere", sphereVertices, sphereIndices);
    */

    //sfera de pe discord arata mai bine chiar daca are acelasi nr de vertecsi
    const int sectorCount = 16;
    const int stackCount = 8;
    const float radius = 0.5f;
    const float pi = 3.14159265359f;

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    for (int stack = 0; stack <= stackCount; stack++) {
        float stackAngle = -pi / 2.0f + pi * stack / stackCount;
        float y = radius * sin(stackAngle);
        float xz = radius * cos(stackAngle);

        for (int sector = 0; sector <= sectorCount; sector++) {
            float sectorAngle = 2.0f * pi * sector / sectorCount;

            float x = xz * cos(sectorAngle);
            float z = xz * sin(sectorAngle);

            glm::vec3 position(x, y, z);

            vertices.push_back(VertexFormat(position, glm::vec3(1,1,0)));
        }
    }

    for (int stack = 0; stack < stackCount; stack++) {
        for (int sector = 0; sector < sectorCount; sector++) {
            unsigned int first = stack * (sectorCount + 1) + sector;
            unsigned int second = first + sectorCount + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }
    std::cout << "Numarul de vertecsi ai sferei: " << vertices.size() << endl;
    CreateMesh("sphere", vertices, indices);
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
    std::cout << "Numarul de vertecsi ai cilindrului: " << vertices.size() << endl;
    // Create the mesh from the data
    CreateMesh("cylinder", vertices, indices);
}

void GPGPU::SpawnShape(Shape shape)
{    
    glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, shape.centerPosition);
	model *= glm::scale(glm::mat4(1), glm::vec3(shape.length));
    RenderSimpleMesh(meshes[shape.type], shaders["LabShader"], model, glm::vec3(1));

}

void GPGPU::UpdateShapes()
{
    for (auto& shape : shapes) {
    // Fall based on gravity
        shape.velocity.y -= gravity * 0.016f; // Assuming 60 FPS, so deltaTime is ~0.016 seconds
        shape.centerPosition += shape.velocity * 0.016f;
        // Check for collision with the ground (y = -bigCubeLength / 2)
        if (shape.centerPosition.y - shape.length / 2 < -bigCubeLength / 2) {
            shape.centerPosition.y = -bigCubeLength / 2 + shape.length / 2;
            shape.velocity.y = 0; // Stop falling
		}
    }
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

    // This is where the mesh info is defined in order to be used later
    {
        DrawQuad();
        DrawCube();
        DrawSphere();
        DrawCylinder();
    }

    // physics
	gravity = 9.81f; 

	// Setting the bounding cube length to 20 for now
	bigCubeLength = 20;

	// Framerate timer initialization
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    std::vector<glm::vec3> simplex = {};
    glm::vec3 direction = glm::vec3(0);

    Shape newShape1("cube", glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 0), 1);
    shapes.push_back(newShape1);
    Shape newShape2("sphere", glm::vec3(2, 0, 0), 1, glm::vec3(0, 0, 0), 1);
    shapes.push_back(newShape2);
}

//produsul vectorial (AxB)xC unde A,B si C sunt vectori
glm::vec3 GPGPU::TripleProduct(glm::vec3 A, glm::vec3 B, glm::vec3 C) 
{
    return glm::cross(glm::cross(A, B), C);
}

//punctul cel mai indepartat de centrul obiectului intr-o directie
glm::vec3 GPGPU::SupportShape(Shape shape, glm::vec3 direction) 
{
    glm::vec3 center = shape.centerPosition;
    
    if (shape.type == "cube") 
    {
        float semiinaltime = shape.length / 2;
        return center + glm::vec3(direction.x >= 0 ? semiinaltime : -semiinaltime,
                                  direction.y >= 0 ? semiinaltime : -semiinaltime,
                                  direction.z >= 0 ? semiinaltime : -semiinaltime);
    }

    if (shape.type == "sphere")
    {
        float raza = shape.length / 2;
        float len = glm::length(direction);
        if (len < EPS) return center + glm::vec3(raza, 0, 0);
        return center + direction * (raza / len); //nu merge doar center + raza?
    }

    if (shape.type == "cylinder") 
    {
        float r = shape.length / 2;
        float hh = shape.length / 2;                   
        glm::vec3 p = center;
        p.y += (direction.y >= 0 ? hh : -hh);          

        glm::vec2 radial(direction.x, direction.z);
        float len = glm::length(radial);
        if (len > EPS) {
            radial = radial / len * r;         
            p.x += radial.x;
            p.z += radial.y;
        }
        return p;
    }

    return center;
}

bool GPGPU::LineCase(std::vector<glm::vec3>& simplex, glm::vec3& direction) 
{
    printf("line case\n");
    glm::vec3 A = simplex[0];
    glm::vec3 B = simplex[1];
    glm::vec3 AB = normalize(B - A);
    glm::vec3 AO = normalize(-A);
    
    //cautam un al treilea punct
    if (glm::dot(AB, AO) > 0) 
    {
        direction = TripleProduct(AB, AO, AB); //normala pe AB catre origine
    }
    else //originea e in partea opusa fata de B, deci ne uitam pe directia AO
    {
        simplex = { A };
        direction = AO;
    }
    //sanse minuscule ca originea sa fie pe linie, deci nu le luam in considerare si returnam mereu fals
    return false;
}

bool GPGPU::TriangleCase(std::vector<glm::vec3>& simplex, glm::vec3& direction)
{
    printf("triangle case\n");
    glm::vec3 A = simplex[0];
    glm::vec3 B = simplex[1];
    glm::vec3 C = simplex[2];

    glm::vec3 AB = normalize(B - A);
    glm::vec3 AC = normalize(C - A);
    glm::vec3 AO = normalize(-A);

    glm::vec3 ABC = normalize(glm::cross(AB, AC)); //produs vectorial intre AB si AC => triunghiul ABC

    glm::vec3 normalaPeAB = TripleProduct(AC, AB, AB); 
    glm::vec3 normalaPeAC = TripleProduct(AB, AC, AC);


    //Rab - cautam un alt punct pentru un triunghi nou
    if (glm::dot(normalaPeAB, AO) > 0) //daca produsul scalar dintre normala pe AB si AO > 0 (merge in directia opusa lui C)
    {
        //originea este dincolo de AB, deci C nu e bun
        simplex = { A, B };
        direction = normalaPeAB;
        return false;
    }

    //Rac - cautam un alt punct pentru un triunghi nou
    if (glm::dot(normalaPeAC, AO) > 0) //daca produsul scalar dintre normala pe AC si AO > 0 (merge in directia opusa lui B)
    {
        //inseamna ca originea e dincolo de AC, deci B nu e bun
        simplex = { A, C };
        direction = normalaPeAC;
        return false;
    }

    //daca originea nu este nici in Rab, nici in Rac, se verifica tot triunghiul
    float side = glm::dot(ABC, AO);
    if (std::abs(side) < EPS) return true; //daca produsul vectorial dintre triunghiul ABC si AO e basically zero => originea e PE triunghi
    if (side > 0) { //originea e deasupra triunghiului => tetraedru
        direction = ABC;
    }
    else { //originea e sub triunghi => tetraedru
        direction = -ABC;
    }
    return false;

    ////glm::dot(glm::cross(ABC,AC), AO) > 0 == glm::dot(normalaPeAB, AO) > 0
    //if (glm::dot(normalaPeAB, AO) > 0) //originea e dincolo de AB, deci eliminam C
    //{
    //    if (glm::dot(AC, AO) > 0) 
    //    {
    //        simplex = {A, C};
    //        direction = normalaPeAC;
    //        //direction = TripleProduct(AC, AO, AC);
    //    }
    //    else 
    //    {
    //        simplex = { A,C };
    //        return LineCase(simplex, direction);
    //    }
    //}
    //else
    //{
    //    if (glm::dot(glm::cross(AB, ABC), AO) > 0) 
    //    {
    //        simplex = { A,B };
    //        return LineCase(simplex, direction);
    //    }
    //    else
    //    {
    //        if (glm::dot(ABC, AO) > 0) 
    //        {
    //            direction = ABC;
    //        }
    //        else 
    //        {
    //            simplex = { A, B, C };
    //            direction = -ABC;
    //        }
    //    }
    //}

    //return false;
}

bool GPGPU::TetrahedronCase(std::vector<glm::vec3>& simplex, glm::vec3& direction) 
{
    printf("tetrahedron case\n");
    glm::vec3 A = simplex[0];
    glm::vec3 B = simplex[1];
    glm::vec3 C = simplex[2];
    glm::vec3 D = simplex[3];

    glm::vec3 AB = normalize(B - A);
    glm::vec3 AC = normalize(C - A);
    glm::vec3 AD = normalize(D - A);
    glm::vec3 AO = normalize(-A);

    glm::vec3 ABC = normalize(glm::cross(AB, AC));
    glm::vec3 ACD = normalize(glm::cross(AC, AD));
    glm::vec3 ABD = normalize(glm::cross(AD, AB));

    if (glm::dot(ABC, AO) > 0) { //daca originea e deasupra ABC
        simplex = { A,B,C };
        return TriangleCase(simplex, direction);
    }
    if (glm::dot(ACD, AO) > 0) { //daca originea e deasupra ACD
        simplex = { A,C,D };
        return TriangleCase(simplex, direction);
    }
    if (glm::dot(ABD, AO)>0) { //daca originea e deasupra ABD
        simplex = { A, D, B };
        return TriangleCase(simplex, direction);
    }

    //daca originea e sub toate cele 3 triunghiuri (+BCD care e fostul ABC de la care am ajuns aici), inseamna ca originea este IN tetraedru
    return true;
}

bool GPGPU::HandleSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction) 
{
    switch (simplex.size()) {
        case 2: return LineCase(simplex, direction);
        case 3: return TriangleCase(simplex, direction);
        case 4: return TetrahedronCase(simplex, direction);
    }
    return false;
}

bool GPGPU::GJK(Shape shapeA, Shape shapeB) 
{
    simplex.clear();
    //direction = normalize(shapeB.centerPosition - shapeA.centerPosition);
    glm::vec3 supportPoint = MinkowskiDifference(shapeA, shapeB, glm::vec3(1,0,0));

    simplex.insert(simplex.begin(), supportPoint);
    direction = normalize(-supportPoint);

    for(int i = 0; i < 50; i++) {
        supportPoint = MinkowskiDifference(shapeA, shapeB, direction);

        if (glm::dot(supportPoint, direction) < 0) 
        {
            return false;
        }

        simplex.insert(simplex.begin(), supportPoint);

        if (HandleSimplex(simplex, direction))
        {
            std::cout << "Numarul de iteratii: " << i << endl;
            return true;
        }
    }

}

//punctul suport (diferenta dintre cel mai indepartat punct al primei forme intr-o directie si cel mai indepartat punct al celei de-a doua forma in opusul directiei)
glm::vec3 GPGPU::MinkowskiDifference(Shape a, Shape b, glm::vec3 direction) 
{
    return SupportShape(a, direction) - SupportShape(b, -direction);
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

{

    for each(const auto& shape in shapes)
    {
        SpawnShape(shape);
    }
    UpdateShapes();

    if (testMoving && shapes.size() >= 2)
    {
        Shape& first = shapes[0];
        Shape& second = shapes[1];

        const float testSpeed = 0.5f;               
        first.centerPosition.x += testSpeed * deltaTimeSeconds;

        bool touching = GJK(first, second);

        if (touching && !testWasTouching)
        {
            std::cout << "cuburile s au atins" << std::endl;
            testMoving = false;                     
        }
        testWasTouching = touching;
    }



    //cub
    {
        //spate
        //glm::mat4 spate = glm::mat4(1);
        //spate = glm::translate(spate, glm::vec3(0, 0, -10));
        //RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], spate, glm::vec3(1));

        //stanga
        glm::mat4 stanga = glm::mat4(1);
        stanga = glm::translate(stanga, glm::vec3(-bigCubeLength / 2, 0, 0));
        stanga = glm::rotate(stanga, glm::radians(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], stanga, glm::vec3(1));

        //dreapta
        glm::mat4 dreapta = glm::mat4(1);
        dreapta = glm::translate(dreapta, glm::vec3(bigCubeLength / 2, 0, 0));
        dreapta = glm::rotate(dreapta, glm::radians(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], dreapta, glm::vec3(1));

        //sus
        glm::mat4 sus = glm::mat4(1);
        sus = glm::translate(sus, glm::vec3(0, bigCubeLength / 2, 0));
        sus = glm::rotate(sus, glm::radians(90.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], sus, glm::vec3(1));

        //jos
        glm::mat4 jos = glm::mat4(1);
        jos = glm::translate(jos, glm::vec3(0, -bigCubeLength / 2, 0));
        jos = glm::rotate(jos, glm::radians(90.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], jos, glm::vec3(1));

        //fata
        glm::mat4 fata = glm::mat4(1);
        fata = glm::translate(fata, glm::vec3(0, 0, 10));
        RenderSimpleMesh(meshes["quad"], shaders["TransparentShader"], fata, glm::vec3(1));
    }

    // Time measurements for framerate calculation
    // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
        // printf and reset timer
		// TODO: find out how to add to overwrite window name with framerate
        double fps = double(nbFrames) / (currentTime - lastTime);
        window->MakeCurrentContext();
		window->props.name = " - FPS: " + std::to_string(fps);

        printf("%.2f ms/frame\n", fps);
        nbFrames = 0;
        lastTime = currentTime;
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
    // Spawn cube within big cube
    if (key == GLFW_KEY_1)
    {
        Shape newShape("cube", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
		shapes.push_back(newShape);
		printf("Spawned cube at position (%.2f, %.2f, %.2f)\n", newShape.centerPosition.x, newShape.centerPosition.y, newShape.centerPosition.z); 
	}
    // Spawn sphere within big cube
    if (key == GLFW_KEY_2)
    {
        Shape newShape("sphere", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
		shapes.push_back(newShape);
		printf("Spawned sphere at position (%.2f, %.2f, %.2f)\n", newShape.centerPosition.x, newShape.centerPosition.y, newShape.centerPosition.z); 
    }
	// Spawn cylinder within big cube
    if (key == GLFW_KEY_3)
    {
        Shape newShape("cylinder", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
		shapes.push_back(newShape);
		printf("Spawned cylinder at position (%.2f, %.2f, %.2f)\n", newShape.centerPosition.x, newShape.centerPosition.y, newShape.centerPosition.z);
    }
	// Spawn 100 spheres, 250 cubes, 500 cylinders within big cube
    if (key == GLFW_KEY_4)
    {
        for (int i = 0; i < 100; ++i) {
            Shape newShape("sphere", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
        for (int i = 0; i < 250; ++i) {
            Shape newShape("cube", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
        for (int i = 0; i < 500; ++i) {
            Shape newShape("cylinder", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
    }
	// Spawn 250 spheres, 500 cubes, 1000 cylinders within big cube
    if (key == GLFW_KEY_5)
    {
        for (int i = 0; i < 250; ++i) {
            Shape newShape("sphere", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
        for (int i = 0; i < 500; ++i) {
            Shape newShape("cube", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
        for (int i = 0; i < 1000; ++i) {
            Shape newShape("cylinder", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
	}
	// Spawn 500 spheres, 1000 cubes, 2000 cylinders within big cube
    if (key == GLFW_KEY_6)
    {
        for (int i = 0; i < 500; ++i) {
            Shape newShape("sphere", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
        for (int i = 0; i < 1000; ++i) {
            Shape newShape("cube", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
        for (int i = 0; i < 2000; ++i) {
            Shape newShape("cylinder", glm::vec3(rand() % bigCubeLength - bigCubeLength / 2, bigCubeLength / 2, rand() % bigCubeLength - bigCubeLength / 2), 1, glm::vec3(0, 0, 0), 1);
            shapes.push_back(newShape);
        }
	}
	// Clear all shapes
    if (key == GLFW_KEY_7)
    {
        shapes.clear();
        printf("Cleared all shapes\n");
	}
    if (key == GLFW_KEY_A)
    {
        testMoving = true;
    }
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
