#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <gtk/gtk.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

using namespace std;
using namespace cv;

GtkWidget *window;
GtkWidget *button1;
GtkWidget *button2;

int clientSocket;

void encryptAES(const vector<uint8_t>& input, vector<uint8_t>& output, const vector<uint8_t>& key) {
    EVP_CIPHER_CTX* ctx;
    int len;
    int ciphertext_len;

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.data(), NULL);

    // Set up the output buffer
    output.resize(input.size() + EVP_CIPHER_block_size(EVP_aes_128_ecb()));

    // Perform the encryption
    EVP_EncryptUpdate(ctx, output.data(), &len, input.data(), input.size());
    ciphertext_len = len;

    // Finalize the encryption
    EVP_EncryptFinal_ex(ctx, output.data() + len, &len);
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    output.resize(ciphertext_len);
}

void button_clicked1(){
    // Carga la imagen de un archivo
    Mat serverImage = imread("/home/jct/Profesor-Server/tec-logo.jpg", IMREAD_UNCHANGED);

    // Convierte la imagen a un set de bytes
    vector<uint8_t> serverImageData;
    imencode(".jpg", serverImage, serverImageData);

    // Genera una clave AES aleatoria
    vector<uint8_t> aesKey(AES_BLOCK_SIZE);
    if (RAND_bytes(aesKey.data(), AES_BLOCK_SIZE) != 1) {
        cerr << "Error generando la clave AES" << endl;
        close(clientSocket);
        return;
    }

    // Encripta la imagen con AES
    vector<uint8_t> encryptedImageData;
    encryptAES(serverImageData, encryptedImageData, aesKey);

    // Envía el tamaño de la imagen cifrada y la clave al cliente
    uint32_t encryptedImageSize = encryptedImageData.size();
    send(clientSocket, &encryptedImageSize, sizeof(encryptedImageSize), 0);

    uint32_t keySize = aesKey.size();
    send(clientSocket, &keySize, sizeof(keySize), 0);

    send(clientSocket, encryptedImageData.data(), encryptedImageSize, 0);
    send(clientSocket, aesKey.data(), keySize, 0);
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
    clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);

    if (clientSocket == -1) {
        cerr << "Error aceptando conexiones" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Conectado al cliente " << inet_ntoa(clientAddress.sin_addr) << endl;

    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/jct/Profesor-Server/algoritmos.glade",NULL);

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