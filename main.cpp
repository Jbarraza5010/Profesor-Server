#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <gtk/gtk.h>

using namespace std;
using namespace cv;

GtkWidget *window;
GtkWidget *button1;
GtkWidget *button2;

void button_clicked1(){
    printf("algoritmo1 \n");
}

void button_clicked2(){
    printf("algoritmo2 \n");
}

int main(int argc, char *argv[]) {
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

    // Recibe el tamaño de la imagen
    uint32_t imageSize;
    recv(clientSocket, &imageSize, sizeof(imageSize), 0);

    // Recibe la información de la imagen
    vector<uint8_t> imageData(imageSize);
    recv(clientSocket, imageData.data(), imageSize, 0);

    // Convierte la imagen
    Mat image = imdecode(imageData, IMREAD_UNCHANGED);

    // Enseña la imagen recibida
    imshow("Imagen recibida por el Profesor", image);
    waitKey(0);

    // Carga la imagen de un archivo
    Mat serverImage = imread("/home/tomeito/CLionProjects/Server/tec-logo.jpg", IMREAD_UNCHANGED);

    // Convierte la imagen a un set de bytes
    vector<uint8_t> serverImageData;
    imencode(".jpg", serverImage, serverImageData);

    // Envia el tamaño de la imagen al cliente
    uint32_t serverImageSize = serverImageData.size();
    send(clientSocket, &serverImageSize, sizeof(serverImageSize), 0);

    // Envia la imagen al cliente
    send(clientSocket, serverImageData.data(), serverImageSize, 0);

    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/tomeito/CLionProjects/Server/algoritmos.glade",NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "myWindow"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "algoritmo"));
    button2 = GTK_WIDGET(gtk_builder_get_object(builder, "algoritmo2"));

    g_signal_connect(button1, "clicked", G_CALLBACK(button_clicked1), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(button_clicked2), NULL);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();

    // Cierra los sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}