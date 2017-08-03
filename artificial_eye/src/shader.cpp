#include "shader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>

using namespace std;

void ee::Shader::initialize(const std::string &vtxPath, const std::string &frgPath)
{
	std::ifstream vtxStream, frgStream;
	std::stringstream vtxCode, frgCode;
	vtxStream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	frgStream.exceptions(std::ios_base::badbit | std::ios_base::failbit);

	// open and load code into stream
	try
	{
		vtxStream.open(vtxPath);
		frgStream.open(frgPath);

		vtxCode << vtxStream.rdbuf();
		frgCode << frgStream.rdbuf();

		vtxStream.close();
		frgStream.close();
	}
	catch (std::ifstream::failure excp)
	{
		throw runtime_error("Issue when loading vertex or fragment shader file");
	}

	GLuint vtxID, frgID;
	GLint resultChk;
	GLchar resultLog[512];

	vtxID = glCreateShader(GL_VERTEX_SHADER);
	frgID = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::string strTmpCode = std::move(vtxCode.str());
	const GLchar *ptrCode = strTmpCode.c_str();
	glShaderSource(vtxID, 1, &ptrCode, nullptr);
	glCompileShader(vtxID);

	glGetShaderiv(vtxID, GL_COMPILE_STATUS, &resultChk);
	if (!resultChk)
	{
		glGetShaderInfoLog(vtxID, 512, nullptr, resultLog);
		std::cout << resultLog << endl;
		assert(false);
	}

	strTmpCode = std::move(frgCode.str());
	ptrCode = strTmpCode.c_str();
	glShaderSource(frgID, 1, &ptrCode, nullptr);
	glCompileShader(frgID);

	glGetShaderiv(frgID, GL_COMPILE_STATUS, &resultChk);
	if (!resultChk)
	{
		glGetShaderInfoLog(frgID, 512, nullptr, resultLog);
		std::cout << resultLog << endl;
		assert(false);
	}

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vtxID);
	glAttachShader(m_programID, frgID);

	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &resultChk);
	if (!resultChk)
	{
		glGetProgramInfoLog(m_programID, 512, nullptr, resultLog);
		throw std::logic_error("Issue when linking program");
	}

	glDeleteShader(vtxID);
	glDeleteShader(frgID);
}

ee::Shader::~Shader()
{
	glDeleteProgram(m_programID);
}

void ee::Shader::use() const
{
	glUseProgram(m_programID);
}