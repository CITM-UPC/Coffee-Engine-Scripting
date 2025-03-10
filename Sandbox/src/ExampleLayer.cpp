#include "ExampleLayer.h"
#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Buffer.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer() : Layer("Example")
    {
        float vertices[8 * 3] = {
            // positions        // colors          // Texture Coords
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f    // top
        };

        unsigned int indices[3] = {
            0,1,2
        };

        float cubeVertices[] = {
            // positions          // texture coords
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

            // Right face
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

            // Top face
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
        };


        unsigned int cubeIndices[] = {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Left face
            8, 9, 10,
            10, 11, 8,

            // Right face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Bottom face
            20, 21, 22,
            22, 23, 20
        };

        m_VertexArray = Coffee::VertexArray::Create();
        m_VertexArray->Bind();

        m_VertexBuffer = Coffee::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)); //el size no estoy muy seguro
        Coffee::BufferLayout layout = {
            {Coffee::ShaderDataType::Vec3, "a_Position"},
            {Coffee::ShaderDataType::Vec2, "a_TexCoord"}
        };

        m_VertexBuffer->SetLayout(layout);

        m_IndexBuffer = Coffee::IndexBuffer::Create(cubeIndices, 36);

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        
        m_defaultShader = Coffee::Shader::Create("assets/shaders/FaceIndexShader.glsl");

        m_Texture = Coffee::Texture::Load("assets/textures/test.jpg");

        m_Texture->Bind(0);

        m_defaultShader->Bind();
        m_defaultShader->setInt("tex", 0);
        

        helmet = Coffee::Model("assets/models/DamagedHelmet.glb");
        plane = Coffee::Model("assets/models/plane.glb");

        m_EditorCamera = Coffee::EditorCamera(45.0f);

        Coffee::Application::Get().GetWindow().SetVSync(false);

    }

void ExampleLayer::OnUpdate(float dt)
{
    //COFFEE_INFO("ExampleLayer::Update");

    m_EditorCamera.OnUpdate();
    
    Coffee::RendererAPI::SetClearColor({.2f,.2f,.2f,1});
    Coffee::RendererAPI::Clear();

    m_defaultShader->Bind();

    glm::mat4 view = glm::mat4(1.0f);


    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)900, 0.1f, 100.0f);

    //TEMP
    view = m_EditorCamera.GetViewMatrix();
    projection = m_EditorCamera.GetProjection();

    m_defaultShader->setMat4("view", view);
    m_defaultShader->setMat4("projection", projection);

    //m_RendererAPI->DrawIndexed(m_VertexArray);

    for (auto& mesh : helmet.GetMeshes()) {
        Coffee::Ref<Coffee::VertexBuffer> vb = mesh->GetVertexBuffer();
        Coffee::Ref<Coffee::IndexBuffer> ib = mesh->GetIndexBuffer();

        Coffee::Ref<Coffee::VertexArray> va = Coffee::VertexArray::Create();
        va->AddVertexBuffer(vb);
        va->SetIndexBuffer(ib);
        Coffee::RendererAPI::DrawIndexed(va);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f));

    m_defaultShader->setMat4("model", model);

    for (auto& mesh : plane.GetMeshes()) {
        Coffee::Ref<Coffee::VertexBuffer> vb = mesh->GetVertexBuffer();
        Coffee::Ref<Coffee::IndexBuffer> ib = mesh->GetIndexBuffer();

        Coffee::Ref<Coffee::VertexArray> va = Coffee::VertexArray::Create();
        va->AddVertexBuffer(vb);
        va->SetIndexBuffer(ib);
        Coffee::RendererAPI::DrawIndexed(va);
    }

    model = glm::mat4(1.0f);
}

void ExampleLayer::OnEvent(Coffee::Event& event)
{
    //COFFEE_TRACE("{0}", event);
    m_EditorCamera.OnEvent(event);
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("debug-cube");

    //ImGui::Text("Texture Parameters")
    ImGui::SeparatorText("Texture Parameters");

    static glm::vec2 textureOffset = glm::vec2(0,0);
    static float textureScale = 1.0f;

    ImGui::DragFloat2("textureOffset", glm::value_ptr(textureOffset), 0.01);

    ImGui::DragFloat("textureScale", &textureScale, 0.01);

    m_defaultShader->setVec2("textureOffset", textureOffset);
    m_defaultShader->setFloat("textureScale", textureScale);

    ImGui::SeparatorText("Transform Parameters");

    static glm::vec3 position = glm::vec3(0.0,0.0,0.0);
    static glm::vec3 eulerAngles = glm::vec3(0.0,0.0,0.0); 
    static glm::vec3 scale = glm::vec3(1.0f);
    

    ImGui::DragFloat3("position", glm::value_ptr(position), 0.01);
    ImGui::DragFloat3("Axis of rotation", glm::value_ptr(eulerAngles));
    ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.05);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    m_defaultShader->setMat4("model", model);

    ImGui::End();
}
