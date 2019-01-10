#include <iostream>
#include <string>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <GLFW/glfw3.h>
#include <benchmark/benchmark.h>


extern void initScreenQuad();
extern void initVAO();
GLFWwindow* g_window=nullptr;
int g_width,g_height;

int main(int argc, char **argv)
{

  std::cerr<<"Creating OpenGL Context\n";
  GLFWwindow* window;

  /* Initialize the library */
  glfwInit();

  // use GL 4.1 as it's the max mac can use.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(1024, 720, "Benchmarks", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  /* Make the window's context current */
  glfwSwapInterval(0);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  glfwShowWindow(window);
  glfwPollEvents();


  // whilst we will do this in a test, best to make sure we have a valid context here
  // incase we run isolated tests on just GL elements
  ngl::NGLInit::instance()->setCommunicationMode(ngl::CommunicationMode::NULLCONSUMER);
  // load shader (not part of BM?)
  auto shader=ngl::ShaderLib::instance();
  shader->loadShader("UBO","shaders/PBRVertex.glsl","shaders/PBRFragment.glsl");
  shader->loadShader("Phong","shaders/PhongVertex.glsl","shaders/PhongFragment.glsl");
  shader->loadShader("ScreenQuad","shaders/ScreenQuadVertex.glsl","shaders/ScreenQuadFragment.glsl");

  shader->use("ScreenQuad");

  glfwGetFramebufferSize  (window,&g_width,&g_height);
  shader->setUniform("screenResolution",ngl::Vec2(g_width,g_height));

  shader->loadShader("ScreenGeo","shaders/ScreenPointVertex.glsl","shaders/ScreenPointFragment.glsl","shaders/ScreenPointGeometry.glsl");


  g_window=window;
  initScreenQuad();
  initVAO();
  std::atexit(glfwTerminate);
  glClearColor(0.8f,0.8f,0.8f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ::benchmark::Initialize (&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks ();
}
