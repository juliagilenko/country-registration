#include "gl_helpers.h"

#include <stdexcept>

void glad_init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* glfw_create_window(int width, int height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(920, 920, "country-registration", NULL, NULL);
    if (!window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return window;
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void fit_to_window(Country* country1, Country* country2, float& s, float& x, float& y)
{
    Eigen::MatrixXd V(2, country1->get_num_coords() + country2->get_num_coords());
    V << country1->get_all_coords(), country2->get_all_coords();

    Eigen::Vector2d top_right = V.rowwise().maxCoeff();
    Eigen::Vector2d bottom_left = V.rowwise().minCoeff();
    s = 1.5 / (top_right - bottom_left).maxCoeff();
    x = s * -bottom_left[0] - 0.75;
    y = s * -bottom_left[1] - 0.75;
}

void populate_multi_draw_vectors(
    Country* country,
    std::vector<int>& landmass_starts,
    std::vector<int>& landmass_sizes,
    float* colour,
    float* mapping,
    int& num_vertices)
{
    for (int i = 0; i < country->get_landmasses().size(); i++)
    {
        Eigen::MatrixXf coords = country->get_landmasses().at(i).cast<float>();
        for (int j = 0; j < coords.cols(); j++)
        {
            float data[] = { coords.col(j)[0], coords.col(j)[1], 0, colour[0], colour[1], colour[2] };
            memcpy(mapping, data, 6 * sizeof(float));
            mapping += 6;
        }

        landmass_starts.push_back(num_vertices);
        landmass_sizes.push_back(coords.cols());
        num_vertices += coords.cols();
    }
}

void draw(Country** countries, float** colours, int num_countries)
{
    float* mapping = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    std::vector<int> landmass_starts;
    std::vector<int> landmass_sizes;
    int num_vertices = 0;
    int num_landmasses = 0;

    for (int i = 0; i < num_countries; i++)
    {
        populate_multi_draw_vectors(countries[i], landmass_starts, landmass_sizes, colours[i], mapping + (6 * num_vertices), num_vertices);
        num_landmasses += countries[i]->get_landmasses().size();
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glMultiDrawArrays(GL_LINE_LOOP, &landmass_starts[0], &landmass_sizes[0], num_landmasses);
}