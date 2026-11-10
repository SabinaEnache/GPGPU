class Shape {
public:
    std::string type;
    glm::vec3 centerPosition;
    int length; // If cube, this is the length of the side. If sphere/cylinder, this is the radius.
    glm::vec3 velocity;
    int weight; // will probably be 1


    Shape(std::string t, glm::vec3 pos, int len, glm::vec3 vel, int w) {
        type = t;
        centerPosition = pos;
        length = len;
        velocity = vel;
        weight = w;
    }

};
