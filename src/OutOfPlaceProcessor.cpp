#include "../include/OutOfPlaceProcessor.hpp"

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string.h>

OutOfPlaceProcessor::OutOfPlaceProcessor(ISource &src, const std::initializer_list<Vertex> &vert) :
    Processor(src)
{
    CreateTexture();
    CreateRectangle(vert);
    // bindujemy VAO, aby renderować odpowiadający mu prostokąt; w całym programie używamy tylko jednego VAO, więc nie trzeba go bindować w pętli
    glBindVertexArray(VAO);
}

OutOfPlaceProcessor::OutOfPlaceProcessor(ISource &source) :
    OutOfPlaceProcessor(source,
        // pozycje wierzchołków prostokąta; tekstura jest odwrócona w pionie
        {
            // pozycja wierzchołka w przestrzeni kamery (od -1 do 1)
                                    // który punkt tekstury powinien znajdować się w danym wierzchołku (od 0 do 1)
            {1.f, 1.f, /* top right */ 1.f, 0.f}, // bottom right
            {1.f, -1.f, /* bottom right */ 1.f, 1.f}, // top right
            {-1.f, -1.f, /* bottom left */ 0.f, 1.f}, // top left
            {-1.f, 1.f, /* top left */ 0.f, 0.f} // bottom left
        })
{}

OutOfPlaceProcessor::~OutOfPlaceProcessor()
{
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &Texture1);
}

void OutOfPlaceProcessor::UploadFrame()
{
    // przesyłamy klatkę do pamięci GPU jako teksturę
    // glTexImage2D rezerwuje nową pamięć dla aktualnie zbindowanego uchwytu tekstury w GPU, a glTexSubImage2D tylko aktualizuje jej istniejącą pamięć
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, InputWidth / 2, InputHeight, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, Input); // tekstura w formacie YUYV422; każdy teksel tekstury ma obejmować 2 kolejne w poziomie piksele obrazu, więc dzielimy InputWidth / 2
}

void OutOfPlaceProcessor::ProcessFrame()
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

void OutOfPlaceProcessor::CreateTexture() // tworzymy teksturę, do której przekazujemy klatkę pobraną z kamery
{
    glGenTextures(1, &Texture1);
    // bindujemy teksturę, ponieważ będziemy wykonywać na niej operacje
    glBindTexture(GL_TEXTURE_2D, Texture1);
    // ustawiamy zawijanie (wrapping) aktualnie zbindowanej tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // ustawiamy filtrowanie (filtering) aktualnie zbindowanej tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // jeżeli używamy int (a nie standardowej float) tekstury, musi być GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI, InputWidth / 2, InputHeight, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, NULL); // tekstura rgba; każdy teksel tekstury ma obejmować 2 kolejne w poziomie piksele obrazu, więc dzielimy InputWidth / 2
}

void OutOfPlaceProcessor::CreateRectangle(const std::initializer_list<Vertex> &vertices)
{
    // VAO tworzy się dla każdego obiektu renderowanego na scenie; my renderujemy tylko 1 prostokąt pokrywający cały ekran, więc tworzymy tylko 1 VAO; łączymy VBO z VAO; zapisujemy atrybuty wierzchołków  VBO; do zapisanych wartości odwołujemy się poprzez zmienne typu attribute występujące w vertex shaderze programu
    glGenVertexArrays(1, &VAO);
    // bindujemy VAO, aby następnie połączyć z nim VBO, w którym znajdują się współrzędne wierzchołków i zapisać w VAO konfiguracje atrybutów wierzchołka
    glBindVertexArray(VAO);

    // tworzymy Vertex Buffer Object, czyli bufor w GPU, w którym umieścimy współrzędne wierzchołków
    glGenBuffers(1, &VBO);
    // bindujemy VBO, czyli aktywujemy bufor, aby przesłać do niego dane
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // przesyłamy wszystkie atrybuty wierzchołków do bufora
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.begin(), GL_STATIC_DRAW);

    // numery kolejnych wierzchołków, z których budujemy prostokąt
    GLuint indices[] =
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + sizeof(Vertex::Position)));

    // kiedy bindować/odbindowywać VAO, VBO, EBO (inaczej IBO): https://gamedev.stackexchange.com/questions/90471/should-unbind-buffers
    // odbindowujemy VAO koniecznie przed odbindowaniem VBO i EBO, bo VAO zapisuje też informację o zbindowaniu/odbindowaniu VBO/EBO, a chcemy, mieć je zbindowane w momencie zbindowania VAO podczas renderowania
    glBindVertexArray(0);
    // odbindowujemy VBO; bindujemy je tylko na czas edycji
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // odbindowujemy EBO; bindujemy je tylko na czas edycji
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
