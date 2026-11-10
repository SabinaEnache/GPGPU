class Shape {
public:
    std::string type;
    glm::vec3 centerPosition;
    float length; // If cube, this is the length of the side. If sphere/cylinder, this is the diameter
    glm::vec3 velocity;
    int weight; // will probably be 1

    Shape() {
        type = "cube";
        centerPosition = glm::vec3(0);
        length = 1.0f;
        velocity = glm::vec3(0);
        weight = 1;
    }

    Shape(std::string t, glm::vec3 pos, float len, glm::vec3 vel, int w) {
        type = t;
        centerPosition = pos;
        length = len;
        velocity = vel;
        weight = w;
    }

};
