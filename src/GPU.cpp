#include "../include/GPU.hpp"
#include "../include/FileIO.hpp"

#include <GL/glew.h>
#include <stdexcept>
#include <string.h>

struct Vertex
{
    float Position[2], TextureCoord[2];
};

GPU::GPU(IBuffer *source, IBuffer *target, const char *vertexShaderFilePath, const char *fragmentShaderFilePath) :
    Processor(source),
    Output(target->GetBuffer())
{
    if (Width != target->GetWidth())
        throw std::runtime_error("GPU::GPU; Source and target have different width.");
    if (Height != target->GetHeight())
        throw std::runtime_error("GPU::GPU; Source and target have different height.");

    CreateTexture();
    CreateProgram(vertexShaderFilePath, fragmentShaderFilePath);
    // aktywujemy już zlinkowany program z shaderami, które będą użyte do renderowania trójkątów tworzących prostokąt; w całym programie używamy tylko jednego shader programu i jednego VAO, więc można aktywować shader program w innym miejscu, niekoniecznie tutaj
    glUseProgram(Program);
    SetConstantUniforms();
    CreateRectangle();
    CreateFramebuffer();

    // bindujemy VAO, aby renderować odpowiadający mu prostokąt; w całym programie używamy tylko jednego VAO, więc nie trzeba go bindować w pętli
    glBindVertexArray(VAO);
    // bindujemy FBO, aby renderować do niego scenę (składającą się tylko z prostokąta)
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

GPU::~GPU()
{
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(Program);
    glDeleteTextures(1, &Texture1);
}

void GPU::UploadFrame()
{
    // przesyłamy klatkę do pamięci GPU jako teksturę
    // glTexImage2D rezerwuje nową pamięć dla aktualnie zbindowanego uchwytu tekstury w GPU, a glTexSubImage2D tylko aktualizuje jej istniejącą pamięć
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width / 2, Height, GL_RGBA, GL_UNSIGNED_BYTE, Input); // tekstura rgba; każdy teksel tekstury ma obejmować 2 kolejne w poziomie piksele obrazu, więc dzielimy WIDTH / 2
    // memcpy((void*)gpuBuffer, Input, (Width * Height) / 2 * 4);
}

void GPU::ProcessFrame()
{
    // aktywujemy jednostkę teksturującą (texture unit) o indeksie 0; nie trzeba tego robić, bo texture unit o indeksie 0 jest zawsze domyślnie aktywny; jeżeli używalibyśmy kilku tekstur jednocześnie, to musielibyśmy kolejno aktywować texture unit o indeksie n i zbindować do niego wybraną teksturę, a następnie to samo zrobić dla texture unita o indeksie n+1
    // glActiveTexture(GL_TEXTURE0);
    // bindujemy teksturę texture1 do aktualnie aktywnego texture unita
    // glBindTexture(GL_TEXTURE_2D, Texture1);

    // rysujemy prostokąt, używając jako typu prymitywu GL_TRIANGLES
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // nie trzeba za każdym razem odbindowywać VAO, bo używamy tylko jednego VAO
    // glBindVertexArray(0);
}

void GPU::DownloadFrame()
{
    glReadPixels(0, 0, Width / 2, Height, GL_RGBA, GL_UNSIGNED_BYTE, Output);
}

void GPU::CheckShaderCompileStatus(unsigned int handle)
{
    int success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE)
        return;
    char infoLog[512];
    glGetShaderInfoLog(handle, 512, NULL, infoLog);
    throw std::runtime_error(std::string("GPU::CheckShaderStatus; Shader was not compiled successfully.\n") + infoLog);
}

void GPU::CheckProgramLinkStatus(unsigned int handle)
{
    int success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (success == GL_TRUE)
        return;
    char infoLog[512];
    glGetProgramInfoLog(handle, 512, NULL, infoLog);
    throw std::runtime_error(std::string("GPU::CheckProgramLinkStatus; Program was not linked successfully.\n") + infoLog);
}

