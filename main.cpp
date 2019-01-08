#include <benchmark/benchmark.h>
#include <array>
#include <iostream>
#include <string>
#include <ngl/NGLInit.h>
#include <GLFW/glfw3.h>
#include <getopt.h>
#include <thread>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>



static void LoadShaderUBO(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();

  shader->use("UBO");
  struct transform
  {
    ngl::Mat4 MVP;
    ngl::Mat4 normalMatrix;
    ngl::Mat4 M;
  };

   transform t;

  for (auto _ : state)
  {
    auto start = std::chrono::high_resolution_clock::now();
    // Simulate some useful workload with a sleep
    shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
    auto end   = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}


static void LoadShaderUBOBindGL(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();

  struct transform
  {
    ngl::Mat4 MVP;
    ngl::Mat4 normalMatrix;
    ngl::Mat4 M;
  };

   transform t;
  auto id=shader->getProgramID("UBO");
  for (auto _ : state)
  {
    auto start = std::chrono::high_resolution_clock::now();
    // Simulate some useful workload with a sleep
    glUseProgram(id);

    shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
    auto end   = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

static void LoadShaderUBOBUseNGL(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();

  struct transform
  {
    ngl::Mat4 MVP;
    ngl::Mat4 normalMatrix;
    ngl::Mat4 M;
  };

   transform t;
  for (auto _ : state)
  {
    auto start = std::chrono::high_resolution_clock::now();
    // Simulate some useful workload with a sleep
    shader->use("UBO");

    shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
    auto end   = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}



static void LoadShaderUniform(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();

  shader->use("Phong");
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;


  for (auto _ : state)
  {
    auto start = std::chrono::high_resolution_clock::now();
    // Simulate some useful workload with a sleep
    shader->setUniform("MVP",MVP);
    shader->setUniform("M",M);
    shader->setUniform("normalMatrix",normalMatrix);

    auto end   = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

BENCHMARK(LoadShaderUniform)->Range(1, 1<<24)->UseManualTime();
BENCHMARK(LoadShaderUBO)->Range(1, 1<<24)->UseManualTime();
BENCHMARK(LoadShaderUBOBindGL)->Range(1, 1<<24)->UseManualTime();
BENCHMARK(LoadShaderUBOBUseNGL)->Range(1, 1<<24)->UseManualTime();


int main(int argc, char **argv)
{

  std::cerr<<"Creating OpenGL Context\n";
  GLFWwindow* window;

  /* Initialize the library */
  auto init=glfwInit();

  // use GL 4.1 as it's the max mac can use.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(1024, 720, "", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  // whilst we will do this in a test, best to make sure we have a valid context here
  // incase we run isolated tests on just GL elements
  ngl::NGLInit::instance()->setCommunicationMode(ngl::CommunicationMode::NULLCONSUMER);
  // load shader (not part of BM?)
  auto shader=ngl::ShaderLib::instance();
  shader->loadShader("UBO","shaders/PBRVertex.glsl","shaders/PBRFragment.glsl");
  shader->loadShader("Phong","shaders/PhongVertex.glsl","shaders/PhongFragment.glsl");

  std::atexit(glfwTerminate);

  ::benchmark::Initialize (&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks ();

}
