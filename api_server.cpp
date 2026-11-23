
#include "wims.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>

#pragma comment(lib, "ws2_32.lib")

static string urlDecode(const string& value)
{
    string result;

    for (size_t i = 0; i < value.size(); ++i)
    {
        if (value[i] == '+')
            result += ' ';

        else if (value[i] == '%' && i + 2 < value.size())
        {
            int code = 0;
            string hex = value.substr(i + 1, 2);

            stringstream ss;
            ss << hex;
            ss >> std::hex >> code;

            result += static_cast<char>(code);
            i += 2;
        }
        else
            result += value[i];
    }

    return result;
}

static string getValue(const string& body, const string& key)
{
    string part = key + "=";
    size_t start = body.find(part);

    if (start == string::npos)
        return "";

    start += part.size();

    size_t end = body.find('&', start);

    return urlDecode(
        body.substr(start,
        end == string::npos ? string::npos : end - start)
    );
}

static string htmlSafe(const string& value)
{
    string out;

    for (size_t i = 0; i < value.size(); ++i)
    {
        if (value[i] == '&') out += "&amp;";
        else if (value[i] == '<') out += "&lt;";
        else if (value[i] == '>') out += "&gt;";
        else if (value[i] == '\"') out += "&quot;";
        else out += value[i];
    }

    return out;
}

static void replaceText(
    string& page,
    const string& oldText,
    const string& newText)
{
    size_t pos = page.find(oldText);

    if (pos != string::npos)
        page.replace(pos, oldText.size(), newText);
}

static string makeProductRows(WarehouseSystem& system)
{
    string rows;
    Product* p = system.getProductHead();

    if (p == NULL)
        return "<tr><td colspan='6'>No products available</td></tr>";

    while (p != NULL)
    {
        string status =
            p->getQuantity() <= 10 ? "Low Stock" : "In Stock";

        rows += "<tr><td class='product-id'>#" +
                to_string(p->getId()) +
                "</td><td><strong>" +
                htmlSafe(p->getName()) +
                "</strong></td><td>" +
                to_string(p->getQuantity()) +
                " Units</td><td>Rs. " +
                to_string(p->getPrice()) +
                "</td><td>" +
                htmlSafe(p->getExpiry()) +
                "</td><td><span class='status " +
                string(p->getQuantity() <= 10 ?
                "low-stock" : "in-stock") +
                "'>" + status +
                "</span></td></tr>";

        p = p->getNext();
    }

    return rows;
}

static string makePage(
    WarehouseSystem& system,
    const string& message)
{
    ifstream file("index.html");

    string page, line;

    if (!file)
    {
        return "<html><body><h2>index.html not found</h2>"
               "<p>Keep index.html beside wims.exe.</p></body></html>";
    }

    while (getline(file, line))
        page += line + "\n";

    string start = "<!-- PRODUCT_ROWS_START -->";
    string finish = "<!-- PRODUCT_ROWS_END -->";

    size_t a = page.find(start);
    size_t b = page.find(finish);

    if (a != string::npos && b != string::npos && b > a)
    {
        page.replace(
            a + start.size(),
            b - (a + start.size()),
            makeProductRows(system)
        );
    }

    string notice = "";

    if (!message.empty())
    {
        notice =
            "<div style='background:#f3dfe5;color:#654754;"
            "padding:12px 16px;border-radius:10px;"
            "margin:0 0 18px;font-weight:600;'>"
            + htmlSafe(message) +
            "</div>";
    }

    replaceText(page, "<!-- SERVER_MESSAGE -->", notice);

    return page;
}

static void sendPage(SOCKET client, const string& page)
{
    string header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Connection: close\r\n"
        "Content-Length: " + to_string(page.size()) +
        "\r\n\r\n";

    string response = header + page;

    send(client, response.c_str(), (int)response.size(), 0);
}

// ================= API SERVER CLASS =================

class APIServer
{
private:
    WarehouseSystem& system;

public:
    APIServer(WarehouseSystem& data) : system(data) {}

