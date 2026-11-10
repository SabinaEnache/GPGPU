#pragma once

#include <string>
#include <unordered_map>
#include <random>
#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab/lab04/transform3D.h"
#include "lab/GPGPU/Shape.h"

namespace lab
{
    class GPGPU : public gfxc::SimpleScene
    {
    public:
        GPGPU();
        ~GPGPU();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, const glm::vec3& object_color);
        Texture2D* LoadTexture(const char* imagePath);
        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        void CreateFramebuffer(int width, int height);

        void DrawQuad();
        void DrawSphere();
        void DrawCube();
        void DrawCylinder();
        void ShowFramerate();
		void UpdateShapes();

        //gjk
        glm::vec3 TripleProduct(glm::vec3 A, glm::vec3 B, glm::vec3 C);
        glm::vec3 SupportShape(Shape shape, glm::vec3 direction);
        glm::vec3 MinkowskiDifference(Shape a, Shape b, glm::vec3 direction);
        bool LineCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
        bool TriangleCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
        bool TetrahedronCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
        bool HandleSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction);
        bool GJK(Shape shapeA, Shape shapeB);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
		void SpawnShape(Shape shape);

        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int depth_texture;

        std::vector<Shape> shapes;
		int bigCubeLength;

        int gravity;

        double lastTime;
        int nbFrames;

        Shape newShape1;
        
        Shape newShape2;


        std::vector<glm::vec3> simplex;
        glm::vec3 direction;
    };
}   // namespace lab
