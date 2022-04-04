#include <filesystem>
#include <iostream>

#include "country.h"
#include "gdal_helpers.h"
#include "gl_helpers.h"
#include "icp.h"
#include "sampling.h"
#include "shader.h"

// vector shader source
const std::string vs_src =
#include "../shaders/shader.vs"
"";

// fragment shader source
const std::string fs_src =
#include "../shaders/shader.fs"
"";

int main(int argc, char** argv) 
{
    if (argc != 4)
    {
        std::cout << "Usage: " << std::filesystem::path(argv[0]).filename().string() << " <shpfile dir> <country name> <country name>" << std::endl;
        return 2;
    }
    
    int exit_code = 0;
    Country* model = NULL;
    Country* data = NULL;
    KD_Tree* model_tree = NULL;

    try {
        std::filesystem::path shp_file = std::filesystem::path(argv[1]).append("ne_10m_admin_0_countries.shp");
        shp_file = std::filesystem::absolute(shp_file);
        OGRLayer* layer = gdal_init(shp_file.string().c_str(), "ne_10m_admin_0_countries");
        model = new Country(get_geometry(layer, argv[2]));
        data = new Country(get_geometry(layer, argv[3]));

        data->translate_to_vicinity_of(model);
        model_tree = new KD_Tree(model);
        Eigen::VectorXd perimeter_sums = relative_perimeter_cumulative_sum(data);

        glfwInit();
        int window_width = 920;
        int window_height = 920;
        GLFWwindow* window = glfw_create_window(window_width, window_height);

        glad_init();
        glViewport(0, 0, window_width, window_height);

        const char* vs_source = vs_src.c_str();
        const char* fs_source = fs_src.c_str();
        Shader shader(&vs_source, &fs_source);
        shader.use();

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * (model->get_num_coords() + data->get_num_coords()), NULL, GL_STATIC_DRAW);

        float orange[] = { 1.0, 0.5, 0.2 };
        float green[] = { 0.0, 1.0, 0.0 };
        float* colours[] = { orange, green };
        Country* countries[] = { model, data };

        while (!glfwWindowShouldClose(window))
        {
            glBindVertexArray(vao);
            process_input(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float s, x, y;
            fit_to_window(model, data, s, x, y);
            shader.set_uniform_float("s", s);
            shader.set_uniform_float("x", x);
            shader.set_uniform_float("y", y);

            shader.set_vertex_attribute("position", GL_FLOAT, 6 * sizeof(float), (void*)0);
            shader.set_vertex_attribute("colour", GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

            draw(countries, colours, 2);
            glfwSwapBuffers(window);
            glfwPollEvents();

            icp_single_iteration(model, data, model_tree, perimeter_sums, 100);
        }

        delete(model);
        delete(data);
        delete(model_tree);
    }
    catch (std::exception e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        exit_code = 1;

        if (model) 
        {
            delete(model); 
        }
        if (data) 
        { 
            delete(data); 
        }
        if (model_tree) 
        { 
            delete(model_tree); 
        }
    }

    glfwTerminate();
    return exit_code;
}