void GPU::CreateTexture() // tworzymy teksturę, do której przekazujemy klatkę pobraną z kamery
{
    glGenTextures(1, &Texture1);
    // bindujemy teksturę, ponieważ będziemy wykonywać na niej operacje
    glBindTexture(GL_TEXTURE_2D, Texture1);
    // ustawiamy zawijanie (wrapping) aktualnie zbindowanej tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // ustawiamy filtrowanie (filtering) aktualnie zbindowanej tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // lub GL_LINEAR; gdy używamy standardowej float tekstury (każdy teksel ma znormalizowane wartości składowych do przedziału [0, 1]), metoda filtrowania ma znaczenie tylko, gdy fragment shader używa do samplowania texture2D, a nie texelFetch
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width / 2, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // tekstura rgba; każdy teksel tekstury ma obejmować 2 kolejne w poziomie piksele obrazu, więc dzielimy width / 2
}

void GPU::CreateProgram(const char *vertexShaderPath, const char *fragmentShaderPath) // kompilujemy shadery i linkujemy je do programu
{
    // kod vertex shadera wykonywany dla każdego wierzchołka umieszczonego w buforze VBO
    char *vertexShaderCode = NULL;
    if (loadText(vertexShaderPath, &vertexShaderCode) < 0)
        throw std::runtime_error(std::string("GPU::CreateProgram; Failed to load vertex shader code from file ") + vertexShaderPath);
    // tworzymy nowy vertex shader w GPU i zapisujemy do niego uchwyt
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // przekazujemy kod do utworzonego shadera
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    free(vertexShaderCode);
    // kompilujemy shader
    glCompileShader(vertexShader);
    // sprawdzamy, czy kompilacja się powiodła
    CheckShaderCompileStatus(vertexShader);

    // kod fragment shadera wykonywany dla każdego piksela wyświetlanej figury (tutaj prostokąta)
    char *fragmentShaderCode = NULL;
    if (loadText(fragmentShaderPath, &fragmentShaderCode) < 0)
        throw std::runtime_error(std::string("GPU::CreateProgram; Failed to load fragment shader code from file ") + vertexShaderPath);
    // tworzymy nowy fragment shader w GPU i zapisujemy do niego uchwyt
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // przekazujemy kod do utworzonego shadera
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    free(fragmentShaderCode);
    // kompilujemy shader
    glCompileShader(fragmentShader);
    // sprawdzamy, czy kompilacja się powiodła
    CheckShaderCompileStatus(fragmentShader);

    // tworzymy program, do którego linkujemy skompilowane shadery, i zapisujemy do niego uchwyt
    Program = glCreateProgram();
    // sprawdzamy, czy program został prawidłowo stworzony
    if (Program == 0)
        throw std::runtime_error("GPU::CreateProgram; Failed to create shader program.");
    // przyłączamy vertex shader do programu
    glAttachShader(Program, vertexShader);
    // przyłączamy fragment shader do programu
    glAttachShader(Program, fragmentShader);
    // linkujemy przyłączone shadery
    glLinkProgram(Program);
    CheckProgramLinkStatus(Program);

    // usuwamy vertex shader po jego zlinkowaniu do programu
    glDeleteShader(vertexShader);
    // usuwamy fragment shader po jego zlinkowaniu do programu
    glDeleteShader(fragmentShader);
}

void GPU::SetConstantUniforms() // nadajemy wartości stałym typu uniform we fragment shaderze aktywnego shader programu
{
    // zapisujemy uchwyt (handle) do zmiennej typu uniform występującej we fragment shaderze (uniform sampler2D texture1)
    int texture1Sampler2D = glGetUniformLocation(Program, "texture1");
    // mając aktywny wybrany shader program, zapisujemy w tej zmiennej indeks texture unita, który odpowiada za wczytaną przez CPU teksturę texture1; za pomocą tego indeksu odwołujemy się we fragment shaderze do texture unita, aby samplować teksturę wczytaną w tym texture unicie
    glUniform1i(texture1Sampler2D, 0);

    int size = glGetUniformLocation(Program, "size");
    glUniform2i(size, Width - 1, Height - 1);
}

