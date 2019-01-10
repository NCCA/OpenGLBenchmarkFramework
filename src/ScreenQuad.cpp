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
std::unique_ptr<ngl::AbstractVAO>  g_screenQuadPoint;

extern GLFWwindow* g_window;
GLuint g_textureName=0;
extern int g_width,g_height;
void initScreenQuad()
{
  g_screenQuad=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_TRIANGLES);
  g_screenQuad->bind();

  std::array<GLfloat,12> quad ={{-1.0f,1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f, 1.0f}};
  g_screenQuad->setData(ngl::AbstractVAO::VertexData(quad.size()*sizeof(GLfloat),quad[0]));
  g_screenQuad->setVertexAttributePointer(0,2,GL_FLOAT,0,0);
  g_screenQuad->setNumIndices(quad.size());
  g_screenQuad->unbind();
  ngl::Texture texture("textures/ratGrid.png");
  g_textureName=texture.setTextureGL();

  g_screenQuadPoint=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS);
  g_screenQuadPoint->bind();
  std::array<GLfloat,2> point={{0,0}};
  g_screenQuadPoint->setData(ngl::AbstractVAO::VertexData(point.size()*sizeof(GLfloat),point[0]));
  g_screenQuadPoint->setVertexAttributePointer(0,2,GL_FLOAT,0,0);
  g_screenQuadPoint->setNumIndices(point.size());
  g_screenQuadPoint->unbind();
}




static void geoScreenQuadBound(benchmark::State& state)
{
  auto shader=ngl::ShaderLib::instance();
  shader->use("ScreenGeo");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,g_width,g_height);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,g_textureName);
  g_screenQuadPoint->bind();

  for (auto _ : state)
  {
    g_screenQuadPoint->draw();
    glfwSwapBuffers(g_window);
  }
  g_screenQuadPoint->unbind();

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
    g_screenQuad->draw();
    glfwSwapBuffers(g_window);
  }

  g_screenQuad->unbind();
}


BENCHMARK(normalScreenQuadBound)->Unit(benchmark::kMillisecond);
BENCHMARK(geoScreenQuadBound)->Unit(benchmark::kMillisecond);

