/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef SHADER_H
#define SHADER_H

#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class QString;
class QFile;

class Shader {
 public:
  Shader() {}
  Shader(const Shader &) = delete;
  Shader(Shader &&) = delete;
  auto operator=(const Shader &) -> Shader & = delete;
  auto operator=(Shader &&) -> Shader & = delete;

  // create the shader
  void create(QString vertexPath, QString fragmentPath);
  void create(QString vertexPath, QString fragmentPath, QString geometryPath);

  // activate the shader
  void use();

  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w);
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

 private:
  // utility function for checking shader compilation/linking errors.
  void checkCompileErrors(unsigned int shader, std::string type);

  unsigned int ID;
};

#endif  // SHADER_H
