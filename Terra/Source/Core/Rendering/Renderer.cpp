#include "Renderer.h"

void Terra::Renderer::Draw(VertexArray& va, const Texture& tx, unsigned int shaderID)
{
    //Activate our shader.
    glUseProgram(shaderID);
    
    //Bind Texture and VAO objects in order for OpenGL to use it. 
    tx.Bind(0);
    va.Bind();

    //Draw all vertices.
    //glDrawElements(GL_TRIANGLES, sizeof(Indices), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, 6 * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}

void Terra::Renderer::Draw(VertexArray& va, const Texture& tx, unsigned int shaderID, int amount)
{
    //Activate our shader.
    glUseProgram(shaderID);
    
    //Bind Texture and VAO objects in order for OpenGL to use it. 
    tx.Bind(0);
    va.Bind();

    //Draw all vertices.
    glDrawElements(GL_TRIANGLES, amount , GL_UNSIGNED_INT, 0);
}

void Terra::Renderer::Clear()
{
    //Clear screen
    glClearColor(0.f, 0.1f, 0.2f, 255.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
