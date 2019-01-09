#include <ngl/AbstractVAO.h>
#include <ngl/SimpleVAO.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>
#include <ngl/Texture.h>
#include <memory>
#include <array>
#include <benchmark/benchmark.h>
#include <GLFW/glfw3.h>

std::unique_ptr<ngl::AbstractVAO>  g_screenQuad;
GLFWwindow* g_window=nullptr;
GLuint g_textureName=0;
int g_width,g_height;
void initScreenQuad(GLFWwindow* _window,int _w, int _h)
{
  g_window=_window;
  g_width=_w;
  g_height=_h;
  g_screenQuad=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_TRIANGLES);
  g_screenQuad->bind();

  std::array<GLfloat,12> quad ={{-1.0f,1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f, 1.0f}};
  g_screenQuad->setData(ngl::AbstractVAO::VertexData(quad.size()*sizeof(GLfloat),quad[0]));
  g_screenQuad->setVertexAttributePointer(0,2,GL_FLOAT,0,0);
  g_screenQuad->setNumIndices(quad.size());
  g_screenQuad->unbind();
  ngl::Texture texture("textures/ratGrid.png");
  g_textureName=texture.setTextureGL();


}


static void normalScreenQuad(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();
  shader->use("ScreenQuad");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,g_width,g_height);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,g_textureName);

  for (auto _ : state)
  {
    auto start = std::chrono::high_resolution_clock::now();
    g_screenQuad->bind();
    g_screenQuad->draw();
    g_screenQuad->unbind();
    auto end   = std::chrono::high_resolution_clock::now();
    auto elapsed =std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed.count());
    glfwSwapBuffers(g_window);
    glfwPollEvents();


  }
  ngl::msg->addMessage("done test");

}

static void normalScreenQuadBound(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();
  shader->use("ScreenQuad");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,g_width,g_height);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,g_textureName);

  g_screenQuad->bind();
  for (auto _ : state)
  {
    auto start = std::chrono::high_resolution_clock::now();
    g_screenQuad->draw();
    auto end   = std::chrono::high_resolution_clock::now();
    auto elapsed =std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed.count());
    glfwSwapBuffers(g_window);
    glfwPollEvents();


  }
  ngl::msg->addMessage("done test");

  g_screenQuad->unbind();
}


constexpr int rangeStart=1;
constexpr int rangeEnd=4; //24

BENCHMARK(normalScreenQuad)->Range(rangeStart,rangeEnd)->UseManualTime();
BENCHMARK(normalScreenQuadBound)->Range(rangeStart,rangeEnd)->UseManualTime();
