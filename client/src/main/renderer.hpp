#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <list>
#include <cstring>

#define SFILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define CHECK_GL() render::checkGL("GL Error " + std::string(SFILE) + "." + LINE_STRING + ": ")

namespace render {
    /** Returns the window object */
    GLFWwindow* getWindow();

    /** Creates the opengl context and displays the window.
     * @param major The major version of opengl used.
     * @param minor The minor version of opengl used.
     * @param title The title to display on the window.
     * @param fullscreen whether to make the window fullscreen, if it is not the size is
     *  set to half the size of the screen. */
    void init(int major, int minor, int samples, std::string title, bool fullscreen);

    /** Renders all items and updates buffers if neccessary.
     **/
    void tick();

    /** @returns true if the OS has signaled the window to close or escape has been pressed
     **/
    bool shouldClose();

    /** Cleans the memory on the graphics card, forgetting to do this is very very bad
     */
    void clean();

    /** Checks the opengl driver for errors, this also clears the error buffer
     *
     * @returns true is there was an error, the error is then logged
     */
    bool checkGL(std::string message);

    /** Signals the renderer to send the camera data to the graphics card next frame
     */
    void updateCamera();

    /** @returns the position of the camera, when editing this make sure to call updateCamera();
     */
    glm::vec2* getCameraPos();

    /** @returns the scale of the camera, when editing this make sure to call updateCamera();
     */
    glm::vec2* getCameraScale();
}

namespace texture {
    /** binds an image to a specified texture unit that can be used in @see texturedTriangle::add and @see text::add.
     *     This overwrites any old bind to this texture unit
     *
     * @param image The opengl id of the image.
     * @param the id to bind the image to, this must be below 32. */
    void bind(GLuint image, int unit);
}

namespace solidTriangle {
    struct Data {
        glm::vec2 a;
        glm::vec2 b;
        glm::vec2 c;
        glm::vec4 colour;
    };

    /** Adds a static solid flat shaded triangle to the renderer queue
     *
     * @param a a point
     * @param b a point
     * @param c yet another point, these must be in counterclockwise order
     * @param colour the colour of the triangle */
    Data* add(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec4 colour);

    /** removes a triangle using the pointer returned in @see solidTriangle::add */
    void remove(Data* triangle);

    void remakeBuffer(std::list<Data*> triangles);
}

namespace texturedTriangle {
    struct Data {
        glm::vec2 a;
        glm::vec2 b;
        glm::vec2 c;

        glm::vec2 p;
        glm::vec2 q;
        glm::vec2 r;

        int texture; //this may be removed later
    };

    /** Adds a static textured triangle to the renderer queue
         *
         * @param a a point
         * @param b a point
         * @param c yet another point, these must be in counterclockwise order
         *
         * @param p Texture coordinates for a
         * @param q b
         * @param r and c
         *
         * @param texture the texture index to draw from */
    Data* add(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p, glm::vec2 q, glm::vec2 r, int texture);
    void remove(Data* triangle);
}

namespace line {
    struct Data {
        glm::vec2 a;
        glm::vec2 b;

        glm::vec4 colour;
    };

    /** Adds a static line to the renderer queue
     *
     * @param a a point
     * @param b a point
     * @param colour the colour of the line */
    Data* add(glm::vec2 a, glm::vec2 b, glm::vec4 colour);
    void remove(Data* line);
}

//not done yet
namespace text {
    struct Data {
        glm::vec2 pos;
        glm::vec4 colour;
        float size;
        std::string text;
        int length;
        int image;
    };

    /** Adds a text job to the renderer, this is not to be used for bulk objects that move, that will cause performance issues.
     * @param pos the lower left corner of the text in world coordinates
     * @param colour the colour of the text in rgba.
     * @param text the string to be rendererd, NB: carrabge returns and tabbs are not yet supported
     * @param image The image index that the text bitmap was loaded into, @see texture::bind()*/
    Data* add(glm::vec2 pos, glm::vec4 colour, float size, std::string text, int image);

    /** Removes a text job via the handle returned in @see text::add
     * @param the data returned from @see text::add */
    void remove(Data* text);
}

#endif // RENDERER_HPP
