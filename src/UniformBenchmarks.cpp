#include <benchmark/benchmark.h>
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
    shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
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
    glUseProgram(id);

    shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
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
    shader->use("UBO");

    shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
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
    shader->setUniform("MVP",MVP);
    shader->setUniform("M",M);
    shader->setUniform("normalMatrix",normalMatrix);
  }
}

constexpr int rangeStart=1024;
constexpr int rangeEnd=1000000; //24
BENCHMARK(LoadShaderUniform)->Range(rangeStart,rangeEnd);
BENCHMARK(LoadShaderUBO)->Range(rangeStart,rangeEnd);
BENCHMARK(LoadShaderUBOBindGL)->Range(rangeStart,rangeEnd);
BENCHMARK(LoadShaderUBOBUseNGL)->Range(rangeStart,rangeEnd);
