#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <GLFW/glfw3.h>

extern void initScreenQuad(GLFWwindow* _window,int _w, int _h);

// key callback
void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){}
// mouse button press callback
void mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods){}
// mouse move callback
void cursorPosCallback(GLFWwindow* _window, double _xpos, double _ypos){}
// mouse wheel callback
void scrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset){}


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

  //glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );
 // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
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

  // set the key callback
  glfwSetKeyCallback(window, keyCallback);
  // set mouse callback
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  // mouse cursor move callback
  glfwSetCursorPosCallback(window, cursorPosCallback);
  // scroll wheel callback
  glfwSetScrollCallback(window, scrollCallback);

  glfwShowWindow(window);
  glfwPollEvents();


  // whilst we will do this in a test, best to make sure we have a valid context here
  // incase we run isolated tests on just GL elements
  ngl::NGLInit::instance()->setCommunicationMode(ngl::CommunicationMode::STDOUT);
  // load shader (not part of BM?)
  auto shader=ngl::ShaderLib::instance();
  shader->loadShader("UBO","shaders/PBRVertex.glsl","shaders/PBRFragment.glsl");
  shader->loadShader("Phong","shaders/PhongVertex.glsl","shaders/PhongFragment.glsl");
  shader->loadShader("ScreenQuad","shaders/ScreenQuadVertex.glsl","shaders/ScreenQuadFragment.glsl");
  shader->use("ScreenQuad");

  int width, height;
  glfwGetFramebufferSize  (window,&width,&height);
  shader->setUniform("screenResolution",ngl::Vec2(width,height));
  initScreenQuad(window,width,height);
  std::atexit(glfwTerminate);
  glClearColor(0.8,0.8,0.8,1.0);
  glClear(GL_COLOR_BUFFER_BIT);


  ::benchmark::Initialize (&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks ();

}