    void start()
    {
        WSADATA wsa;

        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            cout << "Winsock could not start.\n";
            return;
        }

        SOCKET serverSocket =
            socket(AF_INET, SOCK_STREAM, 0);

        if (serverSocket == INVALID_SOCKET)
        {
            cout << "Could not create server socket.\n";
            WSACleanup();
            return;
        }

        sockaddr_in serverAddress;

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(8080);

        if (bind(serverSocket,
                 (sockaddr*)&serverAddress,
                 sizeof(serverAddress)) == SOCKET_ERROR ||
            listen(serverSocket, 5) == SOCKET_ERROR)
        {
            cout << "Port 8080 is busy or unavailable. "
                 << "Close the old WIMS server and try again.\n";

            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        cout << "\nWIMS WEB SERVER RUNNING!\n";
        cout << "Open your browser: http://localhost:8080\n";
        cout << "Keep this black console window open "
             << "while using WIMS.\n";
        cout << "Press Ctrl+C in this window to stop the server.\n\n";

        while (true)
        {
            SOCKET client =
                accept(serverSocket, NULL, NULL);

            if (client == INVALID_SOCKET)
                continue;

            string request;
            char buffer[4096];
            int received = 0;

            do
            {
                received = recv(
                    client, buffer, sizeof(buffer), 0
                );

                if (received > 0)
                    request.append(buffer, received);

            } while (
                received == sizeof(buffer) &&
                request.find("\r\n\r\n") == string::npos
            );

            size_t headerEnd =
                request.find("\r\n\r\n");

            string headers =
                headerEnd == string::npos ?
                request : request.substr(0, headerEnd);

            string body =
                headerEnd == string::npos ?
                "" : request.substr(headerEnd + 4);

            string firstLine =
                headers.substr(0, headers.find("\r\n"));

            string method, path;

            stringstream first(firstLine);
            first >> method >> path;

            int contentLength = 0;

            size_t lengthPos =
                headers.find("Content-Length:");

            if (lengthPos != string::npos)
            {
                contentLength =
                    atoi(headers.c_str() + lengthPos + 15);
            }

            while ((int)body.size() < contentLength)
            {
                received = recv(
                    client, buffer, sizeof(buffer), 0
                );

                if (received <= 0)
                    break;

                body.append(buffer, received);
            }

            string message;

            if (method == "POST")
            {
                if (path == "/add-product")
                {
                    string name = getValue(body, "name");

                    int qty = atoi(
                        getValue(body, "quantity").c_str()
                    );

                    double price = atof(
                        getValue(body, "price").c_str()
                    );

                    string expiry = getValue(body, "expiry");

                    if (!name.empty() && qty >= 0 && price >= 0)
                    {
                        system.addProduct(
                            name, qty, price, expiry
                        );

                        message = "Product added successfully.";
                    }
                    else
                    {
                        message =
                            "Please enter valid product details.";
                    }
                }

                else if (path == "/stock")
                {
                    int id = atoi(
                        getValue(body, "product_id").c_str()
                    );

                    int qty = atoi(
                        getValue(body, "quantity").c_str()
                    );

                    string type = getValue(body, "type");

                    if (type == "Stock In")
                        system.stockIn(id, qty);
                    else
                        system.stockOut(id, qty);

                    message = "Stock request processed.";
                }

                else if (path == "/order")
                {
                    int id = atoi(
                        getValue(body, "product_id").c_str()
                    );

                    int qty = atoi(
                        getValue(body, "quantity").c_str()
                    );

                    system.placeOrder(id, qty);

                    message = "Order request processed.";
                }
            }

            sendPage(client, makePage(system, message));

            shutdown(client, SD_SEND);
            closesocket(client);
        }

        closesocket(serverSocket);
        WSACleanup();
    }
};

// ================= START SERVER =================

void runAPIServer(WarehouseSystem& system)
{
    APIServer server(system);
    server.start();
}
