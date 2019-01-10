#include <ngl/AbstractVAO.h>
#include <ngl/SimpleVAO.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>
#include <ngl/Texture.h>
#include <ngl/Random.h>
#include <memory>
#include <array>
#include <vector>
#include <benchmark/benchmark.h>
#include <GLFW/glfw3.h>

std::unique_ptr<ngl::AbstractVAO>  g_simpleVAO;

extern GLFWwindow* g_window;
extern int g_width,g_height;

std::vector <ngl::Vec3> g_data;

void initVAO()
{
  g_simpleVAO=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_LINES);
  g_simpleVAO->bind();
  g_data.resize(12345);
  auto rng=ngl::Random::instance();
  for(auto &p : g_data)
  {
    p=rng->getRandomVec3()*5;
  }

  g_simpleVAO->setData( ngl::SimpleVAO::VertexData(g_data.size()*sizeof(ngl::Vec3),g_data[0].m_x));
  // We must do this each time as we change the data.
  g_simpleVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  g_simpleVAO->setNumIndices(g_data.size());
  g_simpleVAO->unbind();
  ngl::Mat4 project=ngl::perspective(45.0f,1.0f,0.01f,200.0f);
  ngl::Mat4 view=ngl::lookAt({10,10,10},{0,0,0},{0,1,0});
  auto shader=ngl::ShaderLib::instance();
  shader->use(ngl::nglColourShader);
  shader->setUniform("MVP",project*view);
  shader->setUniform("Colour",1.0f,1.0f,1.0f,1.0f);
}




static void drawVAO(benchmark::State& state)
{
  g_simpleVAO->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,g_width,g_height);
  ngl::ShaderLib::instance()->use(ngl::nglColourShader);

  for (auto _ : state)
  {
    g_simpleVAO->draw();
    glfwSwapBuffers(g_window);
  }
  g_simpleVAO->unbind();


}



static void updateVAO(benchmark::State& state)
{
  g_simpleVAO->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,g_width,g_height);
  ngl::ShaderLib::instance()->use(ngl::nglColourShader);

  for (auto _ : state)
  {
    g_simpleVAO->setData( ngl::SimpleVAO::VertexData(g_data.size()*sizeof(ngl::Vec3),g_data[0].m_x));
    // We must do this each time as we change the data.
    g_simpleVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    g_simpleVAO->setNumIndices(g_data.size());

    g_simpleVAO->draw();
    glfwSwapBuffers(g_window);
  }
  g_simpleVAO->unbind();


}


static void pointerupdateVAO(benchmark::State& state)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,g_width,g_height);
  ngl::ShaderLib::instance()->use(ngl::nglColourShader);
  size_t size=g_data.size()*3;
  for (auto _ : state)
  {
    ngl::Real *ptr=g_simpleVAO->mapBuffer();
    for(size_t i=0; i<size; ++i)
    {
      ptr[i]=0.0f;
    }
    g_simpleVAO->unmapBuffer();
    g_simpleVAO->bind();
    g_simpleVAO->draw();
    g_simpleVAO->unbind();
    glfwSwapBuffers(g_window);
  }


}




BENCHMARK(drawVAO)->Unit(benchmark::kMillisecond);
BENCHMARK(updateVAO)->Unit(benchmark::kMillisecond);
BENCHMARK(pointerupdateVAO)->Unit(benchmark::kMillisecond);