void GPU::CreateRectangle()
{
    // VAO tworzy się dla każdego obiektu renderowanego na scenie; my renderujemy tylko 1 prostokąt pokrywający cały ekran, więc tworzymy tylko 1 VAO; łączymy VBO z VAO; zapisujemy atrybuty wierzchołków  VBO; do zapisanych wartości odwołujemy się poprzez zmienne typu attribute występujące w vertex shaderze programu
    glGenVertexArrays(1, &VAO);
    // bindujemy VAO, aby następnie połączyć z nim VBO, w którym znajdują się współrzędne wierzchołków i zapisać w VAO konfiguracje atrybutów wierzchołka
    glBindVertexArray(VAO);

    // pozycje wierzchołków prostokąta
    struct Vertex vertices[4] = // tekstura pokrywa się z prostokątem
    {
        // pozycja wierzchołka w przestrzeni kamery (od -1 do 1)
                    // który punkt tekstury powinien znajdować się w danym wierzchołku (od 0 do 1)
        1.f, 1.f,  1.f, 1.f, // top right
        1.f, -1.f,  1.f, 0.f, // bottom right
        -1.f, -1.f,  0.f, 0.f, // bottom left
        -1.f, 1.f,  0.f, 1.f // top left
    };
    // tworzymy Vertex Buffer Object, czyli bufor w GPU, w którym umieścimy współrzędne wierzchołków
    glGenBuffers(1, &VBO);
    // bindujemy VBO, czyli aktywujemy bufor, aby przesłać do niego dane
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // przesyłamy wszystkie atrybuty wierzchołków do bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // numery kolejnych wierzchołków, z których budujemy prostokąt
    unsigned int indices[] =
    {
        0, 1, 3, // pierwszy trójkąt: top right, bottom right, top left
        1, 2, 3 // drugi trójkąt: bottom right, bottom left, top left
    };
    // tworzymy Element Buffer Object, czyli bufor w GPU, w którym umieścimy numery kolejnych wierzchołków, z których jest budowana figura podczas renderowania
    glGenBuffers(1, &EBO);
    // bindujemy EBO, czyli aktywujemy bufor, aby przesłać do niego dane
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // przesyłamy numery wierzchołków do bufora
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // aktywujemy atrybut o indeksie (lokacji) 0, czyli vertexCoord
    glEnableVertexAttribArray(0);
    // określamy sposób interpretacji przez aktualnie używany vertex shader współrzędnych wierzchołków, które są pobierane z VBO zbindowanego w momencie, gdy wywołujemy poniższą funkcję glVertexAttribPointer, a następnie umieszczane dla każdego wierzchołka w jego atrybucie wejściowym vertexCoord (layout (location = 0) in vec2 vertexCoord)
    // w funkcji glVertexAttribPointer wskazujemy, jak ma być wypełniany z bufora VBO atrybut o indeksie (lokacji) 0 w vertex shaderze aktualnie używanego (glUseProgram) shader programu; indeks (równy lokacji) atrybutu 0 może się powtarzać w różnych vertex shaderach
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // kiedy bindować/odbindowywać VAO, VBO, EBO (inaczej IBO): https://gamedev.stackexchange.com/questions/90471/should-unbind-buffers
    // odbindowujemy VAO koniecznie przed odbindowaniem VBO i EBO, bo VAO zapisuje też informację o zbindowaniu/odbindowaniu VBO/EBO, a chcemy, mieć je zbindowane w momencie zbindowania VAO podczas renderowania
    glBindVertexArray(0);
    // odbindowujemy VBO; bindujemy je tylko na czas edycji
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // odbindowujemy EBO; bindujemy je tylko na czas edycji
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GPU::CreateFramebuffer() // tworzymy Framebuffer Object (FBO) i Render Buffer Object (RBO) jako color attachment do FBO
{
    // tworzymy uchwyt do FBO
    glGenFramebuffers(1, &FBO);
    // bindujemy FBO zamiast domyślnego Framebuffer Objectu, który jest tworzony wraz z oknem programu i do którego GPU domyślnie renderuje
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // tworzymy uchwyt do RBO
    glGenRenderbuffers(1, &RBO);
    // bindujemy RBO
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    // rezerwujemy pamięć RBO
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width / 2, Height);
    // odbindowujemy RBO, bo zbindowaliśmy uchwyt do niego tylko po to, aby zarezerwować na niego pamięć
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // dołączamy RBO jako color attachment do FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);

    // sprawdzamy, czy FBO jest kompletne
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("GPU::CreateFramebuffer; Framebuffer is not complete.");
    // odbindowujemy FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
