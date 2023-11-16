#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    // Crea el socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error creando el socket" << endl;
        return -1;
    }

    // Conecta el socket a un puerto
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        cerr << "Error conectando el socket" << endl;
        close(serverSocket);
        return -1;
    }

    // Recive la conexion del cliente
    if (listen(serverSocket, 10) == -1) {
        cerr << "Error recibiendo conexiones" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Esperando conexiones..." << endl;

    // Acepyta la conexion
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);

    if (clientSocket == -1) {
        cerr << "Error aceptando conexiones" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Conectado al cliente " << inet_ntoa(clientAddress.sin_addr) << endl;

    // Recive el tamaño de la imagen
    uint32_t imageSize;
    recv(clientSocket, &imageSize, sizeof(imageSize), 0);

    // Recive la información de la imagen
    vector<uint8_t> imageData(imageSize);
    recv(clientSocket, imageData.data(), imageSize, 0);

    // COnvierte la imagen al formato correcto
    Mat image = imdecode(imageData, IMREAD_UNCHANGED);

    // Enseña la imagen recibida
    imshow("Imagen recibida", image);
    waitKey(0);

    // Load an image from file
    Mat serverImage = imread("/home/tomeito/CLionProjects/Server/tec-logo.jpg", IMREAD_UNCHANGED);

    // Convert the image to a set of bytes
    vector<uint8_t> serverImageData;
    imencode(".jpg", serverImage, serverImageData);

    // Send the image size to the client
    uint32_t serverImageSize = serverImageData.size();
    send(clientSocket, &serverImageSize, sizeof(serverImageSize), 0);

    // Send the image data to the client
    send(clientSocket, serverImageData.data(), serverImageSize, 0);

    // Cierra los sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}

